#pragma once

#include <eigen3/Eigen/Dense>
#include <deque>
#include <fstream>
#include <memory>
#include <map>
#include <string>

#include "cereal/messaging/messaging.h"
#include "common/params.h"
#include "common/swaglog.h"
#include "common/timing.h"
#include "common/util.h"

#include "iqpilot/common/transformations/coordinates.hpp"
#include "iqpilot/common/transformations/orientation.hpp"
#include "iqpilot/selfdrive/iqlocd/models/orbit_kf.h"
#include "iqpilot/selfdrive/iqlocd/sensor_event_constants.h"

#define VISION_DECIMATION 2
#define SENSOR_DECIMATION 10
#define POSENET_STD_HIST_HALF 20

enum AtlasGnssMode {
  UBLOX, QCOM
};

class AtlasLocator {
public:
  AtlasLocator(AtlasGnssMode gnss_source = AtlasGnssMode::UBLOX);

  int run();

  void reset_kalman(double current_time = NAN);
  void reset_kalman(double current_time, const Eigen::VectorXd &init_orient, const Eigen::VectorXd &init_pos, const Eigen::VectorXd &init_vel, const MatrixXdr &init_pos_R, const MatrixXdr &init_vel_R);
  void reset_kalman(double current_time, const Eigen::VectorXd &init_x, const MatrixXdr &init_P);
  void run_finite_guard(double current_time = NAN);
  void run_time_guard(double current_time = NAN);
  void cool_reset_tracker();
  bool gps_ready();
  bool critical_services_ok(const std::map<std::string, double> &critical_services);
  bool timestamp_ok(double current_time);
  void refresh_gps_mode(double current_time);
  bool inputs_are_ready();
  void clear_observation_timing_fault();

  kj::ArrayPtr<capnp::byte> pack_state_message(MessageBuilder& msg_builder,
    bool inputsOK, bool sensorsOK, bool gpsOK, bool msgValid);
  void populate_location_packet(cereal::IQLiveLocation::Builder& fix);

  Eigen::VectorXd current_geodetic();
  Eigen::VectorXd current_state_vector();
  Eigen::VectorXd current_sigma_vector();

  void consume_bytes(const char *data, const size_t size);
  void consume_event(const cereal::Event::Reader& log);
  void consume_sensor_frame(double current_time, const cereal::SensorEventData::Reader& log);
  void consume_gps_frame(double current_time, const cereal::GpsLocationData::Reader& log, const double sensor_time_offset);
  void consume_gnss_frame(double current_time, const cereal::GnssMeasurements::Reader& log);
  void consume_car_state_frame(double current_time, const cereal::CarState::Reader& log);
  void consume_camera_odometry(double current_time, const cereal::CameraOdometry::Reader& log);
  void consume_live_calibration(double current_time, const cereal::LiveCalibrationData::Reader& log);

  void seed_fake_gps_observations(double current_time);

private:
  std::unique_ptr<OrbitKalman> kf;

  Eigen::VectorXd calib;
  MatrixXdr device_from_calib;
  MatrixXdr calib_from_device;
  bool calibrated = false;

  double car_speed = 0.0;
  double last_reset_time = NAN;
  std::deque<double> posenet_stds;

  std::unique_ptr<LocalCoord> converter;

  int64_t unix_timestamp_millis = 0;
  double reset_tracker = 0.0;
  bool device_fell = false;
  bool gps_mode = false;
  double first_valid_log_time = NAN;
  double ttff = NAN;
  double last_gps_msg = 0;
  AtlasGnssMode gnss_source;
  bool observation_timings_invalid = false;
  std::map<std::string, double> observation_values_invalid;
  bool standstill = true;
  int32_t orientation_reset_count = 0;
  float gps_std_factor;
  float gps_variance_factor;
  float gps_vertical_variance_factor;
  double gps_time_offset;
  Eigen::VectorXd camodo_yawrate_distribution = Eigen::Vector2d(0.0, 10.0); // mean, std

  void tune_gnss_source(const AtlasGnssMode &source);
};
