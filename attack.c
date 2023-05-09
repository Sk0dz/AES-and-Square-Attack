#include "attack.h"

#include <stdbool.h>
#include <stdio.h>

#include "aes.h"
#include "gmath.h"

void reduced_encryption(uint8_t* state, uint8_t* round_key) {
  add_round_key(0, state, round_key);
  for (int i = 1; i < 4; i++) {
    sub_bytes(state);
    shift_rows(state);
    mix_columns(state);
    add_round_key(i, state, round_key);
  }
  sub_bytes(state);
  shift_rows(state);
  add_round_key(4, state, round_key);
}

uint8_t** delta_set_gen(uint8_t same_byte) {
  uint8_t** delta_set = malloc(256 * sizeof(uint8_t*));
  for (int i = 0; i < 256; i++) {
    delta_set[i] = malloc(16 * sizeof(uint8_t));
    for (int j = 0; j < 16; j++) {
      delta_set[i][j] = same_byte;
    }
  }
  for (int i = 0; i < 256; i++) {
    delta_set[i][0] = i;
  }
  return delta_set;
}

uint8_t* key_unexpand(uint8_t* guess_key) {
  uint8_t* key_storage = malloc(80 * sizeof(uint8_t));
  for (int i = 64; i < 80; i++) {
    key_storage[i] = guess_key[i - 64];
  }
  for (int i = 3; i >= 0; i--) {
    for (int j = 3; j > 0; j--) {
      for (int k = 0; k < 4; k++) {
        key_storage[16 * i + 4 * j + k] =
            key_storage[16 * (i + 1) + 4 * (j - 1) + k] ^ key_storage[16 * (i + 1) + 4 * j + k];
      }
    }
    uint8_t word[4];
    for (int k = 0; k < 4; k++) {
      word[k] = key_storage[16 * i + 12 + k];
    }
    rot_word(word);
    sub_word(word);
    key_storage[16 * i] = word[0] ^ key_storage[16 * (i + 1)] ^ Rcon[i];
    for (int k = 1; k < 4; k++) {
      key_storage[16 * i + k] = word[k] ^ key_storage[16 * (i + 1) + k];
    }
  }
  uint8_t* key = malloc(16 * sizeof(uint8_t));
  for (int i = 0; i < 16; i++) {
    key[i] = key_storage[i];
  }
  free(key_storage);
  return key;
}
