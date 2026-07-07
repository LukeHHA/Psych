#pragma once

#include "Errors/Errors.h"
#include "expected.h"
namespace psych
{
class Serializable
{
public:
  Serializable()                                                                        = default;
  Serializable(Serializable&&)                                                          = default;
  Serializable(const Serializable&)                                                     = default;
  Serializable& operator=(Serializable&&)                                               = default;
  Serializable& operator=(const Serializable&)                                          = default;
  virtual ~Serializable()                                                               = default;

  [[nodiscard]] virtual Expected<void, errors::SerializationError> TrySerialize() const = 0;
  [[nodiscard]] virtual Expected<void, errors::SerializationError> TryDeserialize()     = 0;

private:
};
} // namespace psych
