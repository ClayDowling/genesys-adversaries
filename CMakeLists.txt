cmake_minimum_required(VERSION 3.16)
project(adversaries LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)
include(FetchContent)
include(CTest)
find_package(FLEX)

set(UNITY_EXTENSION_FIXTURE on)

FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.5.2
)

FetchContent_MakeAvailable(unity)

add_executable(${PROJECT_NAME}  
    main.c
)

add_library(adversary
    adversary.c
    ast.c
    token.c
    terminal_tags.c
    lexer.c
    parser.c
)

target_link_libraries(${PROJECT_NAME}
    adversary
)

# add_executable(tokenstream
#     adversary.h
#     adversary-to-tokens.c
#     terminal_tags.c
#     lexer.c
# )

# target_link_libraries(tokenstream
#     ${FLEX_LIBRARIES}
# )

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

add_subdirectory(test)