if(NOT ENABLE_SANITIZE_ADDR
   AND NOT ENABLE_SANITIZE_UNDEF
   AND NOT ENABLE_SANITIZE_LEAK
   AND NOT ENABLE_SANITIZE_THREAD)
    message(STATUS "Sanitizers are deactivated")
endif()

function(add_sanitizer_flags)
    set(oneValueArgs TARGET)
    cmake_parse_arguments(
        ASF
        ""
        "${oneValueArgs}"
        ""
        ${ARGN})

    if(NOT TARGET "${ASF_TARGET}")
        message(
            FATAL_ERROR
                "add_sanitizer_flags(): target '${ASF_TARGET}' does not exist")
    endif()

    if(NOT ENABLE_SANITIZE_ADDR
       AND NOT ENABLE_SANITIZE_UNDEF
       AND NOT ENABLE_SANITIZE_LEAK
       AND NOT ENABLE_SANITIZE_THREAD)
        return()
    endif()

    get_target_property(_tgt_type ${ASF_TARGET} TYPE)
    if(_tgt_type STREQUAL "INTERFACE_LIBRARY")
        set(_scope INTERFACE)
    else()
        set(_scope PRIVATE)
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options("${ASF_TARGET}" ${_scope}
                               -fno-omit-frame-pointer)
        target_link_options("${ASF_TARGET}" ${_scope} -fno-omit-frame-pointer)

        if(ENABLE_SANITIZE_ADDR)
            message(STATUS "Activating Address Sanitizer for ${ASF_TARGET}")
            target_compile_options("${ASF_TARGET}" ${_scope} -fsanitize=address)
            target_link_options("${ASF_TARGET}" ${_scope} -fsanitize=address)
        endif()

        if(ENABLE_SANITIZE_UNDEF)
            message(STATUS "Activating Undefined Sanitizer for ${ASF_TARGET}")
            target_compile_options("${ASF_TARGET}" ${_scope}
                                   -fsanitize=undefined)
            target_link_options("${ASF_TARGET}" ${_scope} -fsanitize=undefined)
        endif()

        if(ENABLE_SANITIZE_LEAK)
            message(STATUS "Activating Leak Sanitizer for ${ASF_TARGET}")
            target_compile_options("${ASF_TARGET}" ${_scope} -fsanitize=leak)
            target_link_options("${ASF_TARGET}" ${_scope} -fsanitize=leak)
        endif()

        if(ENABLE_SANITIZE_THREAD)
            if(ENABLE_SANITIZE_ADDR OR ENABLE_SANITIZE_LEAK)
                message(
                    WARNING
                        "TSan does not work with: Address or Leak (target: ${ASF_TARGET})"
                )
            endif()
            message(STATUS "Activating Thread Sanitizer for ${ASF_TARGET}")
            target_compile_options("${ASF_TARGET}" ${_scope} -fsanitize=thread)
            target_link_options("${ASF_TARGET}" ${_scope} -fsanitize=thread)
        endif()

        return()
    endif()

    if(MSVC)
        if(ENABLE_SANITIZE_ADDR)
            message(
                STATUS "Activating Address Sanitizer for ${ASF_TARGET} (MSVC)")

            # MSVC ASan
            target_compile_options("${ASF_TARGET}" ${_scope} /fsanitize=address)

            # Helps avoid noisy/fragile combos when ASan is on:
            target_link_options("${ASF_TARGET}" ${_scope} /INCREMENTAL:NO)
        endif()

        if(ENABLE_SANITIZE_UNDEF)
            message(
                STATUS
                    "sanitize=undefined is not available for MSVC (target: ${ASF_TARGET})"
            )
        endif()

        if(ENABLE_SANITIZE_LEAK)
            message(
                STATUS
                    "sanitize=leak is not available for MSVC (target: ${ASF_TARGET})"
            )
        endif()

        if(ENABLE_SANITIZE_THREAD)
            message(
                STATUS
                    "sanitize=thread is not available for MSVC (target: ${ASF_TARGET})"
            )
        endif()

        return()
    endif()

    message(
        WARNING
            "Sanitizers not supported for compiler '${CMAKE_CXX_COMPILER_ID}' (target: ${ASF_TARGET})"
    )
endfunction()
