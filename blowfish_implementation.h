#ifndef BLOWFISH_IMPLEMENTATION_H
#define BLOWFISH_IMPLEMENTATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "keys.h"
#include "utils.h"


void initializeBlowfish(const uint8_t *key, size_t key_length);
uint32_t F(uint32_t x);


// Encryption Functions
void blowfish_encrypt_file(FILE *input_fp, const char *output_file);
void encryptWithBlowfish(uint32_t *xl, uint32_t *xr);
void encrypt_file(FILE *plaintext_file, FILE *ciphertext_file, size_t plaintext_size);


// Decryption Functions
void blowfish_decrypt_file(FILE *input_fp, const char *output_file);
void decryptWithBlowfish(uint32_t *xl, uint32_t *xr);
void decrypt_file(FILE *ciphertext_file, FILE *decrypted_file, size_t ciphertext_size);

extern uint32_t blowfish_Sbox[4][256];
extern uint32_t blowfish_Pbox[18];
#endif 