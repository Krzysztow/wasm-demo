#include <iostream>
#include <array>

#include <fmt/format.h>
#include <miniz.h>
#undef compress
#include "shared/compressor.h"

namespace {

void printMzStatus(const char what[], int status) {
    std::cerr << "Failed to " << what <<
      " [status:" << status << ":" << mz_error(status) << "]" << std::endl;
}

}

size_t Compressor::maxCompressedSize(size_t in_size) const
{
  return mz_deflateBound(nullptr, in_size);
}

size_t Compressor::compress(const char* in, size_t size, char* out, size_t out_size) const
{
  std::cout << fmt::format("Compressing {} bytes into {} ({} -> {})...",
                           size, out_size, fmt::ptr(in), fmt::ptr(out)) << std::endl;

  mz_ulong consumed_out_size = out_size;
  const auto status = mz_compress2(reinterpret_cast<unsigned char*>(out), &consumed_out_size,
               reinterpret_cast<const unsigned char*>(in), static_cast<mz_ulong>(size),
               MZ_BEST_COMPRESSION);

  if (MZ_OK != status)
  {
    printMzStatus("compress", status);
    return 0;
  }

  std::cout << "Finished " << consumed_out_size << " bytes." << std::endl;
  return consumed_out_size;
}

size_t Compressor::decompress(const char *in, size_t size, char *out, size_t out_size) const
{
  std::cout << "Decompressing " << size << " bytes into " << out_size << "..." << std::endl;

  mz_ulong consumed_out_size = out_size;
  const auto status = mz_uncompress(reinterpret_cast<unsigned char*>(out), &consumed_out_size,
                                    reinterpret_cast<const unsigned char*>(in), static_cast<mz_ulong>(size));

  if (MZ_OK != status) {
    printMzStatus("decompress", status);
    return 0;
  }

  std::cout << "Finished " << size << " bytes." << std::endl;
  return consumed_out_size;
}

extern "C" {

size_t demo_compress(const char *in, size_t size, char *out, size_t out_size)
{
  return Compressor{}.compress(in, size, out, out_size);
}

size_t demo_decompress(const char *in, size_t size, char *out, size_t out_size)
{
  return Compressor{}.decompress(in, size, out, out_size);
}

size_t demo_maxCompressSize(size_t size)
{
  return Compressor{}.maxCompressedSize(size);
}

}
