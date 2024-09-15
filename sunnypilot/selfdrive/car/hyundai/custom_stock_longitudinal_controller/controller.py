from cereal import car
from openpilot.selfdrive.car import DT_CTRL
from openpilot.selfdrive.car.hyundai import hyundaicanfd, hyundaican
from openpilot.selfdrive.car.hyundai.values import HyundaiFlags, Buttons, CANFD_CAR, LEGACY_SAFETY_MODE_CAR
from openpilot.sunnypilot.controls.lib.custom_stock_longitudinal_controller.controllers import CustomStockLongitudinalControllerBase, \
  SendCan

ButtonType = car.CarState.ButtonEvent.Type


class CustomStockLongitudinalController(CustomStockLongitudinalControllerBase):
  def __init__(self, car, car_controller, car_state, CP):
    super().__init__(car, car_controller, car_state, CP)
    self.accel_button = Buttons.RES_ACCEL
    self.decel_button = Buttons.SET_DECEL
    self.set_speed_buttons = (ButtonType.accelCruise, ButtonType.decelCruise, ButtonType.cancel)

  def create_can_mock_button_messages(self) -> list[SendCan]:
    can_sends = []
    if self.cruise_button is not None:
      if self.CP.carFingerprint in LEGACY_SAFETY_MODE_CAR:
        send_freq = 1
        if not (self.v_tsc_state != 0 or self.m_tsc_state > 1) and abs(self.target_speed - self.v_cruise) <= 2:
          send_freq = 5
        # send resume at a max freq of 10Hz
        if (self.car_controller.frame - self.car_controller.last_button_frame) * DT_CTRL > 0.1 * send_freq:
          # send 25 messages at a time to increases the likelihood of cruise buttons being accepted
          can_sends.extend([hyundaican.create_clu11(self.car_controller.packer, self.car_controller.frame, self.car_state.clu11, self.cruise_button, self.CP)] * 25)
          if (self.car_controller.frame - self.car_controller.last_button_frame) * DT_CTRL >= 0.15 * send_freq:
            self.car_controller.last_button_frame = self.car_controller.frame
      elif self.car_controller.frame % 2 == 0:
        can_sends.extend([hyundaican.create_clu11(self.car_controller.packer, (self.car_controller.frame // 2) + 1, self.car_state.clu11, self.cruise_button, self.CP)] * 25)

    return can_sends

  def create_canfd_mock_button_messages(self) -> list[SendCan]:
    can_sends = []
    if self.CP.flags & HyundaiFlags.CANFD_ALT_BUTTONS:
      # TODO: resume for alt button cars
      pass
    else:
      if self.cruise_button is not None:
        if self.car_controller.frame % 2 == 0:
          can_sends.append(hyundaicanfd.create_buttons(self.car_controller.packer, self.CP, self.car_controller.CAN, ((self.car_controller.frame // 2) + 1) % 0x10, self.cruise_button))

    return can_sends

  def create_mock_button_messages(self) -> list[SendCan]:
    can_sends = []
    if self.CP.carFingerprint in CANFD_CAR:
      can_sends.extend(self.create_canfd_mock_button_messages())
    else:
      can_sends.extend(self.create_can_mock_button_messages())

    return can_sends