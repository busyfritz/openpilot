# Redneck ACC ("CC Long") for VW PQ — Project Vision

> Status: IN PROGRESS. This document is the single source of truth for the project.
> A fresh AI session should be able to read this and understand the goal without re-explaining.
>
> Implementation status (see Stage list in section 10a):
> - Stage 1 (set-speed): DONE (pre-existing — `Motor_2.MO2_GRA_Soll`).
> - Stage 2 (auto-enable OP long for PQ cc-only): DONE — `interface.py`.
> - Stage 3 (control loop `create_pq_cc_spam_command`): DONE — `pqcan.py` (logic unit-validated).
> - Stage 4 (wire into carcontroller + guard native long sends): DONE — `carcontroller.py`.
> - Stage 5 (HUD/alerts polish): TODO (min-speed disengage handled by core via minEnableSpeed).
> - Stage 6 (panda safety test): TODO (no C change expected — TX already permitted).
> - Stage 7 (validation: unit tests, sim, replay, on-car): TODO (needs built openpilot env).

## 1. One-line goal

Bring OPGM's battle-tested "CC long" (a.k.a. **redneck ACC**) to the VW **PQ35/PQ46**
platform in the **iqpilot** fork (`beta-pq` branch). Give openpilot working ACC-style
longitudinal control on PQ cars that have only a **dumb stock cruise control (GRA)** and
**no factory ACC**, by automatically tapping the stock cruise buttons.

## 2. What "redneck ACC" is (confirmed mechanism)

openpilot does NOT send gas/brake or native ACC accel commands. Instead it **spams the
stock cruise-control buttons** (set / resume / speed-up / speed-down) so the car's OWN
dumb cruise control tracks openpilot's desired speed. On VW PQ the buttons are the
`GRA_Neu` CAN message fields (`GRA_Up_kurz` = +1, `GRA_Down_kurz` = -1, `GRA_Recall` =
resume, `GRA_Neu_Setzen` = set, `GRA_Abbrechen` = cancel).

## 3. Target cars

PQ cars that have **stock GRA cruise but no factory ACC**. Detected automatically at
fingerprint time (already implemented in `interface.py`):
- `has_acc = 0x368 (ACC_System) in fp or 0x56A (ACC_GRA_Anzeige) in fp`
- If NOT has_acc:
  - `0x366 (AWV)` present  → `VolkswagenFlagsIQ.IQ_CC_ONLY` (has radar → FCW/AEB)
  - else                   → `VolkswagenFlagsIQ.IQ_CC_ONLY_NO_RADAR`
- No user toggle. If the car qualifies, redneck ACC is the longitudinal mode.

## 4. Functional requirements

- **Full ACC behavior via buttons**: follow lead + hold desired speed. Lead source is
  **vision-only** (openpilot model); these cars have no ACC radar for following.
- **Speed control**: each short button tap changes the stock cruise set-speed by 1 unit
  (km/h on VW). Tap frequency (rate) scales with commanded accel — bigger accel = faster
  tapping, exactly like OPGM.
- **Deceleration is physically limited** (accepted): a dumb GRA cannot actively brake.
  openpilot slows the car by lowering the set-speed (coast / engine-brake) and, when it
  needs to bail, by sending CANCEL (free coast). Driver brakes for anything harder; FCW
  warns. Match OPGM's decel logic.
- **Speed floor**: stock GRA has a minimum (~30 km/h on VW PQ; OPGM notes ~24 mph on GM).
  Below the floor, disengage longitudinal and warn the driver to take over. OPGM sends
  CANCEL when at min speed and still needs to decelerate.
- **Engagement**: match OPGM. In OPGM the feature runs under normal openpilot
  engage/`longActive`; on OP disengage while stock cruise is still on, it sends CANCEL.

## 5. Reference implementation: OPGM "CC long"

Repo: `opgm/openpilot` (GM). Feature named **"CC long"** in README:
"Non-ACC cruise control adjustment, aka 'redneck ACC'. ⚠️ Cannot reduce speed below stock
cruise control limits (usually 24mph)."

