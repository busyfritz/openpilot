"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""

from types import SimpleNamespace

from cereal import custom
from opendbc.car import structs
from opendbc.car.hyundai.values import HyundaiFlags
from openpilot.iqpilot.aol.aol import AlwaysOnLateral
from openpilot.iqpilot.selfdrive.selfdrived.events import IQEvents
from openpilot.selfdrive.selfdrived.events import Events


ButtonType = structs.CarState.ButtonEvent.Type
EventNameIQ = custom.IQOnroadEvent.EventName


class MockParams:
  def __init__(self, main_cruise_allowed: bool = False, aol_enabled: bool = True):
    self.main_cruise_allowed = main_cruise_allowed
    self.aol_enabled = aol_enabled

  def get_bool(self, key: str) -> bool:
    return {
      "AolEnabled": self.aol_enabled,
      "AolMainCruiseAllowed": self.main_cruise_allowed,
      "AolUnifiedEngagementMode": False,
      "JoystickDebugMode": False,
    }.get(key, False)

  def get(self, key: str, return_default: bool = False):
    if key == "AolSteeringMode":
      return 0 if return_default else b"0"
    return None

  def remove(self, key: str) -> None:
    return None


def make_selfdrive(cp_flags: int, brand: str = "hyundai", main_cruise_allowed: bool = False,
                   aol_enabled: bool = True):
  cp = SimpleNamespace(
    brand=brand,
    flags=cp_flags,
    passive=False,
    notCar=False,
    safetyModel=structs.CarParams.SafetyModel.noOutput,
  )
  cp_iq = SimpleNamespace(flags=0)
  return SimpleNamespace(
    CP=cp,
    CP_IQ=cp_iq,
    params=MockParams(main_cruise_allowed, aol_enabled),
    state_machine=SimpleNamespace(soft_disable_timer=0, current_alert_types=[]),
    events=Events(),
    events_iq=IQEvents(),
    CS_prev=SimpleNamespace(
      gasPressed=False,
      cruiseState=SimpleNamespace(available=False),
      lateralAvailable=False,
    ),
    enabled=False,
    enabled_prev=False,
    initialized=True,
  )


def make_car_state():
  return SimpleNamespace(
    started=True,
    standstill=False,
    doorOpen=False,
    seatbeltUnlatched=False,
    gearShifter=structs.CarState.GearShifter.drive,
    vEgo=0.0,
    gasPressed=False,
    brakePressed=False,
    cruiseState=SimpleNamespace(available=False),
    lateralAvailable=False,
    buttonEvents=[structs.CarState.ButtonEvent(pressed=True, type=ButtonType.lkas)],
  )


def make_vw_car_state(cruise_available: bool):
  return SimpleNamespace(
    started=True,
    standstill=False,
    doorOpen=False,
    seatbeltUnlatched=False,
    gearShifter=structs.CarState.GearShifter.drive,
    vEgo=0.0,
    gasPressed=False,
    brakePressed=False,
    cruiseState=SimpleNamespace(available=cruise_available),
    lateralAvailable=True,
    buttonEvents=[],
  )


def test_hyundai_lkas_button_can_enable_aol_with_lda_button_before_lateral_available():
  selfdrive = make_selfdrive(HyundaiFlags.HAS_LDA_BUTTON)
  aol = AlwaysOnLateral(selfdrive)

  aol.update_events(make_car_state())

  assert selfdrive.events_iq.has(EventNameIQ.lkasEnable)


def test_hyundai_lkas_button_does_not_enable_aol_without_hyundai_always_toggle_support():
  selfdrive = make_selfdrive(0)
  aol = AlwaysOnLateral(selfdrive)

  aol.update_events(make_car_state())

  assert not selfdrive.events_iq.has(EventNameIQ.lkasEnable)


def test_main_cruise_off_disables_aol_even_if_lateral_available_stays_true():
  selfdrive = make_selfdrive(0, brand="volkswagen", main_cruise_allowed=True)
  selfdrive.CS_prev = make_vw_car_state(cruise_available=True)
  aol = AlwaysOnLateral(selfdrive)
  aol.enabled = True

  aol.update_events(make_vw_car_state(cruise_available=False))

  assert selfdrive.events_iq.has(EventNameIQ.lkasDisable)


def test_main_cruise_rising_edge_does_not_engage_aol_when_toggle_off():
  # Regression: AOL off in settings (AolEnabled=False). Turning cruise main on (rising edge,
  # VW) must NOT wake the AOL state machine. update() must short-circuit before update_events/
  # update_state so no lkasEnable fires and active never goes true (no gray override border).
  selfdrive = make_selfdrive(0, brand="volkswagen", main_cruise_allowed=True, aol_enabled=False)
  selfdrive.CS_prev = make_vw_car_state(cruise_available=False)
  aol = AlwaysOnLateral(selfdrive)

  aol.update(make_vw_car_state(cruise_available=True))

  assert not selfdrive.events_iq.has(EventNameIQ.lkasEnable)
  assert not aol.active
  assert not aol.enabled
  assert aol.state_machine.state == custom.AlwaysOnLateral.AlwaysOnLateralState.disabled


def test_main_cruise_rising_edge_engages_aol_when_toggle_on():
  # Control for the regression above: same VW rising edge with AOL on does engage.
  selfdrive = make_selfdrive(0, brand="volkswagen", main_cruise_allowed=True, aol_enabled=True)
  selfdrive.CS_prev = make_vw_car_state(cruise_available=False)
  aol = AlwaysOnLateral(selfdrive)

  aol.update(make_vw_car_state(cruise_available=True))

  assert selfdrive.events_iq.has(EventNameIQ.lkasEnable)
  assert aol.active
