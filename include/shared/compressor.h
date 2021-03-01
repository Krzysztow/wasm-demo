#pragma once

class Compressor {
public:
  size_t maxCompressedSize(size_t in_size) const;

  size_t compress(const char* in, size_t size, char* out, size_t out_size) const;
  size_t decompress(const char* in, size_t size, char* out, size_t out_size) const;
};


extern "C" {

size_t demo_maxCompressSize(size_t size);
size_t demo_compress(const char* in, size_t size, char* out, size_t out_size);
size_t demo_decompress(const char* in, size_t size, char* out, size_t out_size);

}
