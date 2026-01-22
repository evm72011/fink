function(add_clang_tidy)
    set(oneValueArgs TARGET ENABLE)
    cmake_parse_arguments(
        ACLT
        ""
        "${oneValueArgs}"
        ""
        ${ARGN})

    if(NOT TARGET ${ACLT_TARGET})
        message(
            FATAL_ERROR
                "add_clang_tidy: Target '${ACLT_TARGET}' does not exist.")
        return()
    endif()

    if(NOT ACLT_ENABLE)
        message(STATUS "Clang-Tidy deactivated for ${ACLT_TARGET}")
        return()
    endif()

    get_target_property(TARGET_SOURCES ${ACLT_TARGET} SOURCES)
    list(
        FILTER
        TARGET_SOURCES
        INCLUDE
        REGEX
        ".*\\.(cc|h|cpp|hpp)$")

    find_package(
        Python3
        COMPONENTS Interpreter
        REQUIRED)

    find_program(CLANGTIDY clang-tidy)
    if(NOT CLANGTIDY)
        message(WARNING "CLANGTIDY NOT FOUND")
        return()
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        message(STATUS "Added MSVC ClangTidy (VS GUI only) for: ${ACLT_TARGET}")
        set_target_properties(
            ${ACLT_TARGET} PROPERTIES VS_GLOBAL_EnableMicrosoftCodeAnalysis
                                      false)

        set_target_properties(
            ${ACLT_TARGET} PROPERTIES VS_GLOBAL_EnableClangTidyCodeAnalysis
                                      true)
    else()
        message(STATUS "Added Clang Tidy for Target: ${ACLT_TARGET}")
        add_custom_target(
            ${ACLT_TARGET}_clangtidy
            COMMAND
                ${Python3_EXECUTABLE}
                ${CMAKE_SOURCE_DIR}/tools/run-clang-tidy.py ${TARGET_SOURCES} -p
                ${CMAKE_BINARY_DIR} -config-file ${CMAKE_SOURCE_DIR}/.clang-tidy
                -extra-arg-before -Wno-unknown-warning-option -extra-arg-before
                -Wno-error -extra-arg-before -std=c++${CMAKE_CXX_STANDARD}
                -quiet
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            USES_TERMINAL)
    endif()
endfunction()
