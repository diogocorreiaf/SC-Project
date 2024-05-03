#ifndef AES_KEY_EXPANSION_H
#define AES_KEY_EXPANSION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <inttypes.h>
#include "utils.h"
#include <gmp.h>

void aes_key_gen(const char *filename);
void aes_key_expansion(const uint8_t *input_key, uint8_t *expanded_key, const uint8_t sbox[256], const uint8_t Rcon[10]) ;
void blowfish_key_gen(const char *filename);
void rsa_key_gen();
#endif 
