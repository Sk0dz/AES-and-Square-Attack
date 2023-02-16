/**
 * @file gmath.h
 * @brief Rijndael's finite field arithmetic
 * @authors Paul Bureau, Victoria Martini, Quentin Bonnel
 **/

#ifndef AES_H
#define AES_H

#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Multiplication by x in the finite field
 * @details Take a byte and multiply it by x in the Rindjael's finite field
 * @param byte the byte to multiply which represent a number in the finite field GF(2^8)
 **/
uint8_t x_mult(uint8_t byte);

/**
 * @brief Multiplication in the finite field
 * @details Take two bytes and multiply them in the Rindjael's finite field
 * @param a the first byte to multiply which represent a number in the finite field GF(2^8)
 * @param b the second byte to multiply which represent a number in the finite field GF(2^8)
 **/
uint8_t g_mult(uint8_t a, uint8_t b);

/**
 * @brief MixColumns operation
 * @details Multiply a column of the state by a fixed matrix to mix the columns
 * @param word a column of the state
 * @param result the result of the operation
 * @pre @p word and @p result must be a valid pointer toward an array with a size of 4
 **/
void mix_columns_mult(uint8_t* word, uint8_t* result);

/**
 * @brief Inverse MixColumns operation
 * @details Multiply a column of the state by a fixed matrix to inverse the MixColumns operation
 * @param word a column of the state
 * @param result the result of the operation
 * @pre @p word and @p result must be a valid pointer toward an array with a size of 4
 **/
void inverse_mix_columns_mult(uint8_t* word, uint8_t* result);

#endif  // AES_H