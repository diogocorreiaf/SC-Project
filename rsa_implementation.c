#include "rsa_implementation.h"

void rsa_encrypt_file(FILE *input_fp, const char *output_file)
{
    mpz_t p, q, n, phi, e, d, message, cipher;
    mpz_inits(p, q, n, phi, e, d, message, cipher, NULL);
    mpz_set_ui(e, 65537);
    int bits = 3330; // Number of bits to ensure that n = p * q has 1000 digits

    // KEY GENERATION, i put it here because i was getting weird errors

    generate_large_primes(p, q, bits);

    //  n = p * q
    mpz_mul(n, p, q);

    compute_phi(phi, p, q);

    compute_mod_inverse(d, e, phi);

    // WRITING THE KEYS

    FILE *public_key_fp = fopen("public_key.txt", "w");
    if (public_key_fp == NULL)
    {
        perror("Error opening public key file");
        exit(EXIT_FAILURE);
    }
    gmp_fprintf(public_key_fp, "%Zd\n%Zd\n", e, n);
    fclose(public_key_fp);

    FILE *private_key_fp = fopen("private_key.txt", "w");
    if (private_key_fp == NULL)
    {
        perror("Error opening private key file");
        exit(EXIT_FAILURE);
    }
    gmp_fprintf(private_key_fp, "%Zd\n%Zd\n", d, n);
    fclose(private_key_fp);

    // READING MESSAGE
    char buffer[1024]; // Buffer to store the text read from the file
    if (fgets(buffer, sizeof(buffer), input_fp) == NULL)
    {
        perror("Error reading message from file");
        exit(EXIT_FAILURE);
    }

    // Convert Message to mpz_t number to apply encryption
    for (size_t i = 0; i < strlen(buffer); i++)
    {
        mpz_mul_ui(message, message, 256);       // Shift left by 8 bits = to multiplying by 256)
        mpz_add_ui(message, message, buffer[i]); // Add ASCII value of the character
    }

    // ENCRYPTION PROCESS
    mpz_powm(cipher, message, d, n);

    // WRITE THE OUTPUT TO FILE
    FILE *output_fp = fopen(output_file, "w");
    if (output_fp == NULL)
    {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    gmp_fprintf(output_fp, "%Zd\n", cipher);
    fclose(output_fp);

    // VAR CLEARING
    mpz_clears(p, q, n, phi, e, d, message, cipher, NULL);
}

void rsa_decrypt_file(FILE *input_fp, const char *output_file)
{
    mpz_t d, n, cipher, decrypted_message;
    mpz_inits(d, n, cipher, decrypted_message, NULL);

    // READING KEY

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

    // READING MESSAGE

    if (gmp_fscanf(input_fp, "%Zd\n", cipher) != 1)
    {
        perror("Error reading encrypted message from file");
        exit(EXIT_FAILURE);
    }

    // DECRYPTION

    mpz_powm(decrypted_message, cipher, d, n);

    FILE *output_fp = fopen(output_file, "w");
    if (output_fp == NULL)
    {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    char decrypted_text[1024];
    size_t index = 0;

    // CONVERT THE MESSAGE BACK TO STRING FROM AN mpz_t
    while (mpz_sgn(decrypted_message) > 0)
    {
        decrypted_text[index++] = mpz_fdiv_q_ui(decrypted_message, decrypted_message, 256); // Dviding by 256
    }
    decrypted_text[index] = '\0';

    // REVERSING THE STRING
    strrev(decrypted_text);

    // WRITING TO FILE

    fprintf(output_fp, "%s\n", decrypted_text);
    fclose(output_fp);

    // VAR CLEARING
    mpz_clears(d, n, cipher, decrypted_message, NULL);
}
