 
/**************************************************************************/
/*  EnginePath.h                                                          */                                                            
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

#include "Debug/Assert.h"

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

namespace psych::EnginePath
{

enum class Schema : std::uint8_t { None = 0, Config, Cache, Engine };

[[nodiscard]] inline std::string_view SchemaToString(const Schema schema)
{
  switch (schema) {
  case Schema::Config:
    return "config://";

  case Schema::Cache:
    return "cache://";

  case Schema::Engine:
    return "engine://";

  case Schema::None:
    break;
  }

  CORE_ASSERT(false, "Unknown Schema type");
  return {};
}

[[nodiscard]] inline bool SchemaEquals(const std::string_view lhs, const std::string_view rhs)
{
  if (lhs.size() != rhs.size()) {
    return false;
  }

  for (std::size_t i = 0; i < lhs.size(); ++i) {
    const auto lhsChar = static_cast<unsigned char>(lhs[i]);
    if (static_cast<char>(std::tolower(lhsChar)) != rhs[i]) {
      return false;
    }
  }

  return true;
}

[[nodiscard]] inline Schema SchemaFromString(const std::string_view schema)
{
  if (SchemaEquals(schema, "config://")) {
    return Schema::Config;
  }

  if (SchemaEquals(schema, "cache://")) {
    return Schema::Cache;
  }

  if (SchemaEquals(schema, "engine://")) {
    return Schema::Engine;
  }

  return Schema::None;
}

class Path
{
public:
  Path() = default;

  explicit Path(const char* path)
  {
    if (path != nullptr) {
      Convert(std::string_view{path});
    }
  }

  explicit Path(const std::string& path) { Convert(path); }

  // Keep malformed user-provided engine paths representable so higher-level Try*
  // APIs can return errors instead of asserting during parsing.
  [[nodiscard]] bool IsValid() const { return m_IsValid; }

  [[nodiscard]] Schema GetSchema() const { return m_Schema; }

  [[nodiscard]] const std::filesystem::path& GetRelativePath() const { return m_RelativePath; }

  [[nodiscard]] std::string ToString() const { return std::string{SchemaToString(m_Schema)} + m_RelativePath.string(); }

  explicit operator std::string() const { return ToString(); }

private:
  void Convert(const std::string_view path)
  {
    const auto schema_end = path.find("://");

    if (schema_end == std::string_view::npos) {
      return;
    }

    const std::string_view schema_string = path.substr(0, schema_end + 3);
    m_Schema                             = SchemaFromString(schema_string);
    if (m_Schema == Schema::None) {
      return;
    }

    const std::string_view relative_path = path.substr(schema_end + 3);

    if (relative_path.find("..") != std::string_view::npos) {
      return;
    }

    m_RelativePath = std::filesystem::path{relative_path};
    m_IsValid      = true;
  }

private:
  std::filesystem::path m_RelativePath;
  Schema m_Schema = Schema::None;
  bool m_IsValid  = false;
};

} // namespace psych::EnginePath
