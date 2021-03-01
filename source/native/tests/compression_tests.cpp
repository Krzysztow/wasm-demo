#include <string>
#include <numeric>
#include <gtest/gtest.h>
#include <fmt/ranges.h>
#include "shared/compressor.h"

namespace {

const std::string_view data_string = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";;
const std::vector<char> data{data_string.begin(), data_string.end()};

const std::vector<unsigned char>& asBytesVectorRef(std::vector<char>& v) {
  return *reinterpret_cast<std::vector<unsigned char>*>(&v);
}

template <typename C>
std::vector<char> compress(Compressor& cr, const C& data_in, size_t out_size = 0)
{
  out_size = (0 == out_size) ? cr.maxCompressedSize(data_in.size()) : out_size;
  std::vector<char> compressedData(out_size, 0);

  const auto compressedSize = cr.compress(data_in.data(), data_in.size(),
                                          compressedData.data(), compressedData.size());
  compressedData.resize(compressedSize);

  fmt::print("After compression: {}\n", asBytesVectorRef(compressedData));

  return compressedData;
}

}


TEST(BasicTest, CompressionFails_OnWrongParameters) 
{
  ASSERT_EQ(0, Compressor{}.compress(nullptr, 0, nullptr, 0));
}


TEST(BasicTest, CompressionFails_OnTooSmallBuffer)
{
  Compressor cr;
  const auto faileddata = compress(cr, data, 4);
  ASSERT_EQ(0, faileddata.size());
}


TEST(BasicTest, CompressionSucceeds_OnSensibleParameters) 
{
  Compressor cr;

  const auto compressedData = compress(cr, data);

  ASSERT_NE(0, compressedData.size());
  ASSERT_LE(compressedData.size(), data.size());
}


TEST(BasicTest, CompressionSucceeds_OnSensibleParameters_AndUncompressibleData)
{
  std::vector<char> uncompressibleData(128, 0x00);
  std::iota(uncompressibleData.begin(), uncompressibleData.end(), 0);

  Compressor cr;
  const auto compressedData = compress(cr, uncompressibleData);

  ASSERT_NE(0, compressedData.size());
  ASSERT_GE(compressedData.size(), uncompressibleData.size());
}


TEST(BasicTest, CompressionSucceeds_CanBeDecompressed)
{
  Compressor cr;
  const auto compressedData = compress(cr, data);

  std::vector<char> decompressedData(data.size(), 0);
  const auto decompressedSize = cr.decompress(compressedData.data(), compressedData.size(),
                                              decompressedData.data(), decompressedData.size());
  fmt::print("After de-compression: {}\n", asBytesVectorRef(decompressedData));

  ASSERT_EQ(decompressedSize, data.size());
  ASSERT_EQ(decompressedData, data);
}
