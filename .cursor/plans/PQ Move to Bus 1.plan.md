---
name: PQ Gateway Bus 1 Fix
overview: Move all PQ platform CAN routing to bus 1 (powertrain) -- panda safety firmware and openpilot Python -- so lowline PQ cars like the 2014 Passat NMS work with IQ.Pilot. This document reflects the AS-IMPLEMENTED change set (verified, tests passing).
todos:
  - id: panda-rx-tx
    content: "volkswagen_pq.h: Move all RX checks from bus 0 to bus 1, update rx_hook to process bus 1, move OP control TX to bus 1 (check_relay=false); keep Motor_2/Motor_5/GRA_Neu spoofs on bus 2 with check_relay=true"
    status: completed
  - id: panda-tests
    content: "test_volkswagen_pq.py: Move RX/TX helper buses to 1, update TX_MSGS, RELAY_MALFUNCTION_ADDRS, FWD_BLACKLISTED_ADDRS, button RX buses"
    status: completed
  - id: canbus-powertrain
    content: "values.py: Add CanBus.powertrain property returning bus 1 for gateway, bus 0 otherwise; store networkLocation; bump NMS docs to 2014-17"
    status: completed
  - id: parser-routing
    content: "carstate.py: Fix get_can_parsers_pq to use powertrain bus, fix ext_cp routing"
    status: completed
  - id: controller-routing
    content: "carcontroller.py: Route PQ control TX through powertrain bus (bus 1 for gateway); leave Motor_2/Motor_5/GRA_Neu spoofs on bus 2"
    status: completed
  - id: realcan-test
    content: "test_vw_real_can.py: Inspect HCA on the powertrain bus instead of hardcoded bus 0"
    status: completed
isProject: false
---

# PQ Bus 1 Routing Fix (as implemented)

## Context

Move all PQ RX checks to bus 1 unconditionally since "every PQ has a bus 1" and powertrain
messages always live there. OP control output (steering, HUD, blinkers, ACC, buttons) moves to
bus 1 too, because on a lowline gateway car (the 2014 Passat NMS) the EPS/ECUs are on bus 1 and
bus 0 is silent -- sending HCA on bus 0 is why steering never engaged.

## Why Bus 1 Works Universally for PQ

- **Lowline gateway (your 2014 NMS)**: No ECAN bus, bus 0 is silent. Bus 1 has everything.
- **Highline gateway (2015+ with ACC)**: ECAN bus forwards copies to bus 0, but bus 1 *still* has the originals.
- **Camera harness (fwdCamera)**: Bus 1 is the OBD/powertrain bus and still has the messages.

Bus 1 is the universal truth for PQ powertrain messages.

## IMPORTANT corrections vs. the original draft of this plan

These two points were wrong in the first draft and were corrected during implementation. Apply
them exactly.

1. **Do NOT set `check_relay = false` on everything.** The bus 2 radar-side spoofs
   (`Motor_2`, `Motor_5`, `GRA_Neu` on bus 2) must keep `check_relay = true`. Two reasons:
   - The AOL (Allow-On-Lateral) state machine only updates inside `stock_ecu_check()`, and that
     function is only invoked from `safety_rx_hook()` for TX entries with `check_relay = true`
     (see `safety/safety.h`, the loop that calls `stock_ecu_check(...)` then
     `aol_state_update(...)`). If *every* TX entry is `check_relay = false`, AOL stops updating
     and `get_controls_allowed_lat()` is stuck/false -> 24 safety tests fail and AOL is broken on
     the car.
   - Those messages genuinely live on the relay-switched bus 2 (radar/camera side), so relay
     malfunction + stock-ECU liveness detection on them is still wanted.
   Only the **bus 1** control messages get `check_relay = false` (bus 1 has no harness relay).

