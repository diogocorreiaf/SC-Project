#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aes_implementation.h"
#include "blowfish_implementation.h"
#include "rsa_implementation.h"

int main() {
    char input_file[256];
    char option[3];
    char output_file[256];
    char algorithm[20];
    int method = 0;

    printf("Enter input file: ");
    scanf("%s", input_file);

    // Input File Validation
    FILE *input_fp = fopen(input_file, "rb");
    if (input_fp == NULL) {
        perror("Error opening input file");
        return 1;
    }

    printf("Enter e for encryption or d for decryption: ");
    scanf("%s", option);

    // Determine if encryption or decryption
    if (strcmp(option, "e") == 0) {
        method = 0;
    } else if (strcmp(option, "d") == 0) {
        method = 1;
    } else {
        printf("Invalid option: %s\n", option);
        fclose(input_fp);
        return 1;
    }

    printf("Enter output file: ");
    scanf("%s", output_file);

    printf("Enter algorithm (AES, blowfish, RSA): ");
    scanf("%s", algorithm);

    // Check the algorithm
    // AES
    if (strcmp(algorithm, "AES") == 0) {
        if (method == 0) {
            // Encryption
            aes_encrypt_file(input_fp, output_file);
        } else if (method == 1) {
            // Decryption
            aes_decrypt_file(input_fp, output_file);
        } else {
            printf("Invalid method: %d\n", method);
            fclose(input_fp);
            return 1;
        }
    }

    // Blowfish
    if (strcmp(algorithm, "blowfish") == 0) {
        if (method == 0) {
            // Encryption
            blowfish_encrypt_file(input_fp, output_file);
        } else if (method == 1) {
            // Decryption
            blowfish_decrypt_file(input_fp, output_file);
        } else {
            printf("Invalid method: %d\n", method);
            fclose(input_fp);
            return 1;
        }
    }

    // RSA
    if (strcmp(algorithm, "RSA") == 0) {
        if (method == 0) {
            // Encryption
            rsa_encrypt_file(input_fp, output_file);
        } else if (method == 1) {
            // Decryption
            rsa_decrypt_file(input_fp, output_file);
        } else {
            printf("Invalid method: %d\n", method);
            fclose(input_fp);
            return 1;
        }
    }

    fclose(input_fp);
    printf("Operation completed successfully.\n");
    return 0;
}

