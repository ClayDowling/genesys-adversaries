set(UNIT_TEST_NAME ${PROJECT_NAME}-test)

include(GoogleTest)
enable_testing()

include_directories(..)
add_executable(${UNIT_TEST_NAME}
    test_ast.cpp
    test_lexer.cpp
    ../ast.c
    ../lexer.c
    ../token.c
)
target_link_libraries(${UNIT_TEST_NAME}
    gtest_main
)

gtest_discover_tests(${UNIT_TEST_NAME})