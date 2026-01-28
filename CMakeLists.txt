cmake_minimum_required(VERSION 3.10)
project(Template VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(
    GE_ENABLE_ASSERTS
    GE_PROFILE
)

add_subdirectory(GameEngine)
add_subdirectory(App)
