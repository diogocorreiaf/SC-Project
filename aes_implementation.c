
#include "aes_implementation.h"


#define KEY_SIZE_BYTES 16
#define KEY_EXPANDED_SIZE_BYTES 176

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

    FILE *key_fp = fopen("keyfile.bin", "rb");
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
/*
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


void InvSubBytes(uint8_t state[4][4]) {
    uint8_t inv_sbox[256];
    initialize_inv_aes_sbox(inv_sbox);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = inv_sbox[state[i][j]];
        }
    }
}


void InvMixColumns(uint8_t state[4][4]) {
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



void key_expansion_decrypt(const uint8_t *last_round_key, uint8_t *expanded_key, const uint8_t sbox[256], const uint8_t inv_sbox[256], const uint8_t Rcon[10]) {
    const int Nk = KEY_SIZE_BYTES / 4; // Number of 32-bit words in the key
    const int Nb = 4; // Number of columns (32-bit words) in the state (always 4 for AES)
    const int Nr = Nk + 6; // Number of rounds (10, 12, or 14 for AES-128, AES-192, or AES-256)

    // Copy the last round key to the beginning of the expanded key
    for (int i = 0; i < KEY_SIZE_BYTES; i++) {
        expanded_key[i] = last_round_key[i];
    }

    // Iterate over the remaining words in the expanded key in reverse order
    for (int i = Nb * (Nr + 1) - 1; i >= Nk; i--) {
        // Perform key schedule core every Nk words
        if (i % Nk == 0) {
            // RotWord: Rotate the previous word
            uint8_t temp_byte = expanded_key[i - 3];
            expanded_key[i - 3] = expanded_key[i - 2];
            expanded_key[i - 2] = expanded_key[i - 1];
            expanded_key[i - 1] = expanded_key[i];
            expanded_key[i] = temp_byte;

            // InvSubWord: Apply the inverse S-box to each byte
            for (int j = i - 3; j <= i; j++) {
                expanded_key[j] = inv_sbox[expanded_key[j]];
            }

            // XOR with Rcon
            expanded_key[i - 3] ^= Rcon[i / Nk - 1];
        }

        // XOR the current word with the word Nk positions ahead
        for (int j = 0; j < 4; j++) {
            expanded_key[i * 4 + j] = expanded_key[(i - Nk) * 4 + j] ^ expanded_key[i * 4 + j];
        }
    }
}


void aes_decrypt_block(const uint8_t *ciphertext_block, const uint8_t *last_round_key, uint8_t *plaintext_block) {
    // Create state matrix from ciphertext block
    uint8_t state[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[j][i] = ciphertext_block[i * 4 + j];
        }
    }

    // Initial Round Key Addition (AddRoundKey) using the last round key
    AddRoundKey(state, last_round_key);

    // Perform AES decryption rounds (excluding the first round)
    for (int round = 9; round > 0; round--) {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, last_round_key + round * 16); // Advance the key to the previous round
        if (round > 1) {
            InvMixColumns(state);
        }
    }

    // Final round (without InvMixColumns) and AddRoundKey
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, last_round_key); // Use the last round key

    // Copy the resulting plaintext block from the state matrix
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            plaintext_block[i * 4 + j] = state[j][i];
        }
    }
}


void aes_decrypt_file(FILE *input_fp, const char *output_file) {

    FILE *output_fp = fopen(output_file, "wb");
    if (output_fp == NULL) {
        perror("Error opening output file");
        return;
    }

    // Generate the original encryption key
    uint8_t original_key[KEY_SIZE_BYTES];
    aes_key_gen("key.bin");

    // Derive the last round key using key expansion
    uint8_t last_round_key[KEY_SIZE_BYTES];
    key_expansion(original_key, last_round_key);

    // Obtain the expanded key for decryption
    uint8_t expanded_key[KEY_EXPANDED_SIZE_BYTES];
    key_expansion_decrypt(last_round_key, expanded_key);

    // Read and process ciphertext blocks
    uint8_t ciphertext_block[16];
    size_t bytes_read;
    while ((bytes_read = fread(ciphertext_block, 1, 16, input_fp)) > 0) {
        // Decrypt the ciphertext block
        uint8_t plaintext_block[16];
        aes_decrypt_block(ciphertext_block, expanded_key, plaintext_block);

        // Write the plaintext block to the output file
        fwrite(plaintext_block, 1, bytes_read, output_fp);
    }

    fclose(output_fp);
}


*/