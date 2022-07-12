cmake_minimum_required(VERSION 3.16)
project(adversaries LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)
include(FetchContent)
include(CTest)

set(UNITY_EXTENSION_FIXTURE on)

FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.5.2
)

FetchContent_Declare(
        incbin
        GIT_REPOSITORY https://github.com/graphitemaster/incbin.git
        GIT_TAG origin/main
)

FetchContent_MakeAvailable(unity incbin)

include_directories(${incbin_SOURCE_DIR})

configure_file(data/core.adv .)
configure_file(data/sciencefiction.adv .)
configure_file(data/spaceopera.adv .)
configure_file(data/steampunk.adv .)
configure_file(data/weirdwar.adv .)
configure_file(data/modern.adv .)
configure_file(data/fantasy.adv .)

add_executable(${PROJECT_NAME}
    main.c
)

add_library(adversary
    adversary.c
    attribute.c
    ast.c
    token.c
    terminal_tags.c
    lexer.c
    parser.c
    character.c
)

target_link_libraries(${PROJECT_NAME}
    adversary
)

add_executable(tokenstream
    adversary.h
    adversary-to-tokens.c
    terminal_tags.c
    lexer.c
    token.c
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
    adversary.h
)

add_custom_command(
    OUTPUT ${GENERATED_FILE_DIR}/terminal_tags.c
    COMMAND symbol-to-name ${GENERATED_FILE_DIR}/adversary.h -o ${GENERATED_FILE_DIR}/terminal_tags.c
    DEPENDS symbol-to-name
)

add_subdirectory(test)