#pragma once

#include <chrono>
#include <compare>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <ostream>

namespace ge::util
{

constexpr std::uint64_t ByteSwap64(std::uint64_t x) noexcept
{
  return ((x & 0x00000000000000FFull) << 56) |
         ((x & 0x000000000000FF00ull) << 40) |
         ((x & 0x0000000000FF0000ull) << 24) |
         ((x & 0x00000000FF000000ull) << 8) |
         ((x & 0x000000FF00000000ull) >> 8) |
         ((x & 0x0000FF0000000000ull) >> 24) |
         ((x & 0x00FF000000000000ull) >> 40) |
         ((x & 0xFF00000000000000ull) >> 56);
}

constexpr std::uint64_t ToBigEndian(std::uint64_t value) noexcept
{
  if constexpr (std::endian::native == std::endian::big)
    return value;
  else if constexpr (std::endian::native == std::endian::little)
    return ByteSwap64(value);
  else
    static_assert(std::endian::native == std::endian::little ||
                      std::endian::native == std::endian::big,
                  "Unsupported mixed-endian platform");
}

#define U48_BIT_TRUNC 0xFFFFFFFFFFFF0000
class UUID_V7
{
public:
  static uint64_t Generate()
  {
    using sys_clock = std::chrono::system_clock;
    using namespace std::chrono;

    std::uint64_t __uuid = 0;

    sys_clock c{};
    auto now       = c.now();
    auto epoch     = duration_cast<milliseconds>(now.time_since_epoch());
    auto timestamp = ToBigEndian(epoch.count());

    std::cout << timestamp << std::endl;
    timestamp &= 0xFFFFFFFFFFFF0000;
    std::cout << timestamp << std::endl;
    __uuid &= timestamp;
    std::cout << __uuid << std::endl;
  }

private:
};

enum class VERSION_NUM : uint8_t { v4 = 4, v7 = 7, v8 = 8 };

class UUID
{
public:
  UUID() { __uuid_value = UUID_V7::Generate(); }

  UUID(const uint8_t version)
  {
    auto to_uint8_t = [](VERSION_NUM v) { return static_cast<uint8_t>(v); };

    switch (version) {
    case to_uint8_t(VERSION_NUM::v7):
      __uuid_value = UUID_V7::Generate();
      break;
    }
    std::perror("Unkown version number.");
  }

  ~UUID()                          = default;

  UUID(const UUID&)                = default;
  UUID& operator=(const UUID&)     = default;
  UUID(UUID&&) noexcept            = default;
  UUID& operator=(UUID&&) noexcept = default;

  inline std::strong_ordering operator<=>(const UUID& other) const noexcept
  {
    return __uuid_value <=> other.__uuid_value;
  }

  inline bool operator==(const UUID& other) const noexcept
  {
    return __uuid_value == other.__uuid_value;
  }

  inline explicit operator bool() const noexcept { return __uuid_value != 0; }

  inline int weak_id() const { return __debug_value; }
  uint64_t value() const noexcept { return __uuid_value; }

private:
  uint64_t __uuid_value = 0;
  int __debug_value     = 0;
  friend std::ostream& operator<<(std::ostream& os, const UUID& uuid);
};

inline std::ostream& operator<<(std::ostream& os, const UUID& uuid)
{
  return os << uuid.__uuid_value;
}

} // namespace ge::util
