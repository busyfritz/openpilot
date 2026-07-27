import pickle

import numpy as np
import pytest

from openpilot.selfdrive.modeld.dmonitoringmodeld import get_driverstate_packet, get_model_paths, parse_model_output, slice_outputs


@pytest.mark.parametrize("device_type, model_name", [
  ("mici", "dmonitoring_model_mici"),
  ("tici", "dmonitoring_model"),
  ("tizi", "dmonitoring_model"),
])
def test_model_paths(device_type, model_name):
  model_path, metadata_path = get_model_paths(device_type)

  assert model_path.name == f"{model_name}_tinygrad.pkl"
  assert metadata_path.name == f"{model_name}_metadata.pkl"


@pytest.mark.parametrize("device_type, expected_sleep_prob", [
  ("mici", 0.),
  ("tici", 0.5),
  ("tizi", 0.5),
])
def test_sleep_probability_output(device_type, expected_sleep_prob):
  _, metadata_path = get_model_paths(device_type)
  with open(metadata_path, 'rb') as f:
    metadata = pickle.load(f)

  output = np.zeros(metadata['output_shapes']['outputs'][1], dtype=np.float32)
  parsed = parse_model_output(slice_outputs(output, metadata['output_slices']))
  parsed['raw_pred'] = b''
  msg = get_driverstate_packet(parsed, 1, 0, 0., 0.)

  assert msg.driverStateV2.leftDriverData.sleepProb == expected_sleep_prob
  assert msg.driverStateV2.rightDriverData.sleepProb == expected_sleep_prob
