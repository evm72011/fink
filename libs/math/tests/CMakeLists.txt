find_package(GTest CONFIG REQUIRED)

set(TEST_NAME stats_test)

add_executable(
    ${TEST_NAME}
    discount_test.cpp
    normal_test.cpp
    stats_mean_test.cpp
    stats_variance_test.cpp
    stats_stddev_test.cpp)

target_link_libraries(${TEST_NAME} PRIVATE ${LIB_NAME} GTest::gtest
                                           GTest::gtest_main)

add_sanitizer_flags(TARGET ${TEST_NAME})

include(GoogleTest)

gtest_discover_tests(${TEST_NAME} EXTRA_ARGS --gtest_color=yes)
