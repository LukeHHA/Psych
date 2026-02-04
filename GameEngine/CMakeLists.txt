set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Logging/Logging.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/GameEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Layers/LayerStack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glad/src/glad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/RendererContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Platform/Opengl/OpenglContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Platform/Vulkan/VulkanContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Platform/Metal/MetalContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window/EngineWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window/HeadlessWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Events/EventHandler.cpp
)

add_library(GameEngine STATIC)

set(GLFW_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glfw")
set(GLFW_INCLUDE_DIR "${GLFW_ROOT}/include")
set(GLFW_LIB_DIR "${GLFW_ROOT}/lib-arm64")

# Choose ONE:
set(GLFW_LIB_STATIC "${GLFW_LIB_DIR}/libglfw3.a")
set(GLFW_LIB_SHARED "${GLFW_LIB_DIR}/libglfw.3.dylib")

option(USE_GLFW_SHARED "Link GLFW as a shared library (.dylib)" OFF)

if (USE_GLFW_SHARED)
    add_library(glfw3 SHARED IMPORTED GLOBAL)
    set_target_properties(glfw3 PROPERTIES
        IMPORTED_LOCATION "${GLFW_LIB_SHARED}"
        INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIR}"
    )
else()
    add_library(glfw3 STATIC IMPORTED GLOBAL)
    set_target_properties(glfw3 PROPERTIES
        IMPORTED_LOCATION "${GLFW_LIB_STATIC}"
        INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIR}"
    )
endif()

target_link_libraries(GameEngine PUBLIC glfw3)

# macOS frameworks GLFW needs
target_link_libraries(GameEngine PUBLIC
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
    "-framework OpenGL"
)

if (MSVC)
  target_compile_options(GameEngine PRIVATE
    /W4
    $<$<BOOL:${WARNINGS_AS_ERRORS}>:/WX>
  )
else() # GCC/Clang
  target_compile_options(GameEngine PRIVATE
    -Wall -Wextra -Wpedantic 
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic -fdiagnostics-color=always >
  )
endif()

target_sources(GameEngine PRIVATE ${SOURCES})

target_include_directories(GameEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/spdlog
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/kz/src
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glad/include
)

if (BUILD_TESTING)
    target_compile_definitions(GameEngine PRIVATE GE_TESTS_ENABLED)
endif()

add_subdirectory(Vendor/spdlog)
add_subdirectory(Vendor/kz)

target_link_libraries(GameEngine PUBLIC spdlog)

