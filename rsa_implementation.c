#include "rsa_implementation.h"
#define CHUNK_SIZE 4096

void rsa_encrypt_file(FILE *input_fp, const char *output_file)
{
    mpz_t p, q, n, phi, e, d, message, cipher;
    mpz_inits(p, q, n, phi, e, d, message, cipher, NULL);
    mpz_set_ui(e, 65537);
    int bits = 3330; // Number of bits to ensure that n = p * q has 1000 digits

    // Generating the keys, i put it here because i was getting weird errors

    generate_large_primes(p, q, bits);

    //  n = p * q
    mpz_mul(n, p, q);

    compute_phi(phi, p, q);

    compute_mod_inverse(d, e, phi);

    // Writes the keys to the files

    FILE *public_key_fp = fopen("public_key.txt", "w");
    if (public_key_fp == NULL)
    {
        perror("Error opening public key file");
        exit(EXIT_FAILURE);
    }
    gmp_fprintf(public_key_fp, "%Zd\n%Zd\n", e, n);
    fclose(public_key_fp);

    // Reads the key from the file
    FILE *private_key_fp = fopen("private_key.txt", "w");
    if (private_key_fp == NULL)
    {
        perror("Error opening private key file");
        exit(EXIT_FAILURE);
    }
    gmp_fprintf(private_key_fp, "%Zd\n%Zd\n", d, n);
    fclose(private_key_fp);

    FILE *output_fp = fopen(output_file, "w");
    if (output_fp == NULL)
    {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    // Reads Message line by line
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), input_fp) != NULL)
    {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0'; 
        }

        mpz_set_ui(message, 0); 

        // Convert Message to mpz_t number to apply encryption
        for (size_t i = 0; i < strlen(buffer); i++)
        {
            mpz_mul_ui(message, message, 256);       // Shift left by 8 bits = to multiplying by 256)
            mpz_add_ui(message, message, buffer[i]); // Add ASCII value of the character
        }

        // Encryption
        mpz_powm(cipher, message, e, n); 

        gmp_fprintf(output_fp, "%Zd\n", cipher);
    }

    fclose(output_fp);

    mpz_clears(p, q, n, phi, e, d, message, cipher, NULL);
}

void rsa_decrypt_file(FILE *input_fp, const char *output_file)
{
    mpz_t d, n, cipher, decrypted_message;
    mpz_inits(d, n, cipher, decrypted_message, NULL);

    // Reads the key from the file
    FILE *private_key_fp = fopen("private_key.txt", "r");
    if (private_key_fp == NULL)
    {
        perror("Error opening private key file");
        exit(EXIT_FAILURE);
    }
    if (gmp_fscanf(private_key_fp, "%Zd\n%Zd\n", d, n) != 2)
    {
        perror("Error reading private key from file");
        exit(EXIT_FAILURE);
    }
    fclose(private_key_fp);

    FILE *output_fp = fopen(output_file, "w");
    if (output_fp == NULL)
    {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    // Reads Message line by line
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), input_fp) != NULL)
    {
        mpz_set_str(cipher, buffer, 10); 

        // Decrypting
        mpz_powm(decrypted_message, cipher, d, n);

        char decrypted_text[1024];
        size_t index = 0;

        // Convert Message to string
        while (mpz_sgn(decrypted_message) > 0)
        {
            decrypted_text[index++] = mpz_fdiv_q_ui(decrypted_message, decrypted_message, 256); 
        }
        decrypted_text[index] = '\0';

        // Reverses string
        strrev(decrypted_text);
        fprintf(output_fp, "%s\n", decrypted_text);
    }

    fclose(output_fp);

    mpz_clears(d, n, cipher, decrypted_message, NULL);
}