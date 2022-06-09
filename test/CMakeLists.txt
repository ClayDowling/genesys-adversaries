set(UNIT_TEST_NAME ${PROJECT_NAME}-test)

enable_testing()

include_directories(..)
add_executable(${UNIT_TEST_NAME}
    testmain.c
    test_ast.c
    test_parser.c
    test_lexer.c
)

target_link_libraries(${UNIT_TEST_NAME}
    unity
    adversary
    ${FLEX_LIBRARIES}
)
