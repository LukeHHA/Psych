#pragma once

// clang-format off
#ifdef __linux__
#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#elif defined(__APPLE__)
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#endif
// clang-format on
