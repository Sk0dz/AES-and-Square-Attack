#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"
#include "attack.h"
#include "gmath.h"

int main(int argc, char* argv[]) {
  uint8_t key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  uint8_t expanded_key[176];
  key_expansion(key, expanded_key);

  printf("Key:\n");
  for (int i = 0; i < 16; i++) {
    printf("%02x ", key[i]);
  }
  printf("\n\n");
  printf("Round 4 key:\n");
  for (int i = 64; i < 80; i++) {
    printf("%02x ", expanded_key[i]);
  }

  uint8_t** delta_set = delta_set_gen(0x00);
  uint8_t** delta_set_2 = delta_set_gen(0x23);

  for (int i = 0; i < 256; i++) {
    reduced_encryption(delta_set[i], expanded_key);
    reduced_encryption(delta_set_2[i], expanded_key);
  }
  uint8_t guess_key[16];
  for (int i = 0; i < 16; i++) {
    for (int k = 0; k < 256; k++) {
      uint8_t s1 = 0;
      uint8_t s2 = 0;

      for (int j = 0; j < 256; j++) {
        s1 = inverse_s_box[delta_set[j][i] ^ k] ^ s1;
        s2 = inverse_s_box[delta_set_2[j][i] ^ k] ^ s2;
      }
      if (s1 == 0 && s2 == 0) {
        guess_key[i] = k;
      }
    }
  }

  printf("\n\nRound 4 key guess: \n");
  for (int i = 0; i < 16; i++) {
    printf("%02x ", guess_key[i]);
  }

  uint8_t* key_find = key_unexpand(guess_key);

  printf("\n\nKey guess after rewind: \n");
  for (int i = 0; i < 16; i++) {
    printf("%02x ", key_find[i]);
  }

  return EXIT_SUCCESS;
}