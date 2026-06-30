#!/usr/bin/env python3
"""
Copyright (c) IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""
from cereal import custom
from openpilot.iqpilot._proprietary_loader import load_private_module

load_private_module(__name__, "iqpilot_private.models.fetcher")


if "ModelParser" in globals() and hasattr(ModelParser, "_parse_bundle"):
  _ORIGINAL_PARSE_BUNDLE = ModelParser._parse_bundle

  def _parse_bundle(bundle):
    model_bundle = _ORIGINAL_PARSE_BUNDLE(bundle)
    if not bundle.get("runner"):
      model_bundle.runner = custom.IQModelManager.Runner.tinygrad
    return model_bundle

  ModelParser._parse_bundle = staticmethod(_parse_bundle)
