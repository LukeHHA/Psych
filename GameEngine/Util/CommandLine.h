#pragma once

#include <span>
#include <string>
#include <string_view>

#include "Renderer/RendererAPI.h"

namespace ge::cli
{

struct CommandLineOptions {
  RendererAPIType RenderingAPI = RendererAPIType::OPENGL;
  bool ShowHelp                = false;
};

struct ParseResult {
  bool Success = true;
  CommandLineOptions Options{};
  std::string ErrorMessage{};
};

struct ValueResult {
  bool Success = true;
  std::string_view Value{};
  std::string ErrorMessage{};
};

class CommandLineParser
{
public:
  [[nodiscard]] static ParseResult Parse(int argc, char** argv)
  {
    return Parse(MakeArgs(argc, argv));
  }

  [[nodiscard]] static ParseResult Parse(std::span<char* const> args)
  {
    ParseResult result{};

    for (std::size_t i = 1; i < args.size(); ++i) {
      const std::string_view arg = ToStringView(args[i]);

      if (arg.empty()) {
        continue;
      }

      if (arg == "--help" || arg == "-h") {
        result.Options.ShowHelp = true;
        continue;
      }

      if (arg == "--headless") {
        result.Options.RenderingAPI = RendererAPIType::TEST_HEADLESS;
        continue;
      }

      if (arg == "--api") {
        const ValueResult valueResult = ReadNextValue(args, i, "--api");
        if (!valueResult.Success) {
          result.Success      = false;
          result.ErrorMessage = valueResult.ErrorMessage;
          return result;
        }

        const auto apiResult = ParseAPI(valueResult.Value);
        if (!apiResult.Success) {
          return apiResult;
        }

        result.Options.RenderingAPI = apiResult.Options.RenderingAPI;
        continue;
      }

      result.Success = false;
      result.ErrorMessage =
          "Unknown argument: " + std::string(arg) + "\n\n" + HelpText();
      return result;
    }

    return result;
  }

  [[nodiscard]] static std::string HelpText()
  {
    return "Usage:\n"
           "  App [--headless] [--api <opengl|headless>] [--help]\n\n"
           "Options:\n"
           "  --headless            Use the headless renderer API.\n"
           "  --api <value>         Explicitly choose a renderer API.\n"
           "                        Supported values: opengl, headless\n"
           "  --help, -h            Show this help message.\n";
  }

private:
  [[nodiscard]] static std::span<char* const> MakeArgs(int argc, char** argv)
  {
    if (argc <= 0 || argv == nullptr) {
      return {};
    }

    return {argv, static_cast<std::size_t>(argc)};
  }

  [[nodiscard]] static std::string_view ToStringView(const char* arg)
  {
    return (arg != nullptr) ? std::string_view{arg} : std::string_view{};
  }

  [[nodiscard]] static ValueResult ReadNextValue(std::span<char* const> args,
                                                 std::size_t& index,
                                                 std::string_view optionName)
  {
    const std::size_t valueIndex = index + 1;
    if (valueIndex >= args.size()) {
      return {.Success      = false,
              .Value        = {},
              .ErrorMessage = "Missing value for " + std::string(optionName) +
                              "\n\n" + HelpText()};
    }

    const std::string_view value = ToStringView(args[valueIndex]);
    if (value.empty()) {
      return {.Success      = false,
              .Value        = {},
              .ErrorMessage = "Empty value for " + std::string(optionName) +
                              "\n\n" + HelpText()};
    }

    index = valueIndex;
    return {.Success = true, .Value = value, .ErrorMessage = {}};
  }

  [[nodiscard]] static ParseResult ParseAPI(std::string_view value)
  {
    ParseResult result{};

    if (value == "opengl") {
      result.Options.RenderingAPI = RendererAPIType::OPENGL;
      return result;
    }

    if (value == "headless") {
      result.Options.RenderingAPI = RendererAPIType::TEST_HEADLESS;
      return result;
    }

    result.Success      = false;
    result.ErrorMessage = "Invalid value for --api: " + std::string(value) +
                          "\n"
                          "Expected one of: opengl, headless\n\n" +
                          HelpText();
    return result;
  }
};

} // namespace ge::cli
