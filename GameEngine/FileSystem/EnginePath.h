#pragma once

#include "Debug/Assert.h"

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

namespace ge::EnginePath
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

} // namespace ge::EnginePath
