#pragma once

#include <memory>
#include <utility>

namespace psych
{
// MACROS
//
inline constexpr const char* PsychEngineName = "PsychEngine";

#if defined(__APPLE__)
#include <TargetConditionals.h>

#if TARGET_OS_MAC && !TARGET_OS_IPHONE
#define GE_PLATFORM_MACOS
#elif TARGET_OS_IPHONE
#define GE_PLATFORM_IOS
#endif
#elif defined(__linux__)
#define GE_PLATFORM_LINUX
#endif

// Pointer Utils
template <typename T, typename Deleter = std::default_delete<T>>
using Unique = std::unique_ptr<T, Deleter>;
template <typename T, typename... Args>
constexpr Unique<T> CreateUnique(Args&&... args)
{
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Shared = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Shared<T> CreateShared(Args&&... args)
{
  return std::make_shared<T>(std::forward<Args>(args)...);
}

#define CORE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// Constructor Macros
#define CORE_DELETE_COPY_OPS(Type)                                                                                                                                       \
  Type(const Type&)            = delete;                                                                                                                                 \
  Type& operator=(const Type&) = delete

#define CORE_DELETE_MOVE_OPS(Type)                                                                                                                                       \
  Type(Type&&) noexcept            = default;                                                                                                                            \
  Type& operator=(Type&&) noexcept = default

#define CORE_MOVE_ONLY(Type)                                                                                                                                             \
  Type(const Type&)                = delete;                                                                                                                             \
  Type& operator=(const Type&)     = delete;                                                                                                                             \
  Type(Type&&) noexcept            = default;                                                                                                                            \
  Type& operator=(Type&&) noexcept = default

#define CORE_NO_COPY_NO_MOVE(Type)                                                                                                                                       \
  Type(const Type&)            = delete;                                                                                                                                 \
  Type& operator=(const Type&) = delete;                                                                                                                                 \
  Type(Type&&)                 = delete;                                                                                                                                 \
  Type& operator=(Type&&)      = delete

} // namespace psych
