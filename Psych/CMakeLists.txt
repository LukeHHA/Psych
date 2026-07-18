set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Logging/Logging.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/PsychEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Fonts/FontManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Layers/Layer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Layers/LayerStack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Config/PsychEngineConfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Project/Project.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Project/ProjectManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/RendererAPI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/RendererContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/RendererContextHeadless.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Chunk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/VertexArray.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/VertexFormat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Framebuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Platform/Opengl/OpenglContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Platform/Opengl/OpenglShader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Platform/Opengl/OpenglRendererAPI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Platform/Opengl/OpenglBuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Platform/Opengl/OpenglVertexArray.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Platform/Opengl/OpenglFramebuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Platform/Vulkan/VulkanContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Platform/Metal/MetalContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Window/EngineWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Window/HeadlessWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Events/EventHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Events/EventQueue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FileSystem/FileSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FileSystem/PathResolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FileSystem/CoreFilesystemAPI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Imgui/ImguiLayer.cpp
)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(PSYCH_ENGINE_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(PSYCH_ENGINE_EMBEDDED_HEADER "${PSYCH_ENGINE_GENERATED_DIR}/EmbeddedResources.h")
set(PSYCH_ENGINE_EMBEDDED_SOURCE "${PSYCH_ENGINE_GENERATED_DIR}/EmbeddedResources.cpp")
set(PSYCH_ENGINE_EMBEDDED_RESOURCE_FILES
    "${PROJECT_SOURCE_DIR}/Resources/Fonts/JetBrainsMonoNerdFont-Regular.ttf"
)
set(PSYCH_ENGINE_EMBEDDED_RESOURCES
    "${PROJECT_SOURCE_DIR}/Resources/Fonts/JetBrainsMonoNerdFont-Regular.ttf=JetBrainsMonoNerdFontRegular"
)

add_custom_command(
  OUTPUT
    "${PSYCH_ENGINE_EMBEDDED_HEADER}"
    "${PSYCH_ENGINE_EMBEDDED_SOURCE}"
  COMMAND "${Python3_EXECUTABLE}"
    "${PROJECT_SOURCE_DIR}/tools/embed_resources.py"
    --namespace psych::embedded
    --output-header "${PSYCH_ENGINE_EMBEDDED_HEADER}"
    --output-source "${PSYCH_ENGINE_EMBEDDED_SOURCE}"
    ${PSYCH_ENGINE_EMBEDDED_RESOURCES}
  DEPENDS
    "${PROJECT_SOURCE_DIR}/tools/embed_resources.py"
    ${PSYCH_ENGINE_EMBEDDED_RESOURCE_FILES}
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

add_library(PsychEngine STATIC)
target_sources(PsychEngine PRIVATE ${SOURCES} "${PSYCH_ENGINE_EMBEDDED_SOURCE}")
target_compile_features(PsychEngine PUBLIC cxx_std_20)
set_target_properties(PsychEngine PROPERTIES CXX_EXTENSIONS OFF)

if(APPLE)
  enable_language(OBJCXX)
  enable_language(OBJC)

  target_sources(PsychEngine PRIVATE
        src/FileSystem/Platform/MacOSFilesystemAPI.cpp
        src/FileSystem/Platform/MacOSFilesystem.mm
    )

  target_link_libraries(PsychEngine PUBLIC
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework OpenGL"
        "-framework Foundation"
    )
elseif(UNIX AND NOT APPLE)
  # find_package(OpenGL REQUIRED)
  find_package(Threads REQUIRED)

  target_sources(PsychEngine PRIVATE
        src/FileSystem/Platform/LinuxFilesystemAPI.cpp
    )

  # target_link_libraries(PsychEngine PUBLIC OpenGL::GL Threads::Threads)
  target_link_libraries(PsychEngine PUBLIC Threads::Threads)
endif()

if(
  (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR
  (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
)
  if(PSYCH_ENABLE_SANITIZERS)
    set(SAN_FLAGS -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_compile_options(PsychEngine PRIVATE ${SAN_FLAGS})
    target_link_options(PsychEngine PUBLIC ${SAN_FLAGS})
  endif()
  target_compile_options(
    PsychEngine PRIVATE -g3 -Wall -Wextra
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

add_library(PsychEngineImgui OBJECT)
target_sources(PsychEngineImgui PRIVATE ${IMGUI_SOURCES})
target_compile_features(PsychEngineImgui PRIVATE cxx_std_20)
set_target_properties(PsychEngineImgui PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(PsychEngineImgui PRIVATE
  glad
  glfw
)

target_include_directories(PsychEngineImgui SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui
)

target_compile_definitions(PsychEngineImgui PRIVATE GL_SILENCE_DEPRECATION)

find_package(Boost CONFIG REQUIRED)

target_link_libraries(PsychEngine PUBLIC
  spdlog::spdlog
  glad
  kiznit::expected
  pugixml::pugixml
  assimp
  glfw
  Boost::headers
  # cereal
)

target_link_libraries(PsychEngine PRIVATE PsychEngineImgui)

target_include_directories(PsychEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    ${PSYCH_ENGINE_GENERATED_DIR}
)

target_include_directories(PsychEngine SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/cereal/include
)

if(PSYCH_ENGINE_BUILD_TESTS)
  target_compile_definitions(PsychEngine PRIVATE PSYCH_TESTS_ENABLED)
endif()

target_compile_definitions(PsychEngine
  PUBLIC
    $<$<BOOL:${PSYCH_ENABLE_ASSERTS}>:PSYCH_ENABLE_ASSERTS>
    $<$<BOOL:${PSYCH_ENABLE_PROFILING}>:PSYCH_PROFILE>
  PRIVATE
    GL_SILENCE_DEPRECATION
)

target_link_libraries(PsychEngine PUBLIC glm::glm)

psych_engine_enable_clang_tidy(PsychEngine)
