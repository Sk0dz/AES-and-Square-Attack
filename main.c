

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"
#include "gmath.h"

int main(int argc, char* argv[]) {
  uint8_t key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  uint8_t expanded_key[176];
  printf("Key:\n");
  for (int i = 0; i < 16; i++) {
    printf("%02x ", key[i]);
  }
  key_expansion(key, expanded_key);

  uint8_t text[] = {0x74, 0x68, 0x65, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x62, 0x72, 0x65, 0x61, 0x6b, 0x65, 0x72, 0x73};

  printf("\n\nPlaintext message:\n");
  for (int i = 0; i < 4; i++) {
    printf("%02x %02x %02x %02x ", text[4 * i + 0], text[4 * i + 1], text[4 * i + 2], text[4 * i + 3]);
  }

  block_encryption(text, expanded_key);
  printf("\n\nCiphertext message:\n");
  for (int i = 0; i < 4; i++) {
    printf("%02x %02x %02x %02x ", text[4 * i + 0], text[4 * i + 1], text[4 * i + 2], text[4 * i + 3]);
  }

  block_decryption(text, expanded_key);

  printf("\n\nDecrypted message:\n");
  for (int i = 0; i < 4; i++) {
    printf("%02x %02x %02x %02x ", text[4 * i + 0], text[4 * i + 1], text[4 * i + 2], text[4 * i + 3]);
  }
}