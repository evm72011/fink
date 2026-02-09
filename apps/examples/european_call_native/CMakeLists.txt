set(EXE_NAME example-european-call-native)
set(EXE_SOURCES "src/main.cpp")

add_executable(${EXE_NAME} ${EXE_SOURCES})

target_link_libraries(${EXE_NAME} PRIVATE ${ALL_LIBS})

target_set_warnings(
    TARGET
    ${EXE_NAME}
    ENABLE
    ${ENABLE_WARNINGS}
    AS_ERRORS
    ${ENABLE_WARNINGS_AS_ERRORS})

target_enable_lto(
    TARGET
    ${EXE_NAME}
    ENABLE
    ${ENABLE_LTO})

add_sanitizer_flags(TARGET ${EXE_NAME})

add_clang_tidy(
    TARGET
    ${EXE_NAME}
    ENABLE
    ${ENABLE_CLANG_TIDY})
