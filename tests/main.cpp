#include "FileSystem/FileSystem.h"
#include "Logging/Logging.h"
#include "OS/OS.h"
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
  psych::OS::Init();
  psych::Filesystem::Init();
  psych::Log::Init();

  ::testing::InitGoogleTest(&argc, argv);
  const int result = RUN_ALL_TESTS();

  psych::Log::Shutdown();
  psych::Filesystem::Shutdown();
  psych::OS::Shutdown();
  return result;
}
