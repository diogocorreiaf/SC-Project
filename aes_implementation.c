#include "aes_implementation.h"

const uint8_t Rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

const uint8_t invSbox[256] = {
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
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};

void SubBytes(uint8_t state[4][4])
{

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[i][j] = sbox[state[i][j]];
        }
    }
}

void ShiftRows(uint8_t state[4][4])
{
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

void MixColumns(uint8_t state[4][4])
{
    uint8_t temp, tmp, tm;

    for (int i = 0; i < 4; ++i)
    {
        tm = state[0][i];
        temp = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i];
        tmp = state[0][i] ^ state[1][i];
        tmp = xtime(tmp);
        state[0][i] ^= temp ^ tmp;
        tmp = state[1][i] ^ state[2][i];
        tmp = xtime(tmp);
        state[1][i] ^= temp ^ tmp;
        tmp = state[2][i] ^ state[3][i];
        tmp = xtime(tmp);
        state[2][i] ^= temp ^ tmp;
        tmp = state[3][i] ^ tm;
        tmp = xtime(tmp);
        state[3][i] ^= temp ^ tmp;
    }
}

void AddRoundKey(uint8_t state[4][4], const uint8_t *round_key)
{
    // XOR each byte of the state matrix with the corresponding byte of the round key
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] ^= round_key[i * 4 + j];
        }
    }
}

void InvShiftRows(uint8_t state[4][4])
{
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

void InvSubBytes(uint8_t state[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[i][j] = invSbox[state[i][j]];
        }
    }
}

void InvMixColumns(uint8_t state[4][4])
{
    uint8_t a, b, c, d;
    for (int i = 0; i < 4; ++i)
    {
        a = state[0][i];
        b = state[1][i];
        c = state[2][i];
        d = state[3][i];

        state[0][i] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
        state[1][i] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
        state[2][i] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
        state[3][i] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
    }
}

void aes_encrypt_block(const uint8_t *plaintext_block, const uint8_t *key, uint8_t *ciphertext_block)
{
    // Create state matrix from plaintext block
    uint8_t state[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] = plaintext_block[i * 4 + j];
        }
    }

    // Initial Round Key Addition (AddRoundKey)
    AddRoundKey(state, key);

    // Perform AES encryption rounds (excluding the last round)
    for (int round = 1; round < 10; round++)
    {
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
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ciphertext_block[i * 4 + j] = state[j][i];
        }
    }
}

void aes_decrypt_block(const uint8_t *ciphertext_block, const uint8_t *key, uint8_t *plaintext_block)
{
    // Create state matrix from ciphertext block
    uint8_t state[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] = ciphertext_block[i * 4 + j];
        }
    }

    // Initial Round Key Addition (AddRoundKey)
    AddRoundKey(state, key + 160); // Use the last round key

    // Perform AES decryption rounds (excluding the first round)
    for (int round = 9; round > 0; round--)
    {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, key + round * 16);
        InvMixColumns(state);
    }

    // Final round (without InvMixColumns)
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, key);

    // Copy the resulting plaintext block from the state matrix
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            plaintext_block[i * 4 + j] = state[j][i];
        }
    }
}

void aes_encrypt_file(FILE *input_fp, const char *output_file)
{
    // Generate the key and write it to a file
    aes_key_gen("AESKey.bin");

    // Read the key from the file
    FILE *keyFile = fopen("AESKey.bin", "rb");
    uint8_t key[16];
    fread(key, sizeof(uint8_t), 16, keyFile);
    fclose(keyFile);

    // Define an array to store the expanded key
    uint8_t expandedKey[176]; // 11 * 16 bytes for AES-128
    aes_key_expansion(key, expandedKey, sbox, Rcon);

    // Open the encrypted file
    FILE *encryptedFile = fopen(output_file, "wb");

    // Define buffers for the message block and the cipher block
    uint8_t messageBlock[BLOCK_SIZE];
    uint8_t cipherBlock[BLOCK_SIZE];

    // Encryption
    size_t bytesRead;
    while ((bytesRead = fread(messageBlock, sizeof(uint8_t), BLOCK_SIZE, input_fp)) > 0)
    {
        // If we didn't read a full block, pad the rest of the block with PKCS#7 padding
        if (bytesRead < BLOCK_SIZE)
        {
            uint8_t paddingSize = BLOCK_SIZE - bytesRead;
            memset(messageBlock + bytesRead, paddingSize, paddingSize);
        }

        aes_encrypt_block(messageBlock, expandedKey, cipherBlock);
        fwrite(cipherBlock, sizeof(uint8_t), BLOCK_SIZE, encryptedFile);
    }

    fclose(encryptedFile);
}

void aes_decrypt_file(FILE *input_fp, const char *output_file)
{
    // Read the key from the file
    FILE *keyFile = fopen("AESKey.bin", "rb");
    uint8_t key[16];
    fread(key, sizeof(uint8_t), 16, keyFile);
    fclose(keyFile);

    // Define an array to store the expanded key
    uint8_t expandedKey[176];
    aes_key_expansion(key, expandedKey, sbox, Rcon);

    // Determine the file size
    fseek(input_fp, 0, SEEK_END);
    long fileSize = ftell(input_fp);
    rewind(input_fp);

    // Open the decrypted file
    FILE *decryptedFile = fopen(output_file, "wb");

    // Define a buffer for the decrypted block
    uint8_t decryptedBlock[BLOCK_SIZE];

    // Decryption
    long bytesWritten = 0;
    size_t bytesRead;
    while ((bytesRead = fread(decryptedBlock, sizeof(uint8_t), BLOCK_SIZE, input_fp)) > 0)
    {
        aes_decrypt_block(decryptedBlock, expandedKey, decryptedBlock);

        // REMOVE PADDING IF LAST BLOCK
        if (ftell(input_fp) == fileSize)
        {
            int paddingSize = decryptedBlock[BLOCK_SIZE - 1];
            if (paddingSize < 0 || paddingSize > BLOCK_SIZE)
            {
                // Handle invalid padding size
                printf("Invalid padding size.\n");
                return;
            }

            // Write the block to the file, excluding the padding
            fwrite(decryptedBlock, sizeof(uint8_t), BLOCK_SIZE - paddingSize, decryptedFile);
        }
        else
        {
            fwrite(decryptedBlock, sizeof(uint8_t), BLOCK_SIZE, decryptedFile);
        }

        bytesWritten += bytesRead;
    }
    fclose(decryptedFile);
}