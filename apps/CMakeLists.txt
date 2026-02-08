set(ALL_LIBS
    ${INSTRUMENTS_LIB_NAME}
    ${MATH_LIB_NAME}
    #    ${MC_LIB_NAME}
    ${MODELS_LIB_NAME}
    ${PRICERS_LIB_NAME}
    ${RNG_LIB_NAME})

add_subdirectory(cli)
add_subdirectory(examples)
