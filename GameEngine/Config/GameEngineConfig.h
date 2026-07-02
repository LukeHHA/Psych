#pragma once

#include "Config/DefaultConfig.h"
#include "Errors/Errors.h"
#include "ge_expected"
#include <filesystem>

namespace ge
{
class GameEngineConfig
{
public:
  GameEngineConfig() = default;
  explicit GameEngineConfig(GameEngineSpecification engineSpec);
  GameEngineConfig(GameEngineConfig&&)                 = default;
  GameEngineConfig(const GameEngineConfig&)            = default;
  GameEngineConfig& operator=(GameEngineConfig&&)      = default;
  GameEngineConfig& operator=(const GameEngineConfig&) = default;
  ~GameEngineConfig()                                  = default;

  [[nodiscard]] Expected<void, errors::SerializationError> Init();
  [[nodiscard]] Expected<void, errors::SerializationError> Shutdown();
  [[nodiscard]] Expected<void, errors::SerializationError> TrySerialize() const;
  [[nodiscard]] Expected<void, errors::SerializationError> TryDeserialize();

  [[nodiscard]] const GameEngineSpecification& GetGameEngineSpec() const;

private:
  GameEngineSpecification m_EngineSpec_;
  bool m_HasBeenLoaded                                        = false;
  static inline const std::filesystem::path s_ConfigFileName_ = "config.xml";
};
} // namespace ge
