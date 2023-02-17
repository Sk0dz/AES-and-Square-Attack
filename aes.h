/**
 * @file aes.h
 * @brief AES (Rijndael) block cipher implementation
 * @authors Paul Bureau, Victoria Martini, Quentin Bonnel
 **/

#ifndef AES_H
#define AES_H

#include <stdint.h>
#include <stdlib.h>

#define SWAP(x, y, z) (z) = (x), (x) = (y), (y) = (z)

/**
 * @brief SubBytes operation
 * @details Replace each byte of the state by his equivalent using the gived S-Box
 * @param state the current state of the cipher
 * @pre @p state must be a valid pointer toward an array with a size of Nb * 4
 **/
void sub_bytes(uint8_t *state);

/**
 * @brief Inverse SubBytes operation
 * @details Replace each byte of the state by his equivalent using the gived inverse S-Box
 * @param state the current state of the cipher
 * @pre @p state must be a valid pointer toward an array with a size of Nb * 4
 **/
void inverse_sub_bytes(uint8_t *state);

/**
 * @brief ShiftRows operation
 * @details Shift each row of the state by a number of bytes
 * @param state the current state of the cipher
 * @pre @p state must be a valid pointer toward an array with a size of Nb * 4
 **/
void shift_rows(uint8_t *state);

/**
 * @brief Inverse ShiftRows operation
 * @details Shift each row of the state by a number of bytes in the opposite direction
 * @param state the current state of the cipher
 * @pre @p state must be a valid pointer toward an array with a size of Nb * 4
 **/
void inverse_shift_rows(uint8_t *state);

/**
 * @brief MixColumns operation
 * @details Multiply a column of the state by a fixed matrix to mix the columns
 * @param state the current state of the cipher
 * @pre @p state must be a valid pointer toward an array with a size of Nb * 4
 **/
void mix_columns(uint8_t *state);

/**
 * @brief Inverse MixColumns operation
 * @details Multiply a column of the state by a fixed matrix to inverse the MixColumns operation
 * @param state the current state of the cipher
 * @pre @p state must be a valid pointer toward an array with a size of Nb * 4
 **/
void inverse_mix_columns(uint8_t *state);

/**
 * @brief AddRoundKey operation
 * @details Add the round key to the state
 * @param state the current state of the cipher
 * @param round_key the round key to add
 * @pre @p state and @p round_key must be a valid pointer toward an array with a size of Nb * 4
 **/
void add_round_key(int round, uint8_t *state, uint8_t *round_key);

#endif /* AES_H */