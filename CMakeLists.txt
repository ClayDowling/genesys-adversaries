cmake_minimum_required(VERSION 3.16)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

project(adversaries LANGUAGES C CXX VERSION 1.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)
include(FetchContent)
include(CTest)

configure_file(installer.nsis.in installer.nsi)
configure_file(configure.h.in configure.h)

set(UNITY_EXTENSION_FIXTURE on)

FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.5.2
)

FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.3.1
)

FetchContent_MakeAvailable(unity catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

include_directories( ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${CMAKE_CURRENT_SOURCE_DIR}/external/cjson
    ${CMAKE_CURRENT_SOURCE_DIR}/external/mustach
)

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
    external/cjson/cJSON.c
    external/mustach/mustach.c
    external/mustach/mustach-wrap.c
    external/mustach/mustach-cjson.c
        namedlist.cpp
        world.cpp)

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
