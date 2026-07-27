import hashlib
import json

import pytest

from openpilot.selfdrive.modeld import prebuilt_models


def write_outputs(models_dir, check_path, model_names):
  checks = {}
  for model_name in model_names:
    outputs = {}
    for suffix, contents in {
      'tinygrad.pkl': b'tinygrad',
      'metadata.pkl': b'metadata',
    }.items():
      name = f'{model_name}_{suffix}'
      (models_dir / name).write_bytes(contents)
      outputs[name] = hashlib.sha256(contents).hexdigest()
    checks[model_name] = {'outputs': outputs}
  check_path.write_text(json.dumps(checks))


@pytest.fixture
def packaged_models(tmp_path, monkeypatch):
  models_dir = tmp_path / 'models'
  models_dir.mkdir()
  check_path = models_dir / 'prebuilt_check.json'
  write_outputs(models_dir, check_path, prebuilt_models.MODEL_NAMES)
  monkeypatch.setattr(prebuilt_models, 'MODELS_DIR', models_dir)
  monkeypatch.setattr(prebuilt_models, 'CHECK_PATH', check_path)
  return models_dir


@pytest.mark.parametrize("model_name", prebuilt_models.MODEL_NAMES)
def test_packaged_prebuilt_without_onnx(packaged_models, model_name):
  assert prebuilt_models.packaged_prebuilt_matches(model_name)
  assert not prebuilt_models.verify_prebuilt(model_name, 'flags')


@pytest.mark.parametrize("model_name", prebuilt_models.MODEL_NAMES)
def test_packaged_prebuilt_rejects_corrupt_output(packaged_models, model_name):
  (packaged_models / f'{model_name}_tinygrad.pkl').write_bytes(b'corrupt')

  assert not prebuilt_models.packaged_prebuilt_matches(model_name)


@pytest.mark.parametrize("model_name", prebuilt_models.MODEL_NAMES)
def test_source_checkout_is_not_packaged_prebuilt(packaged_models, model_name):
  (packaged_models / f'{model_name}.onnx').write_bytes(b'onnx')

  assert not prebuilt_models.packaged_prebuilt_matches(model_name)
