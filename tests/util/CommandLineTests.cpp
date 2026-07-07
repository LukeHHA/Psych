#include "Util/CommandLine.h"
#include <filesystem>
#include <gtest/gtest.h>
#include <initializer_list>
#include <span>
#include <string>
#include <vector>

namespace
{
psych::cli::ParseResult Parse(std::initializer_list<const char*> args)
{
  std::vector<char*> mutableArgs;
  mutableArgs.reserve(args.size());

  for (const char* arg : args) {
    mutableArgs.push_back(const_cast<char*>(arg));
  }

  return psych::cli::CommandLineParser::Parse(std::span<char* const>(mutableArgs.data(), mutableArgs.size()));
}

TEST(CommandLineTests, ParsesRequiredDataDirectory)
{
  const psych::cli::ParseResult result = Parse({"App", "--data-dir", "./data"});

  ASSERT_TRUE(result.Success) << result.ErrorMessage;
  EXPECT_EQ(result.Options.DataDirectory, std::filesystem::path("./data"));
  EXPECT_EQ(result.Options.RenderingAPI, psych::RendererAPIType::OPENGL);
  EXPECT_FALSE(result.Options.ShowHelp);
}

TEST(CommandLineTests, ParsesHeadlessShortcut)
{
  const psych::cli::ParseResult result = Parse({"App", "--data-dir", "./data", "--headless"});

  ASSERT_TRUE(result.Success) << result.ErrorMessage;
  EXPECT_EQ(result.Options.RenderingAPI, psych::RendererAPIType::TEST_HEADLESS);
}

TEST(CommandLineTests, ParsesExplicitRendererAPI)
{
  const psych::cli::ParseResult result = Parse({"App", "--data-dir", "./data", "--api", "headless"});

  ASSERT_TRUE(result.Success) << result.ErrorMessage;
  EXPECT_EQ(result.Options.RenderingAPI, psych::RendererAPIType::TEST_HEADLESS);
}

TEST(CommandLineTests, HelpDoesNotRequireDataDirectory)
{
  const psych::cli::ParseResult result = Parse({"App", "--help"});

  EXPECT_TRUE(result.Success);
  EXPECT_TRUE(result.Options.ShowHelp);
  EXPECT_TRUE(result.ErrorMessage.empty());
}

TEST(CommandLineTests, MissingDataDirectoryFailsWithActionableMessage)
{
  const psych::cli::ParseResult result = Parse({"App"});

  EXPECT_FALSE(result.Success);
  EXPECT_NE(result.ErrorMessage.find("Missing required option: --data-dir"), std::string::npos);
}

TEST(CommandLineTests, MissingOptionValueFails)
{
  const psych::cli::ParseResult result = Parse({"App", "--data-dir"});

  EXPECT_FALSE(result.Success);
  EXPECT_NE(result.ErrorMessage.find("Missing value for --data-dir"), std::string::npos);
}

TEST(CommandLineTests, InvalidRendererAPIFails)
{
  const psych::cli::ParseResult result = Parse({"App", "--data-dir", "./data", "--api", "vulkan"});

  EXPECT_FALSE(result.Success);
  EXPECT_NE(result.ErrorMessage.find("Invalid value for --api: vulkan"), std::string::npos);
}
} // namespace
