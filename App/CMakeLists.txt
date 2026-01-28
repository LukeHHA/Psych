set(SOURCES
    AppLayer.cpp
    App.cpp
)

add_executable(App)

target_sources(App PRIVATE ${SOURCES})

target_link_libraries(App GameEngine)

target_include_directories(App PRIVATE App)

set_target_properties(App PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out"
)
