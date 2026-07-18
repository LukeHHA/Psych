include(FetchContent)

find_package(GTest CONFIG QUIET)
if(GTest_FOUND)
    if(TARGET GTest::gtest)
        set(PSYCH_ENGINE_GTEST_TARGET GTest::gtest)
    else()
        set(PSYCH_ENGINE_GTEST_TARGET GTest::GTest)
    endif()
elseif(EXISTS "${PROJECT_SOURCE_DIR}/PsychEngine/Vendor/assimp/contrib/googletest/CMakeLists.txt")
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    add_subdirectory(
        "${PROJECT_SOURCE_DIR}/PsychEngine/Vendor/assimp/contrib/googletest"
        "${CMAKE_BINARY_DIR}/_deps/PSYCH_ENGINE_googletest-build"
        EXCLUDE_FROM_ALL
    )
    set(PSYCH_ENGINE_GTEST_TARGET gtest)
else()
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    )
    FetchContent_MakeAvailable(googletest)
    set(PSYCH_ENGINE_GTEST_TARGET GTest::gtest)
endif()

add_executable(PsychEngineTests
    main.cpp
    core/UUIDTests.cpp
    events/EventTests.cpp
    events/EventQueueTests.cpp
    filesystem/FilesystemTests.cpp
    layers/LayerStackTests.cpp
    util/CommandLineTests.cpp
)

target_link_libraries(PsychEngineTests PRIVATE
    PsychEngine
    ${PSYCH_ENGINE_GTEST_TARGET}
)

target_compile_definitions(PsychEngineTests PRIVATE PSYCH_TESTS_ENABLED)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/runtime")

include(GoogleTest)
gtest_discover_tests(PsychEngineTests
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/runtime"
)
