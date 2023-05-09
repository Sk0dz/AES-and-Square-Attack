/**
 * @file attack.h
 * @brief Square attack on reduced AES
 * @authors Paul Bureau, Victoria Martini, Quentin Bonnel
 **/

#ifndef ATTACK_H
#define ATTACK_H

#include <stdint.h>
#include <stdlib.h>

/**
 * @brief AES encryption with 4 rounds
 * @details Encrypt a block of data with 4 rounds
 * @param state the current state of the cipher
 * @param round_key the round key to add
 * @pre @p state and @p round_key must be a valid pointer toward an array with a size of Nb * 4 and Nb * (Nr + 1)
 *respectively
 **/
void reduced_encryption(uint8_t* state, uint8_t* round_key);

/**
 *@brief Delta set generation
 *@details Generate a delta set, i.e. a set of 256 plaintexts with the same byte in the same position, and the same byte
 *in the other positions
 *@param same_byte the byte in common in the plaintexts
 *@return a pointer toward the delta set, represented as a 2D array
 **/
uint8_t** delta_set_gen(uint8_t same_byte);

/**
 *@brief Retrieve the key
 *@details Retrieve the key from the 4th round key by inversing key_expansion operations
 *@param key_guess the round key
 **/
uint8_t* key_unexpand(uint8_t* key_guess);

#endif  // ATTACK_H