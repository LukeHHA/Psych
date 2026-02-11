add_library(glad STATIC src/glad.c)

target_include_directories(glad PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optional: silence vendor warnings
if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(glad PRIVATE -w)
endif()
