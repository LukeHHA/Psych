set(SOURCES
    src/EditorApp.cpp
    src/EditorLayer.cpp
    src/ProjectWindow.cpp
    src/UI/Modules/EditorMenuBar.cpp
    src/UI/Modules/MainFileTree.cpp
    src/UI/Modules/FileViewer.cpp
)

add_executable(Editor)

target_sources(Editor PRIVATE ${SOURCES})

target_link_libraries(Editor PRIVATE PsychEngine)

target_include_directories(Editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

psych_engine_enable_clang_tidy(Editor)
