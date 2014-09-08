#include <stdio.h>
#include <stdlib.h>

#include <gmp.h>
#include "RSACrypto.h"

int main(void){
    //Example 1: Generating a 8-bit random number
    mpz_t num;
    mpz_init(num);
    int bitsize = 8;
    RandomNumber(num, bitsize);
    gmp_printf("Random number is %Zd\n", num);

    //Example 2: Check if number from Example 1 is odd or even
    if(CheckOdd(num)==1){
        gmp_printf("%Zd  is  an odd number\n", num);
    }
    else{
        gmp_printf("%Zd  is  an even number\n", num);
    }

    //Example 3a: Check if number from Example 1 passes the TrialDivision by the default numbers given
    if(TrialDivision(num)==1){
        gmp_printf("%Zd is not divisible by ",num);
    }
    else{
        gmp_printf("%Zd is divisible by ", num);
    }
    int newTest2[10];
    getDNum(newTest2);
    int i;
    printf("[ ");
    for(i=0; i < 9; i++){
        printf("%d, ", newTest2[i]);
    }
    i = 9;
    printf("%d]\n", newTest2[i]);

    //Example 3b: Check if number from Example passes the TrialDivision by changing the numbers to be tested
    int newTest[10] = {37,41,43,47,53,59,61,67,71,73};
    setDNum(newTest);
    if(TrialDivision(num)==1){
        gmp_printf("%Zd is not divisible by ",num);
    }
    else{
        gmp_printf("%Zd is divisible by ", num);
    }
    newTest2[10];
    getDNum(newTest2);
    printf("[ ");
    for(i=0; i < 9; i++){
        printf("%d, ", newTest2[i]);
    }
    i = 9;
    printf("%d]\n", newTest2[i]);

    //Example 4: Check if 153 passes the Miller Rabin Primality test
    int iterations = 3;
    mpz_set_str(num, "153", 10);
    if(Miller(num, iterations)==1){
        gmp_printf("%Zd passes the Miller Rabin Primality test with %d iterations\n", num, iterations);
    }
    else{
        gmp_printf("%Zd fails the Miller Rabin Primality test with %d iterations\n", num, iterations);
    }

    //Example 5: Check if 153 passes the Fermat Little Theorem Primality test
     if(Fermat(num, iterations)==1){
        gmp_printf("%Zd passes the Fermat Little Theorem with %d iterations\n", num, iterations);
    }
    else{
        gmp_printf("%Zd fails the Fermat Little Theorem with %d iterations\n", num, iterations);
    }

   //Example 6: Generate 1024-bit prime number
    int bitUsed = 1024;
    mpz_t prime;
    mpz_init(prime);
    generatePrime(prime, bitUsed);
    gmp_printf("The prime number with a bit size of %d is %Zd\n",bitUsed, prime);

    //Example 7: Find coefficient when n = 17, k = 3 using binomial theorem
    mpz_t n;
    mpz_t k;
    mpz_t bin;
    mpz_init(n);
    mpz_init(k);
    mpz_init(bin);
    mpz_set_ui(n, 17);
    mpz_set_ui(k, 3);
    binomial(n, k, &bin);
    gmp_printf("The coefficient when n = %Zd and k = %Zd is %Zd\n",n,k,bin);

    //Example 8: Factorial of 17.
    factorial(n, &bin);
    gmp_printf("The factorial of %Zd! is %Zd\n",n, bin);

    //Example 9: Find private key of m = 97
    mpz_t e;
    mpz_init(e);
    mpz_t m;
    mpz_init_set_str(m, "97", 10);
    findESimple(e, m);
    gmp_printf("Private key of m = %Zd is %Zd \n", m, e);

    //Example 10: Find GCD of 521489185 and 135913531325
    mpz_set_str(e, "521489185", 10);
    mpz_set_str(m, "135913531325", 10);
    gmp_printf("The GCD of %Zd and %Zd is ",e,m);
    GCDlarge(e, m);
    gmp_printf("%Zd\n", e);

    //Example 11: Find RSA keys with bit size of 1024
    mpz_t d;
    mpz_init(d);
    findRSAKeys(e, d, n, bitUsed);
    gmp_printf("e is %Zd\n", e);
    gmp_printf("d is %Zd\n", d);
    gmp_printf("n is %Zd\n", n);

    //Example 12: Using Extended Euclidean with a = 8 and b = 12
    mpz_t a, b, s, t;
    mpz_init(s);
    mpz_init(t);
    mpz_init_set_str(a, "8", 10);
    mpz_init_set_str(b, "12", 10);
    extendedEuclid(a,b,s,t);
    gmp_printf("Extended Euclid : GCD is %Zd, s is %Zd, t is %Zd\n", a, s, t);

    //Example 13: Converting values to base 64 and export as text
    FILE *fp;
    fp = fopen("d:\\text.txt", "w");
    mpz_out_str(fp, 64, n);
    fclose(fp);



}
