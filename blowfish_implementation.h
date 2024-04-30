#ifndef BLOWFISH_IMPLEMENTATION_H
#define BLOWFISH_IMPLEMENTATION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "keys.h"
#include "utils.h"

// Encryption
void blowfish_encrypt_file(FILE *input_fp, const char *output_file);
void sub_key_gen(const char *key);

#endif 