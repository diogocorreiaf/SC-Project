#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aes_implementation.h"
#include "blowfish_implementation.h"
#include "rsa_implementation.h"




int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <input_file> <-d or -e> <output_file> <algorithm>\n", argv[0]);
        return 1;
    }
    
    int method = 0;


    // Extract arguments
    char *input_file = argv[1];
    char *option = argv[2];
    char *output_file = argv[3];
    char *algorithm = argv[4];

    // Input File Validation
    FILE *input_fp = fopen(input_file, "rb");
    if (input_fp == NULL) {
        perror("Error opening input file");
        return 1;
    }

    // Determine if encryption or decryption
    if (strcmp(option, "-e") == 0) {
        method = 0;
    } else if (strcmp(option, "-d") == 0) {
        method = 1;
    } else {
        printf("Invalid option: %s\n", option);
        fclose(input_fp);
        exit(33);
    }

    // Check the algorithm 
        // AES
    if(strcmp(algorithm, "AES") == 0 ) {
        if (method == 0) {
            // Encryption
            aes_encrypt_file(input_fp, output_file);
        } else if (method == 1) {
            // Decryption
           aes_decrypt_file(input_fp, output_file);
        } else {
            printf("Invalid method: %d\n", method);
            fclose(input_fp);
            exit(0);
        }
    }

       // Blowfish
    if(strcmp(algorithm, "blowfish") == 0 ) {
        if (method == 0) {
            // Encryption
            blowfish_encrypt_file(input_fp, output_file);
        } else if (method == 1) {
            // Decryption
           blowfish_decrypt_file(input_fp, output_file);
        } else {
            printf("Invalid method: %d\n", method);
            fclose(input_fp);
            exit(0);
        }
    }

        // RSA
    if(strcmp(algorithm, "RSA") == 0 ) {
        if (method == 0) {
            // Encryption
            rsa_encrypt_file(input_fp, output_file);
        } else if (method == 1) {
            // Decryption
           rsa_decrypt_file(input_fp, output_file);
        } else {
            printf("Invalid method: %d\n", method);
            fclose(input_fp);
            exit(0);
        }
    }


    fclose(input_fp);
    printf("Operation completed successfully.\n");
    return 0;
}
