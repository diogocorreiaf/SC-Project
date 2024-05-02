#ifndef BLOWFISH_IMPLEMENTATION_H
#define BLOWFISH_IMPLEMENTATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <math.h>
#include "keys.h"
#include "utils.h"


#define SWAP(x, y, temp) {temp = (x); (x) = (y); (y) = temp;}


// Encryption

void blowfish_encrypt_file(FILE *input_file, const char *output_file);
void blowfish_decrypt_file(FILE *input_file, const char *output_file);
uint32_t 
feistel_function(uint32_t arg);

void 
_encrypt(uint32_t *left, uint32_t *right);

void
_decrypt(uint32_t *left, uint32_t *right);

void
blowfish_init(uint8_t key[], int padsize);

uint8_t *
blowfish_encrypt(uint8_t data[], int padsize);

uint8_t *
blowfish_decrypt(uint8_t crypt_data[], int padsize);


extern uint32_t blowfish_Sbox[4][256];
extern uint32_t blowfish_Pbox[18];
#endif 