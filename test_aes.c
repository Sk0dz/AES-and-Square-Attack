#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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

bool test_mix_columns() {
  uint8_t state[16] = {0xdb, 0x13, 0x53, 0x45, 0xf2, 0x0a, 0x22, 0x5c, 0x01, 0x01, 0x01, 0x01, 0xc6, 0xc6, 0xc6, 0xc6};
  mix_columns(state);

  ASSERT(state[0] == 0x8e);
  ASSERT(state[1] == 0x4d);
  ASSERT(state[2] == 0xa1);
  ASSERT(state[3] == 0xbc);
  ASSERT(state[4] == 0x9f);
  ASSERT(state[5] == 0xdc);
  ASSERT(state[6] == 0x58);
  ASSERT(state[7] == 0x9d);
  ASSERT(state[8] == 0x01);
  ASSERT(state[9] == 0x01);
  ASSERT(state[10] == 0x01);
  ASSERT(state[11] == 0x01);
  ASSERT(state[12] == 0xc6);
  ASSERT(state[13] == 0xc6);
  ASSERT(state[14] == 0xc6);
  ASSERT(state[15] == 0xc6);
  return true;
}

bool test_inverse_mix_columns() {
  uint8_t state[16] = {0x8e, 0x4d, 0xa1, 0xbc, 0x9f, 0xdc, 0x58, 0x9d, 0x01, 0x01, 0x01, 0x01, 0xc6, 0xc6, 0xc6, 0xc6};
  inverse_mix_columns(state);

  ASSERT(state[0] == 0xdb);
  ASSERT(state[1] == 0x13);
  ASSERT(state[2] == 0x53);
  ASSERT(state[3] == 0x45);
  ASSERT(state[4] == 0xf2);
  ASSERT(state[5] == 0x0a);
  ASSERT(state[6] == 0x22);
  ASSERT(state[7] == 0x5c);
  ASSERT(state[8] == 0x01);
  ASSERT(state[9] == 0x01);
  ASSERT(state[10] == 0x01);
  ASSERT(state[11] == 0x01);
  ASSERT(state[12] == 0xc6);
  ASSERT(state[13] == 0xc6);
  ASSERT(state[14] == 0xc6);
  ASSERT(state[15] == 0xc6);
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