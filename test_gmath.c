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
  uint8_t word2[4] = {0xf2, 0x0a, 0x22, 0x5c};
  mix_columns_mult(word2, result);
  ASSERT(result[0] == 0x9f);
  ASSERT(result[1] == 0xdc);
  ASSERT(result[2] == 0x58);
  ASSERT(result[3] == 0x9d);
  uint8_t word3[4] = {0x01, 0x01, 0x01, 0x01};
  mix_columns_mult(word3, result);
  ASSERT(result[0] == 0x01);
  ASSERT(result[1] == 0x01);
  ASSERT(result[2] == 0x01);
  ASSERT(result[3] == 0x01);
  uint8_t word4[4] = {0xc6, 0xc6, 0xc6, 0xc6};
  mix_columns_mult(word4, result);
  ASSERT(result[0] == 0xc6);
  ASSERT(result[1] == 0xc6);
  ASSERT(result[2] == 0xc6);
  ASSERT(result[3] == 0xc6);
  uint8_t word5[4] = {0xd4, 0xd4, 0xd4, 0xd5};
  mix_columns_mult(word5, result);
  ASSERT(result[0] == 0xd5);
  ASSERT(result[1] == 0xd5);
  ASSERT(result[2] == 0xd7);
  ASSERT(result[3] == 0xd6);
  uint8_t word6[4] = {0x2d, 0x26, 0x31, 0x4c};
  mix_columns_mult(word6, result);
  ASSERT(result[0] == 0x4d);
  ASSERT(result[1] == 0x7e);
  ASSERT(result[2] == 0xbd);
  ASSERT(result[3] == 0xf8);
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
  uint8_t word2[4] = {0x9f, 0xdc, 0x58, 0x9d};
  inverse_mix_columns_mult(word2, result);
  ASSERT(result[0] == 0xf2);
  ASSERT(result[1] == 0x0a);
  ASSERT(result[2] == 0x22);
  ASSERT(result[3] == 0x5c);
  uint8_t word3[4] = {0x01, 0x01, 0x01, 0x01};
  inverse_mix_columns_mult(word3, result);
  ASSERT(result[0] == 0x01);
  ASSERT(result[1] == 0x01);
  ASSERT(result[2] == 0x01);
  ASSERT(result[3] == 0x01);
  uint8_t word4[4] = {0xc6, 0xc6, 0xc6, 0xc6};
  inverse_mix_columns_mult(word4, result);
  ASSERT(result[0] == 0xc6);
  ASSERT(result[1] == 0xc6);
  ASSERT(result[2] == 0xc6);
  ASSERT(result[3] == 0xc6);
  uint8_t word5[4] = {0xd5, 0xd5, 0xd7, 0xd6};
  inverse_mix_columns_mult(word5, result);
  ASSERT(result[0] == 0xd4);
  ASSERT(result[1] == 0xd4);
  ASSERT(result[2] == 0xd4);
  ASSERT(result[3] == 0xd5);
  uint8_t word6[4] = {0x4d, 0x7e, 0xbd, 0xf8};
  inverse_mix_columns_mult(word6, result);
  ASSERT(result[0] == 0x2d);
  ASSERT(result[1] == 0x26);
  ASSERT(result[2] == 0x31);
  ASSERT(result[3] == 0x4c);
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