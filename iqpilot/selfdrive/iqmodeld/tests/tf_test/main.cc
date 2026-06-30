// Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "tensorflow/c/c_api.h"

namespace {

struct FileBlob {
  std::vector<uint8_t> bytes;
};

FileBlob read_blob(const std::filesystem::path &path) {
  FILE *handle = fopen(path.c_str(), "rb");
  if (handle == nullptr) {
    return {};
  }

  fseek(handle, 0, SEEK_END);
  const long byte_count = ftell(handle);
  rewind(handle);

  FileBlob blob;
  blob.bytes.resize(byte_count);
  const size_t read_count = fread(blob.bytes.data(), static_cast<size_t>(byte_count), 1, handle);
  fclose(handle);

  if (read_count != 1) {
    blob.bytes.clear();
  }
  return blob;
}

void free_tf_buffer(void *data, size_t) {
  free(data);
}

TF_Buffer *make_tf_buffer(FileBlob &&blob) {
  auto *buffer = TF_NewBuffer();
  auto *payload = static_cast<uint8_t *>(malloc(blob.bytes.size()));
  assert(payload != nullptr);
  memcpy(payload, blob.bytes.data(), blob.bytes.size());
  buffer->data = payload;
  buffer->length = blob.bytes.size();
  buffer->data_deallocator = free_tf_buffer;
  return buffer;
}

std::string pb_path_from_prefix(const char *prefix) {
  return std::string(prefix) + ".pb";
}

}  // namespace

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("usage: %s <graph-prefix>\n", argv[0]);
    return 1;
  }

  const std::string pb_path = pb_path_from_prefix(argv[1]);
  printf("loading model %s\n", pb_path.c_str());

  FileBlob blob = read_blob(pb_path);
  if (blob.bytes.empty()) {
    printf("FAIL: unable to read graph bytes\n");
    return 1;
  }
  printf("loaded model of size %zu\n", blob.bytes.size());

  std::unique_ptr<TF_Status, decltype(&TF_DeleteStatus)> status(TF_NewStatus(), TF_DeleteStatus);
  std::unique_ptr<TF_Graph, decltype(&TF_DeleteGraph)> graph(TF_NewGraph(), TF_DeleteGraph);
  std::unique_ptr<TF_ImportGraphDefOptions, decltype(&TF_DeleteImportGraphDefOptions)> options(
    TF_NewImportGraphDefOptions(), TF_DeleteImportGraphDefOptions);
  std::unique_ptr<TF_Buffer, decltype(&TF_DeleteBuffer)> buffer(make_tf_buffer(std::move(blob)), TF_DeleteBuffer);

  TF_GraphImportGraphDef(graph.get(), buffer.get(), options.get(), status.get());
  if (TF_GetCode(status.get()) != TF_OK) {
    printf("FAIL: %s\n", TF_Message(status.get()));
    return 1;
  }

  printf("SUCCESS\n");
  return 0;
}
