#include "rsa_implementation.h"



void rsa_encrypt_file(FILE *input_fp, const char *output_file){
    char buffer[1024];
    mpz_t n, e , message, cipher;
    mpz_inits(n, e, message, cipher, NULL);
    rsa_key_gen();

    FILE *public_key_file = fopen("public_key.txt", "r");
    if (public_key_file == NULL) {
        perror("Error opening public key file");
        exit(EXIT_FAILURE);
    }

    // Read the public key
    if (gmp_fscanf(public_key_file, "%Zd\n%Zd\n", n, e) != 2) {
        perror("Error reading public key from file");
        exit(EXIT_FAILURE);
    }

    if (fgets(buffer, sizeof(buffer), input_fp) == NULL) {
        perror("Error reading message from file");
        exit(EXIT_FAILURE);
    }

    fclose(input_fp);

    for (size_t i = 0; i < strlen(buffer); i++) {
        mpz_mul_ui(message, message, 256); 
        mpz_add_ui(message, message, buffer[i]);
    }

    mpz_powm(cipher, message, e, n);

    FILE *cipher_file = fopen(output_file, "w");
    if (cipher_file == NULL) {
        perror("Error opening cipher file");
        exit(EXIT_FAILURE);
    }

    // Write the cipher to the file
    gmp_fprintf(cipher_file, "%Zd", cipher);

    // Close the output file
    fclose(cipher_file);
    mpz_clears(n, e, message, cipher, NULL);
}





void rsa_decrypt_file(FILE *input_fp, const char *output_file){
    char decrypted_text[1024];
    size_t index = 0;
    mpz_t n, d, cipher, decrypted_message;
    mpz_inits( n, d, cipher,decrypted_message, NULL);

    
    // Read the private key from the file
    FILE *private_key_file = fopen("private_key.txt", "r");
    if (private_key_file == NULL) {
        perror("Error opening private key file");
        exit(EXIT_FAILURE);
    }

    // Read the private key
    if (gmp_fscanf(private_key_file, "%Zd\n%Zd\n", n, d) != 2) {
        perror("Error reading private key from file");
        exit(EXIT_FAILURE);
    }
    fclose(private_key_file);

    FILE *cipher_file = fopen("cipher.txt", "r");
    if (cipher_file == NULL) {
        perror("Error opening cipher file");
        exit(EXIT_FAILURE);
    }

    // Read the cipher from the file
    if (gmp_fscanf(cipher_file, "%Zd", cipher) != 1) {
        perror("Error reading cipher from file");
        exit(EXIT_FAILURE);
    }

    // Close the cipher file
    fclose(cipher_file);

    mpz_powm(decrypted_message, cipher, d, n);

    // Extract each character from the decrypted message
    while (mpz_sgn(decrypted_message) > 0) {
        mpz_tdiv_q_ui(decrypted_message, decrypted_message, 256); // Divide by 256 to extract the next character
        decrypted_text[index++] = mpz_get_ui(decrypted_message) & 0xFF; // Get the lowest byte (ASCII value of the character)
    }

    decrypted_text[index] = '\0';
    strrev(decrypted_text);
    
    FILE *output_fp = fopen(output_file, "w");
    if (cipher_file == NULL) {
        perror("Error opening cipher file");
        exit(EXIT_FAILURE);
    }

    // Write the cipher to the file
    gmp_fprintf(output_fp, "%Zd", cipher);

    // Close the output file
    fclose(output_fp);
    mpz_clears(n, d,cipher, decrypted_message, NULL);
}