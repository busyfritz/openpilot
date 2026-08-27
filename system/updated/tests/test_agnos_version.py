from openpilot.system.ui.lib.os_update import agnos_version_allowed


def test_exact_match_iqos_4_9():
  assert agnos_version_allowed("IQ.OS 4.9", "IQ.OS 4.9")


def test_exact_match_15_1():
  assert agnos_version_allowed("15.1", "15.1")


def test_allows_dot_patch():
  assert agnos_version_allowed("IQ.OS 4.9.7", "IQ.OS 4.9")


def test_allows_dash_suffix():
  assert agnos_version_allowed("IQ.OS 4.9-rc1", "IQ.OS 4.9")


def test_denies_next_minor():
  assert not agnos_version_allowed("IQ.OS 4.10", "IQ.OS 4.9")


def test_denies_raw_prefix_4_90():
  assert not agnos_version_allowed("IQ.OS 4.90", "IQ.OS 4.9")


def test_denies_older_series():
  assert not agnos_version_allowed("IQ.OS 3.4", "IQ.OS 4.9")


def test_compat_list_allows_only_when_listed():
  assert not agnos_version_allowed("IQ.OS 4.8.2", "IQ.OS 4.9")
  assert not agnos_version_allowed("IQ.OS 4.8.2", "IQ.OS 4.9", [])
  assert agnos_version_allowed("IQ.OS 4.8.2", "IQ.OS 4.9", ["IQ.OS 4.8.2"])
