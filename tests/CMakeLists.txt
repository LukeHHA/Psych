include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(UnitTests
    TestMain.cpp
)

target_link_libraries(UnitTests PRIVATE
    GameEngine
    GTest::gtest_main
)

target_compile_definitions(UnitTests PRIVATE GE_TESTS_ENABLED)

target_compile_options(UnitTests PRIVATE -fsanitize=address -fno-omit-frame-pointer)
target_link_options(UnitTests PRIVATE -fsanitize=address)

include(GoogleTest)
gtest_discover_tests(UnitTests)