2. **Keep `Motor_2` AND `Motor_5` (and `GRA_Neu`) on bus 2 everywhere.** The stock ACC radar is on
   bus 2, so `filter_motor2` / `filter_motor5` and the radar handler stay on `CAN.ext` (bus 2).
   The original draft's "move filter_motor2 to bus 1" line was a mistake -- ignore it. Also re-add
   `MSG_MOTOR_5` to the panda long TX list (the draft dropped it).

## Files touched (6)

All under `opendbc_repo/opendbc/` unless noted. `opendbc_repo/opendbc` is symlinked from
`opendbc/`, so there is only one physical copy of each file.

| # | File | Change |
|---|------|--------|
| 1 | `safety/modes/volkswagen_pq.h` | RX checks + `rx_hook` to bus 1; OP control TX to bus 1 (`check_relay=false`); bus 2 spoofs kept with `check_relay=true`; re-add `MSG_MOTOR_5` |
| 2 | `safety/tests/test_volkswagen_pq.py` | RX/TX helper buses, `TX_MSGS`, `RELAY_MALFUNCTION_ADDRS`, `FWD_BLACKLISTED_ADDRS`, button RX buses |
| 3 | `car/volkswagen/values.py` | `CanBus.powertrain` property + store `networkLocation`; NMS docs `2014-17` |
| 4 | `car/volkswagen/carstate.py` | `get_can_parsers_pq` uses powertrain bus; `ext_cp = pt_cp` for PQ |
| 5 | `car/volkswagen/carcontroller.py` | `_pt_tx_bus` + route PQ control TX through it |
| 6 | `car/tests/test_vw_real_can.py` | Inspect HCA on the powertrain bus, not hardcoded bus 0 |

---

## 1. `safety/modes/volkswagen_pq.h`

### RX checks -- every `0` -> `1` for the powertrain messages (PQ_SAFETY_1 was already bus 1):

```c
static RxCheck volkswagen_pq_rx_checks[] = {
  {.msg = {{MSG_LENKHILFE_3, 1, 6, 100U, .max_counter = 15U, .ignore_quality_flag = true}, { 0 }, { 0 }}},
  {.msg = {{MSG_BREMSE_1, 1, 8, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
  {.msg = {{MSG_MOTOR_2, 1, 8, 50U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
  {.msg = {{MSG_MOTOR_3, 1, 8, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
  {.msg = {{MSG_MOTOR_5, 1, 8, 50U, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
  {.msg = {{MSG_GRA_NEU, 1, 4, 30U, .max_counter = 15U, .ignore_quality_flag = true}, { 0 }, { 0 }}},
  {.msg = {{MSG_PQ_SAFETY_1, 1, 8, 50U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
};
```

### rx_hook -- change `msg->bus == 0U` to `msg->bus == 1U`:

```c
static void volkswagen_pq_rx_hook(const CANPacket_t *msg) {
  if (msg->bus == 1U) {
    // ... all existing speed, torque, cruise state, button, gas, brake processing (unchanged) ...
  }
  // The ALC module block (also bus 1) stays as-is
}
```

### TX message lists -- bus 1 control = `check_relay=false`; bus 2 spoofs = `check_relay=true`:

Stock TX (note: bus 0 `GRA_Neu` and bus 0 `HCA_1`/`LDW_1` are gone; `GRA_Neu` bus 2 keeps `check_relay=true`):

```c
static const CanMsg VOLKSWAGEN_PQ_STOCK_TX_MSGS[] = {{MSG_HCA_1, 1, 5, .check_relay = false}, {MSG_LDW_1, 1, 8, .check_relay = false},
                                              {MSG_GRA_NEU, 1, 4, .check_relay = false}, {MSG_GRA_NEU, 2, 4, .check_relay = true},
                                              {MSG_BLINKMODI_02, 1, 8, .check_relay = false}, {MSG_APD_1, 1, 8, .check_relay = false}};
```

Long TX (`Motor_2`, `Motor_5`, `GRA_Neu` on bus 2 keep `check_relay=true`; `MSG_MOTOR_5` re-added):

