function(print_parsed)
    # 1) ОПРЕДЕЛЯЕМ, какие ключи мы понимаем
    set(options FLAG) # флаги без значений: просто "есть/нет"
    set(oneValueArgs TARGET MODE
    )# ключи с одним значением: TARGET <x>, MODE <y>
    set(multiValueArgs FILES) # ключи со списком: FILES a b c

    # 2) ПАРСИМ то, что передали в функцию (ARGN)
    cmake_parse_arguments(
        P
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN})

    # 3) ПЕЧАТАЕМ результат
    message(STATUS "RAW ARGN: ${ARGN}")
    message(STATUS "P_FLAG: ${P_FLAG}")
    message(STATUS "P_TARGET: ${P_TARGET}")
    message(STATUS "P_MODE: ${P_MODE}")
    message(STATUS "P_FILES: ${P_FILES}")
    message(STATUS "UNPARSED: ${P_UNPARSED_ARGUMENTS}")
    message(STATUS "MISSING_VALUES: ${P_KEYWORDS_MISSING_VALUES}")
endfunction()
