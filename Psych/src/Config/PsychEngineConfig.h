
/**************************************************************************/
/*  PsychEngineConfig.h                                                   */
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
