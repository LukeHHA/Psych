set(SOURCES
    EditorApp.cpp
    EditorLayer.cpp
    UI/Modules/EditorMenuBar.cpp
    UI/Modules/MainFileTree.cpp
)

add_executable(Editor)

target_sources(Editor PRIVATE ${SOURCES})

target_link_libraries(Editor GameEngine)

target_include_directories(Editor PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/)

set_target_properties(Editor PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out"
)
