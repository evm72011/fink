function(add_cmake_format_target)
    if(NOT ENABLE_CMAKE_FORMAT)
        return()
    endif()

    if(NOT VENV_DIR)
        message(WARNING "VENV_DIR is not set, cmake-format disabled")
        return()
    endif()

    if(WIN32)
        set(CMAKE_FORMAT_EXE "${VENV_DIR}/Scripts/cmake-format.exe")
    else()
        set(CMAKE_FORMAT_EXE "${VENV_DIR}/bin/cmake-format")
    endif()

    add_custom_target(
        run_cmake_format
        COMMAND
            ${CMAKE_COMMAND} -E env "VENV_DIR=${VENV_DIR}"
            "${CMAKE_SOURCE_DIR}/tools/run-cmake-format.sh"
            "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/.cmake-format.yaml"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        USES_TERMINAL)

    message(STATUS "Added cmake-format target: run_cmake_format")
endfunction()
