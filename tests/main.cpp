#include "Logging/Logging.h"
#include "OS/OS.h"
#include <filesystem>
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
  std::filesystem::create_directories("logs");
  psych::Log::Init();
  psych::OS::Init();

  ::testing::InitGoogleTest(&argc, argv);
  const int result = RUN_ALL_TESTS();

  psych::OS::Shutdown();
  psych::Log::Shutdown();
  return result;
}
