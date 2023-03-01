set(UNIT_TEST_NAME ${PROJECT_NAME}-test)
set(CPP_TEST_NAME ${PROJECT_NAME}-cpp-test)

enable_testing()

include_directories(..)
add_executable(${UNIT_TEST_NAME}
    testmain.c
    test_ast.c
    test_parser.c
    test_lexer.c
    test_character.c
)

target_link_libraries(${UNIT_TEST_NAME}
    unity
    adversary
)

add_executable(${CPP_TEST_NAME}
        test_world.cpp
        )

target_link_libraries(${CPP_TEST_NAME}
        PRIVATE
        adversary
        Catch2::Catch2WithMain
        )

add_test(NAME unitytests COMMAND ${UNIT_TEST_NAME})
set_tests_properties(unitytests PROPERTIES
    PASS_REGULAR_EXPRESSION " 0 Failures"
)

catch_discover_tests(${CPP_TEST_NAME})