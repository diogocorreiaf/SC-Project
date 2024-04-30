
#include "aes_implementation.h"


#define KEY_SIZE_BYTES 16
#define KEY_EXPANDED_SIZE_BYTES 176

const uint8_t inv_sbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

static const uint8_t Rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};



// Encrypt functions

void SubBytes(uint8_t state[4][4]) {
    uint8_t sbox[256];
    initialize_aes_sbox(sbox);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = sbox[state[i][j]];
        }
    }
}

void ShiftRows(uint8_t state[4][4]) {
    uint8_t temp;

    // Shift second row 1 byte to the left
    temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;

    // Shift third row 2 bytes to the left
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;

    // Shift fourth row 3 bytes to the left
    temp = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = temp;
}

void MixColumns(uint8_t state[4][4]) {
    uint8_t temp, tmp, tm;

    for (int i = 0; i < 4; ++i) {
        tm = state[0][i];
        temp = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i];
        tmp = state[0][i] ^ state[1][i]; tmp = xtime(tmp); state[0][i] ^= temp ^ tmp;
        tmp = state[1][i] ^ state[2][i]; tmp = xtime(tmp); state[1][i] ^= temp ^ tmp;
        tmp = state[2][i] ^ state[3][i]; tmp = xtime(tmp); state[2][i] ^= temp ^ tmp;
        tmp = state[3][i] ^ tm;          tmp = xtime(tmp); state[3][i] ^= temp ^ tmp;
    }
}

void AddRoundKey(uint8_t state[4][4], const uint8_t *round_key) {
    // XOR each byte of the state matrix with the corresponding byte of the round key
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[j][i] ^= round_key[i * 4 + j];
        }
    }
}

void aes_encrypt_block(const uint8_t *plaintext_block, const uint8_t *key, uint8_t *ciphertext_block) {
    // Create state matrix from plaintext block
    uint8_t state[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[j][i] = plaintext_block[i * 4 + j];
        }
    }

    // Initial Round Key Addition (AddRoundKey)
    AddRoundKey(state, key);

    // Perform AES encryption rounds (excluding the last round)
    for (int round = 1; round < 10; round++) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, key + round * 16); // Advance the key to the next round
    }

    // Final round (without MixColumns)
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, key + 10 * 16); // Use the last round key

    // Copy the resulting ciphertext block from the state matrix
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ciphertext_block[i * 4 + j] = state[j][i];
        }
    }
}


void aes_encrypt_file(FILE *input_fp, const char *output_file) {

    uint8_t sbox[256];
    uint8_t key[KEY_SIZE_BYTES];
    uint8_t expanded_key[KEY_EXPANDED_SIZE_BYTES];

    aes_key_gen("AESKey.txt");


    FILE *key_fp = fopen("AESKey.txt", "rb");
    if (key_fp == NULL) {
        perror("Error opening key file");
        return;
    }
    fread(key, 1, KEY_SIZE_BYTES, key_fp);
    fclose(key_fp);

    // Expand the key
    initialize_aes_sbox(sbox);
    aes_key_expansion(key, expanded_key, sbox, Rcon);

    // Open the output file
    FILE *output_fp = fopen(output_file, "wb");
    if (output_fp == NULL) {
        perror("Error opening output file");
        return;
    }

    // Read and process plaintext blocks
    uint8_t plaintext_block[16];
    size_t bytes_read;
    while ((bytes_read = fread(plaintext_block, 1, 16, input_fp)) > 0) {
        // Pad the last block if needed
        if (bytes_read < 16) {
            for (size_t i = bytes_read; i < 16; i++) {
                plaintext_block[i] = 0; // Padding with zeros
            }
        }

        // Encrypt the plaintext block
        uint8_t ciphertext_block[16];
        aes_encrypt_block(plaintext_block, expanded_key, ciphertext_block);

        // Write the ciphertext block to the output file
        fwrite(ciphertext_block, 1, 16, output_fp);
    }

    // Close the output file
    fclose(output_fp);
}


// Decrypt Functions

