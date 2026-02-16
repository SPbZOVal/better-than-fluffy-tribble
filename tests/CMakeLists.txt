cmake_minimum_required(VERSION 3.5)

# Find GoogleTest
find_package(GTest REQUIRED)

# Create test executable
add_executable(btft_tests)

target_sources(btft_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_mock_channels.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_channel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_commands.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_registry.cpp"
)

target_include_directories(btft_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(btft_tests PRIVATE
    btft_obj
    GTest::gtest
    GTest::gtest_main
    pthread
)

# Add test
include(GoogleTest)
gtest_discover_tests(btft_tests)
