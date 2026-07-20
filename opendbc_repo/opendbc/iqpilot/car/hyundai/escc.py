from opendbc.car import structs

from opendbc.iqpilot.car.hyundai.values import HyundaiFlagsIQ

ESCC_MSG = 0x2AB


class EnhancedSmartCruiseControl:
  def __init__(self, CP: structs.CarParams, CP_IQ: structs.IQCarParams):
    self.CP = CP
    self.CP_IQ = CP_IQ

  @property
  def enabled(self):
    return self.CP_IQ.flags & HyundaiFlagsIQ.ENHANCED_SCC

  @property
  def trigger_msg(self):
    return ESCC_MSG

  def update_car_state(self, car_state):
    self.car_state = car_state

  def update_scc12(self, values):
    values["AEB_CmdAct"] = self.car_state.escc_cmd_act
    values["CF_VSM_Warn"] = self.car_state.escc_aeb_warning
    values["CF_VSM_DecCmdAct"] = self.car_state.escc_aeb_dec_cmd_act
    values["CR_VSM_DecCmd"] = self.car_state.escc_aeb_dec_cmd
    values["AEB_Status"] = 2  # AEB enabled


class EsccCarStateBase:
  def __init__(self):
    self.escc_aeb_warning = 0
    self.escc_aeb_dec_cmd_act = 0
    self.escc_cmd_act = 0
    self.escc_aeb_dec_cmd = 0


class EsccCarController:
  def __init__(self, CP: structs.CarParams, CP_IQ: structs.IQCarParams):
    self.ESCC = EnhancedSmartCruiseControl(CP, CP_IQ)

  def update(self, car_state):
    self.ESCC.update_car_state(car_state)


class EsccRadarInterfaceBase:
  def __init__(self, CP: structs.CarParams, CP_IQ: structs.IQCarParams):
    self.ESCC = EnhancedSmartCruiseControl(CP, CP_IQ)
    self.use_escc = False
