from __future__ import annotations

from dataclasses import dataclass, field

from opendbc.car import Bus, DbcDict, PlatformConfig, structs
from opendbc.car.lateral import CurvatureSteeringLimits


@dataclass
class VolkswagenMEBPlatformConfig(PlatformConfig):
  dbc_dict: DbcDict = field(default_factory=lambda: {Bus.pt: 'vw_meb', Bus.radar: 'vw_meb'})
  chassis_codes: set[str] = field(default_factory=set)
  wmis: set[WMI] = field(default_factory=set)  # noqa: F821 — string annotation, never evaluated
  model_years: set[str] = field(default_factory=set)

  def init(self):
    from opendbc.car.volkswagen.values import VolkswagenFlags
    self.flags |= VolkswagenFlags.MEB
    if self.flags & VolkswagenFlags.MEB_GEN2:
      self.dbc_dict = {Bus.pt: 'vw_meb_2024', Bus.radar: 'vw_meb_2024'}


@dataclass
class VolkswagenMQBevoPlatformConfig(PlatformConfig):
  dbc_dict: DbcDict = field(default_factory=lambda: {Bus.pt: 'vw_mqbevo', Bus.radar: 'vw_mqbevo'})
  chassis_codes: set[str] = field(default_factory=set)
  wmis: set[WMI] = field(default_factory=set)  # noqa: F821 — string annotation, never evaluated

  def init(self):
    from opendbc.car.volkswagen.values import VolkswagenFlags
    self.flags |= VolkswagenFlags.MQB_EVO


def configure_meb_ccp(ccp, alt_gear: bool, can_define, Button) -> None:
  """Populate a CarControllerParams instance with MEB / MQBevo values.

  ``Button`` is passed in by the caller (defined in values.py) to avoid importing
  from values.py at module load time.
  """
  ccp.LDW_STEP = 10
  ccp.ACC_HUD_STEP = 6
  ccp.STEER_DRIVER_ALLOWANCE = 60
  ccp.STEER_DRIVER_MAX = 300
  ccp.STEERING_POWER_MAX = 90
  ccp.STEERING_POWER_MIN = 4
  ccp.STEERING_POWER_STEP = 2

  ccp.CURVATURE_PID: structs.CarParams.LateralPIDTuning = structs.CarParams.LateralPIDTuning(
    kpBP=[10., 40.],
    kiBP=[10., 40.],
    kf=1.,
    kpV=[0., 1.45],
    kiV=[0., 0.12],
  )

  ccp.CURVATURE_LIMITS: CurvatureSteeringLimits = CurvatureSteeringLimits(
    0.195,
  )

  if alt_gear:
    ccp.shifter_values = can_define.dv["Gateway_73"]["GE_Fahrstufe"]
  else:
    ccp.shifter_values = can_define.dv["Getriebe_11"]["GE_Fahrstufe"]

  ccp.hca_status_values = can_define.dv["QFK_01"]["LatCon_HCA_Status"]

  BASE_BUTTONS = [
    Button(structs.CarState.ButtonEvent.Type.setCruise, "GRA_ACC_01", "GRA_Tip_Setzen", [1]),
    Button(structs.CarState.ButtonEvent.Type.resumeCruise, "GRA_ACC_01", "GRA_Tip_Wiederaufnahme", [1]),
    Button(structs.CarState.ButtonEvent.Type.accelCruise, "GRA_ACC_01", "GRA_Tip_Hoch", [1]),
    Button(structs.CarState.ButtonEvent.Type.decelCruise, "GRA_ACC_01", "GRA_Tip_Runter", [1]),
    Button(structs.CarState.ButtonEvent.Type.gapAdjustCruise, "GRA_ACC_01", "GRA_Verstellung_Zeitluecke", [1, 2, 3]),
  ]
  ccp.BUTTONS = BASE_BUTTONS + [
    Button(structs.CarState.ButtonEvent.Type.cancel, "GRA_ACC_01", "GRA_Hauptschalter", [1]),
  ]
  ccp.BUTTONS_ALT = BASE_BUTTONS + [
    Button(structs.CarState.ButtonEvent.Type.cancel, "GRA_ACC_01", "GRA_Abbrechen", [1]),
  ]

  ccp.LDW_MESSAGES = {
    "none": 0,
    "laneAssistTakeOverUrgent": 4,
    "laneAssistTakeOver": 8,
  }
  ccp.LDW_SOUNDS = {
    "None": 0,
    "Chime": 1,
    "Beep": 2,
  }
