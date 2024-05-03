#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

void initialize_aes_sbox(uint8_t sbox[256]);
void initialize_inverse_aes_sbox(uint8_t inv_sbox[256]);
uint8_t xtime(uint8_t x);
char* xor(char *a, char *b);
char *valueOf(char digit);
bool primenumber(uint16_t  number);
bool coprime_check(uint16_t num1, uint32_t num2);
uint32_t modInverse(uint16_t e, uint32_t phi);
int gcdExtended(int a, int b, int* x, int* y); 
bool primenumber(uint16_t  number);
unsigned long long int crt_function(uint16_t i, uint32_t d);
uint32_t findD(uint16_t e, uint32_t phi);
uint32_t extendedEuclidean(uint32_t a, uint32_t b, int32_t *x, int32_t *y);

#endif 