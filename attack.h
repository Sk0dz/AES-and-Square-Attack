/**
 * @file attack.h
 * @brief Square attack on reduced AES
 * @authors Paul Bureau, Victoria Martini, Quentin Bonnel
 **/

#ifndef ATTACK_H
#define ATTACK_H

#include <stdint.h>
#include <stdlib.h>

void reduced_encryption(uint8_t* state, uint8_t* round_key);

uint8_t** delta_set_gen();

uint8_t* delta_byte_extract(uint8_t** delta_set, int byte);

void inverse_sub_bytes_delta(uint8_t* delta_byte);

uint8_t sum_of_bytes(uint8_t* delta_byte);

uint8_t key_guess(uint8_t** delta_set, int byte);

void key_unexpand(uint8_t* key_guess);

#endif  // ATTACK_H