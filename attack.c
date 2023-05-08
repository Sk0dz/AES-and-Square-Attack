#include <stdio.h>
#include <stdbool.h>

#include "aes.h"
#include "gmath.h"
#include "attack.h"

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

uint8_t** delta_set_gen() {
    uint8_t** delta_set = malloc(256 * sizeof(uint8_t*));
    for (int i = 0; i < 256; i++) {
        delta_set[i] = malloc(16 * sizeof(uint8_t));
        for (int j = 0; j < 16; j++) {
        delta_set[i][j] = 0x00;
        }
    }
    for (int i = 0; i < 256; i++) {
        delta_set[i][0] = i;
    }
    return delta_set;
}

uint8_t* delta_byte_extract(uint8_t** delta_set, int byte) {
    uint8_t* delta_byte = malloc(256 * sizeof(uint8_t));
    for (int i = 0; i < 256; i++) {
        delta_byte[i] = delta_set[i][byte];
    }
    return delta_byte;
}

void inverse_sub_bytes_delta(uint8_t* delta_byte){
    for (int i = 0; i < 256; i++) {
        delta_byte[i] = inverse_s_box[delta_byte[i]];
    }
}

uint8_t sum_of_bytes(uint8_t* delta_byte){
    uint8_t sum = 0x00;
    for (int i = 0; i < 256; i++) {
        sum = sum ^ delta_byte[i];
    }
    return sum;
}

uint8_t key_guess(uint8_t** delta_set, int byte){
    for (int i = 0; i < 256; i++) {
        uint8_t key_guess = i;
        uint8_t* delta_byte = delta_byte_extract(delta_set, byte);
        for (int j = 0; j < 256; j++) {
            delta_byte[j] = delta_byte[j] ^ key_guess;
        }
        inverse_sub_bytes_delta(delta_byte);
        uint8_t sum = sum_of_bytes(delta_byte);
        if (sum == 0x00){
            free(delta_byte);
            return key_guess;
        }
        free(delta_byte);
    }
}


void key_unexpand(uint8_t* key_guess) {
  for (int i = 3; i >= 0; i--) {
    int tmp = 0;
    int row, column = 0;

    for (row = 3; row >= 0; row--) {
        for (column = 3; column >= 1; column--) {
        key_guess[(4 * row) + column] ^= key_guess[(4 * row) + column - 1];
        }
    }

    tmp = key_guess[3];
    key_guess[0] = s_box[key_guess[7]] ^ Rcon[i] ^ key_guess[0];
    key_guess[4] = s_box[key_guess[11]] ^ key_guess[4];
    key_guess[8] = s_box[key_guess[15]] ^ key_guess[8];
    key_guess[12] = s_box[tmp] ^ key_guess[12];
        
  }
  
}
