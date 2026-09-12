#include "OS/OS.h"
#include <filesystem>
#include <gtest/gtest.h>

namespace
{
TEST(OSTests, ReturnsCurrentTestExecutable)
{
  const auto result = psych::OS::TryGetExecPath();
  ASSERT_TRUE(result);

  const auto& executable = result.value();
  EXPECT_TRUE(executable.is_absolute());
  EXPECT_TRUE(std::filesystem::exists(executable));
  EXPECT_TRUE(std::filesystem::is_regular_file(executable));
  EXPECT_EQ(executable.stem(), "PsychEngineTests");
}

} // namespace
