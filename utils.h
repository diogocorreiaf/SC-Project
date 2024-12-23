#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <gmp.h>
#include <time.h>


uint8_t xtime(uint8_t x);
char *valueOf(char digit);
void compute_mod_inverse(mpz_t d, mpz_t e, mpz_t phi);
void compute_phi(mpz_t phi, mpz_t p, mpz_t q);
void strrev(char* str);
void generate_large_primes(mpz_t p, mpz_t q, int bits);
uint8_t Multiply(uint8_t x, uint8_t y);
#endif 