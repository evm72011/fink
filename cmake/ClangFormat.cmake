function(add_clang_format_target)
    if(NOT ENABLE_CLANG_FORMAT)
        return()
    endif()

    find_package(
        Python3
        COMPONENTS Interpreter
        REQUIRED)

    find_program(CLANGFORMAT_EXE NAMES clang-format)
    if(NOT CLANGFORMAT_EXE)
        message(WARNING "clang-format not found")
        return()
    endif()

    set(CLANG_FORMAT_ROOTS apps libs)
    set(CLANG_FORMAT_EXTS
        cc
        cpp
        h
        hpp)

    set(CPP_FILES "")
    foreach(root IN LISTS CLANG_FORMAT_ROOTS)
        foreach(ext IN LISTS CLANG_FORMAT_EXTS)
            file(
                GLOB_RECURSE
                _tmp
                CONFIGURE_DEPENDS
                "${CMAKE_SOURCE_DIR}/${root}/*.${ext}")
            list(APPEND CPP_FILES ${_tmp})
        endforeach()
    endforeach()

    list(REMOVE_DUPLICATES CPP_FILES)

    if(CPP_FILES)
        add_custom_target(
            run_clang_format
            COMMAND
                ${Python3_EXECUTABLE}
                ${CMAKE_SOURCE_DIR}/tools/run-clang-format.py --in-place
                ${CPP_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            USES_TERMINAL)
        message(STATUS "Added Clang Format target: run_clang_format")
    else()
        message(STATUS "No C/C++ files found for clang-format")
    endif()
endfunction()
