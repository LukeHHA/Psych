 
/**************************************************************************/
/*  AssetHandle.h                                                         */                                                            
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

#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <compare>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>

namespace psych
{
class AssetHandle
{
public:
  using uuid_type = boost::uuids::uuid;

  AssetHandle() : m_Id_(Generate()) {}
  explicit AssetHandle(uuid_type id) noexcept : m_Id_(id) {}

  AssetHandle(const AssetHandle&)                = default;
  AssetHandle(AssetHandle&&) noexcept            = default;
  AssetHandle& operator=(const AssetHandle&)     = default;
  AssetHandle& operator=(AssetHandle&&) noexcept = default;
  ~AssetHandle()                                 = default;

  [[nodiscard]] static AssetHandle Nill() noexcept { return AssetHandle{boost::uuids::nil_uuid()}; }
  [[nodiscard]] static AssetHandle FromString(std::string_view value) { return AssetHandle{boost::uuids::string_generator{}(std::string{value})}; }

  [[nodiscard]] bool IsNil() const noexcept
  {
    return std::ranges::all_of(m_Id_.begin(), m_Id_.end(), [](std::uint8_t byte) { return byte == 0; });
  }
  [[nodiscard]] bool IsValid() const noexcept { return !IsNil(); }
  [[nodiscard]] const uuid_type& Value() const noexcept { return m_Id_; }
  [[nodiscard]] std::string ToString() const { return boost::uuids::to_string(m_Id_); }

  friend bool operator==(const AssetHandle& lhs, const AssetHandle& rhs) noexcept { return lhs.m_Id_ == rhs.m_Id_; }

  friend std::strong_ordering operator<=>(const AssetHandle& lhs, const AssetHandle& rhs) noexcept
  {
    if (std::lexicographical_compare(lhs.m_Id_.begin(), lhs.m_Id_.end(), rhs.m_Id_.begin(), rhs.m_Id_.end())) {
      return std::strong_ordering::less;
    }
    if (std::lexicographical_compare(rhs.m_Id_.begin(), rhs.m_Id_.end(), lhs.m_Id_.begin(), lhs.m_Id_.end())) {
      return std::strong_ordering::greater;
    }
    return std::strong_ordering::equal;
  }

  friend std::size_t hash_value(const AssetHandle& handle) noexcept
  {
    auto hash = static_cast<std::size_t>(1469598103934665603ull);
    for (const auto byte : handle.m_Id_) {
      hash ^= static_cast<std::size_t>(byte);
      hash *= static_cast<std::size_t>(1099511628211ull);
    }
    return hash;
  }

private:
  [[nodiscard]] static uuid_type Generate()
  {
    static thread_local boost::uuids::random_generator generator;
    return generator();
  }

private:
  uuid_type m_Id_;
};
} // namespace psych

namespace std
{
template <>
struct hash<psych::AssetHandle> {
  std::size_t operator()(const psych::AssetHandle& handle) const noexcept { return hash_value(handle); }
};
} // namespace std
