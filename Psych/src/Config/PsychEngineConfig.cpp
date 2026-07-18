 
/**************************************************************************/
/*  PsychEngineConfig.cpp                                                 */                                                            
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

#include "PsychEngineConfig.h"

#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "Errors/Errors.h"
#include "FileSystem/FileSystem.h"
#include "cereal/archives/xml.hpp"
#include "expected.h"

#include <fstream>
#include <utility>

namespace psych
{
PsychEngineConfig::PsychEngineConfig(PsychEngineSpecification engineSpec) : m_EngineSpec_(std::move(engineSpec)) {}

Expected<void, errors::SerializationError> PsychEngineConfig::Init() { return TryDeserialize(); }

Expected<void, errors::SerializationError> PsychEngineConfig::Shutdown()
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

Expected<void, errors::SerializationError> PsychEngineConfig::TrySerialize() const
{
  CORE_PROFILE_FUNCTION();

  const auto baseConfigPath = Filesystem::TryGetBaseConfigPath();
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
    archive(cereal::make_nvp("PsychEngineSpecification", engineSpec));
  } catch (const cereal::Exception&) {
    CORE_ASSERT(false, "Failed to serialize config")
    return Unexpected(errors::SerializationError::SerializationFailed);
  }

  return {};
}

Expected<void, errors::SerializationError> PsychEngineConfig::TryDeserialize()
{
  CORE_PROFILE_FUNCTION();
  if (m_HasBeenLoaded) {
    return {};
  }

  const auto baseConfigPath = Filesystem::TryGetBaseConfigPath();
  if (!baseConfigPath) {
    /// Could assert on baseConfigPath itself but the future intent is to not assert
    CORE_ASSERT(false, "Failed to get config path")
    return Unexpected(errors::SerializationError::DeserializationFailed);
  }
  const auto configPath = baseConfigPath.value() / s_ConfigFileName_;

  if (!Filesystem::FileExists(configPath)) {
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
    archive(cereal::make_nvp("PsychEngineSpecification", m_EngineSpec_));
  } catch (const cereal::Exception&) {
    CORE_ASSERT(false, "Failed to deserialize config")
    return Unexpected(errors::SerializationError::DeserializationFailed);
  }

  m_HasBeenLoaded = true;
  return {};
}

const PsychEngineSpecification& PsychEngineConfig::GetPsychEngineSpec() const { return m_EngineSpec_; }

} // namespace psych
