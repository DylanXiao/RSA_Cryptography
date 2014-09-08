#ifndef INCLUDE_H_INCLUDED
#define INCLUDE_H_INCLUDED

#include <stdio.h>
#include <gmp.h>

/*
*   It is a function that generate random number
*   input: variable to be returned(mpz_t) and the bitsize(int) of the number
*   output: variable where argument in main function must be void e.g. int main(void)
*/
void RandomNumber(mpz_t num, int bitsize);

/*
*   It is a function that checks if a (large) number is odd or even
*   input: number(mpz_t) to be tested
*   output: int 1 and 0 which represents odd and even number respectively
*/
int CheckOdd(mpz_t n);

/*
*   It is a function that is used to set new testList for the trial division
*   input: int array of size 10
*   output: none
*/
void setDNum(int newTestList[]);

/*
*   It is a function that is used to get the list of integer used for the trial division
*   input: int array of size 10
*   output: int array of the input
*/
void getDNum(int getList[]);

/*
*   It is a function that does trial division on the number
*   input: number(mpz_t) to be tested
*   output: int 1 and 0 which represents pass and fail the test respectively
*   The array of trial division number can be change as by overriding the global variable,
*   testList, which is {3,5,7,11,13,17,19,23,29,31} by default
*/
int TrialDivision(mpz_t num);


/*
*   It is a function that performs Miller Rabin Primality test
*   input: number(mpz_t) to be tested and number of iterations(int) to run the test
*   output: int 1 and 0 which represents pass and fail the test respectively
*/
int Miller(mpz_t p, int iterations);

/*
*   It is a functions that performs Fermat Little Theoem
*   input: number(mpz_t) to be tested and number of iterations(int) to run the test
*   output: int 1 and 0 which represents pass and fail the test respectively
*/
int Fermat(mpz_t p, int iterations);

/*
*   It is a function that generates random prime numbers
*   input: prime (mpz_t) for retrieving the prime and bitUsed for determining the bits size
*   output: mpz_t prime will be captured in host if parameter of main is void, e.g. main(void)
*/
void generatePrime(mpz_t prime, int bitUsed);

/*
*   It is a function that performs binomial theorem/expansion
*   (a+b)^n = {n "summation" k} [n k]*a^(n-k)b*^k
*   input: mpz_t n, mpz_t k and mpz_t *bin(pointer) to retrieve the output
*   output: mpz_t *bin as a pointer
*/
void binomial(mpz_t n, mpz_t k, mpz_t *bin);

/*
*   It is a function that performs simple factorization, n!
*   input: mpz_t n and mpz_t *bin(piinter) to retrieve the output
*   output: mpz_t *bin as a pointer
*/
void factorial(mpz_t n, mpz_t *f);

/*
*   It is a function that find small value of e(private key) for RSA algorithm
*   input: mpz_t e and mpz_t m
*   output: mpz_t e
*/
void findESimple(mpz_t e, mpz_t m);


/*
*  It is a function that generates the keys for RSA algorithm
*   input: mpz_t e, mpz_t, d, mpz_t n and int bitUsed;
*   output: mpz_t e, mpz_t d, mpz_t n
*/
void findRSAKeys(mpz_t e, mpz_t d, mpz_t n, int bitUsed);

/*
*   It is a function that to perform GCD for large numbers
*   input: mpz_t a, mpz_t b
*   output: mpz_t a
*/
void GCDlarge(mpz_t a, mpz_t b);

/*
*   It is a function that performs extended Euclid
*   input: int a and int b, and pointer int s and t
*   output: return int as the GCD and get s and t values through pointers
*/
void extendedEuclid(mpz_t a, mpz_t b, mpz_t s, mpz_t t);

#endif // INCLUDE_H_INCLUDED
