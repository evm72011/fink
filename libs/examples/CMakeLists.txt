set(LIB_NAME ${EXAMPLES_LIB_NAME})

add_library(${LIB_NAME} STATIC src/utils.cpp)

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(${LIB_NAME} PRIVATE ${MODELS_LIB_NAME} ${INSTRUMENTS_LIB_NAME})

target_set_warnings(
    TARGET
    ${LIB_NAME}
    ENABLE
    ${ENABLE_WARNINGS}
    AS_ERRORS
    ${ENABLE_WARNINGS_AS_ERRORS})

add_sanitizer_flags(TARGET ${LIB_NAME})

target_enable_lto(
    TARGET
    ${LIB_NAME}
    ENABLE
    ${ENABLE_LTO})

add_clang_tidy(
    TARGET
    ${LIB_NAME}
    ENABLE
    ${ENABLE_CLANG_TIDY})
