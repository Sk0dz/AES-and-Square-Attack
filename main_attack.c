#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"
#include "gmath.h"
#include "attack.h"


int main(int argc, char** argv[]){
    uint8_t key[] = {0xd0, 0xc9, 0xe1, 0xb6, 0x14, 0xee, 0x3f, 0x63, 0xf9, 0x25, 0x0c, 0x0c, 0xa8, 0x89, 0xc8, 0xa6};
    uint8_t expanded_key[176];
    key_expansion(key, expanded_key);
    for(int i=64; i<81; i++){
        printf("%02x ", expanded_key[i]);
    }

    uint8_t** delta_set = delta_set_gen();
    for(int i = 0; i<256; i++){
        reduced_encryption(delta_set[i], expanded_key);
    }

    uint8_t guess_key[16];
    for(int i=0; i<16; i++){
        guess_key[i] = key_guess(delta_set, i);
    }
    printf("Key guess: ");
    for(int i=0; i<16; i++){
        printf("%02x ", guess_key[i]);
    }
    key_unexpand(guess_key);
    printf("\nKey guess after rewind: ");
    for(int i=0; i<16; i++){
        printf("%02x ", guess_key[i]);
    }
    return EXIT_SUCCESS;
}