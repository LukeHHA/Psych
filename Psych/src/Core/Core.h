 
/**************************************************************************/
/*  Core.h                                                                */                                                            
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
