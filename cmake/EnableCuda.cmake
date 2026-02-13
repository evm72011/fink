include_guard(GLOBAL)

if(ENABLE_CUDA)
    set(CMAKE_CUDA_STANDARD 17)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
    set(CMAKE_CUDA_EXTENSIONS OFF)

    find_package(CUDAToolkit REQUIRED)
    enable_language(CUDA)

    message(STATUS "CUDA enabled: ${CUDAToolkit_VERSION}")
else()
    message(STATUS "CUDA disabled")
endif()
