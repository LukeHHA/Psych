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
#ifdef GE_ENABLE_ASSERTS
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
