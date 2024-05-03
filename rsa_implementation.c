#include "rsa_implementation.h"

void rsa_decrypt(mpz_t plaintext, mpz_t ciphertext, mpz_t d, mpz_t n) {
    mpz_powm(plaintext, ciphertext, d, n);
}

void rsa_encrypt(mpz_t ciphertext, mpz_t message, mpz_t e, mpz_t n) {
    mpz_powm(ciphertext, message, e, n);
}

void rsa_encrypt_file(FILE *input_fp, const char *output_file) {
    mpz_t p, q, n, phi, e, d, message, cipher;
    mpz_inits(p, q, n, phi, e, d, message, cipher, NULL);
    mpz_set_ui(e, 65537);

    // Generate two 500-bit prime numbers
    int bits = 3330; // Total bits for both p and q
    generate_large_primes(p, q, bits);

    // Compute the modulus n = p * q
    mpz_mul(n, p, q);

    // Compute φ(n)
    compute_phi(phi, p, q);

    // Compute d such that (d * e) % φ(n) = 1
    compute_mod_inverse(d, e, phi);

    // Open the public key file for writing
    FILE *public_key_fp = fopen("public_key.txt", "w");
    if (public_key_fp == NULL) {
        perror("Error opening public key file");
        exit(EXIT_FAILURE);
    }

    // Write the public key (n, e) to the public key file
    gmp_fprintf(public_key_fp, "%Zd\n%Zd\n", e, n);

    // Close the public key file
    fclose(public_key_fp);

    // Open the private key file for writing
    FILE *private_key_fp = fopen("private_key.txt", "w");
    if (private_key_fp == NULL) {
        perror("Error opening private key file");
        exit(EXIT_FAILURE);
    }

    // Write the private key (d) to the private key file
    gmp_fprintf(private_key_fp, "%Zd\n%Zd\n", d, n);

    // Close the private key file
    fclose(private_key_fp);

    // Read the message from the input file as a string
    char buffer[1024]; // Buffer to store the text read from the file
    if (fgets(buffer, sizeof(buffer), input_fp) == NULL) {
        perror("Error reading message from file");
        exit(EXIT_FAILURE);
    }

    // Convert the text read from the file to an mpz_t number
    for (size_t i = 0; i < strlen(buffer); i++) {
        mpz_mul_ui(message, message, 256); // Shift left by 8 bits (equivalent to multiplying by 256)
        mpz_add_ui(message, message, buffer[i]); // Add ASCII value of the character
    }

    // Encrypt the message
    rsa_encrypt(cipher, message, e, n);

    // Write the encrypted message to the output file
    FILE *output_fp = fopen(output_file, "w");
    if (output_fp == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    gmp_fprintf(output_fp, "%Zd\n", cipher);

    // Close the output file
    fclose(output_fp);

    mpz_clears(p, q, n, phi, e, d, message, cipher, NULL);
}


void rsa_decrypt_file(FILE *input_fp, const char *output_file) {
    mpz_t d, n, cipher, decrypted_message;
    mpz_inits(d, n, cipher, decrypted_message, NULL);


    FILE *private_key_fp = fopen("private_key.txt", "r");
    if (private_key_fp == NULL) {
        perror("Error opening private key file");
        exit(EXIT_FAILURE);
    }

    // Read the private key from the input file
    if (gmp_fscanf(private_key_fp, "%Zd\n%Zd\n", d, n) != 2) {
        perror("Error reading private key from file");
        exit(EXIT_FAILURE);
    }

    // Read the encrypted message from the input file
    if (gmp_fscanf(input_fp, "%Zd\n", cipher) != 1) {
        perror("Error reading encrypted message from file");
        exit(EXIT_FAILURE);
    }

    // Decrypt the message
    rsa_decrypt(decrypted_message, cipher, d, n);

    // Open the output file for writing
    FILE *output_fp = fopen(output_file, "w");
    if (output_fp == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    // Convert the decrypted message from mpz_t to a string
    char decrypted_text[1024];
    size_t index = 0;

    // Extract each character from the decrypted message
    while (mpz_sgn(decrypted_message) > 0) {
        decrypted_text[index++] = mpz_fdiv_q_ui(decrypted_message, decrypted_message, 256); // Divide by 256 to extract the next character
    }

    // Null-terminate the string
    decrypted_text[index] = '\0';

    // Reverse the string
    strrev(decrypted_text);

    // Print the decrypted message
    fprintf(output_fp, "%s\n", decrypted_text);

    // Close the output file
    fclose(output_fp);

    mpz_clears(d, n, cipher, decrypted_message, NULL);
}
