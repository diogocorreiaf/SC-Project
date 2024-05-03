#ifndef RSA_IMPLEMENTATION_H
#define RSA_IMPLEMENTATION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "keys.h"
#include <gmp.h>

void rsa_encrypt_file(FILE *input_fp, const char *output_file);
void rsa_decrypt_file(FILE *input_fp, const char *output_file);
#endif 