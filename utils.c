#include "utils.h"
#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))
#define KEY_SIZE_BYTES 16


void initialize_aes_sbox(uint8_t sbox[256]) {
    uint8_t p = 1, q = 1;
    
    do {
        p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);
        q ^= q << 1;
        q ^= q << 2;
        q ^= q << 4;
        q ^= q & 0x80 ? 0x09 : 0;
        uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);
        sbox[p] = xformed ^ 0x63;
    } while (p != 1);
    
    sbox[0] = 0x63;
}

void initialize_inverse_aes_sbox(uint8_t inv_sbox[256]) {
    uint8_t p = 1, q = 1;
    
    do {
        p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);
        q ^= q << 1;
        q ^= q << 2;
        q ^= q << 4;
        q ^= q & 0x80 ? 0x09 : 0;
        uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 3) ^ ROTL8(q, 6); // Use the inverse transformation
        inv_sbox[p] = xformed;
    } while (p != 1);
    
    inv_sbox[0] = 0x05; // Inverse of 0x63
}



uint8_t xtime(uint8_t x) {
    return (x & 0x80) ? ((x << 1) ^ 0x1B) : (x << 1);
}
char* xor(char *a, char *b) {
    unsigned long long int bin_a = strtoull(a, NULL, 16);
    unsigned long long int bin_b = strtoull(b, NULL, 16);
    
    unsigned long long int result = bin_a ^ bin_b;
    
    static char ans[9];
    sprintf(ans, "%08llx", result);
    
    return ans;
}

char *xor_strings(const char *str1, const char *str2) {
    size_t len = strlen(str1);
    char *result = malloc(len + 1);
    for (size_t i = 0; i < len; i++) {
        result[i] = str1[i] ^ str2[i];
    }
    result[len] = '\0';
    return result;
}


void compute_phi(mpz_t phi, mpz_t p, mpz_t q) {
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi, p, q); 
}

void compute_mod_inverse(mpz_t d, mpz_t e, mpz_t phi) {
    mpz_invert(d, e, phi);
}

void strrev(char* str) {
    if (str == NULL)
        return;

    int length = strlen(str);
    for (int i = 0; i < length / 2; ++i) {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

void generate_large_primes(mpz_t p, mpz_t q, int bits) {
    gmp_randstate_t state;
    gmp_randinit_default(state);
    unsigned long seed = time(NULL);
    gmp_randseed_ui(state, seed);

    // Generate first prime p
    mpz_urandomb(p, state, bits / 2);
    mpz_setbit(p, bits / 2 - 1);
    mpz_nextprime(p, p);

    mpz_urandomb(q, state, bits / 2);
    mpz_setbit(q, bits / 2 - 1);
    mpz_nextprime(q, q);

    gmp_randclear(state);
}