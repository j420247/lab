#include "deepmind/level_generation/compile_map.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace deepmind {
namespace lab {
namespace {

// Length of file containing 'A'-valued bytes, and corresponding md5 hex string
struct LengthAndMd5 {
  int length;
  const char* md5;
};

// Manual checking can be done in bash with e.g.:
// printf 'A%.0s' {1..2049} | md5sum
constexpr LengthAndMd5 kMD5TestCases[] = {
    {0, "d41d8cd98f00b204e9800998ecf8427e"},
    {2047, "5972cf2ae2e853d61a6fa4cea47817cf"},
    {2048, "ef4ec7e7b54da951a91e8d10c85ca394"},
    {2049, "c3501f191a4a16b3f6bdef03c4beb26c"},
};

TEST(CompileMapTest, CorrectMD5Hashes) {
  const std::string test_file_name =
      std::string(std::getenv("TEST_TMPDIR")) + "/test_file";
  for (const LengthAndMd5& test_case : kMD5TestCases) {
    std::ofstream file(test_file_name, std::ios::binary);
    for (int i = 0; i < test_case.length; i++) {
      file.write("A", 1);
    }
    file.close();
    EXPECT_EQ(test_case.md5, internal::CalculateMd5(test_file_name));
  }
}

}  // namespace
}  // namespace lab
}  // namespace deepmind
