#include "gmath.h"

#include "aes.h"

uint8_t x_mult(uint8_t byte) { return (byte & 0x80) ? ((byte << 1) ^ 0x1b) : (byte << 1); }

uint8_t g_mult(uint8_t a, uint8_t b) {
  uint8_t result = 0;
  uint8_t tmp = b;
  for (int i = 0; i < 8; i++) {
    if (a & 0x01) {
      result ^= tmp;
    }
    a >>= 1;
    tmp = x_mult(tmp);
  }
  return result;
}

void mix_columns_mult(uint8_t* word, uint8_t* result) {
  result[0] = g_mult(0x02, word[0]) ^ g_mult(0x03, word[1]) ^ word[2] ^ word[3];
  result[1] = word[0] ^ g_mult(0x02, word[1]) ^ g_mult(0x03, word[2]) ^ word[3];
  result[2] = word[0] ^ word[1] ^ g_mult(0x02, word[2]) ^ g_mult(0x03, word[3]);
  result[3] = g_mult(0x03, word[0]) ^ word[1] ^ word[2] ^ g_mult(0x02, word[3]);
}

void inverse_mix_columns_mult(uint8_t* word, uint8_t* result) {
  result[0] = g_mult(0x0e, word[0]) ^ g_mult(0x0b, word[1]) ^ g_mult(0x0d, word[2]) ^ g_mult(0x09, word[3]);
  result[1] = g_mult(0x09, word[0]) ^ g_mult(0x0e, word[1]) ^ g_mult(0x0b, word[2]) ^ g_mult(0x0d, word[3]);
  result[2] = g_mult(0x0d, word[0]) ^ g_mult(0x09, word[1]) ^ g_mult(0x0e, word[2]) ^ g_mult(0x0b, word[3]);
  result[3] = g_mult(0x0b, word[0]) ^ g_mult(0x0d, word[1]) ^ g_mult(0x09, word[2]) ^ g_mult(0x0e, word[3]);
}