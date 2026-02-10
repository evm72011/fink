set(ALL_LIBS
    ${BACKENDS_LIB_NAME}
    ${EXAMPLES_LIB_NAME}
    ${INSTRUMENTS_LIB_NAME}
    ${MATH_LIB_NAME}
    ${MC_LIB_NAME}
    ${MODELS_LIB_NAME}
    ${PRICERS_LIB_NAME}
    ${RNG_LIB_NAME})

add_subdirectory(cli)
add_subdirectory(examples)
