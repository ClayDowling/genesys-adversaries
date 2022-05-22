cmake_minimum_required(VERSION 3.16)
project(adversaries LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
include(CTest)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)

FetchContent_Declare(
    re2c
    GIT_REPOSITORY https://github.com/skvadrik/re2c.git
    GIT_TAG 3.0
)

FetchContent_MakeAvailable(googletest re2c)

add_executable(${PROJECT_NAME}  
    adversary.c
    ast.c
    lexer.c
    main.c
    token.c
)

add_executable(lemon
    lemon.c
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/adversary.c
    COMMAND lemon -T${CMAKE_CURRENT_SOURCE_DIR}/lempar.c ${CMAKE_CURRENT_SOURCE_DIR}/adversary.y
    DEPENDS lemon adversary.y
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/lexer.c
    COMMAND re2c -T -o ${CMAKE_CURRENT_SOURCE_DIR}/lexer.c ${CMAKE_CURRENT_SOURCE_DIR}/lexer.re
    DEPENDS re2c lexer.re
)

add_subdirectory(test)