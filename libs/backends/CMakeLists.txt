set(LIB_NAME ${BACKENDS_LIB_NAME})

add_library(${LIB_NAME} STATIC)

target_include_directories(${LIB_NAME}
                           PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
                           
if(ENABLE_CUDA)
    set(BACKENDS_SOURCES src/gpu/mc_backend_cuda.cu)
    target_link_libraries(${LIB_NAME} PRIVATE CUDA::cudart CUDA::curand)
else()
    set(BACKENDS_SOURCES src/gpu/stub.cpp)
endif()

target_sources(${LIB_NAME} PRIVATE ${BACKENDS_SOURCES})
target_link_libraries(${LIB_NAME} PUBLIC ${MC_LIB_NAME})

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
