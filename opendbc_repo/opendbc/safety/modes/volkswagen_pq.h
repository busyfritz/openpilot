#pragma once

#include "opendbc/safety/declarations.h"
#include "opendbc/safety/modes/volkswagen_common.h"

#define MSG_LENKHILFE_3         0x0D0U   // RX from EPS, for steering angle and driver steering torque
#define MSG_HCA_1               0x0D2U   // TX by OP, Heading Control Assist steering torque
#define MSG_BREMSE_1            0x1A0U   // RX from ABS, for ego speed
#define MSG_MOTOR_3             0x380U   // RX from ECU
#define MSG_MOTOR_2             0x288U   // RX from ECU, for CC state and brake switch state
#define MSG_ACC_SYSTEM          0x368U   // TX by OP, longitudinal acceleration controls
#define MSG_MOTOR_3             0x380U   // RX from ECU, for driver throttle input
#define MSG_GRA_NEU             0x38AU   // TX by OP, ACC control buttons for cancel/resume
#define MSG_MOTOR_5             0x480U   // RX from ECU, for ACC main switch state
#define MSG_ACC_GRA_ANZEIGE     0x56AU   // TX by OP, ACC HUD
#define MSG_LDW_1               0x5BEU   // TX by OP, Lane line recognition and text alerts
#define MSG_BLINKMODI_02        0x0AAU   // TX by OP, Blinker control
#define MSG_APD_1               0x3D6U   // TX by OP, CarParams
#define MSG_SNG_1               0x3D7U   // TX by OP
#define MSG_PQ_SAFETY_1         0x6A0U   // RX by OP
#define MSG_PQ_DEBUG_LA         0x6A1U   // TX by panda, internal safety state debug
#define MSG_IQ                  0x6A1U   // TX by OP

static bool volkswagen_pq_alc_module_present = false;
static bool volkswagen_pq_acc_tsk_ready = false;
static bool volkswagen_pq_lowline = false;
static bool volkswagen_pq_acc_fts_epb = false;
static bool volkswagen_pq_sng_ecd = false;

static uint32_t volkswagen_pq_get_checksum(const CANPacket_t *msg) {
  return (uint32_t)msg->data[(msg->addr == MSG_MOTOR_5) ? 7 : 0];
}

static uint8_t volkswagen_pq_get_counter(const CANPacket_t *msg) {
  uint8_t counter = 0U;

  if (msg->addr == MSG_LENKHILFE_3) {
    counter = (uint8_t)(msg->data[1] & 0xF0U) >> 4;
  } else if (msg->addr == MSG_GRA_NEU) {
    counter = (uint8_t)(msg->data[2] & 0xF0U) >> 4;
  } else {
  }

  return counter;
}

static uint32_t volkswagen_pq_compute_checksum(const CANPacket_t *msg) {
  int len = GET_LEN(msg);
  uint8_t checksum = 0U;
  int checksum_byte = (msg->addr == MSG_MOTOR_5) ? 7 : 0;

  // Simple XOR over the payload, except for the byte where the checksum lives.
  for (int i = 0; i < len; i++) {
    if (i != checksum_byte) {
      checksum ^= (uint8_t)msg->data[i];
    }
  }

  return checksum;
}