```c
static const CanMsg VOLKSWAGEN_PQ_LONG_TX_MSGS[] =  {{MSG_HCA_1, 1, 5, .check_relay = false}, {MSG_LDW_1, 1, 8, .check_relay = false},
                                              {MSG_ACC_SYSTEM, 1, 8, .check_relay = false}, {MSG_ACC_GRA_ANZEIGE, 1, 8, .check_relay = false},
                                              {MSG_GRA_NEU, 1, 4, .check_relay = false}, {MSG_GRA_NEU, 2, 4, .check_relay = true},
                                              {MSG_BLINKMODI_02, 1, 8, .check_relay = false}, {MSG_MOTOR_2, 2, 8, .check_relay = true},
                                              {MSG_MOTOR_5, 2, 8, .check_relay = true}, {MSG_APD_1, 1, 8, .check_relay = false}};
```

Rationale recap: bus 1 (OBD/powertrain) has no harness relay, so its messages are software-only
(`check_relay = false`), matching how `MSG_PQ_SAFETY_1` already operates. The bus 2 spoofs target
the stock ACC radar across the relay, so they keep `check_relay = true` (relay malfunction
detection + the liveness call path that drives the AOL state machine).

---

## 2. `safety/tests/test_volkswagen_pq.py`

The unit tests encode the expected buses, so they must move in lockstep with the C code.

### RX/TX message helpers -- bus `0` -> `1` for the powertrain messages:

- `_speed_msg` (Bremse_1): bus 0 -> 1
- `_accel_msg` (ACC_System, a TX msg): bus 0 -> 1
- `_torque_driver_msg` (Lenkhilfe_3): bus 0 -> 1
- `_torque_cmd_msg` (HCA_1, a TX msg): bus 0 -> 1
- `_motor_2_msg` (Motor_2): bus 0 -> 1
- `_motor_5_msg` (Motor_5): bus 0 -> 1
- `_user_gas_msg` (Motor_3): bus 0 -> 1
- `_button_msg`: keep default `bus=2` (used for the spam/cancel TX check). All RX call sites that
  passed `bus=0` (in `test_set_and_resume_buttons`, `test_cancel_button`,
  `test_set_and_resume_buttons_with_tsk_only`) change to `bus=1`.

### Base class:

```python
class TestVolkswagenPqSafetyBase(common.CarSafetyTest):
  # Powertrain control TX is on bus 1 (no relay there). Only the bus 2 radar-side spoofs are relay-checked.
  RELAY_MALFUNCTION_ADDRS: dict[int, tuple[int, ...]] = {2: (MSG_GRA_NEU,)}
```

### Stock test class:

```python
class TestVolkswagenPqStockSafety(TestVolkswagenPqSafetyBase):
  TX_MSGS = [[MSG_HCA_1, 1], [MSG_GRA_NEU, 1], [MSG_GRA_NEU, 2], [MSG_LDW_1, 1], [MSG_BLINKMODI_02, 1], [MSG_APD_1, 1]]
  FWD_BLACKLISTED_ADDRS: dict[int, list[int]] = {0: [MSG_GRA_NEU]}
```

### Long test class (its own `RELAY_MALFUNCTION_ADDRS` override is restored):

```python
class TestVolkswagenPqLongSafety(TestVolkswagenPqSafetyBase, common.LongitudinalAccelSafetyTest):
  TX_MSGS = [[MSG_HCA_1, 1], [MSG_LDW_1, 1], [MSG_ACC_SYSTEM, 1], [MSG_ACC_GRA_ANZEIGE, 1],
             [MSG_GRA_NEU, 1], [MSG_GRA_NEU, 2], [MSG_BLINKMODI_02, 1], [MSG_MOTOR_2, 2], [MSG_MOTOR_5, 2], [MSG_APD_1, 1]]
  FWD_BLACKLISTED_ADDRS: dict[int, list[int]] = {0: [MSG_MOTOR_2, MSG_MOTOR_5, MSG_GRA_NEU]}
  RELAY_MALFUNCTION_ADDRS = {2: (MSG_MOTOR_2, MSG_GRA_NEU, MSG_MOTOR_5)}
  INACTIVE_ACCEL = 3.01
```

