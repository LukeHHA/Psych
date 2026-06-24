#include "GameEngineConfig.h"

#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "Errors/Errors.h"
#include "FileSystem/FileSystem.h"
#include "cereal/archives/xml.hpp"
#include "ge_expected"

#include <fstream>
#include <utility>

namespace ge
{
GameEngineConfig::GameEngineConfig(GameEngineSpecification engineSpec) : m_EngineSpec_(std::move(engineSpec)) {}

Expected<void, errors::SerializationError> GameEngineConfig::Init() { return TryDeserialize(); }

Expected<void, errors::SerializationError> GameEngineConfig::Shutdown()
{
  if (!m_HasBeenLoaded) {
    return {};
  }

  const auto result = TrySerialize();
  if (!result) {
    return result;
  }

  m_HasBeenLoaded = false;
  return {};
}

Expected<void, errors::SerializationError> GameEngineConfig::TrySerialize() const
{
  CORE_PROFILE_FUNCTION();

  const auto baseConfigPath = util::Filesystem::TryGetBaseConfigPath();
  if (!baseConfigPath) {
    CORE_ASSERT(false, "Failed to get config path")
    return Unexpected(errors::SerializationError::SerializationFailed);
  }

  const auto configPath = baseConfigPath.value() / s_ConfigFileName_;
  std::ofstream stream(configPath);
  if (!stream) {
    CORE_ASSERT(false, "Failed to open config file for writing")
    return Unexpected(errors::SerializationError::SerializationFailed);
  }

  try {
    cereal::XMLOutputArchive archive(stream);
    auto engineSpec = m_EngineSpec_;
    archive(cereal::make_nvp("GameEngineSpecification", engineSpec));
  } catch (const cereal::Exception&) {
    CORE_ASSERT(false, "Failed to serialize config")
    return Unexpected(errors::SerializationError::SerializationFailed);
  }

  return {};
}

Expected<void, errors::SerializationError> GameEngineConfig::TryDeserialize()
{
  CORE_PROFILE_FUNCTION();
  if (m_HasBeenLoaded) {
    return {};
  }

  const auto baseConfigPath = util::Filesystem::TryGetBaseConfigPath();
  if (!baseConfigPath) {
    /// Could assert on baseConfigPath itself but the future intent is to not assert
    CORE_ASSERT(false, "Failed to get config path")
    return Unexpected(errors::SerializationError::DeserializationFailed);
  }
  const auto configPath = baseConfigPath.value() / s_ConfigFileName_;

  if (!util::Filesystem::FileExists(configPath)) {
    const auto res = TrySerialize();
    if (!res) {
      CORE_ASSERT(false, "Failed to create config file on disk")
      return Unexpected(errors::SerializationError::SerializationFailed);
    }
    m_HasBeenLoaded = true;
    return {};
  }

  std::ifstream stream(configPath);
  if (!stream) {
    CORE_ASSERT(false, "Failed to open config file for reading")
    return Unexpected(errors::SerializationError::DeserializationFailed);
  }

  try {
    cereal::XMLInputArchive archive(stream);
    archive(cereal::make_nvp("GameEngineSpecification", m_EngineSpec_));
  } catch (const cereal::Exception&) {
    CORE_ASSERT(false, "Failed to deserialize config")
    return Unexpected(errors::SerializationError::DeserializationFailed);
  }

  m_HasBeenLoaded = true;
  return {};
}

const GameEngineSpecification& GameEngineConfig::GetGameEngineSpec() const { return m_EngineSpec_; }

} // namespace ge
