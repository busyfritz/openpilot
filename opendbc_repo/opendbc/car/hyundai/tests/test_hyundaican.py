from opendbc.can import CANParser, CANPacker
from opendbc.car import Bus
from opendbc.car.hyundai import hyundaican
from opendbc.car.hyundai.values import CAR, DBC


class DummyHudControl:
  leadDistanceBars = 3
  leadVisible = True


class DummyLeadData:
  lead_visible = False
  lead_rel_speed = -7
  lead_distance = 42
  object_gap = 5
  object_rel_gap = 2


class DummyTuning:
  comfort_band_upper = 0.2
  comfort_band_lower = 0.3
  jerk_upper = 1.7
  jerk_lower = 1.2
  desired_accel = 0.4
  actual_accel = 0.3
  stopping = False


class DummyCP:
  carFingerprint = CAR.HYUNDAI_PALISADE
  flags = 0


def _decode(msg_name: str, addr: int, dat: bytes):
  cp = CANParser(DBC[CAR.HYUNDAI_PALISADE][Bus.pt], [(msg_name, 0)], 0)
  cp.update([(0, [(addr, dat, 0)])])
  return cp.vl[msg_name]


def test_palisade_uses_stock_scc_surrogates():
  packer = CANPacker(DBC[CAR.HYUNDAI_PALISADE][Bus.pt])
  cp = DummyCP()
  hud = DummyHudControl()
  tuning = DummyTuning()
  lead = DummyLeadData()

  scc11 = hyundaican.create_acc_commands(
    packer, True, 0.5, 3.0, 1, lead, hud, 72, False, False, True, cp, False, tuning
  )[0]
  scc14 = hyundaican.create_acc_commands(
    packer, True, 0.5, 3.0, 1, lead, hud, 72, False, False, True, cp, False, tuning
  )[2]

  scc11_vals = _decode("SCC11", scc11[0], scc11[1])
  assert scc11_vals["ObjValid"] == 1
  assert scc11_vals["ACC_ObjStatus"] == 1
  assert scc11_vals["ACC_ObjDist"] == 1

  scc14_vals = _decode("SCC14", scc14[0], scc14[1])
  assert "ObjDistStat" not in scc14_vals or scc14_vals["ObjDistStat"] == 0
