#pragma once

#include <algorithm>
#include <array>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <random>
#include <utility>

namespace psych
{
class UUID
{
public:
  using storage_type = std::array<std::uint8_t, 16>;

  UUID() : m_id_(Generate())
  {
#ifdef GE_DEBUG
    m_debug_value_ = m_debug_counter_++;
#endif
  }

  static UUID Nill()
  {
    UUID id;
    id.m_id_.fill(0);
    return id;
  }

  ~UUID()               = default;

  using value_type      = storage_type::value_type;

  using reference       = storage_type::reference;
  using const_reference = storage_type::const_reference;

  using iterator        = storage_type::iterator;
  using const_iterator  = storage_type::const_iterator;

  using size_type       = storage_type::size_type;
  using difference_type = storage_type::difference_type;

  iterator begin() noexcept { return m_id_.begin(); }

  iterator end() noexcept { return m_id_.end(); }

  const_iterator begin() const noexcept { return m_id_.begin(); }

  const_iterator end() const noexcept { return m_id_.end(); }

  // data

  std::uint8_t* data() noexcept { return m_id_.data(); }

  std::uint8_t const* data() const noexcept { return m_id_.data(); }

  // size

  constexpr size_type size() const noexcept { return m_id_.size(); }

  static constexpr size_type static_size() noexcept { return storage_type{}.size(); }

  // is_nil

  bool is_nil() const noexcept
  {
    return std::all_of(m_id_.begin(), m_id_.end(), [](std::uint8_t value) { return value == 0; });
  }

  [[nodiscard]] const storage_type& Value() const noexcept { return m_id_; }

  [[nodiscard]] storage_type& Value() noexcept { return m_id_; }

  friend bool operator==(UUID const& lhs, UUID const& rhs) noexcept { return lhs.m_id_ == rhs.m_id_; }

  friend bool operator!=(UUID const& lhs, UUID const& rhs) noexcept { return lhs.m_id_ != rhs.m_id_; }

  friend bool operator<(UUID const& lhs, UUID const& rhs) noexcept { return lhs.m_id_ < rhs.m_id_; }

  friend bool operator>(UUID const& lhs, UUID const& rhs) noexcept { return lhs.m_id_ > rhs.m_id_; }

  friend bool operator<=(UUID const& lhs, UUID const& rhs) noexcept { return lhs.m_id_ <= rhs.m_id_; }

  friend bool operator>=(UUID const& lhs, UUID const& rhs) noexcept { return lhs.m_id_ >= rhs.m_id_; }

  friend std::strong_ordering operator<=>(UUID const& lhs, UUID const& rhs) noexcept
  {
    if (lhs.m_id_ < rhs.m_id_)
      return std::strong_ordering::less;
    if (lhs.m_id_ > rhs.m_id_)
      return std::strong_ordering::greater;
    return std::strong_ordering::equal;
  }

  friend void swap(UUID& lhs, UUID& rhs) noexcept { lhs.m_id_.swap(rhs.m_id_); }

  friend std::size_t hash_value(UUID const& u) noexcept
  {
    std::size_t hash = static_cast<std::size_t>(1469598103934665603ull);
    for (std::uint8_t byte : u.m_id_) {
      hash ^= static_cast<std::size_t>(byte);
      hash *= static_cast<std::size_t>(1099511628211ull);
    }
    return hash;
  }

private:
  static storage_type Generate()
  {
    storage_type id{};
    static thread_local std::mt19937_64 generator{static_cast<std::mt19937_64::result_type>(std::random_device{}())};
    std::uniform_int_distribution<unsigned int> distribution(0, 255);

    for (std::uint8_t& byte : id) {
      byte = static_cast<std::uint8_t>(distribution(generator));
    }

    // Mark generated IDs as RFC 4122-style version 4 UUIDs.
    id[6] = static_cast<std::uint8_t>((id[6] & 0x0fU) | 0x40U);
    id[8] = static_cast<std::uint8_t>((id[8] & 0x3fU) | 0x80U);
    return id;
  }

  storage_type m_id_;

#ifdef GE_DEBUG
  std::uint64_t m_debug_value_                 = 0;
  inline static std::uint64_t m_debug_counter_ = 0;
#endif
};

} // namespace psych
