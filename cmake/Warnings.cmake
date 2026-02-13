function(target_set_warnings)
    set(oneValueArgs TARGET ENABLE AS_ERRORS)
    cmake_parse_arguments(
        TSW
        "${options}"
        "${oneValueArgs}"
        ""
        ${ARGN})

    if(NOT TARGET ${TSW_TARGET})
        message(
            FATAL_ERROR
                "target_set_warnings: Target '${TSW_TARGET}' does not exist.")
        return()
    endif()

    if(NOT TSW_ENABLE)
        message(WARNING "Warnings Disabled for: ${TSW_TARGET}")
        return()
    endif()

    message(
        STATUS
            "Warnings Active for: ${TSW_TARGET}. Warnings as Errors: ${TSW_AS_ERRORS}"
    )

    # Detect whether target is INTERFACE (header-only) or a normal library/exe.
    get_target_property(_tgt_type ${TSW_TARGET} TYPE)
    if(_tgt_type STREQUAL "INTERFACE_LIBRARY")
        set(_scope INTERFACE)
    else()
        set(_scope PRIVATE)
    endif()

    set(MSVC_WARNINGS
        # Baseline
        /W4 # Baseline reasonable warnings
        /permissive- # standards conformance mode for MSVC compiler
        # C and C++ Warnings
        /w14242 # conversion from 'type1' to 'type1', possible loss of data
        /w14254 # 'operator': conversion from 't1:field_bits' to 't2:field_bits'
        /w14287 # unsigned/negative constant mismatch
        /w14296 # expression is always 'boolean_value'
        /w14311 # pointer truncation from 'type1' to 'type2'
        /w44062 # enumerator in a switch of enum 'enumeration' is not handled
        /w44242 # conversion from 'type1' to 'type2', possible loss of data
        /w14826 # Conversion from 'type1' to 'type_2' is sign-extended
        /w14905 # wide string literal cast to 'LPSTR'
        /w14906 # string literal cast to 'LPWSTR'
        # C++ Only
        /w14263 # function does not override any base class virtual function
        /w14265 # class has virtual functions, but destructor is not virtual
        /w14640 # Enable warning on thread un-safe static member initialization
        /w14928 # more than one implicitly user-defined conversion
        /we4289 # nonstandard extension used: 'variable'
    )

    set(CLANG_WARNINGS
        # Baseline
        -Wall
        -Wextra # reasonable and standard
        -Wshadow # if a variable declaration shadows one from a parent context
        -Wpedantic # warn if non-standard is used
        # C and C++ Warnings
        -Wunused # warn on anything being unused
        -Wformat=2 # warn on security issues around functions that format output
        -Wcast-align # warn for potential performance problem casts
        -Wconversion # warn on type conversions that may lose data
        -Wsign-conversion # warn on sign conversions
        -Wnull-dereference # warn if a null dereference is detected
        -Wdouble-promotion # warn if float is implicit promoted to double
        -Wextra-semi # warn on extraneous semicolons (e.g. ';;')
        # C++ Warnings
        -Wnon-virtual-dtor # if a class with virtual func has a non-virtual dest
        -Wold-style-cast # warn for c-style casts
        -Woverloaded-virtual # if you overload (not override) a virtual function
        -Weffc++ # violations from Scott Meyers’ Effective C++
    )

    set(GCC_WARNINGS
        ${CLANG_WARNINGS}
        #    -Wduplicated-cond     # warn if if / else chain has duplicated conditions
        #    -Wduplicated-branches # warn if if / else branches have duplicated code
        #    -Wlogical-op          # warn about logical operations being used where bitwise were probably wanted
    )

    if(TSW_AS_ERRORS)
        list(APPEND CLANG_WARNINGS -Werror)
        list(APPEND GCC_WARNINGS -Werror)
        list(APPEND MSVC_WARNINGS /WX)
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(WARNINGS ${MSVC_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(WARNINGS ${CLANG_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(WARNINGS ${GCC_WARNINGS})
    endif()

    target_compile_options(${TSW_TARGET} ${_scope}
    $<$<COMPILE_LANGUAGE:CXX>:${WARNINGS}>
    )

endfunction(target_set_warnings)