static safety_config volkswagen_pq_init(uint16_t param) {
  // Transmit of GRA_Neu is allowed on bus 0/1/2 for compatibility across camera and gateway integrations
  static const CanMsg VOLKSWAGEN_PQ_STOCK_TX_MSGS[] = {{MSG_HCA_1, 0, 5, .check_relay = true}, {MSG_LDW_1, 0, 8, .check_relay = true},
                                                {MSG_GRA_NEU, 0, 4, .check_relay = false}, {MSG_GRA_NEU, 1, 4, .check_relay = false},
                                                {MSG_GRA_NEU, 2, 4, .check_relay = false}, {MSG_BLINKMODI_02, 0, 8, .check_relay = false},
                                                {MSG_APD_1, 1, 8, .check_relay = false}, {MSG_IQ, 1, 8, .check_relay = false}};

  // Lowline (non-ECAN) lateral-only cars: ptCAN (bus 1) is the only active bus, no J533 gateway.
  // HCA_1 and lateral messages go directly on bus 1 to the EPS. GRA_Neu bus 0 dropped (dead).
  static const CanMsg VOLKSWAGEN_PQ_STOCK_TX_MSGS_BUS1[] = {{MSG_HCA_1, 1, 5, .check_relay = true}, {MSG_LDW_1, 1, 8, .check_relay = true},
                                                {MSG_GRA_NEU, 1, 4, .check_relay = false}, {MSG_GRA_NEU, 2, 4, .check_relay = false},
                                                {MSG_BLINKMODI_02, 1, 8, .check_relay = false},
                                                {MSG_APD_1, 1, 8, .check_relay = false}, {MSG_IQ, 1, 8, .check_relay = false}};

  static const CanMsg VOLKSWAGEN_PQ_LONG_TX_MSGS[] =  {{MSG_HCA_1, 0, 5, .check_relay = true}, {MSG_LDW_1, 0, 8, .check_relay = true},
                                                {MSG_ACC_SYSTEM, 0, 8, .check_relay = true}, {MSG_ACC_GRA_ANZEIGE, 0, 8, .check_relay = true},
                                                {MSG_GRA_NEU, 1, 4, .check_relay = false}, {MSG_GRA_NEU, 2, 4, .check_relay = true},
                                                {MSG_BLINKMODI_02, 0, 8, .check_relay = false}, {MSG_MOTOR_2, 2, 8, .check_relay = true},
                                                {MSG_MOTOR_5, 2, 8, .check_relay = true}, {MSG_MOTOR_3, 1, 8, .check_relay = false},
                                                {MSG_APD_1, 1, 8, .check_relay = false}, {MSG_IQ, 1, 8, .check_relay = false},
                                                {MSG_SNG_1, 1, 8, .check_relay = false}};

  static RxCheck volkswagen_pq_rx_checks[] = {
    {.msg = {{MSG_LENKHILFE_3, 1, 6, 100U, .max_counter = 15U, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{MSG_BREMSE_1, 1, 8, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{MSG_MOTOR_2, 1, 8, 50U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{MSG_MOTOR_3, 1, 8, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{MSG_MOTOR_5, 1, 8, 50U, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{MSG_GRA_NEU, 1, 4, 30U, .max_counter = 15U, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{MSG_PQ_SAFETY_1, 1, 8, 50U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
  };

  volkswagen_common_init();
  volkswagen_pq_alc_module_present = GET_FLAG(param, FLAG_VOLKSWAGEN_PQ_ALC_MODULE);
  volkswagen_pq_lowline = GET_FLAG(param, FLAG_VOLKSWAGEN_PQ_LOWLINE);
  vw_iq_no_cam = GET_FLAG(param, FLAG_VOLKSWAGEN_PQ_NO_CAM_BUS);
  volkswagen_pq_acc_fts_epb = GET_FLAG(param, FLAG_VOLKSWAGEN_PQ_ACC_FTS_EPB);
  volkswagen_pq_sng_ecd = GET_FLAG(param, FLAG_VOLKSWAGEN_PQ_SNG_ECD);
  volkswagen_pq_acc_tsk_ready = false;

#ifdef ALLOW_DEBUG
  volkswagen_longitudinal = GET_FLAG(param, FLAG_VOLKSWAGEN_LONG_CONTROL);
  volkswagen_allow_long_accel_with_gas_pressed = GET_FLAG(param, FLAG_VOLKSWAGEN_ALLOW_LONG_ACCEL_WITH_GAS_PRESSED);
#else
  SAFETY_UNUSED(param);
#endif
  safety_config ret = volkswagen_longitudinal ? BUILD_SAFETY_CFG(volkswagen_pq_rx_checks, VOLKSWAGEN_PQ_LONG_TX_MSGS) : \
                      volkswagen_pq_lowline    ? BUILD_SAFETY_CFG(volkswagen_pq_rx_checks, VOLKSWAGEN_PQ_STOCK_TX_MSGS_BUS1) : \
                                                 BUILD_SAFETY_CFG(volkswagen_pq_rx_checks, VOLKSWAGEN_PQ_STOCK_TX_MSGS);
  if (!volkswagen_pq_alc_module_present) {
    ret.rx_checks_len -= 1;
  }
  return ret;
}

static void volkswagen_pq_rx_hook(const CANPacket_t *msg) {
  // All PQ RX processing is on bus 1 (ptCAN). Messages exist on both bus 0 and bus 1 for ECAN
  // gateway cars; on lowline non-ECAN cars bus 1 is the only active bus.
  if (msg->bus == 1U) {
    // Update in-motion state from speed value.
    // Signal: Bremse_1.BR1_Rad_kmh
    if (msg->addr == MSG_BREMSE_1) {
      int speed = ((msg->data[2] & 0xFEU) >> 1) | (msg->data[3] << 7);
      vehicle_moving = speed > 0;
    }

    // Update driver input torque samples
    // Signal: Lenkhilfe_3.LH3_LM (absolute torque)
    // Signal: Lenkhilfe_3.LH3_LMSign (direction)
    if (msg->addr == MSG_LENKHILFE_3) {
      int torque_driver_new = msg->data[2] | ((msg->data[3] & 0x3U) << 8);
      int sign = (msg->data[3] & 0x4U) >> 2;
      if (sign == 1) {
        torque_driver_new *= -1;
      }
      update_sample(&torque_driver, torque_driver_new);

      uint16_t angle_raw = (uint16_t)msg->data[4] | (((uint16_t)msg->data[5] & 0x0FU) << 8);
      bool angle_sign = ((msg->data[5] >> 4) & 0x1U) != 0U;
      float angle_deg = (float)angle_raw * 0.15f;
      vw_iq_measured_angle_deg = angle_sign ? -angle_deg : angle_deg;
    }

    // acc_main_on tracked unconditionally so main-switch disengagement works for both long
    // and lateral-only (pcmCruise) configurations.
    if (msg->addr == MSG_MOTOR_5) {
      acc_main_on = GET_BIT(msg, 50U);
    }

    if (volkswagen_longitudinal) {
      if (msg->addr == MSG_MOTOR_5) {
        if (!acc_main_on && !volkswagen_pq_acc_tsk_ready) {
          controls_allowed = false;
        }
      }

      if (msg->addr == MSG_MOTOR_2) {
        volkswagen_pq_acc_tsk_ready = GET_BIT(msg, 21U);
        if (!acc_main_on && !volkswagen_pq_acc_tsk_ready) {
          controls_allowed = false;
        }
      }

      if (msg->addr == MSG_GRA_NEU) {
        bool set_button = GET_BIT(msg, 16U);
        bool resume_button = GET_BIT(msg, 17U);
        if ((volkswagen_set_button_prev && !set_button) || (volkswagen_resume_button_prev && !resume_button)) {
          controls_allowed = acc_main_on || volkswagen_pq_acc_tsk_ready;
        }
        volkswagen_set_button_prev = set_button;
        volkswagen_resume_button_prev = resume_button;
        if (GET_BIT(msg, 9U)) {
          controls_allowed = false;
        }
      }
    } else {
      if (msg->addr == MSG_MOTOR_2) {
        int acc_status = (msg->data[2] & 0xC0U) >> 6;
        bool cruise_engaged = (acc_status == 1) || (acc_status == 2);
        pcm_cruise_check(cruise_engaged);
      }
    }

    if (msg->addr == MSG_MOTOR_3) {
      gas_pressed = (msg->data[2]);
    }

    if (msg->addr == MSG_MOTOR_2) {
      brake_pressed = (msg->data[2] & 0x1U);
    }

    if (volkswagen_pq_alc_module_present && (msg->addr == MSG_PQ_SAFETY_1)) {
      const uint16_t desired_angle_raw = (uint16_t)msg->data[6] | (((uint16_t)msg->data[7] & 0x7FU) << 8);
      const bool desired_angle_sign = (msg->data[7] & 0x80U) != 0U;
      const float desired_angle_deg = (float)desired_angle_raw * 0.04375f;
      vw_iq_alc_desired_angle_deg = desired_angle_sign ? -desired_angle_deg : desired_angle_deg;
    }
  }
}

static bool volkswagen_pq_tx_hook(const CANPacket_t *msg) {
  bool tx = true;

  if (msg->addr == MSG_APD_1) {
    volkswagen_iq_decode_apd(msg);
  }

  if (msg->addr == MSG_HCA_1) {
    volkswagen_iq_send_debug_la(MSG_PQ_DEBUG_LA, 1U);
    const uint8_t hca_status = (msg->data[1] >> 4) & 0x0FU;

    if (volkswagen_pq_alc_module_present && (hca_status == 8U)) {
      if (volkswagen_iq_alc_angle_accel_check(false)) {
        tx = false;
      }
    } else if ((hca_status == 5U) || (hca_status == 7U)) {
      int desired_torque = msg->data[2] | ((msg->data[3] & 0x7FU) << 8);
      desired_torque = desired_torque / 32;
      int sign = (msg->data[3] & 0x80U) >> 7;
      if (sign == 1) {
        desired_torque *= -1;
      }

      if (volkswagen_iq_lat_accel_torque_check(desired_torque)) {
        tx = false;
      }
    } else {
    }
  }

  if (msg->addr == MSG_ACC_SYSTEM) {
    int desired_accel = ((((msg->data[4] & 0x7U) << 8) | msg->data[3]) * 5U) - 7220U;
    if (volkswagen_iq_long_accel_check(desired_accel)) {
      tx = false;
    }
  }

  if ((msg->addr == MSG_GRA_NEU) && !controls_allowed) {
    if (GET_BIT(msg, 16U) || GET_BIT(msg, 17U)) {
      tx = false;
    }
  }

  if (msg->addr == MSG_MOTOR_3) {
    if (!volkswagen_pq_acc_fts_epb) {
      tx = false;
    }
  }

  if (msg->addr == MSG_SNG_1) {
    if (!volkswagen_pq_sng_ecd) {
      tx = false;
    }
  }

  return tx;
}

static bool volkswagen_pq_fwd_hook(int bus_num, int addr) {
  SAFETY_UNUSED(addr);
  return vw_iq_no_cam && (bus_num == 0);
}

const safety_hooks volkswagen_pq_hooks = {
  .init = volkswagen_pq_init,
  .rx = volkswagen_pq_rx_hook,
  .tx = volkswagen_pq_tx_hook,
  .fwd = volkswagen_pq_fwd_hook,
  .get_counter = volkswagen_pq_get_counter,
  .get_checksum = volkswagen_pq_get_checksum,
  .compute_checksum = volkswagen_pq_compute_checksum,
};
