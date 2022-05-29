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

FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME}  
    main.c
)

add_library(adversary
    adversary.c
    ast.c
    token.c
    terminal_tags.c
    lexer.cpp
    World.cpp
)

target_link_libraries(${PROJECT_NAME}
    adversary
)

add_executable(lemon
    lemon.c
)

set(GENERATED_FILE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(
    OUTPUT ${GENERATED_FILE_DIR}/adversary.c ${GENERATED_FILE_DIR}/adversary.h
    COMMAND lemon -T${CMAKE_CURRENT_SOURCE_DIR}/lempar.c ${CMAKE_CURRENT_SOURCE_DIR}/adversary.y
    DEPENDS lemon adversary.y
)

add_executable(symbol-to-name
    symbol-to-name.cpp
)

add_custom_command(
    OUTPUT ${GENERATED_FILE_DIR}/terminal_tags.c
    COMMAND symbol-to-name -o ${GENERATED_FILE_DIR}/terminal_tags.c ${GENERATED_FILE_DIR}/adversary.h
    DEPENDS symbol-to-name ${GENERATED_FILE_DIR}/adversary.h
)

add_subdirectory(test)