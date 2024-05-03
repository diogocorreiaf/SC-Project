#include "utils.h"
#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))
#define KEY_SIZE_BYTES 16


void initialize_aes_sbox(uint8_t sbox[256]) {
    uint8_t p = 1, q = 1;
    
    do {
        p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);
        q ^= q << 1;
        q ^= q << 2;
        q ^= q << 4;
        q ^= q & 0x80 ? 0x09 : 0;
        uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);
        sbox[p] = xformed ^ 0x63;
    } while (p != 1);
    
    sbox[0] = 0x63;
}

void initialize_inverse_aes_sbox(uint8_t inv_sbox[256]) {
    uint8_t p = 1, q = 1;
    
    do {
        p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);
        q ^= q << 1;
        q ^= q << 2;
        q ^= q << 4;
        q ^= q & 0x80 ? 0x09 : 0;
        uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 3) ^ ROTL8(q, 6); // Use the inverse transformation
        inv_sbox[p] = xformed;
    } while (p != 1);
    
    inv_sbox[0] = 0x05; // Inverse of 0x63
}



uint8_t xtime(uint8_t x) {
    return (x & 0x80) ? ((x << 1) ^ 0x1B) : (x << 1);
}
char* xor(char *a, char *b) {
    unsigned long long int bin_a = strtoull(a, NULL, 16);
    unsigned long long int bin_b = strtoull(b, NULL, 16);
    
    unsigned long long int result = bin_a ^ bin_b;
    
    static char ans[9];
    sprintf(ans, "%08llx", result);
    
    return ans;
}

char *xor_strings(const char *str1, const char *str2) {
    size_t len = strlen(str1);
    char *result = malloc(len + 1);
    for (size_t i = 0; i < len; i++) {
        result[i] = str1[i] ^ str2[i];
    }
    result[len] = '\0';
    return result;
}


bool coprime_check(uint16_t num1, uint32_t num2){
    
    for (uint16_t i = num1; i > 0; i--)
	{
		if (num1 % i == 0 && num2 % i == 0)
			return true;
	}

    return false;
}

bool primenumber(uint16_t  number) { 
    int i;  
    for (i = 2; i <= number - 1; i++)  
    { 
        if (number % i == 0) 
            return false; 
    } 
    return true; 
} 


uint32_t modInverse(uint16_t e, uint32_t phi) {
    int x, y;
    int g = gcdExtended(e, phi, &x, &y);
    if (g != 1)
        return 0;
    else {
        return (uint32_t)((x % phi + phi) % phi);
    }
}

// Function for extended Euclidean Algorithm
int gcdExtended(int a, int b, int* x, int* y) {
    if (a == 0) {
        *x = 0, *y = 1;
        return b;
    }
    int x1, y1;
    int gcd = gcdExtended(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}

unsigned long long int crt_function(uint16_t i, uint32_t d){
unsigned long long int d_2;
    
    d_2 = d % (i - 1);
    return d_2;
}



uint32_t extendedEuclidean(uint32_t a, uint32_t b, int32_t *x, int32_t *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }

    int32_t x1, y1;
    uint32_t gcd = extendedEuclidean(b, a % b, &x1, &y1);

    *x = y1;
    *y = x1 - (a / b) * y1;

    return gcd;
}

uint32_t findD(uint16_t e, uint32_t phi) {
    int32_t x, y;
    extendedEuclidean(phi, e, &x, &y);
    
    while (y < 0) {
        y += phi;
    }

    return (uint32_t)y;
}