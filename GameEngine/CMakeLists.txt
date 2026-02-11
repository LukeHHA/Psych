set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Logging/Logging.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/GameEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Layers/LayerStack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/RendererContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Platform/Opengl/OpenglContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Platform/Vulkan/VulkanContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Platform/Metal/MetalContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window/EngineWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window/HeadlessWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Events/EventHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Events/EventQueue.cpp
)

add_library(GameEngine STATIC)

set(GLFW_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/glfw")
set(GLFW_INCLUDE_DIR "${GLFW_ROOT}/include")
set(GLFW_LIB_DIR "${GLFW_ROOT}/lib-arm64")

# Choose ONE:
set(GLFW_LIB_STATIC "${GLFW_LIB_DIR}/libglfw3.a")
set(GLFW_LIB_SHARED "${GLFW_LIB_DIR}/libglfw.3.dylib")

option(USE_GLFW_SHARED "Link GLFW as a shared library (.dylib)" OFF)

if(USE_GLFW_SHARED)
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

option(GE_ENABLE_SANITIZERS "Enable ASan/UBSan" ON)

if(
  (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR
  (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
)
  if(GE_ENABLE_SANITIZERS)
    set(SAN_FLAGS -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_compile_options(GameEngine PRIVATE ${SAN_FLAGS})
    target_link_options(GameEngine PUBLIC ${SAN_FLAGS})
  endif()
  target_compile_options(
    GameEngine PRIVATE -g3 -Wall -Wextra
    -Wconversion -Wdouble-promotion
    -Wno-unused-parameter -Wno-sign-conversion
  )
endif()

target_sources(GameEngine PRIVATE ${SOURCES})

add_subdirectory(Vendor/spdlog)
add_subdirectory(Vendor/kz)
add_subdirectory(Vendor/glad)

target_link_libraries(GameEngine PUBLIC
  spdlog::spdlog
  glad
  kiznit::expected
)

target_include_directories(GameEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/kz/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(BUILD_TESTING)
  target_compile_definitions(GameEngine PRIVATE GE_TESTS_ENABLED)
endif()

if(GE_ENABLE_ASSERTS)
  message(STATUS "Asserts are ENABLED")
else()
  message(STATUS "Asserts are DISABLED")
endif()

