#include "aes.h"

#include "gmath.h"

/**
 * Number of columns in the state matrix, which is the input to the encryption algorithm.
 * The value of Nb is fixed at 4 for AES encryption.
 **/
#define Nb 4

/**
 * The key size, expressed in terms of the number of 32-bit words in the key.
 * The value of Nk can be 4 (128-bit key), 6 (192-bit key) or 8 (256-bit key).
 **/
#define Nk 4

/**
 * The number of rounds in the encryption process.
 * The number of rounds is determined by the key size and is equal to 10 for a 128-bit key, 12 for a 192-bit key and 14
 *for a 256-bit key.
 **/
#define Nr 10

void rot_word(uint8_t* word) {
  const uint8_t square = word[0];
  word[0] = word[1];
  word[1] = word[2];
  word[2] = word[3];
  word[3] = square;
}

void sub_word(uint8_t* word) {
  word[0] = s_box[word[0]];
  word[1] = s_box[word[1]];
  word[2] = s_box[word[2]];
  word[3] = s_box[word[3]];
}

void key_expansion(uint8_t* key, uint8_t* add_round_key) {
  // Copie de la clef
  for (int i = 0; i < 4 * Nk; i++) {
    add_round_key[i] = key[i];
  }

  uint8_t* lastCol = malloc(sizeof(uint8_t) * Nk);
  int count = 0;
  for (int i = Nk; i < Nk * (Nr + 1); ++i) {
    // Isoler la dernière colonne
    int k = (i - 1) * 4;

    for (int j = 0; j < 4; j++) {
      lastCol[j] = add_round_key[k + j];
    }
    if (i % Nk == 0) {
      rot_word(lastCol);
      sub_word(lastCol);
      lastCol[0] = lastCol[0] ^ Rcon[count];
      count++;
    }

    // XOR
    k = (i - Nk) * 4;
    for (int j = 0; j < Nk; j++) {
      add_round_key[i * 4 + j] = add_round_key[k + j] ^ lastCol[j];
    }
  }
  free(lastCol);
}

void sub_bytes(uint8_t* state) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < Nb; j++) {
      state[i * Nb + j] = s_box[state[i * Nb + j]];
    }
  }
}

void inverse_sub_bytes(uint8_t* state) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < Nb; j++) {
      state[i * Nb + j] = inverse_s_box[state[i * Nb + j]];
    }
  }
}

void shift_rows(uint8_t* state) {
  uint8_t tmp;

  for (int i = 1; i < 4; i++) {
    for (int j = 0; j < i; j++) {
      for (int k = 0; k < Nb - 1; k++) {
        SWAP(state[i + k * 4], state[i + (k + 1) * 4], tmp);
      }
    }
  }
}

void inverse_shift_rows(uint8_t* state) {
  uint8_t tmp;

  for (int i = 1; i < 4; i++) {
    for (int j = 0; j < i; j++) {
      for (int k = Nb - 1; k > 0; k--) {
        SWAP(state[i + k * 4], state[i + (k - 1) * 4], tmp);
      }
    }
  }
}

void add_round_key(int round, uint8_t* state, uint8_t* round_key) {
  for (int i = 0; i < Nk; i++) {
    for (int j = 0; j < Nk; j++) {
      state[Nk * i + j] = state[Nk * i + j] ^ round_key[round * Nk * Nk + Nk * i + j];
    }
  }
}

void mix_columns(uint8_t* state) {
  uint8_t tmp[4], col[4];
  for (int i = 0; i < Nb; i++) {
    for (int j = 0; j < 4; j++) {
      col[j] = state[i * Nb + j];
    }
    mix_columns_mult(col, tmp);
    for (int j = 0; j < 4; j++) {
      state[i * Nb + j] = tmp[j];
    }
  }
}

void inverse_mix_columns(uint8_t* state) {
  uint8_t tmp[4], col[4];
  for (int i = 0; i < Nb; i++) {
    for (int j = 0; j < 4; j++) {
      col[j] = state[i * Nb + j];
    }
    inverse_mix_columns_mult(col, tmp);
    for (int j = 0; j < 4; j++) {
      state[i * Nb + j] = tmp[j];
    }
  }
}

void block_encryption(uint8_t* state, uint8_t* round_key) {
  add_round_key(0, state, round_key);
  for (int i = 1; i < Nr; i++) {
    sub_bytes(state);
    shift_rows(state);
    mix_columns(state);
    add_round_key(i, state, round_key);
  }
  sub_bytes(state);
  shift_rows(state);
  add_round_key(Nr, state, round_key);
}

void block_decryption(uint8_t* state, uint8_t* round_key) {
  add_round_key(Nr, state, round_key);
  inverse_shift_rows(state);
  inverse_sub_bytes(state);
  for (int i = Nr - 1; i > 0; i--) {
    add_round_key(i, state, round_key);
    inverse_mix_columns(state);
    inverse_shift_rows(state);
    inverse_sub_bytes(state);
  }
  add_round_key(0, state, round_key);
}