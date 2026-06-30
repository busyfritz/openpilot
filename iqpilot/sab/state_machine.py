"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""
from dataclasses import dataclass
from typing import Optional
from cereal import log, custom
State = custom.AlwaysOnLateral.AlwaysOnLateralState
from openpilot.common.realtime import DT_CTRL
from openpilot.selfdrive.selfdrived.events import ET
from openpilot.selfdrive.selfdrived.state import SOFT_DISABLE_TIME
EventName = log.OnroadEvent.EventName
EventNameIQ = custom.IQOnroadEvent.EventName
TORQUE_DELIVERING_STATES = (State.enabled, State.softDisabling, State.overriding)
LATERAL_CONTROLLED_STATES = (State.paused, *TORQUE_DELIVERING_STATES)
GUIDANCE_AVAILABLE_SIGNAL = ET.ENABLE
GUIDANCE_GATE_BLOCK_SIGNAL = ET.NO_ENTRY
GUIDANCE_SUPPRESSION_SIGNAL = ET.SOFT_DISABLE
GUIDANCE_OPERATOR_OFF_SIGNAL = ET.USER_DISABLE
GUIDANCE_HARD_CUT_SIGNAL = ET.IMMEDIATE_DISABLE
GUIDANCE_DRIVER_OVERRIDE_SIGNAL = ET.OVERRIDE_LATERAL
GUIDANCE_ACTIVE_ALERT = ET.WARNING

PAUSE_WITH_IQ_EVENTS = (
  EventNameIQ.silentWrongGear,
  EventNameIQ.silentReverseGear,
  EventNameIQ.silentBrakeHold,
  EventNameIQ.silentDoorOpen,
  EventNameIQ.silentSeatbeltNotLatched,
  EventNameIQ.silentParkBrake,
)
PAUSE_WITH_STOCK_EVENTS = (
  EventName.wrongGear,
  EventName.reverseGear,
  EventName.brakeHold,
  EventName.doorOpen,
  EventName.seatbeltNotLatched,
  EventName.parkBrake,
)
GEARS_ALLOW_PAUSED_SILENT = PAUSE_WITH_IQ_EVENTS
GEARS_ALLOW_PAUSED = PAUSE_WITH_STOCK_EVENTS


@dataclass(frozen=True)
class GuidancePulse:
  wake_ping: bool
  gate_closed: bool
  cooldown_call: bool
  driver_kill: bool
  hard_cut: bool
  hands_on_wheel: bool
  hush_cut: bool
  pit_stop_ready: bool


