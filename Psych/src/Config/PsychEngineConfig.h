#pragma once

#include "Config/DefaultConfig.h"
#include "Errors/Errors.h"
#include "expected.h"
#include <filesystem>

namespace psych
{
class PsychEngineConfig
{
public:
  PsychEngineConfig() = default;
  explicit PsychEngineConfig(PsychEngineSpecification engineSpec);
  PsychEngineConfig(PsychEngineConfig&&)                 = default;
  PsychEngineConfig(const PsychEngineConfig&)            = default;
  PsychEngineConfig& operator=(PsychEngineConfig&&)      = default;
  PsychEngineConfig& operator=(const PsychEngineConfig&) = default;
  ~PsychEngineConfig()                                   = default;

  [[nodiscard]] Expected<void, errors::SerializationError> Init();
  [[nodiscard]] Expected<void, errors::SerializationError> Shutdown();
  [[nodiscard]] Expected<void, errors::SerializationError> TrySerialize() const;
  [[nodiscard]] Expected<void, errors::SerializationError> TryDeserialize();

  [[nodiscard]] const PsychEngineSpecification& GetPsychEngineSpec() const;

private:
  PsychEngineSpecification m_EngineSpec_;
  bool m_HasBeenLoaded                                        = false;
  static inline const std::filesystem::path s_ConfigFileName_ = "config.xml";
};
} // namespace psych
