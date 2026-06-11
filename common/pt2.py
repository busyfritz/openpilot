import math


class PT2Filter:
  """
  Discrete 2nd-order (PT2) low-pass filter via Tustin (bilinear) transform.
  Continuous transfer function:
      H(s) = w0^2 / (s^2 + 2*zeta*w0*s + w0^2)
  Ported from infiniteCable2's openpilot (used to smooth the desired curvature
  command and reduce lateral wobble on VW MEB).
  """
  def __init__(self, w0: float, zeta: float, dt: float):
    """
    w0:   natural frequency [rad/s] (sets the rise time)
    zeta: damping ratio (zeta=1 => critically damped)
    dt:   sample time [s]
    """
    self.w0 = w0
    self.zeta = zeta
    self.dt = dt
    self.a1, self.a2, self.b0, self.b1, self.b2 = self._design_pt2(self.w0, self.zeta, self.dt)
    self.y1 = 0.0
    self.y2 = 0.0
    self.u1 = 0.0
    self.u2 = 0.0

  def _design_pt2(self, w0, zeta, dt):
    # Tustin (s = (2/dt)*(1 - z^-1)/(1 + z^-1)) of H(s) above, brought to:
    #   Y(z)/U(z) = (b0 + b1 z^-1 + b2 z^-2) / (1 + a1 z^-1 + a2 z^-2)
    # i.e. y[k] = -a1*y[k-1] - a2*y[k-2] + b0*u[k] + b1*u[k-1] + b2*u[k-2]
    Ts = dt
    wd = w0
    alpha = 2.0 / Ts
    b2_ = w0**2
    b1_ = 2.0 * w0**2
    b0_ = w0**2
    A2_f1 = alpha**2
    A1_f1 = -2.0 * alpha**2
    A0_f1 = alpha**2
    factor2 = 2.0 * zeta * wd * alpha
    A2_f2 = factor2
    A1_f2 = 0.0
    A0_f2 = -factor2
    A2_f3 = wd**2
    A1_f3 = 2.0 * (wd**2)
    A0_f3 = wd**2
    A2 = A2_f1 + A2_f2 + A2_f3
    A1 = A1_f1 + A1_f2 + A1_f3
    A0 = A0_f1 + A0_f2 + A0_f3
    b2d = b2_ / A2
    b1d = b1_ / A2
    b0d = b0_ / A2
    a2d = A0 / A2
    a1d = A1 / A2
    return (a1d, a2d, b0d, b1d, b2d)

  def reset(self, value: float = 0.0):
    # Seed the filter state so re-engaging doesn't ramp from zero.
    self.y1 = value
    self.y2 = value
    self.u1 = value
    self.u2 = value

  def update(self, u: float) -> float:
    y = (
        - self.a1 * self.y1
        - self.a2 * self.y2
        + self.b0 * u
        + self.b1 * self.u1
        + self.b2 * self.u2
    )
    self.y2 = self.y1
    self.y1 = y
    self.u2 = self.u1
    self.u1 = u
    return y

  def steady_state_steps(self) -> int:
    # ~95% settling estimate: Ts = 4 / (zeta*w0), N = Ts/dt
    return math.ceil((4.0 / (self.zeta * self.w0)) / self.dt)
