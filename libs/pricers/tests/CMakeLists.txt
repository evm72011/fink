find_package(GTest CONFIG REQUIRED)

set(TEST_NAME ${LIB_NAME}_test)

add_executable(
    ${TEST_NAME} test_black_scholes_call.cpp test_black_scholes_put.cpp
                 test_european_mc_native.cpp)

target_link_libraries(${TEST_NAME} PRIVATE ${LIB_NAME} GTest::gtest
                                           GTest::gtest_main)

add_sanitizer_flags(TARGET ${TEST_NAME})

include(GoogleTest)

gtest_discover_tests(${TEST_NAME} EXTRA_ARGS --gtest_color=yes)