class GuidanceStateMachine:
  def __init__(self, sab):
    self.selfdrive = sab.selfdrive
    self.ss_state_machine = sab.selfdrive.state_machine
    self._events = sab.selfdrive.events
    self._events_iq = sab.selfdrive.events_iq
    self.state = State.disabled

  def _queue_alert_if_solo(self, alert_type: str) -> None:
    if not self.selfdrive.enabled:
      self.ss_state_machine.current_alert_types.append(alert_type)

  def _sees_event(self, event_type: str) -> bool:
    return self._events.contains(event_type) or self._events_iq.contains(event_type)

  def _can_take_pit_stop(self) -> bool:
    return self._events.contains_in_list(PAUSE_WITH_STOCK_EVENTS) or self._events_iq.contains_in_list(PAUSE_WITH_IQ_EVENTS)

  def _capture_pulse(self) -> GuidancePulse:
    return GuidancePulse(
      wake_ping=self._sees_event(GUIDANCE_AVAILABLE_SIGNAL),
      gate_closed=self._sees_event(GUIDANCE_GATE_BLOCK_SIGNAL),
      cooldown_call=self._sees_event(GUIDANCE_SUPPRESSION_SIGNAL),
      driver_kill=self._sees_event(GUIDANCE_OPERATOR_OFF_SIGNAL),
      hard_cut=self._sees_event(GUIDANCE_HARD_CUT_SIGNAL),
      hands_on_wheel=self._sees_event(GUIDANCE_DRIVER_OVERRIDE_SIGNAL),
      hush_cut=self._events_iq.has(EventNameIQ.silentLkasDisable),
      pit_stop_ready=self._can_take_pit_stop(),
    )

  def _start_grace_period(self) -> None:
    if not self.selfdrive.enabled:
      self.ss_state_machine.soft_disable_timer = int(SOFT_DISABLE_TIME / DT_CTRL)
      self.ss_state_machine.current_alert_types.append(GUIDANCE_SUPPRESSION_SIGNAL)

  def _run_global_cutoffs(self, pulse: GuidancePulse) -> Optional[object]:
    if pulse.driver_kill:
      self.ss_state_machine.current_alert_types.append(GUIDANCE_OPERATOR_OFF_SIGNAL)
      return State.paused if pulse.hush_cut else State.disabled
    if pulse.hard_cut:
      self._queue_alert_if_solo(GUIDANCE_HARD_CUT_SIGNAL)
      return State.disabled
    return None

  def _handle_disabled(self, pulse: GuidancePulse) -> State:
    if not pulse.wake_ping:
      return State.disabled
    if pulse.gate_closed:
      self._queue_alert_if_solo(GUIDANCE_GATE_BLOCK_SIGNAL)
      return State.paused if pulse.pit_stop_ready else State.disabled
    self._queue_alert_if_solo(GUIDANCE_AVAILABLE_SIGNAL)
    return State.overriding if pulse.hands_on_wheel else State.enabled

  def _handle_enabled(self, pulse: GuidancePulse) -> State:
    forced_state = self._run_global_cutoffs(pulse)
    if forced_state is not None:
      return forced_state
    if pulse.cooldown_call:
      self._start_grace_period()
      return State.softDisabling
    if pulse.hands_on_wheel:
      self._queue_alert_if_solo(GUIDANCE_DRIVER_OVERRIDE_SIGNAL)
      return State.overriding
    return State.enabled

  def _handle_soft_disabling(self, pulse: GuidancePulse) -> State:
    forced_state = self._run_global_cutoffs(pulse)
    if forced_state is not None:
      return forced_state
    if not pulse.cooldown_call:
      return State.enabled
    if self.ss_state_machine.soft_disable_timer > 0:
      self._queue_alert_if_solo(GUIDANCE_SUPPRESSION_SIGNAL)
      return State.softDisabling
    return State.disabled

  def _handle_paused(self, pulse: GuidancePulse) -> State:
    forced_state = self._run_global_cutoffs(pulse)
    if forced_state is not None:
      return forced_state
    if not pulse.wake_ping:
      return State.paused
    if pulse.gate_closed:
      self._queue_alert_if_solo(GUIDANCE_GATE_BLOCK_SIGNAL)
      return State.paused
    self._queue_alert_if_solo(GUIDANCE_AVAILABLE_SIGNAL)
    return State.overriding if pulse.hands_on_wheel else State.enabled

  def _handle_overriding(self, pulse: GuidancePulse) -> State:
    forced_state = self._run_global_cutoffs(pulse)
    if forced_state is not None:
      return forced_state
    if pulse.cooldown_call:
      self._start_grace_period()
      return State.softDisabling
    if pulse.hands_on_wheel:
      self.ss_state_machine.current_alert_types.append(GUIDANCE_DRIVER_OVERRIDE_SIGNAL)
      return State.overriding
    return State.enabled

  def update(self):
    pulse = self._capture_pulse()
    handler = {
      State.disabled: self._handle_disabled,
      State.enabled: self._handle_enabled,
      State.softDisabling: self._handle_soft_disabling,
      State.paused: self._handle_paused,
      State.overriding: self._handle_overriding,
    }[self.state]

    self.state = handler(pulse)
    enabled = self.state in LATERAL_CONTROLLED_STATES
    active = self.state in TORQUE_DELIVERING_STATES
    if active:
      self._queue_alert_if_solo(GUIDANCE_ACTIVE_ALERT)
    return enabled, active
