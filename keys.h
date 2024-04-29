#ifndef AES_KEY_EXPANSION_H
#define AES_KEY_EXPANSION_H

#include <stdint.h>

void aes_key_gen(const char *filename);
void aes_key_expansion(const uint8_t *input_key, uint8_t *expanded_key, const uint8_t sbox[256], const uint8_t Rcon[10]) ;

#endif /* AES_KEY_EXPANSION_H */
