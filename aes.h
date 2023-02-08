/**
 * @file aes.h
 * @brief AES (Rijndael) block cipher implementation
 * @authors Paul Bureau, Victoria Martini, Quentin Bonnel
 **/

#ifndef AES_H
#define AES_H

#include <stdint.h>
#include <stdlib.h>

/**
 * @brief SubBytes operation
 * @details Replace each byte of the state by his equivalent using the gived S-Box
 * @param state the current state of the cipher
 * @pre @p state must be a valid pointer toward an array with a size of Nb * 4
 **/
void sub_bytes(uint8_t *state);

#endif /* AES_H */