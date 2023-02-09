#include "aes.h"

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

// S-box transformation table in hexadecimal

static const uint8_t s_box[256] = {
    // 00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,   // 00
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,   // 10
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,   // 20
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,   // 30
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,   // 40
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,   // 50
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,   // 60
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,   // 70
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,   // 80
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,   // 90
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,   // A0
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,   // B0
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,   // C0
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,   // D0
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,   // E0
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};  // F0

// Inverse S-box transformation table in hexadecimal

static const uint8_t inverse_s_box[256] = {
    // 00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,   // 00
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,   // 10
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,   // 20
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,   // 30
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,   // 40
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,   // 50
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,   // 60
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,   // 70
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,   // 80
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,   // 90
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,   // A0
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,   // B0
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,   // C0
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,   // D0
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,   // E0
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};  // F0

// Rcon[i], contains the values given by x to the power (i-1) being powers of x in the field GF(2^8)
static const uint8_t Rcon[11] = {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

void RotWord(uint8_t* temp) {
  const uint8_t square = temp[0];
  temp[0] = temp[1];
  temp[1] = temp[2];
  temp[2] = temp[3];
  temp[3] = square;
}

void SubWord(uint8_t* temp) {
  temp[0] = s_box[temp[0]];
  temp[1] = s_box[temp[1]];
  temp[2] = s_box[temp[2]];
  temp[3] = s_box[temp[3]];
}

void key_expension(const uint8_t* key, uint8_t* add_round_key) {
  // Copie de la clef
  for (int i = 0; i < Nk; i++) {
    for (int j = 0; j < Nk; j++) {
      add_round_key[(i * 4) + j] = key[(i * 4) + j];
    }
  }

  for (int i = Nk; i < Nb * (Nr + 1); ++i) {
    // Isoler la dernière colonne
    int k = (i - 1) * Nk;
    uint8_t* lastCol = malloc(sizeof(uint8_t) * Nk);

    for (int j = 0; j < Nk; j++) {
      lastCol[j] = add_round_key[k + j];
    }
    if (i % Nk == 0) {
      RotWord(lastCol);
      SubWord(lastCol);
      lastCol[0] = lastCol[0] ^ Rcon[i];
    }

    // XOR
    k = (i - Nk) * Nk;
    for (int j = 0; j < Nk; j++) {
      add_round_key[i * Nk + j] = add_round_key[k + j] ^ lastCol[j];
    }
    free(lastCol);
  }
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
        SWAP(state[Nb * i + k], state[Nb * i + k + 1], tmp);
      }
    }
  }
}

void inverse_shift_rows(uint8_t* state) {
  uint8_t tmp;

  for (int i = 1; i < 4; i++) {
    for (int j = 0; j < i; j++) {
      for (int k = Nb - 1; k > 0; k--) {
        SWAP(state[Nb * i + k], state[Nb * i + k - 1], tmp);
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