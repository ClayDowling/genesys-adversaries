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
    ast.c
    main.c
)

add_subdirectory(test)