from types import SimpleNamespace

from openpilot.system.hardware.hardwared import ALLOWED_TICI_BRANCHES, is_supported_tici_branch


def test_beta_pq_allowed_for_tici():
  metadata = SimpleNamespace(channel="beta-pq", channel_type="dev")
  assert "beta-pq" in ALLOWED_TICI_BRANCHES
  assert is_supported_tici_branch(metadata)


def test_tici_channel_type_allowed():
  metadata = SimpleNamespace(channel="random-branch", channel_type="tici")
  assert is_supported_tici_branch(metadata)


def test_unsupported_branch_rejected_for_tici():
  metadata = SimpleNamespace(channel="random-branch", channel_type="dev")
  assert not is_supported_tici_branch(metadata)