void InvSubBytes(uint8_t state[4][4]) {
const uint8_t inv_sbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};


    // Perform inverse substitution for each byte in the state matrix
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = inv_sbox[state[i][j]];
        }
    }
}


void InvShiftRows(uint8_t state[4][4]) {
    uint8_t temp;

    // Shift second row 1 byte to the right
    temp = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = temp;

    // Shift third row 2 bytes to the right
    temp = state[2][3];
    state[2][3] = state[2][1];
    state[2][1] = temp;
    temp = state[2][2];
    state[2][2] = state[2][0];
    state[2][0] = temp;

    // Shift fourth row 3 bytes to the right
    temp = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = temp;
}


void InvMixColumns(uint8_t state[4][4]) {
    uint8_t tmp, tm, t;
    for (int i = 0; i < 4; ++i) {
        t = state[0][i];
        tm = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i];
        tmp = state[0][i] ^ state[1][i]; tmp = xtime(tmp); tmp ^= tm ^ t; state[0][i] ^= tmp;
        tmp = state[1][i] ^ state[2][i]; tmp = xtime(tmp); tmp ^= tm ^ t; state[1][i] ^= tmp;
        tmp = state[2][i] ^ state[3][i]; tmp = xtime(tmp); tmp ^= tm ^ t; state[2][i] ^= tmp;
        tmp = state[3][i] ^ t; tmp = xtime(tmp); tmp ^= tm ^ t; state[3][i] ^= tmp;
    }
}


void aes_decrypt_block(const uint8_t *ciphertext_block, const uint8_t *expanded_key, uint8_t *plaintext_block) {
    // Create state matrix from ciphertext block
    uint8_t state[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[j][i] = ciphertext_block[i * 4 + j];
        }
    }

    // Final decryption round
    AddRoundKey(state, expanded_key + 10 * 16); // Use the last round key

    // Apply additional steps to obtain plaintext block
    InvShiftRows(state);
    InvSubBytes(state);

    // Perform decryption rounds
    for (int round = 9; round > 0; round--) {
        AddRoundKey(state, expanded_key + round * 16);
        InvMixColumns(state);
        InvShiftRows(state);
        InvSubBytes(state);
    }

    // Initial decryption round
    AddRoundKey(state, expanded_key); // Use the initial round key

    // Copy the resulting plaintext block from the state matrix
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            plaintext_block[i * 4 + j] = state[j][i];
        }
    }
}




void aes_decrypt_file(FILE *input_fp, const char *output_file) {

    uint8_t sbox[256];
    uint8_t key[KEY_SIZE_BYTES];
    uint8_t expanded_key[KEY_EXPANDED_SIZE_BYTES];

    // obtain key from file
    FILE *key_fp = fopen("AESKey.txt", "rb");
    if (key_fp == NULL) {
        perror("Error opening key file");
        return;
    }
    fread(key, 1, KEY_SIZE_BYTES, key_fp);
    fclose(key_fp);

    printf("Key for decryption: ");
    for (int i = 0; i < KEY_SIZE_BYTES; i++) {
            printf("%02X ", key[i]);
        }
    printf("\n");

    // Initialize the S-box
    initialize_aes_sbox(sbox);

    // Expand the key
    aes_key_expansion(key, expanded_key, sbox, Rcon);

    // Open the output file for writing
    FILE *output_fp = fopen(output_file, "wb");
    if (output_fp == NULL) {
        perror("Error opening output file");
        return;
    }

    // Read and process ciphertext blocks
    uint8_t ciphertext_block[16];
    size_t bytes_read;
    while ((bytes_read = fread(ciphertext_block, 1, 16, input_fp)) > 0) {
        // Decrypt the ciphertext block
        uint8_t plaintext_block[16];
        aes_decrypt_block(ciphertext_block, expanded_key, plaintext_block);

        // Remove padding if necessary
        size_t padding = plaintext_block[15];
        if (padding < 16) {
            fwrite(plaintext_block, 1, 16 - padding, output_fp);
        } else {
            fwrite(plaintext_block, 1, 16, output_fp);
        }
    }

    // Close the output file
    fclose(output_fp);
}

