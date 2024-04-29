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

uint8_t xtime(uint8_t x) {
    return (x & 0x80) ? ((x << 1) ^ 0x1B) : (x << 1);
}

uint8_t *read_key_from_bin_file(const char *filename) {
    FILE *bin_file = fopen(filename, "rb");
    if (bin_file == NULL) {
        perror("Error opening bin file");
        exit(EXIT_FAILURE);
    }

    uint8_t *key = (uint8_t *)malloc(KEY_SIZE_BYTES);
    if (key == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    size_t bytes_read = fread(key, 1, KEY_SIZE_BYTES, bin_file);
    if (bytes_read != KEY_SIZE_BYTES) {
        perror("Error reading key from bin file");
        exit(EXIT_FAILURE);
    }

    fclose(bin_file);

    return key;
}