Key files (in `opgm/opendbc` submodule, `opendbc/car/gm/`):
- `values.py`: `GMFlags.CC_LONG` flag, `CruiseButtons` (RES_ACCEL, DECEL_SET, CANCEL, INIT), `CC_ONLY_CAR`.
- `carcontroller.py` (gate):
  ```python
  if self.CP.flags & GMFlags.CC_LONG.value:
      if CC.longActive and CS.out.vEgo > self.CP.minEnableSpeed:
          can_sends.extend(gmcan.create_gm_cc_spam_command(self.packer_pt, self, CS, actuators))
  # On OP disengage while stock cruise still enabled -> send CANCEL
  ```
- `gmcan.py` (the algorithm — "accel" mode is default):
  ```python
  _CV = CV.MS_TO_MPH
  RATE_UP_MAX = 0.2; RATE_DOWN_MAX = 0.2   # min seconds between taps
  accel = actuators.accel * _CV            # mph/s
  speed_setpoint = round(CS.out.cruiseState.speed * _CV)  # current stock setpoint

  if speed_setpoint == minEnableSpeed and accel < -1:
      button = CANCEL; rate = 0.04                       # at floor, can't go lower -> bail
  elif accel < 0:
      button = DECEL_SET                                 # tap "-": setpoint -1
      rate = RATE_DOWN_MAX if setpoint > vEgo+3 else max(-1/accel, RATE_DOWN_MAX)
  elif accel > 0:
      button = RES_ACCEL                                 # tap "+": setpoint +1
      rate = RATE_UP_MAX if setpoint < vEgo-3 else max(1/accel, RATE_UP_MAX)
  else:
      button = INIT; rate = inf                          # do nothing

  # send button only if enough time since last tap:
  if button != INIT and (frame - last_button_frame)*DT_CTRL > rate:
      last_button_frame = frame
      send create_buttons(...)   # GM: ASCMSteeringButton with rolling counter + checksum
  ```
  Note: `controller.apply_speed` dead-reckons the predicted setpoint (±1 per tap).

## 6. VW PQ side today (iqpilot `beta-pq`)

Car code: `opendbc_repo/opendbc/car/volkswagen/`.
- **Detection + flags DONE**: `interface.py` sets `IQ_CC_ONLY` / `IQ_CC_ONLY_NO_RADAR`;
  cc-only safety params set (`PQ_NO_CAM_BUS`, `PQ_LOWLINE`).
- **Button primitives DONE** (`pqcan.py`):
  - `create_radar_gra(..., up_short, down_short, up_long, down_long, set_btn, cancel, resume, zeitluecke)` → `GRA_Neu`
  - `create_acc_buttons_control(..., cancel, resume, set_button)` → `GRA_Neu`
  - `create_gra_neu`, `create_acc_hud_control`, `create_acc_accel_control` (native ACC — NOT used for redneck)
- **Native long path**: PQ normally sends `create_acc_accel_control` (ACC_System accel).
  For redneck cars there is no ACC computer, so this path must be replaced by button spam.

### What is MISSING (the actual work)
1. **Control loop**: a PQ analog of `create_gm_cc_spam_command` — map `actuators.accel`
   (and speed) to `GRA_Up_kurz` / `GRA_Down_kurz` taps at a variable, accel-scaled rate,
   with min-speed CANCEL and OP-disengage CANCEL. Units in km/h.
2. **Enable OP long for cc-only cars**: today `openpilotLongitudinalControl` is only set
   when the `alpha_long` param is passed. Redneck cars need OP long ON (so the planner
   produces `actuators.accel`) while NOT sending native ACC accel.
3. **Stock set-speed** — RESOLVED and ALREADY IMPLEMENTED for PQ. `carstate.update_pq`
   already sets `ret.cruiseState.speed = Motor_2.MO2_GRA_Soll * KPH_TO_MS` for cc-only
   (with a >70 m/s sentinel guard). `Motor_2` (0x288/648) `MO2_GRA_Soll` = "GRA target
   speed setpoint" (km/h). No dead-reckoning drift risk. (Note: `Motor_8.MO8_GRA_v_Wunsch`
   also carries it but the fork already uses `Motor_2`.) STAGE 1 = DONE.
4. **HUD/UX**: show set-speed and ACC status appropriately; low-speed / take-over alerts.

