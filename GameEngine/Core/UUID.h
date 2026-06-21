#pragma once

#include <boost/uuid.hpp>

namespace ge
{
class UUID
{
public:
  UUID() : m_id_(rg_())
  {
#ifdef GE_DEBUG
    m_debug_value_ = m_debug_counter_++;
#endif
  }

  static UUID Nill()
  {
    UUID id;
    id.m_id_ = {};
    return id;
  }

  ~UUID()               = default;

  using value_type      = std::uint8_t;

  using reference       = std::uint8_t&;
  using const_reference = std::uint8_t const&;

  using iterator        = std::uint8_t*;
  using const_iterator  = std::uint8_t const*;

  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;

  iterator begin() noexcept { return m_id_.begin(); }

  iterator end() noexcept { return m_id_.end(); }

  const_iterator begin() const noexcept { return m_id_.begin(); }

  const_iterator end() const noexcept { return m_id_.end(); }

  // data

  std::uint8_t* data() noexcept { return m_id_.data; }

  std::uint8_t const* data() const noexcept { return m_id_.data; }

  // size

  constexpr size_type size() const noexcept { return m_id_.size(); }

  static constexpr size_type static_size() noexcept
  {
    return boost::uuids::uuid::static_size();
  }

  // is_nil

  bool is_nil() const noexcept { return m_id_.is_nil(); }

  [[nodiscard]] const boost::uuids::uuid& Value() const noexcept
  {
    return m_id_;
  }

  [[nodiscard]] boost::uuids::uuid& Value() noexcept { return m_id_; }

  friend bool operator==(UUID const& lhs, UUID const& rhs) noexcept
  {
    return lhs.m_id_ == rhs.m_id_;
  }

  friend bool operator!=(UUID const& lhs, UUID const& rhs) noexcept
  {
    return lhs.m_id_ != rhs.m_id_;
  }

  friend bool operator<(UUID const& lhs, UUID const& rhs) noexcept
  {
    return lhs.m_id_ < rhs.m_id_;
  }

  friend bool operator>(UUID const& lhs, UUID const& rhs) noexcept
  {
    return lhs.m_id_ > rhs.m_id_;
  }

  friend bool operator<=(UUID const& lhs, UUID const& rhs) noexcept
  {
    return lhs.m_id_ <= rhs.m_id_;
  }

  friend bool operator>=(UUID const& lhs, UUID const& rhs) noexcept
  {
    return lhs.m_id_ >= rhs.m_id_;
  }

  friend std::strong_ordering operator<=>(UUID const& lhs,
                                          UUID const& rhs) noexcept
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
    return boost::uuids::hash_value(u.m_id_);
  }

private:
  boost::uuids::uuid m_id_;
  inline static boost::uuids::random_generator rg_{};

#ifdef GE_DEBUG
  std::uint64_t m_debug_value_                 = 0;
  inline static std::uint64_t m_debug_counter_ = 0;
#endif
};

} // namespace ge
