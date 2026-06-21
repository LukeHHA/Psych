#include "Logging/Logging.h"
#include <filesystem>
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
  std::filesystem::create_directories("logs");
  ge::Log::Init();

  ::testing::InitGoogleTest(&argc, argv);
  const int result = RUN_ALL_TESTS();

  ge::Log::Shutdown();
  return result;
}
