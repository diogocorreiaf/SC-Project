#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <inttypes.h>

int gcdExtended(int a, int b, int* x, int* y);


uint32_t findD(uint16_t e, uint32_t phi)
{
	uint32_t eprev, dprev, d = 1, etemp, dtemp;

	eprev = phi, dprev = phi;
	while (e != 1)
	{
		etemp = e;
		dtemp = d;
		e = eprev - eprev / etemp * e;
		d = dprev - eprev / etemp * d;
		eprev = etemp;
		dprev = dtemp;
		while (d < 0)
			d += phi;
	}

	return d;
}



uint32_t modInverse(uint16_t e, uint32_t phi) {
    int x, y;
    int g = gcdExtended(e, phi, &x, &y);
    if (g != 1)
        return 0; // Inverse doesn't exist
    else {
        // phi is added to handle negative x
        return (uint32_t)((x % phi + phi) % phi);
    }
}

// Function for extended Euclidean Algorithm
int gcdExtended(int a, int b, int* x, int* y) {
    // Base Case
    if (a == 0) {
        *x = 0, *y = 1;
        return b;
    }

    // To store results of recursive call
    int x1, y1;
    int gcd = gcdExtended(b % a, a, &x1, &y1);

    // Update x and y using results of recursive call
    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}

int main(){
uint16_t e = 7;
uint32_t phi = 20;

uint32_t d;

    d = findD(e,phi);
    printf("The value with Find D is: %PRIu32\n", d);    
    d = modInverse(e,phi);
    printf("The value with Mod Inverse is: %PRIu32\n", d);

return 0;
}