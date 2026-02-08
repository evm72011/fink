set(LIB_NAME ${MC_LIB_NAME})

add_library(${LIB_NAME} INTERFACE)

target_include_directories(${LIB_NAME}
                           INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

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

if(ENABLE_TESTING)
   add_subdirectory(tests)
endif()
