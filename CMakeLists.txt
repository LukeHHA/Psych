cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0135 NEW)
project(Template VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(
    GE_ENABLE_ASSERTS
    GE_PROFILE
)

include(CTest)
if (BUILD_TESTING)
  add_subdirectory(tests)
endif()

option(BUILD_EDITOR "Build the editor target" ON)

add_subdirectory(GameEngine)
if (BUILD_EDITOR)
  add_subdirectory(Editor)
else()
  add_subdirectory(App)
endif()
