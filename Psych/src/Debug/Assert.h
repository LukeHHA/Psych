 
/**************************************************************************/
/*  Assert.h                                                              */                                                            
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

#include "Logging/Logging.h"

#include <cstdio>

#if defined(__APPLE__) || defined(__linux__)

// --- Breakpoint for debugger on MacOS ---
#if defined(__APPLE__) || defined(__linux__)
#include <signal.h>
#define DEBUG_BREAK() raise(SIGTRAP)
#elif defined(__clang__)
#define DEBUG_BREAK() __builtin_debugtrap()
#else
#define DEBUG_BREAK() __builtin_trap()
#endif

#define BIT(x) (1 << x)

// --- Core and App specific ASSERT ---
#ifdef PSYCH_ENABLE_ASSERTS
#define DEBUG_TRACE()                                                          \
  do {                                                                         \
    std::fprintf(stderr,                                                       \
                 "\033[31;1;4mDEBUG TRACE: %s:%d in %s()\033[0m\n",            \
                 __FILE__,                                                     \
                 __LINE__,                                                     \
                 __func__);                                                    \
  } while (0)

#define CORE_ASSERT(x, ...)                                                    \
  {                                                                            \
    if (!(x)) {                                                                \
      CORE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__);                    \
      DEBUG_TRACE();                                                           \
      DEBUG_BREAK();                                                           \
    }                                                                          \
  }

#define APP_ASSERT(x, ...)                                                     \
  {                                                                            \
    if (!(x)) {                                                                \
      APP_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__);                     \
      DEBUG_TRACE();                                                           \
      DEBUG_BREAK();                                                           \
    }                                                                          \
  }
#else
#define CORE_ASSERT(x, ...)                                                    \
  {                                                                            \
    (void)sizeof(x);                                                           \
  }
#define APP_ASSERT(x, ...)                                                     \
  {                                                                            \
    (void)sizeof(x);                                                           \
  }
#endif

#else
#error Core only supports MacOS and Linux
#endif
