set(LIB_NAME ${CUDA_LIB_NAME})

add_library(${LIB_NAME} INTERFACE)

target_include_directories(${LIB_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

if(ENABLE_CUDA)
    target_compile_definitions(${LIB_NAME} INTERFACE CUDA_ENABLED=1)
endif()

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
