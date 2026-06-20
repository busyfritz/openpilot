---
name: PQ Gateway Bus 1 Fix
overview: Move all PQ platform CAN routing to bus 1 (powertrain) -- panda safety firmware and openpilot Python -- so lowline PQ cars like the 2014 Passat NMS work with IQ.Pilot.
todos:
  - id: panda-rx-tx
    content: "volkswagen_pq.h: Move all RX checks from bus 0 to bus 1, update rx_hook to process bus 1, move TX messages to bus 1"
    status: pending
  - id: canbus-powertrain
    content: "values.py: Add CanBus.powertrain property returning bus 1 for gateway, bus 0 otherwise"
    status: pending
  - id: parser-routing
    content: "carstate.py: Fix get_can_parsers_pq to use powertrain bus, fix ext_cp routing"
    status: pending
  - id: controller-routing
    content: "carcontroller.py: Route all PQ TX through powertrain bus (bus 1 for gateway)"
    status: pending
isProject: false
---

# PQ Bus 1 Routing Fix

## Context

Move all PQ RX checks to bus 1 unconditionally since "every PQ has a bus 1" and powertrain messages always live there. This plan documents the full change set.

## Why Bus 1 Works Universally for PQ

- **Lowline gateway (your 2014 NMS)**: No ECAN bus, bus 0 is silent. Bus 1 has everything.
- **Highline gateway (2015+ with ACC)**: ECAN bus forwards copies to bus 0, but bus 1 *still* has the originals.
- **Camera harness (fwdCamera)**: Bus 1 is the OBD/powertrain bus and still has the messages.

Bus 1 is the universal truth for PQ powertrain messages. No conditional flag needed on the panda side.

## Changes (4 files)

All paths relative to `opendbc_repo/opendbc/`.

### 1. `safety/modes/volkswagen_pq.h` -- Panda safety (bus 0 -> bus 1)

**RX checks** -- Change every `0` to `1` for the powertrain messages:

```c
static RxCheck volkswagen_pq_rx_checks[] = {
    {.msg = {{MSG_LENKHILFE_3, 1, 6, 100U, .max_counter = 15U, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{MSG_BREMSE_1,    1, 8, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{MSG_MOTOR_2,     1, 8, 50U,  .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{MSG_MOTOR_3,     1, 8, 100U, .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{MSG_MOTOR_5,     1, 8, 50U,  .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{MSG_GRA_NEU,     1, 4, 30U,  .max_counter = 15U, .ignore_quality_flag = true}, { 0 }, { 0 }}},
    {.msg = {{MSG_PQ_SAFETY_1, 1, 8, 50U,  .ignore_checksum = true, .ignore_counter = true, .ignore_quality_flag = true}, { 0 }, { 0 }}},
};
```

**rx_hook** -- Change `msg->bus == 0U` to `msg->bus == 1U`:

```c
static void volkswagen_pq_rx_hook(const CANPacket_t *msg) {
    if (msg->bus == 1U) {
        // ... all existing speed, torque, cruise state, button, gas, brake processing
    }
    // The ALC module block (also bus 1) stays as-is
}
```

**TX message lists** -- Move HCA_1, LDW_1, BLINKMODI_02 to bus 1. GRA_NEU already has a bus 1 entry. For long TX, move ACC_SYSTEM, ACC_GRA_ANZEIGE to bus 1:

Stock TX:

```c
static const CanMsg VOLKSWAGEN_PQ_STOCK_TX_MSGS[] = {
    {MSG_HCA_1, 1, 5, .check_relay = false},
    {MSG_LDW_1, 1, 8, .check_relay = false},
    {MSG_GRA_NEU, 1, 4, .check_relay = false},
    {MSG_GRA_NEU, 2, 4, .check_relay = false},
    {MSG_BLINKMODI_02, 1, 8, .check_relay = false},
    {MSG_APD_1, 1, 8, .check_relay = false},
};
```

Long TX:

```c
static const CanMsg VOLKSWAGEN_PQ_LONG_TX_MSGS[] = {
    {MSG_HCA_1, 1, 5, .check_relay = false},
    {MSG_LDW_1, 1, 8, .check_relay = false},
    {MSG_ACC_SYSTEM, 1, 8, .check_relay = false},
    {MSG_ACC_GRA_ANZEIGE, 1, 8, .check_relay = false},
    {MSG_GRA_NEU, 1, 4, .check_relay = false},
    {MSG_BLINKMODI_02, 1, 8, .check_relay = false},
    {MSG_MOTOR_2, 2, 8, .check_relay = false},
    {MSG_APD_1, 1, 8, .check_relay = false},
};
```

Note: `check_relay = false` because bus 1 (OBD) has no harness relay. Safety is software-only on this bus, which matches how MSG_PQ_SAFETY_1 already operates.

### 2. `car/volkswagen/values.py` -- CanBus.powertrain property

Store `networkLocation` in `CanBus.__init_`_:

```python
self._network_location = CP.networkLocation if CP is not None else None
```

Add property after `cam`:

```python
@property
def powertrain(self) -> int:
    # PQ powertrain messages always live on bus 1.
    # Gateway harnesses reach them via aux; camera harnesses via pt.
    if self._network_location == NetworkLocation.gateway:
        return self.aux  # bus 1
    return self.pt  # bus 0
```

### 3. `car/volkswagen/carstate.py` -- Parser bus routing

`**get_can_parsers_pq**` -- Bind pt parser to `powertrain` bus:

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

`**update` method** -- Fix `ext_cp` for PQ. ACC/AWV messages share the powertrain segment:

```python
if self.CP.flags & VolkswagenFlags.PQ:
    aux_cp = can_parsers.get(Bus.aux)
    ext_cp = pt_cp  # PQ ACC/AWV share the powertrain bus
    return self.update_pq(pt_cp, cam_cp, ext_cp, aux_cp)
```

### 4. `car/volkswagen/carcontroller.py` -- TX bus routing

In `__init__`, add powertrain TX bus:

```python
self._pt_tx_bus = self.CAN.powertrain if CP.flags & VolkswagenFlags.PQ else self.CAN.pt
```

Replace `self.CAN.pt` with `self._pt_tx_bus` on PQ TX paths:

- `create_hca_steering_control` (HCA/LKAS steering)
- `create_lka_hud_control` (LDW HUD)
- `create_acc_accel_control` (ACC accel, non-MEB branch)
- `create_acc_hud_control` (ACC HUD, non-MEB branch)
- `create_blinker_control` (blinkers)
- `filter_motor2` (currently wrongly using `CAN.ext`/bus 2)
- GRA buttons: `bus_send = self._pt_tx_bus` for PQ

## Trade-offs

- **No hardware relay on bus 1**: Bus 0 has a relay as a hardware safety cutoff. Bus 1 (OBD) has no relay. Safety is software-only, matching how ALC module already operates on bus 1.
- **Highline PQ cars**: Moving RX to bus 1 means highline cars that previously worked on bus 0 now use bus 1 instead. This should be fine since bus 1 always has the messages, but worth testing on a highline car if possible.
- **Model year docs**: `VOLKSWAGEN_PASSAT_NMS` CarDocs says "2015-17" -- optionally update to "2014-17".

## Deploy and Verify

1. SSH into C3X, apply changes to files in `/data/openpilot/opendbc_repo/opendbc/`
2. Reboot (`sudo reboot`) -- triggers panda reflash + card restart
3. Verify in `tmux a`:
  - No `Bremse_1 not valid` spam
  - No "CAN Bus Disconnected" alert
  - No "Controls Mismatch" alert
4. Test cruise engage via steering stalk
5. Test LKAS steering assist activates above min steer speed