`FWD_BLACKLISTED_ADDRS` / `RELAY_MALFUNCTION_ADDRS` are derived directly from the `check_relay=true`
entries: a `check_relay=true` TX on bus 2 means (a) RX of that addr on bus 2 triggers relay
malfunction, and (b) forwarding that addr from bus 0 -> 2 is blocked.

---

## 3. `car/volkswagen/values.py`

Store `networkLocation` in `CanBus.__init__`:

```python
def __init__(self, CP=None, fingerprint=None) -> None:
    super().__init__(CP, fingerprint)
    self.offset = 0
    self._network_location = CP.networkLocation if CP is not None else None
```

Add the `powertrain` property after `cam`:

```python
@property
def powertrain(self) -> int:
    # PQ powertrain messages always live on bus 1.
    # Gateway harnesses reach them via aux (bus 1); camera harnesses via pt (bus 0).
    if self._network_location == NetworkLocation.gateway:
        return self.aux  # bus 1
    return self.pt  # bus 0
```

Bump the NMS doc string to cover the 2014:

```python
VOLKSWAGEN_PASSAT_NMS = VolkswagenPQPlatformConfig(
    [VWCarDocs("Volkswagen Passat NMS 2014-17")],
    ...
```

---

## 4. `car/volkswagen/carstate.py`

`get_can_parsers_pq` -- bind the pt parser to the powertrain bus:

```python
@staticmethod
def get_can_parsers_pq(CP):
    can = CanBus(CP)
    aux_messages = [("Diagnose_1", 0)]
    return {
        Bus.pt: CANParser(DBC[CP.carFingerprint][Bus.pt], [], can.powertrain),
        Bus.aux: CANParser(DBC[CP.carFingerprint][Bus.pt], aux_messages, can.aux),
        Bus.cam: CANParser(DBC[CP.carFingerprint][Bus.pt], [], can.cam),
    }
```

`update` -- for PQ, ACC/AWV/SWA share the powertrain segment, so point `ext_cp` at `pt_cp`:

```python
if self.CP.flags & VolkswagenFlags.PQ:
    aux_cp = can_parsers.get(Bus.aux)
    ext_cp = pt_cp  # PQ ACC/AWV/SWA share the powertrain bus
    return self.update_pq(pt_cp, cam_cp, ext_cp, aux_cp)
```

---

## 5. `car/volkswagen/carcontroller.py`

In `__init__`, add the powertrain TX bus right after `self.CAN = CanBus(CP)`:

```python
self.CAN = CanBus(CP)
# PQ powertrain control TX lives on bus 1 for gateway harnesses; all other platforms use bus 0
self._pt_tx_bus = self.CAN.powertrain if CP.flags & VolkswagenFlags.PQ else self.CAN.pt
```

Replace `self.CAN.pt` with `self._pt_tx_bus` on these PQ TX paths (each is the non-MEB branch; for
non-PQ platforms `_pt_tx_bus == self.CAN.pt`, so behavior is unchanged for them):

- `create_hca_steering_control` (HCA/LKAS steering, the `not (AngleLateralControl and ...)` send)
- `create_acc_accel_control` (ACC accel, the non-mqbcan `else` send)
- `create_lka_hud_control` (LDW HUD, the non-MEB `else` send)
- `create_acc_hud_control` (ACC HUD, the non-MEB `else` send)
- `create_blinker_control` (the `if self.CP.flags & VolkswagenFlags.PQ:` blinker send)
- GRA buttons: `bus_send = self._pt_tx_bus if self.CP.flags & VolkswagenFlags.PQ else self.CAN.ext`

