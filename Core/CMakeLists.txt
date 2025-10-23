set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Logging/Logging.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Layers/LayerStack.cpp
)

add_library(Core STATIC)

if (MSVC)
  target_compile_options(Core PRIVATE
    /W4
    $<$<BOOL:${WARNINGS_AS_ERRORS}>:/WX>
  )
else() # GCC/Clang
  target_compile_options(Core PRIVATE
    -Wall -Wextra -Wpedantic
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic -fdiagnostics-color=always>
  )
endif()

target_sources(Core PRIVATE ${SOURCES})

target_include_directories(Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_subdirectory(Vendor/spdlog)
target_link_libraries(Core PUBLIC spdlog)

