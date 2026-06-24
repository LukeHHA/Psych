#pragma once

#include "Debug/Assert.h"

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

namespace ge::EnginePath
{

enum class Schema : std::uint8_t { None = 0, Config, Cache, Assets };

[[nodiscard]] inline std::string_view SchemaToString(const Schema schema)
{
  switch (schema) {
  case Schema::Config:
    return "config://";

  case Schema::Cache:
    return "cache://";

  case Schema::Assets:
    return "assets://";

  case Schema::None:
    break;
  }

  CORE_ASSERT(false, "Unknown Schema type");
  return {};
}

[[nodiscard]] inline Schema SchemaFromString(const std::string_view schema)
{
  if (schema == "config://") {
    return Schema::Config;
  }

  if (schema == "cache://") {
    return Schema::Cache;
  }

  if (schema == "assets://") {
    return Schema::Assets;
  }

  CORE_ASSERT(false, "Unknown Schema string");
  return Schema::None;
}

class Path
{
public:
  Path() = default;

  Path(const char* path) { Convert(std::string_view{path}); }

  Path(const std::string& path) { Convert(path); }

  // validity is redundant here due to assertion but I plan to allow the caller to handle
  // invalide paths in the future. It would not make sense for a user to type an invalid
  // path in the editor and the engine crashes
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
      CORE_ASSERT(false, "Invalid engine path. Missing schema.");
      return;
    }

    const std::string_view schema_string = path.substr(0, schema_end + 3);
    m_Schema                             = SchemaFromString(schema_string);

    const std::string_view relative_path = path.substr(schema_end + 3);

    if (relative_path.empty()) {
      CORE_ASSERT(false, "Invalid engine path. Missing relative path.");
      return;
    }

    if (relative_path.find("..") != std::string_view::npos) {
      CORE_ASSERT(false, "Engine path cannot contain '..'.");
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

} // namespace ge::EnginePath
