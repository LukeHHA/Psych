#pragma once

#include <cstddef>

namespace ge
{
class BinaryBlob
{
public:
  constexpr BinaryBlob() = default;
  constexpr BinaryBlob(unsigned char* data, std::size_t size) : m_Data_(data), m_Size_(size) {}

  [[nodiscard]] constexpr bool IsValid() const { return m_Data_ != nullptr && m_Size_ > 0; }
  [[nodiscard]] constexpr unsigned char* data() const { return m_Data_; }
  [[nodiscard]] constexpr std::size_t size() const { return m_Size_; }

private:
  unsigned char* m_Data_ = nullptr;
  std::size_t m_Size_    = 0;
};
} // namespace ge
