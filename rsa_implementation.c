#include "rsa_implementation.h"



unsigned long long int rsa_encrypt(int base, uint16_t power, uint32_t mod) {
    unsigned long long int result = 1;
    for (uint16_t i = 0; i < power; i++) {
        result = (result * base) % mod;
    }
    return result;
}



int inverse(int a, int mod)  {
        int aprev, iprev, i = 1, atemp, itemp;

        aprev = mod, iprev = mod;
        while (a != 1)
        {
                atemp = a;
                itemp = i;
                a = aprev - aprev / atemp * a;
                i = iprev - aprev / atemp * i;
                aprev = atemp;
                iprev = itemp;
                while (i < 0)
                        i += mod;
        }

        return i;
}


unsigned long long int rsa_decrypt(unsigned long long int base, int power, int mod) {
        int i;
        unsigned long long int result = 1;
        for (i = 0; i < power; i++)
        {
                result = (result * base) % mod;
        }
        return result;
}

void rsa_encrypt_file(FILE *input_fp, const char *output_file){
    uint32_t n;
    uint16_t e;
    int i;

    // Gen Key
    rsa_key_gen();

    FILE *public_key_file = fopen("public_key.txt", "r");
    if (public_key_file == NULL) {
        printf("Error opening public_key.txt\n");
        return;
    }

    // Read public key components from file
    if (fscanf(public_key_file, "%" SCNu32 ", %" SCNu16, &n, &e) != 2) {
        printf("Error reading public key\n");
        fclose(public_key_file);
        return;
    }

    printf("Public Key: (n, e) = (%" PRIu32 ", %" PRIu16 ")\n", n, e);

    // Open Output File to Write
    FILE *output_fp = fopen(output_file, "w");
    if (output_fp == NULL) {
        printf("Error opening %s\n", output_file);
        return;
    }

    while ((i = fgetc(input_fp)) != EOF) {
        unsigned long long int encrypted_ch = rsa_encrypt(i, e, n);
        fprintf(output_fp, "%llu\n", encrypted_ch);
    }
}






void rsa_decrypt_file(FILE *input_fp, const char *output_file) {
    int n, d, p, q, h, m, qInv, m1m2;
    unsigned long long int c, dP, dQ, m1, m2;

    FILE *private_key_file = fopen("private_key.txt", "r");
    if (private_key_file == NULL) {
        printf("Error opening private_key.txt\n");
        return;
    }

    // Read private key components from file
    if (fscanf(private_key_file, "%d, %d", &n, &d) != 2) {
        printf("Error reading private key\n");
        fclose(private_key_file);
        return;
    }   

    printf("Private Key: (n, d) = (%d, %d)\n", n, d);
    fclose(private_key_file);   

    // Read prime numbers from file
    FILE *p_q_file = fopen("p_q_file.txt", "r");
    if (p_q_file == NULL) {
        printf("Error opening p_q.txt\n");
        return;
    }

    if (fscanf(p_q_file, "%d, %d", &p, &q) != 2) {
        printf("Error reading prime numbers\n");
        fclose(p_q_file);
        return;
    }

    printf("p: %d, q: %d\n", p, q);

    fclose(p_q_file);

    // Open output file to write
    FILE *output_fp = fopen(output_file, "w");
    if (output_fp == NULL) {
        printf("Error opening %s\n", output_file);
        return;
    }



    while (fscanf(input_fp, "%llu", &c) != EOF) {
        dP = crt_function(p, d);
        dQ = crt_function(q, d);
        qInv = inverse(q, p);
        m1 = rsa_decrypt(c, dP, p);
        m2 = rsa_decrypt(c, dQ, q);
        m1m2 = m1 - m2;
        if (m1m2 < 0)
            m1m2 += p;
        h = (qInv * m1m2) % p;
        m = m2 + h * q;

        // Write decrypted message to output file
        fprintf(output_fp, "%c", m);
    }



    // Close output file
    fclose(output_fp);

    printf("Decryption complete. Decrypted message written to %s\n", output_file);
}