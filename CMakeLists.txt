cmake_minimum_required(VERSION 3.20)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

project(GameEngine VERSION 0.1.0 LANGUAGES C CXX)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    Debug Release RelWithDebInfo MinSizeRel
  )
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out")

option(GAME_ENGINE_BUILD_TESTS "Build GameEngine unit tests" OFF)
option(GAME_ENGINE_BUILD_EDITOR "Build the editor target" OFF)
option(GE_ENABLE_ASSERTS "Enable GameEngine runtime assertions" ON)
option(GE_ENABLE_PROFILING "Enable GameEngine profiling instrumentation" ON)
option(GE_ENABLE_SANITIZERS "Enable ASan/UBSan for GameEngine targets" OFF)
option(GAME_ENGINE_ENABLE_CLANG_TIDY "Run clang-tidy on GameEngine and Editor sources" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  add_compile_options(-fdiagnostics-color=always)
endif()

# Backwards compatibility for existing local commands that used -DBUILD_EDITOR=ON.
if(DEFINED BUILD_EDITOR)
  set(GAME_ENGINE_BUILD_EDITOR "${BUILD_EDITOR}" CACHE BOOL "Build the editor target" FORCE)
endif()

message(STATUS "GameEngine build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "GameEngine editor: ${GAME_ENGINE_BUILD_EDITOR}")
message(STATUS "GameEngine tests: ${GAME_ENGINE_BUILD_TESTS}")
message(STATUS "GameEngine asserts: ${GE_ENABLE_ASSERTS}")
message(STATUS "GameEngine profiling: ${GE_ENABLE_PROFILING}")
message(STATUS "GameEngine sanitizers: ${GE_ENABLE_SANITIZERS}")
message(STATUS "GameEngine clang-tidy: ${GAME_ENGINE_ENABLE_CLANG_TIDY}")

if(GAME_ENGINE_ENABLE_CLANG_TIDY)
  find_program(GAME_ENGINE_CLANG_TIDY_EXE NAMES clang-tidy REQUIRED)
  set(GAME_ENGINE_CLANG_TIDY_COMMAND "${GAME_ENGINE_CLANG_TIDY_EXE}")
endif()

function(game_engine_enable_clang_tidy target_name)
  if(GAME_ENGINE_ENABLE_CLANG_TIDY)
    set_target_properties("${target_name}" PROPERTIES
      CXX_CLANG_TIDY "${GAME_ENGINE_CLANG_TIDY_COMMAND}"
    )
  endif()
endfunction()

add_subdirectory(GameEngine)
if(GAME_ENGINE_BUILD_EDITOR)
  add_subdirectory(Editor)
else()
  add_subdirectory(App)
endif()

if(GAME_ENGINE_BUILD_TESTS)
  set(BUILD_TESTING ON CACHE BOOL "Build tests" FORCE)
  include(CTest)
  add_subdirectory(tests)
endif()
