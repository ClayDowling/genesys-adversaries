#include <stdio.h>
#include <getopt.h>

#include "unity_fixture.h"

void run_all_tests() {
    RUN_TEST_GROUP(AST);
    RUN_TEST_GROUP(Parser);
}

int main(int argc, const char* argv[]) {
    return UnityMain(argc, argv, run_all_tests);
}