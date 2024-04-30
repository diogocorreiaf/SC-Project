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
    // Convert hexadecimal strings to binary
    unsigned long long int bin_a = strtoull(a, NULL, 16);
    unsigned long long int bin_b = strtoull(b, NULL, 16);
    
    // XOR the binary values
    unsigned long long int result = bin_a ^ bin_b;
    
    // Convert the result back to hexadecimal
    static char ans[9];
    sprintf(ans, "%08llx", result);
    
    return ans;
}