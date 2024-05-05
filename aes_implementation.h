#ifndef AES_IMPLEMENTATION_H
#define AES_IMPLEMENTATION_H

#include <stdio.h>
#include <stdint.h>
#include "keys.h"
#include "utils.h"

#define NUM_ROUNDS 10
#define AES_KEY_SIZE 16
#define BLOCK_SIZE 16

// Encryption Functions
void SubBytes(uint8_t state[4][4]);
void ShiftRows(uint8_t state[4][4]);
void MixColumns(uint8_t state[4][4]);
void AddRoundKey(uint8_t state[4][4], const uint8_t *round_key);
void aes_encrypt_block(const uint8_t *plaintext_block, const uint8_t *key, uint8_t *ciphertext_block);
void aes_encrypt_file(FILE *input_fp, const char *output_file);

// Decryption Functions
void InvSubBytes(uint8_t state[4][4]);
void InvShiftRows(uint8_t state[4][4]);
void InvMixColumns(uint8_t state[4][4]);
void aes_decrypt_block(const uint8_t *ciphertext_block, const uint8_t *last_round_key, uint8_t *plaintext_block);
void aes_decrypt_file(FILE *input_fp, const char *output_file);
#endif /* AES_IMPLEMENTATION_H */
