#pragma once

#include <cstddef>

namespace psych
{
class BinaryBuffer
{
public:
  constexpr BinaryBuffer()                = default;
  BinaryBuffer(const BinaryBuffer&)       = default;
  BinaryBuffer(BinaryBuffer&&)            = default;
  BinaryBuffer& operator=(BinaryBuffer&&) = default;
  constexpr BinaryBuffer(unsigned char* data, std::size_t size) : m_Data_(data), m_Size_(size) {}
  constexpr BinaryBuffer& operator=(const BinaryBuffer&) = default;

  [[nodiscard]] constexpr bool IsValid() const { return m_Data_ != nullptr && m_Size_ > 0; }
  [[nodiscard]] constexpr unsigned char* data() const { return m_Data_; }
  [[nodiscard]] constexpr std::size_t size() const { return m_Size_; }

private:
  unsigned char* m_Data_ = nullptr;
  std::size_t m_Size_    = 0;
};
} // namespace psych