### Panda safety — RESOLVED via investigation
`opendbc/safety/modes/volkswagen_pq.h`: `GRA_Neu` (0x38A) is already in the long-config
TX allowlist on bus 1/2. The tx hook only blocks `GRA_Neu_Setzen` (bit16) and `GRA_Recall`
(bit17) when `!controls_allowed`; it does NOT restrict `GRA_Down_kurz` (bit10) or
`GRA_Up_kurz` (bit11). So up/down button spam is already permitted once the long config is
active and `controls_allowed` is set (a set/resume press with main-on flips it on).
=> Likely ZERO panda C changes; only need to enable the long config for cc-only cars
(`ALLOW_DEBUG` firmware + `VolkswagenSafetyFlags.LONG_CONTROL`), plus add a safety test.

## 7. Hard-to-undo / high-risk items (flag early)

- **Panda safety changes** — safety-critical firmware. Wrong TX allow-list can let
  openpilot command unintended speed changes. Must be reviewed carefully and match OPGM.
- **Stock set-speed source** — if PQ does not broadcast the GRA setpoint, dead-reckoning
  can drift; a drift bug means openpilot's idea of the setpoint diverges from the car's,
  causing wrong-direction taps. Decide and validate this first.
- **Deceleration authority** — physically capped; must never rely on it for collision
  avoidance. FCW/driver-brake is the safety net.

## 8. Platform / deploy

- Device: comma 3 / 3X (or newer).
- Branch: `iqpilot` fork, `beta-pq`.
- VW uses km/h internally; port GM's mph constants to km/h.

## 9. Open questions to resolve during build

- ~~Does any PQ CAN message expose the live GRA set-speed?~~ RESOLVED: `Motor_8.MO8_GRA_v_Wunsch` (km/h).
- ~~Exact OPGM panda change?~~ RESOLVED: none expected; enable long config for cc-only.
- Exact PQ GRA min set-speed and tap increment (assume 1 km/h/tap, floor ~30 km/h — confirm on car).
- Which bus must `GRA_Neu` be transmitted on for cc-only / lowline harnesses
  (`self.CAN.aux` vs `self.CAN.ext`)? Safety allows bus 1 and 2; pick per harness.
- Confirm `MO8_GRA_v_Wunsch` reads correctly on a real cc-only car (via cabana/rlog).

## 10a. Build plan (small, reviewable stages)

- **Stage 1 — Read stock set-speed (read-only):** Add `Motor_8` (0x48A/1162) to the PQ
  carstate parser; populate `ret.cruiseState.speed` from `MO8_GRA_v_Wunsch` (km/h→m/s) for
  cc-only (today forced 0); handle unset sentinel. No actuation, zero risk.
- **Stage 2 — Enable OP long for cc-only (config only):** In `interface.py`, for
  `IQ_CC_ONLY | IQ_CC_ONLY_NO_RADAR` set `openpilotLongitudinalControl=True` +
  `VolkswagenSafetyFlags.LONG_CONTROL`; set `minEnableSpeed` to GRA floor (~30 km/h).
- **Stage 3 — Control loop (port OPGM, no wiring):** `create_pq_cc_spam_command` in
  `pqcan.py`; accel<0→`GRA_Down_kurz`, accel>0→`GRA_Up_kurz`, floor+decel→`GRA_Abbrechen`;
  accel-scaled tap rate; `apply_speed` ±1 prediction. Unit-test the pure function.
- **Stage 4 — Wire into carcontroller:** gate on `cc_only and CC.longActive and vEgo >
  minEnableSpeed`; send spam on correct bus; skip native `create_acc_accel_control`;
  CANCEL on OP disengage while stock cruise still on.
- **Stage 5 — HUD + engage/handoff + alerts:** ACC HUD status/set-speed; below-floor
  disengage + take-over warning; FCW passthrough.
- **Stage 6 — Panda safety test:** add cc-only long TX case to `test_volkswagen_pq.py`
  (up/down allowed when engaged). Likely no C change.
- **Stage 7 — Validation:** car unit tests, longitudinal maneuver sim, log replay, on-car.

## 10. Tools / connectors that help this project

- **opendbc + PQ DBC**: confirm `GRA_Neu` signal names, min speed, and whether a setpoint
  signal exists.
- **comma cabana / plotjuggler / jotpluggler**: inspect real PQ CAN logs (rlogs) to verify
  the stock cruise setpoint behavior and button effects.
- **cursor security-review skill / subagent**: use on the panda safety diff (safety-critical).
- **route/segment replay + longitudinal maneuver tests**: validate the control loop offline.
