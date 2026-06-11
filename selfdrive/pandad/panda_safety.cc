#include "selfdrive/pandad/pandad.h"
#include "cereal/messaging/messaging.h"
#include "common/swaglog.h"

void PandaSafety::configureSafetyMode(bool is_onroad) {
  // Bring CAN FD up for VW MEB/MQBevo before the car is identified (see ensureCanFdForCachedMeb).
  ensureCanFdForCachedMeb();

  if (is_onroad && !safety_configured_) {
    updateMultiplexingMode();

    auto car_params = fetchCarParams();
    if (!car_params.empty()) {
      LOGW("got %lu bytes CarParams", car_params[0].size());
      LOGW("got %lu bytes IQCarParams", car_params[1].size());
      setSafetyMode(car_params);
      safety_configured_ = true;
    }
  } else if (!is_onroad) {
    initialized_ = false;
    safety_configured_ = false;
    log_once_ = false;
  }
}

void PandaSafety::updateMultiplexingMode() {
  // Initialize to ELM327 without OBD multiplexing for initial fingerprinting
  if (!initialized_) {
    prev_obd_multiplexing_ = false;
    for (int i = 0; i < pandas_.size(); ++i) {
      pandas_[i]->set_safety_model(cereal::CarParams::SafetyModel::ELM327, 1U);
    }
    initialized_ = true;
  }

  // Switch between multiplexing modes based on the OBD multiplexing request
  bool obd_multiplexing_requested = params_.getBool("ObdMultiplexingEnabled");
  if (obd_multiplexing_requested != prev_obd_multiplexing_) {
    for (int i = 0; i < pandas_.size(); ++i) {
      const uint16_t safety_param = (i > 0 || !obd_multiplexing_requested) ? 1U : 0U;
      pandas_[i]->set_safety_model(cereal::CarParams::SafetyModel::ELM327, safety_param);
    }
    prev_obd_multiplexing_ = obd_multiplexing_requested;
    params_.putBool("ObdMultiplexingChanged", true);
  }
}

// TODO-IQ: Use structs instead of vector
std::vector<std::string> PandaSafety::fetchCarParams() {
  if (!params_.getBool("FirmwareQueryDone")) {
    return {};
  }

  if (!log_once_) {
    LOGW("Finished FW query, Waiting for params to set safety model");
    log_once_ = true;
  }

  if (!params_.getBool("ControlsReady")) {
    return {};
  }
  return {params_.get("CarParams"), params_.get("IQCarParams")};
}

// TODO-IQ: Use structs instead of vector
void PandaSafety::setSafetyMode(const std::vector<std::string> &params_string) {
  AlignedBuffer aligned_buf;
  AlignedBuffer aligned_buf_iq;

  capnp::FlatArrayMessageReader cmsg(aligned_buf.align(params_string[0].data(), params_string[0].size()));
  cereal::CarParams::Reader car_params = cmsg.getRoot<cereal::CarParams>();

  capnp::FlatArrayMessageReader cmsg_iq(aligned_buf_iq.align(params_string[1].data(), params_string[1].size()));
  cereal::IQCarParams::Reader car_params_iq = cmsg_iq.getRoot<cereal::IQCarParams>();

  auto safety_configs = car_params.getSafetyConfigs();
  uint16_t alternative_experience = car_params.getAlternativeExperience();
  uint16_t safety_param_iq = car_params_iq.getSafetyParam();

  for (int i = 0; i < pandas_.size(); ++i) {
    // Default to SILENT safety model if not specified
    cereal::CarParams::SafetyModel safety_model = cereal::CarParams::SafetyModel::SILENT;
    uint16_t safety_param = 0U;
    if (i < safety_configs.size()) {
      safety_model = safety_configs[i].getSafetyModel();
      safety_param = safety_configs[i].getSafetyParam();
    }

    LOGW("Panda %d: setting safety model: %d, param: %d, alternative experience: %d, param_iq: %d", i, (int)safety_model, safety_param, alternative_experience, safety_param_iq);
    pandas_[i]->set_alternative_experience(alternative_experience, safety_param_iq);
    pandas_[i]->set_safety_model(safety_model, safety_param);
  }
}

void PandaSafety::ensureCanFdForCachedMeb() {
  // VW MEB / MQBevo have no ignition line — ignition is the 0x3C0 CAN message on the CAN FD
  // powertrain bus. The real car safety model is only set onroad (setSafetyMode, gated on
  // FirmwareQueryDone + ControlsReady), but going onroad needs ignition, so CAN FD must already
  // be up while still offroad/noOutput. The panda zeroes can_data_speed on every safety-model
  // change until a bus is explicitly requested via 0xf9, so request FD here from the *cached*
  // CarParams. Gated on the cached safety model → only VW MEB/MQBevo devices are touched, and the
  // panda latches the request (canfd_requested) so it survives subsequent safety-model changes.
  if (canfd_configured_) {
    return;
  }

  std::string cp_bytes = params_.get("CarParamsPersistent");
  if (cp_bytes.empty()) {
    return;  // no cache yet (e.g. first-ever setup) — retry next loop
  }

  AlignedBuffer aligned_buf;
  capnp::FlatArrayMessageReader cmsg(aligned_buf.align(cp_bytes.data(), cp_bytes.size()));
  cereal::CarParams::Reader car_params = cmsg.getRoot<cereal::CarParams>();

  bool is_meb = false;
  auto safety_configs = car_params.getSafetyConfigs();
  for (int i = 0; i < safety_configs.size(); ++i) {
    auto model = safety_configs[i].getSafetyModel();
    if ((model == cereal::CarParams::SafetyModel::VOLKSWAGEN_MEB) ||
        (model == cereal::CarParams::SafetyModel::VOLKSWAGEN_MQB_EVO)) {
      is_meb = true;
    }
  }

  if (is_meb) {
    for (int i = 0; i < pandas_.size(); ++i) {
      for (uint16_t bus = 0U; bus < 3U; ++bus) {
        pandas_[i]->set_data_speed_kbps(bus, 2000);  // 2 Mbps CAN FD data phase
      }
    }
    LOGW("VW MEB/MQBevo cached: requested CAN FD (2Mbps) on all buses for pre-ignition CAN FD");
  }
  canfd_configured_ = true;
}

bool PandaSafety::getOffroadMode() {
  auto offroad_mode = params_.getBool("OffroadMode");
  return offroad_mode;
}
