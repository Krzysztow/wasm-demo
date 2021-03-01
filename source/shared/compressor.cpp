#include <iostream>
#include <array>

#include <fmt/format.h>
#include <miniz.h>
#undef compress
#include "shared/compressor.h"

namespace {

mz_stream constructZStream(const char* in, size_t size, char* out, size_t out_size)
{
  mz_stream stream;

  static_assert(sizeof(unsigned char) == sizeof(char));
  memset(&stream, 0, sizeof(stream));
  stream.next_in = reinterpret_cast<const unsigned char*>(in);
  stream.avail_in = size;
  stream.next_out = reinterpret_cast<unsigned char*>(out);
  stream.avail_out = out_size;

  return stream;
}

}

void printMzStatus(const char what[], int status) {
    std::cerr << "Failed to " << what <<
      " [status:" << status << ":" << mz_error(status) << "]" << std::endl;
}

template <typename F>
bool doMzAction(const char what[], F&& f, const std::initializer_list<int>& expectedStatuses={MZ_OK})
{
  const auto isStatusExpected = [&](int status) {
      return std::any_of(expectedStatuses.begin(), expectedStatuses.end(), [&](int s){return s == status;});
  };

  if (const auto status = f(); !isStatusExpected(status))
  {
    std::cerr << "Failed to " << what <<
      " [status:" << status << ":" << mz_error(status) << "]" << std::endl;
    return false;
  }

  return true;
};

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
