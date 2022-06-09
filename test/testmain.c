#include <getopt.h>
#include <stdio.h>

#include "unity_fixture.h"

void run_all_tests() {
  RUN_TEST_GROUP(AST);
  // RUN_TEST_GROUP(Parser);
  RUN_TEST_GROUP(Lexer);
}

int main(int argc, const char *argv[]) {
  return UnityMain(argc, argv, run_all_tests);
}