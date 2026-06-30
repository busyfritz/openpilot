"""
Copyright ©️ IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""

import base64
import hashlib
import json
import re
import zlib
from pathlib import Path

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import rsa

from iqpilot.konn3kt.backups.crypto import AESCipher
from openpilot.system.hardware.hw import Paths


class BackupKeyDerivation:
  @staticmethod
  def _load_key(file_path: str) -> bytes:
    with open(file_path, 'rb') as f:
      return f.read()

  @staticmethod
  def derive_aes_key_iv_from_rsa(key_path: str, use_aes_256: bool) -> tuple[bytes, bytes]:
    rsa_key_pem: bytes = BackupKeyDerivation._load_key(key_path)
    key_plain = rsa_key_pem.decode(errors="ignore")

    if "private" in key_plain.lower():
      private_key = serialization.load_pem_private_key(rsa_key_pem, password=None, backend=default_backend())
      if not isinstance(private_key, rsa.RSAPrivateKey):
        raise ValueError("Invalid RSA key format: Unable to determine if key is public or private.")

      der_data = private_key.private_bytes(
        encoding=serialization.Encoding.DER,
        format=serialization.PrivateFormat.TraditionalOpenSSL,
        encryption_algorithm=serialization.NoEncryption()
      )
    elif "public" in key_plain.lower():
      public_key = serialization.load_pem_public_key(rsa_key_pem, backend=default_backend())
      if not isinstance(public_key, rsa.RSAPublicKey):
        raise ValueError("Invalid RSA key format: Unable to determine if key is public or private.")

      der_data = public_key.public_bytes(encoding=serialization.Encoding.DER, format=serialization.PublicFormat.PKCS1)
    else:
      raise ValueError("Unknown key format: Unable to determine if key is public or private.")

    sha256_hash = hashlib.sha256(der_data).digest()
    aes_key = sha256_hash[:32] if use_aes_256 else sha256_hash[:16]
    aes_iv = sha256_hash[16:32]

    return aes_key, aes_iv


def unpack_zlib_blob(blob: bytes) -> bytes:
  return zlib.decompress(blob[4:])


def pack_zlib_blob(payload: bytes) -> bytes:
  return b"ZLIB" + zlib.compress(payload, level=9)


class BackupPayloadCodec:
  @staticmethod
  def _key_path(use_aes_256: bool) -> Path:
    key_name = "id_rsa" if use_aes_256 else "id_rsa.pub"
    return Path(Paths.persist_root()) / "comma" / key_name

  @classmethod
  def _build_cipher(cls, use_aes_256: bool) -> AESCipher:
    key_path = cls._key_path(use_aes_256)
    key, iv = BackupKeyDerivation.derive_aes_key_iv_from_rsa(str(key_path), use_aes_256)
    return AESCipher(key, iv)

  @classmethod
  def decode_text(cls, encrypted_base64: str, use_aes_256: bool = False) -> str:
    try:
      encrypted_bytes = base64.b64decode(encrypted_base64)
      cipher = cls._build_cipher(use_aes_256)
      plaintext = cipher.decrypt(encrypted_bytes)
      return unpack_zlib_blob(plaintext).decode("utf-8")
    except Exception as e:
      print(f"Decryption and decompression failed: {e}")
      return ""

  @classmethod
  def encode_text(cls, text: str, use_aes_256: bool = True) -> str:
    try:
      packed = pack_zlib_blob(text.encode("utf-8"))
      cipher = cls._build_cipher(use_aes_256)
      encrypted_bytes = cipher.encrypt(packed)
      return base64.b64encode(encrypted_bytes).decode("utf-8")
    except Exception as e:
      print(f"Compression and encryption failed: {e}")
      return ""


def decrypt_compressed_data(encrypted_base64, use_aes_256=False):
  return BackupPayloadCodec.decode_text(encrypted_base64, use_aes_256=use_aes_256)


def encrypt_compress_data(text, use_aes_256=True):
  return BackupPayloadCodec.encode_text(text, use_aes_256=use_aes_256)


def to_snake_case(name):
  name = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
  return re.sub('([a-z0-9])([A-Z])', r'\1_\2', name).lower()


def normalize_json_keys(obj):
  if isinstance(obj, dict):
    return {to_snake_case(k): normalize_json_keys(v) for k, v in obj.items()}
  elif isinstance(obj, list):
    return [normalize_json_keys(item) for item in obj]
  return obj


class SnakeCaseEncoder(json.JSONEncoder):
  def encode(self, obj):
    return super().encode(normalize_json_keys(obj))
