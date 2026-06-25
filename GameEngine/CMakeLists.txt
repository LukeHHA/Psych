set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Logging/Logging.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/GameEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Fonts/FontManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Layers/Layer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Layers/LayerStack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Config/GameEngineConfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/Project.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Project/ProjectManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/RendererAPI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/RendererContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/RendererContextHeadless.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Chunk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/VertexArray.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/VertexFormat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Framebuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Platform/Opengl/OpenglContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Platform/Opengl/OpenglShader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Platform/Opengl/OpenglRendererAPI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Platform/Opengl/OpenglBuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Platform/Opengl/OpenglVertexArray.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Platform/Opengl/OpenglFramebuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Platform/Vulkan/VulkanContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Platform/Metal/MetalContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window/EngineWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window/HeadlessWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Events/EventHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Events/EventQueue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileSystem/EngineFilesystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileSystem/FileSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileSystem/PathResolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileSystem/CoreFilesystemAPI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Imgui/ImguiLayer.cpp
)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(GAME_ENGINE_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(GAME_ENGINE_EMBEDDED_HEADER "${GAME_ENGINE_GENERATED_DIR}/EmbeddedResources.h")
set(GAME_ENGINE_EMBEDDED_SOURCE "${GAME_ENGINE_GENERATED_DIR}/EmbeddedResources.cpp")
set(GAME_ENGINE_EMBEDDED_RESOURCE_FILES
    "${PROJECT_SOURCE_DIR}/Resources/Fonts/JetBrainsMonoNerdFont-Regular.ttf"
)
set(GAME_ENGINE_EMBEDDED_RESOURCES
    "${PROJECT_SOURCE_DIR}/Resources/Fonts/JetBrainsMonoNerdFont-Regular.ttf=JetBrainsMonoNerdFontRegular"
)

add_custom_command(
  OUTPUT
    "${GAME_ENGINE_EMBEDDED_HEADER}"
    "${GAME_ENGINE_EMBEDDED_SOURCE}"
  COMMAND "${Python3_EXECUTABLE}"
    "${PROJECT_SOURCE_DIR}/tools/embed_resources.py"
    --namespace ge::embedded
    --output-header "${GAME_ENGINE_EMBEDDED_HEADER}"
    --output-source "${GAME_ENGINE_EMBEDDED_SOURCE}"
    ${GAME_ENGINE_EMBEDDED_RESOURCES}
  DEPENDS
    "${PROJECT_SOURCE_DIR}/tools/embed_resources.py"
    ${GAME_ENGINE_EMBEDDED_RESOURCE_FILES}
  VERBATIM
)

set(IMGUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui/misc/cpp/imgui_stdlib.cpp
)

add_library(GameEngine STATIC)
target_sources(GameEngine PRIVATE ${SOURCES} "${GAME_ENGINE_EMBEDDED_SOURCE}")
target_compile_features(GameEngine PUBLIC cxx_std_20)
set_target_properties(GameEngine PROPERTIES CXX_EXTENSIONS OFF)

if(APPLE)
  enable_language(OBJCXX)
  enable_language(OBJC)

  target_sources(GameEngine PRIVATE
        FileSystem/Platform/MacOSFilesystemAPI.cpp
        FileSystem/Platform/MacOSFilesystem.mm
    )

  target_link_libraries(GameEngine PUBLIC
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework OpenGL"
        "-framework Foundation"
    )
elseif(UNIX AND NOT APPLE)
  # find_package(OpenGL REQUIRED)
  find_package(Threads REQUIRED)

  target_sources(GameEngine PRIVATE
        FileSystem/Platform/LinuxFilesystemAPI.cpp
    )

  # target_link_libraries(GameEngine PUBLIC OpenGL::GL Threads::Threads)
  target_link_libraries(GameEngine PUBLIC Threads::Threads)
endif()

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
    -Wno-unused-parameter -Wno-sign-conversion -Wno-switch
  )
endif()

set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)
set(SPDLOG_INSTALL OFF CACHE BOOL "" FORCE)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
if(UNIX AND NOT APPLE)
  set(GLFW_BUILD_X11 OFF CACHE BOOL "Build GLFW X11 support")
  set(GLFW_BUILD_WAYLAND ON CACHE BOOL "Build GLFW Wayland support")
endif()

set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(PUGIXML_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(PUGIXML_INSTALL OFF CACHE BOOL "" FORCE)
set(CEREAL_INSTALL OFF CACHE BOOL "" FORCE)
set(CEREAL_BUILD_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(Vendor/spdlog EXCLUDE_FROM_ALL)
add_subdirectory(Vendor/kz EXCLUDE_FROM_ALL)
add_subdirectory(Vendor/glad EXCLUDE_FROM_ALL)
add_subdirectory(Vendor/glfw-3.4 EXCLUDE_FROM_ALL)
add_subdirectory(Vendor/pugixml EXCLUDE_FROM_ALL)
add_subdirectory(Vendor/assimp EXCLUDE_FROM_ALL)
add_subdirectory(Vendor/glm EXCLUDE_FROM_ALL)
# add_subdirectory(Vendor/cereal EXCLUDE_FROM_ALL)

add_library(GameEngineImgui OBJECT)
target_sources(GameEngineImgui PRIVATE ${IMGUI_SOURCES})
target_compile_features(GameEngineImgui PRIVATE cxx_std_20)
set_target_properties(GameEngineImgui PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(GameEngineImgui PRIVATE
  glad
  glfw
)

target_include_directories(GameEngineImgui SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui
)

target_compile_definitions(GameEngineImgui PRIVATE GL_SILENCE_DEPRECATION)

target_link_libraries(GameEngine PUBLIC
  spdlog::spdlog
  glad
  kiznit::expected
  pugixml::pugixml
  assimp
  glfw
  # cereal
)

target_link_libraries(GameEngine PRIVATE GameEngineImgui)

target_include_directories(GameEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GAME_ENGINE_GENERATED_DIR}
)

target_include_directories(GameEngine SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/cereal/include
)

if(GAME_ENGINE_BUILD_TESTS)
  target_compile_definitions(GameEngine PRIVATE GE_TESTS_ENABLED)
endif()

target_compile_definitions(GameEngine
  PUBLIC
    $<$<BOOL:${GE_ENABLE_ASSERTS}>:GE_ENABLE_ASSERTS>
    $<$<BOOL:${GE_ENABLE_PROFILING}>:GE_PROFILE>
  PRIVATE
    GL_SILENCE_DEPRECATION
)

target_link_libraries(GameEngine PUBLIC glm::glm)

game_engine_enable_clang_tidy(GameEngine)
