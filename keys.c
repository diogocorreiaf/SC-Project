#include "keys.h"

#define BLOWFISH_MIN_KEY_SIZE_BYTES 4 
#define BLOWFISH_MAX_KEY_SIZE_BYTES 56 
#define AES_KEY_SIZE 16
#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))
#define MAX_VALUE 65535 


void blowfish_key_gen(const char *filename) {
    FILE *key_file = fopen(filename, "wb");
    if (key_file == NULL) {
        perror("Error opening key file");
        exit(EXIT_FAILURE);
    }

    // Seed the random number generator
    srand(time(NULL));

    // Generate random key size within the range
    int key_size = BLOWFISH_MIN_KEY_SIZE_BYTES +
                   rand() % (BLOWFISH_MAX_KEY_SIZE_BYTES - BLOWFISH_MIN_KEY_SIZE_BYTES + 1);

    // Write key size to file
    fwrite(&key_size, sizeof(int), 1, key_file);

    // Generate random key
    uint8_t key[key_size];
    for (int i = 0; i < key_size; i++) {
        key[i] = rand() % 256;
    }

    // Write key to file
    fwrite(key, 1, key_size, key_file);

    // Close the file
    fclose(key_file);
}


void aes_key_gen(const char *filename) {
    FILE *key_file = fopen(filename, "wb");
    if (key_file == NULL) {
        perror("Error opening key file");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    uint8_t key[AES_KEY_SIZE];
    for (int i = 0; i < AES_KEY_SIZE; i++) {
        key[i] = rand() % 256;
    }

    fwrite(key, 1, AES_KEY_SIZE, key_file);
    fclose(key_file);
}



void aes_key_expansion(const uint8_t *input_key, uint8_t *expanded_key, const uint8_t sbox[256], const uint8_t Rcon[10]) {
    const int Nk = AES_KEY_SIZE / 4; // Number of 32-bit words in the key
    const int Nb = 4; // Number of columns (32-bit words) in the state (always 4 for AES)
    const int Nr = Nk + 6; // Number of rounds (10, 12, or 14 for AES-128, AES-192, or AES-256)

    // Copy the original key to the beginning of the expanded key
    for (int i = 0; i < AES_KEY_SIZE; i++) {
        expanded_key[i] = input_key[i];
    }

    uint8_t temp[4]; // Temporary storage for 4-byte words

    // Iterate over the remaining words in the expanded key
    for (int i = Nk; i < Nb * (Nr + 1); i++) {
        // Copy the previous word to temp
        for (int j = 0; j < 4; j++) {
            temp[j] = expanded_key[(i - 1) * 4 + j];
        }

        // Perform key schedule core every Nk words
        if (i % Nk == 0) {
            // RotWord: Rotate the previous word
            uint8_t temp_byte = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = temp_byte;

            // SubWord: Apply the S-box to each byte
            for (int j = 0; j < 4; j++) {
                temp[j] = sbox[temp[j]];
            }

            // XOR with Rcon
            temp[0] ^= Rcon[i / Nk - 1];
        }

        // XOR the temp with the word Nk positions back
        for (int j = 0; j < 4; j++) {
            expanded_key[i * 4 + j] = expanded_key[(i - Nk) * 4 + j] ^ temp[j];
        }
    }
}

void rsa_key_gen(){
    printf("STASRT OF KEY GEN\n");
    mpz_t p_keygen, q_keygen, n_keygen, phi_keygen, e_keygen, d_keygen;   
    printf("Declare vars\n");
    mpz_inits(p_keygen, q_keygen, n_keygen, phi_keygen, e_keygen, d_keygen);
    printf("init vars\n");
    mpz_set_ui(e_keygen, 65537);
    printf("Set UI\n");
    int bits = 3330;    

    printf("Before prime ggen\n");
    generate_large_primes(p_keygen, q_keygen, bits);

    mpz_mul(n_keygen, p_keygen, q_keygen);

    printf("Before phi\n");
    compute_phi(phi_keygen, p_keygen, q_keygen);

    printf("Before mod inverse\n");
    compute_mod_inverse(d_keygen, e_keygen, phi_keygen);

    FILE *public_key_file = fopen("public_key.txt", "w");
    FILE *private_key_file = fopen("private_key.txt", "w");
    
    printf("Before write to file\n");
    if (public_key_file && private_key_file) {
        gmp_fprintf(public_key_file, "%Zd\n%Zd\n", n_keygen, e_keygen);
        gmp_fprintf(private_key_file, "%Zd\n", d_keygen);
        fclose(public_key_file);
        fclose(private_key_file);
    }
    printf("Before clear\n");
    mpz_clears(p_keygen, q_keygen, n_keygen, phi_keygen, e_keygen, d_keygen, NULL);
}
