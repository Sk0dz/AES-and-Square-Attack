#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"
#include "gmath.h"

#define ASSERT(expr)                                                                  \
  do {                                                                                \
    if ((expr) == 0) {                                                                \
      fprintf(stderr, "[%s:%d] Assertion '%s' failed!\n", __FILE__, __LINE__, #expr); \
      abort();                                                                        \
    }                                                                                 \
  } while (0)

/********* TESTS **********/

bool test_mix_columns_mult() {
  uint8_t word[4] = {0xdb, 0x13, 0x53, 0x45};
  uint8_t result[4];
  mix_columns_mult(word, result);
  ASSERT(result[0] == 0x8e);
  ASSERT(result[1] == 0x4d);
  ASSERT(result[2] == 0xa1);
  ASSERT(result[3] == 0xbc);
  return true;
}

bool test_inverse_mix_columns_mult() {
  uint8_t word[4] = {0x8e, 0x4d, 0xa1, 0xbc};
  uint8_t result[4];
  inverse_mix_columns_mult(word, result);
  ASSERT(result[0] == 0xdb);
  ASSERT(result[1] == 0x13);
  ASSERT(result[2] == 0x53);
  ASSERT(result[3] == 0x45);
  return true;
}

//************* USAGE **********************//

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/********* MAIN **********/

int main(int argc, char *argv[]) {
  if (argc == 1) usage(argc, argv);

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("mix_columns_mult", argv[1]) == 0) {
    ok = test_mix_columns_mult();
  } else if (strcmp("inverse_mix_columns_mult", argv[1]) == 0) {
    ok = test_inverse_mix_columns_mult();
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