**Do NOT change** `filter_motor2` / `filter_motor5` (line ~539-540) or `create_gra_neu`
(line ~559): they intentionally stay on `self.CAN.ext` (bus 2) because the stock ACC radar lives on
bus 2. The `PQRadarHandler` (`pq_radar_handler.py`) is likewise unchanged (it uses `CAN.ext`).

---

## 6. `car/tests/test_vw_real_can.py`

The replay invariant previously inspected HCA on hardcoded bus 0. Since gateway PQ cars now TX HCA
on bus 1, derive the bus the same way the controller does.

Import:

```python
from opendbc.car.volkswagen.values import CAR, DBC, CanBus, VolkswagenFlags
```

Compute the expected bus and use it in the `sendcan` filter:

```python
# PQ control TX lives on the powertrain bus (bus 1 for gateway harnesses); MQB uses bus 0.
hca_bus = CanBus(CP).powertrain if CP.flags & VolkswagenFlags.PQ else CanBus(CP).pt
...
for addr, dat, bus in sendcan:
    if addr != hca_addr or bus != hca_bus:
        continue
```

---

## Build & test (dev machine)

From `opendbc_repo/`:

```bash
# build the safety lib (do NOT `source setup.sh` if it tries to apt-install mull and fails;
# scons alone is enough)
scons -j$(nproc) opendbc/safety/tests/libsafety/libsafety.so

# PQ safety unit tests -- expect 77 passed, 2 skipped
python3 -m pytest opendbc/safety/tests/test_volkswagen_pq.py -q

# PQ car interfaces (exercises carcontroller + carstate)
python3 -m pytest opendbc/car/tests/test_car_interfaces.py -q -k "PASSAT_NMS or PASSAT_MK7 or JETTA_MK6 or CADDY or SHARAN"

# replay invariant (needs konn3kt network access; one route is public)
python3 -m pytest opendbc/car/tests/test_vw_real_can.py -q
```

### Verified results on the C3 device

- `test_volkswagen_pq.py`: **77 passed, 2 skipped** (identical to baseline).
- Full safety suite: 5811 passed; only 6 **Tesla** failures, which are **pre-existing** on baseline
  (unrelated to this change).
- PQ `test_car_interfaces`: **pass** (all PQ platforms).
- `test_platform_configs`: **pass**.
- `test_vw_real_can`: the public Passat NMS route ("PQ without stock LKAS camera") **passed** with
  HCA on bus 1; the second route returned `HTTP 401` at download (no konn3kt auth token), which is
  environmental, not a code failure.

### Known-unrelated / environmental on this device (not caused by the change, verified on baseline)

- `test_lateral_limits.py` PQ `test_jerk_limits`: fails on baseline too (fork's PQ steering tuning).
- `test_docs.py`: cannot be collected -- `jinja2` not installed.

## Trade-offs / follow-ups

- **No hardware relay on bus 1**: bus 1 (OBD) has no relay cutoff; safety for the bus 1 control
  messages is software-only, matching how `MSG_PQ_SAFETY_1` / the ALC module already operate.
- **Camera-harness PQ cars (latent inconsistency)**: `CanBus.powertrain` returns bus 0 for
  `fwdCamera`, while the panda RX/TX is unconditionally bus 1. Gateway cars (your 2014 NMS) are
  consistent (both bus 1). If camera-harness PQ support is ever needed, make `powertrain` return
  bus 1 unconditionally (and re-check the panda side) so openpilot and panda agree.

## Deploy and Verify (on the car)

1. Apply the 6 file changes in `/data/openpilot/opendbc_repo/opendbc/`.
2. Reboot (`sudo reboot`) -- triggers panda reflash + card restart.
3. In `tmux a`:
   - No `Bremse_1 not valid` spam
   - No "CAN Bus Disconnected" alert
   - No "Controls Mismatch" alert
4. Test cruise engage via steering stalk.
5. Test LKAS steering assist activates above min steer speed.
