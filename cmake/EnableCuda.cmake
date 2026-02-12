include_guard(GLOBAL)

if(ENABLE_CUDA)
    find_package(CUDAToolkit REQUIRED)
    enable_language(CUDA)
    message(STATUS "CUDA enabled: ${CUDAToolkit_VERSION}")
else()
    message(STATUS "CUDA disabled")
endif()
