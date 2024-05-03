#ifndef RSA_IMPLEMENTATION_H
#define RSA_IMPLEMENTATION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "keys.h"


void rsa_encrypt_file(FILE *input_fp, const char *output_file);
void rsa_decrypt_file(FILE *input_fp, const char *output_file);
unsigned long long int rsa_encrypt(int base, uint16_t power, uint32_t mod);
unsigned long long int rsa_decrypt(unsigned long long int base, int power, int mod);
#endif 