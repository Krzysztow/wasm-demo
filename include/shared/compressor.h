#pragma once

class Compressor {
public:
  size_t maxCompressedSize(size_t in_size) const;

  size_t compress(const char* in, size_t size, char* out, size_t out_size) const;
  size_t decompress(const char* in, size_t size, char* out, size_t out_size) const;
};


