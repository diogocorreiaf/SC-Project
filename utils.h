#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void initialize_aes_sbox(uint8_t sbox[256]);
void initialize_inverse_aes_sbox(uint8_t inv_sbox[256]);
uint8_t xtime(uint8_t x);
char* xor(char *a, char *b);
char *valueOf(char digit);


#endif 