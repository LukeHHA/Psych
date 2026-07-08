 
/**************************************************************************/
/*  CommandLine.h                                                         */                                                            
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PSYCH ENGINE                               */
/**************************************************************************/
/* Copyright (c)  Luke Howe                                               */                                                  
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include <filesystem>
#include <span>
#include <string>
#include <string_view>

#include "Renderer/RendererAPI.h"

namespace psych::cli
{

struct CommandLineOptions {
  std::filesystem::path DataDirectory{};
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
  [[nodiscard]] static ParseResult Parse(int argc, char** argv) { return Parse(MakeArgs(argc, argv)); }

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

      if (arg == "--data-dir") {
        const ValueResult valueResult = ReadNextValue(args, i, "--data-dir");
        if (!valueResult.Success) {
          result.Success      = false;
          result.ErrorMessage = valueResult.ErrorMessage;
          return result;
        }

        result.Options.DataDirectory = valueResult.Value;
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

      result.Success      = false;
      result.ErrorMessage = "Unknown argument: " + std::string(arg) + "\n\n" + HelpText();
      return result;
    }

    if (!result.Options.ShowHelp && result.Options.DataDirectory.empty()) {
      result.Success      = false;
      result.ErrorMessage = "Missing required option: --data-dir <path>\n\n" + HelpText();
    }

    return result;
  }

  [[nodiscard]] static std::string HelpText()
  {
    return "Usage:\n"
           "  App --data-dir <path> [--headless] [--api <opengl|headless>]\n"
           "  App --help\n\n"
           "Options:\n"
           "  --data-dir <path>    Required unless --help is provided.\n"
           "                        Path to the application data directory.\n"
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

  [[nodiscard]] static std::string_view ToStringView(const char* arg) { return (arg != nullptr) ? std::string_view{arg} : std::string_view{}; }

  [[nodiscard]] static ValueResult ReadNextValue(std::span<char* const> args, std::size_t& index, std::string_view optionName)
  {
    const std::size_t valueIndex = index + 1;
    if (valueIndex >= args.size()) {
      return {.Success = false, .Value = {}, .ErrorMessage = "Missing value for " + std::string(optionName) + "\n\n" + HelpText()};
    }

    const std::string_view value = ToStringView(args[valueIndex]);
    if (value.empty()) {
      return {.Success = false, .Value = {}, .ErrorMessage = "Empty value for " + std::string(optionName) + "\n\n" + HelpText()};
    }

    if (value.starts_with('-') && value != "-" && value != "--") {
      return {.Success = false, .Value = {}, .ErrorMessage = "Missing value for " + std::string(optionName) + "\n\n" + HelpText()};
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

} // namespace psych::cli
