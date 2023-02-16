#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "aes.h"

#define ASSERT(expr)                                                                  \
  do {                                                                                \
    if ((expr) == 0) {                                                                \
      fprintf(stderr, "[%s:%d] Assertion '%s' failed!\n", __FILE__, __LINE__, #expr); \
      abort();                                                                        \
    }                                                                                 \
  } while (0)

/********* TESTS **********/

bool test_mix_columns() {}

/********* MAIN **********/

int main(int argc, char *argv[]) {
  if (argc == 1) usage(argc, argv);

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("mix_columns", argv[1]) == 0) {
    ok = test_mix_columns();
  } else if (strcmp("inverse_mix_columns", argv[1]) == 0) {
    ok = test_inverse_mix_columns();
  } else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}