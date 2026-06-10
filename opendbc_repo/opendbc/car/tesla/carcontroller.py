import numpy as np
from opendbc.can import CANPacker
from opendbc.car import Bus
from opendbc.car.lateral import apply_steer_angle_limits_vm
from opendbc.car.interfaces import CarControllerBase
from opendbc.car.tesla.teslacan import TeslaCAN
from opendbc.car.tesla.values import CarControllerParams, TeslaFlags
from opendbc.car.vehicle_model import VehicleModel
from openpilot.iqpilot.selfdrive.car.enhanced_stock_longitudinal_control import get_set_speed_kph_from_params
from opendbc.iqpilot.car.tesla.coop_steering import CoopSteeringCarController


def get_safety_CP():
  # We use the TESLA_MODEL_Y platform for lateral limiting to match safety
  # A Model 3 at 40 m/s using the Model Y limits sees a <0.3% difference in max angle (from curvature factor)
  from opendbc.car.tesla.interface import CarInterface
  return CarInterface.get_non_essential_params("TESLA_MODEL_Y")


class CarController(CarControllerBase):
  def __init__(self, dbc_names, CP, CP_IQ):
    CarControllerBase.__init__(self, dbc_names, CP, CP_IQ)
    self.coop_steer = CoopSteeringCarController()
    self.apply_angle_last = 0
    self.packer = CANPacker(dbc_names[Bus.party])
    self.tesla_can = TeslaCAN(CP, self.packer)

    # Vehicle model used for lateral limiting
    self.VM = VehicleModel(get_safety_CP())

    # HW4.5 shared bus: counter-spoof tracking
    self.hw45_shared_bus = bool(CP.flags & TeslaFlags.HW45_SHARED_BUS)
    self.stock_steer_counter_last = -1
    self.stock_eac_counter_last = -1

  def update(self, CC, CC_IQ, CS, now_nanos):
    actuators = CC.actuators
    can_sends = []

    # Tesla EPS enforces disabling steering on heavy lateral override force.
    # When enabling in a tight curve, we wait until user reduces steering force to start steering.
    # Canceling is done on rising edge and is handled generically with CC.cruiseControl.cancel
    lat_active = CC.latActive and CS.hands_on_level < 3

    if self.hw45_shared_bus:
      # HW4.5: counter-spoof — send only when stock counter changes, with counter+1
      steer_send_ready = CS.stock_steer_counter != self.stock_steer_counter_last and CS.stock_steer_counter >= 0
      eac_send_ready = CS.stock_eac_counter != self.stock_eac_counter_last and CS.stock_eac_counter >= 0

      if steer_send_ready:
        self.apply_angle_last = apply_steer_angle_limits_vm(actuators.steeringAngleDeg, self.apply_angle_last, CS.out.vEgoRaw, CS.out.steeringAngleDeg,
                                                            lat_active, CarControllerParams, self.VM)
        spoof_counter = (CS.stock_steer_counter + 1) % 16
        can_sends.append(self.tesla_can.create_steering_control(*self.coop_steer.update(self.apply_angle_last, lat_active, self.CP_IQ, CS, self.VM),
                                                                 counter=spoof_counter))
      self.stock_steer_counter_last = CS.stock_steer_counter

      if eac_send_ready:
        spoof_counter = (CS.stock_eac_counter + 1) % 16
        can_sends.append(self.tesla_can.create_steering_allowed(counter=spoof_counter))
      self.stock_eac_counter_last = CS.stock_eac_counter
    else:
      if self.frame % CarControllerParams.STEER_STEP == 0:
        # Angular rate limit based on speed
        self.apply_angle_last = apply_steer_angle_limits_vm(actuators.steeringAngleDeg, self.apply_angle_last, CS.out.vEgoRaw, CS.out.steeringAngleDeg,
                                                            lat_active, CarControllerParams, self.VM)

        can_sends.append(self.tesla_can.create_steering_control(*self.coop_steer.update(self.apply_angle_last, lat_active, self.CP_IQ, CS, self.VM)))

      if self.frame % 10 == 0:
        can_sends.append(self.tesla_can.create_steering_allowed())

    # Longitudinal control
    if self.CP.openpilotLongitudinalControl:
      if self.frame % 4 == 0:
        state = 13 if CC.cruiseControl.cancel or CS.das_accCancel else 4  # 4=ACC_ON, 13=ACC_CANCEL_GENERIC_SILENT
        accel = float(np.clip(actuators.accel, CarControllerParams.ACCEL_MIN, CarControllerParams.ACCEL_MAX))
        if not CC.longActive:
          accel = 0.
        cntr = (self.frame // 4) % 8
        set_speed_kph = get_set_speed_kph_from_params(CC_IQ.params)
        can_sends.append(self.tesla_can.create_longitudinal_command(state, accel, cntr, CS.out.vEgo, CC.longActive,
                                                                    CS.cruise_override, set_speed_kph=set_speed_kph))

    else:
      # Increment counter so cancel is prioritized even without openpilot longitudinal
      if CC.cruiseControl.cancel:
        cntr = (CS.das_control["DAS_controlCounter"] + 1) % 8
        can_sends.append(self.tesla_can.create_longitudinal_command(13, 0, cntr, CS.out.vEgo, False, True))

    # TODO: HUD control
    new_actuators = actuators.as_builder()
    new_actuators.steeringAngleDeg = self.apply_angle_last
    new_actuators.accel = self.coop_steer.coop_apply_angle_last_sat  # debug
    new_actuators.curvature = float(self.coop_steer.debug_angle_desired_limited)  # debug
    new_actuators.torque = float(self.coop_steer.override_angle_accu)  # debug

    self.frame += 1
    return new_actuators, can_sends
