cmake_minimum_required(VERSION 3.20)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

project(PsychEngine VERSION 0.1.0 LANGUAGES C CXX)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    Debug Release RelWithDebInfo MinSizeRel
  )
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out")

option(PSYCH_ENGINE_BUILD_TESTS "Build PsychEngine unit tests" OFF)
option(PSYCH_ENGINE_BUILD_EDITOR "Build the editor target" OFF)
option(PSYCH_ENABLE_ASSERTS "Enable PsychEngine runtime assertions" ON)
option(PSYCH_ENABLE_PROFILING "Enable PsychEngine profiling instrumentation" ON)
option(PSYCH_ENABLE_SANITIZERS "Enable ASan/UBSan for PsychEngine targets" OFF)
option(PSYCH_ENGINE_ENABLE_CLANG_TIDY "Run clang-tidy on PsychEngine and Editor sources" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  add_compile_options(-fdiagnostics-color=always)
endif()

# Backwards compatibility for existing local commands that used -DBUILD_EDITOR=ON.
if(DEFINED BUILD_EDITOR)
  set(PSYCH_ENGINE_BUILD_EDITOR "${BUILD_EDITOR}" CACHE BOOL "Build the editor target" FORCE)
endif()

message(STATUS "PsychEngine build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "PsychEngine editor: ${PSYCH_ENGINE_BUILD_EDITOR}")
message(STATUS "PsychEngine tests: ${PSYCH_ENGINE_BUILD_TESTS}")
message(STATUS "PsychEngine asserts: ${PSYCH_ENABLE_ASSERTS}")
message(STATUS "PsychEngine profiling: ${PSYCH_ENABLE_PROFILING}")
message(STATUS "PsychEngine sanitizers: ${PSYCH_ENABLE_SANITIZERS}")
message(STATUS "PsychEngine clang-tidy: ${PSYCH_ENGINE_ENABLE_CLANG_TIDY}")

if(PSYCH_ENGINE_ENABLE_CLANG_TIDY)
  find_program(PSYCH_ENGINE_CLANG_TIDY_EXE NAMES clang-tidy REQUIRED)
  set(PSYCH_ENGINE_CLANG_TIDY_COMMAND "${PSYCH_ENGINE_CLANG_TIDY_EXE}")
endif()

function(PSYCH_ENGINE_enable_clang_tidy target_name)
  if(PSYCH_ENGINE_ENABLE_CLANG_TIDY)
    set_target_properties("${target_name}" PROPERTIES
      CXX_CLANG_TIDY "${PSYCH_ENGINE_CLANG_TIDY_COMMAND}"
    )
  endif()
endfunction()

add_subdirectory(Psych)
if(PSYCH_ENGINE_BUILD_EDITOR)
  add_subdirectory(Editor)
else()
  add_subdirectory(App)
endif()

if(PSYCH_ENGINE_BUILD_TESTS)
  set(BUILD_TESTING ON CACHE BOOL "Build tests" FORCE)
  include(CTest)
  add_subdirectory(tests)
endif()
