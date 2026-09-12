set(SOURCES
    src/EditorApp.cpp
    src/EditorLayer.cpp
    src/UI/Modules/EditorMenuBar.cpp
    src/UI/Modules/MainFileTree.cpp
    src/UI/Modules/FileViewer.cpp
    src/UI/Modules/ProjectWindow.cpp
    src/UI/Modules/Viewport.cpp
)

add_executable(Editor)

if(APPLE AND PSYCH_ENGINE_BUILD_MACOS_BUNDLE)
  set_target_properties(Editor PROPERTIES MACOSX_BUNDLE TRUE)
endif()

target_sources(Editor PRIVATE ${SOURCES})

target_link_libraries(Editor PRIVATE PsychEngine)

target_include_directories(Editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

psych_engine_enable_clang_tidy(Editor)
