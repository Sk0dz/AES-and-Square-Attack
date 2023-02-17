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

bool test_rot_word() {
  uint8_t word[4] = {0x01, 0x02, 0x03, 0x04};
  rot_word(word);
  ASSERT(word[0] == 0x02);
  ASSERT(word[1] == 0x03);
  ASSERT(word[2] == 0x04);
  ASSERT(word[3] == 0x01);
  uint8_t word2[4] = {0x0a, 0x0b, 0x0c, 0x0d};
  rot_word(word2);
  ASSERT(word2[0] == 0x0b);
  ASSERT(word2[1] == 0x0c);
  ASSERT(word2[2] == 0x0d);
  ASSERT(word2[3] == 0x0a);
  return true;
}

bool test_sub_word() {
  uint8_t word[4] = {0x19, 0xa0, 0x9a, 0xe9};
  sub_word(word);
  ASSERT(word[0] == 0xd4);
  ASSERT(word[1] == 0xe0);
  ASSERT(word[2] == 0xb8);
  ASSERT(word[3] == 0x1e);
  uint8_t word2[4] = {0x3d, 0xef, 0x49, 0x10};
  sub_word(word2);
  ASSERT(word2[0] == 0x27);
  ASSERT(word2[1] == 0xdf);
  ASSERT(word2[2] == 0x3b);
  ASSERT(word2[3] == 0xca);
  return true;
}

bool test_key_expansion() {
  uint8_t key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  uint8_t expanded_key[176];
  key_expansion(key, expanded_key);
  uint8_t expected_key[176] = {
      0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c, 0xa0, 0xfa,
      0xfe, 0x17, 0x88, 0x54, 0x2c, 0xb1, 0x23, 0xa3, 0x39, 0x39, 0x2a, 0x6c, 0x76, 0x05, 0xf2, 0xc2, 0x95, 0xf2,
      0x7a, 0x96, 0xb9, 0x43, 0x59, 0x35, 0x80, 0x7a, 0x73, 0x59, 0xf6, 0x7f, 0x3d, 0x80, 0x47, 0x7d, 0x47, 0x16,
      0xfe, 0x3e, 0x1e, 0x23, 0x7e, 0x44, 0x6d, 0x7a, 0x88, 0x3b, 0xef, 0x44, 0xa5, 0x41, 0xa8, 0x52, 0x5b, 0x7f,
      0xb6, 0x71, 0x25, 0x3b, 0xdb, 0x0b, 0xad, 0x00, 0xd4, 0xd1, 0xc6, 0xf8, 0x7c, 0x83, 0x9d, 0x87, 0xca, 0xf2,
      0xb8, 0xbc, 0x11, 0xf9, 0x15, 0xbc, 0x6d, 0x88, 0xa3, 0x7a, 0x11, 0x0b, 0x3e, 0xfd, 0xdb, 0xf9, 0x86, 0x41,
      0xca, 0x00, 0x93, 0xfd, 0x4e, 0x54, 0xf7, 0x0e, 0x5f, 0x5f, 0xc9, 0xf3, 0x84, 0xa6, 0x4f, 0xb2, 0x4e, 0xa6,
      0xdc, 0x4f, 0xea, 0xd2, 0x73, 0x21, 0xb5, 0x8d, 0xba, 0xd2, 0x31, 0x2b, 0xf5, 0x60, 0x7f, 0x8d, 0x29, 0x2f,
      0xac, 0x77, 0x66, 0xf3, 0x19, 0xfa, 0xdc, 0x21, 0x28, 0xd1, 0x29, 0x41, 0x57, 0x5c, 0x00, 0x6e, 0xd0, 0x14,
      0xf9, 0xa8, 0xc9, 0xee, 0x25, 0x89, 0xe1, 0x3f, 0x0c, 0xc8, 0xb6, 0x63, 0x0c, 0xa6};
  for (int i = 0; i < 176; i++) {
    ASSERT(expanded_key[i] == expected_key[i]);
  }
  return true;
}

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
  if (strcmp("rot_word", argv[1]) == 0) {
    ok = test_rot_word();
  } else if (strcmp("sub_word", argv[1]) == 0) {
    ok = test_sub_word();
  } else if (strcmp("key_expansion", argv[1]) == 0) {
    ok = test_key_expansion();
  } else if (strcmp("mix_columns", argv[1]) == 0) {
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