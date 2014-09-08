#include <stdio.h>
#include <gmp.h>
#include "RSACrypto.h"

/*
*   It is a function that generate random number
*   input: variable to be returned(mpz_t) and the bitsize(int) of the number
*   output: variable where argument in main function must be void e.g. int main(void)
*/
void RandomNumber(mpz_t num, int bitsize){
    unsigned long int seed;
    gmp_randstate_t state;
    srand(time(NULL));
    seed = rand();
    gmp_randinit_default (state);
    gmp_randseed_ui(state, seed);
    mpz_urandomb(num, state, bitsize);
}

/*
*   It is a function that checks if a (large) number is odd or even
*   input: number(mpz_t) to be tested
*   output: int 1 and 0 which represents odd and even number respectively
*/
int CheckOdd(mpz_t n){
    mpz_t r;
    mpz_init(r);
    mpz_cdiv_r_ui(r, n, 2); //n divides by 2 get r
    if(mpz_cmp_ui(r, 0) == 0) return 0;
    else return 1;
}

// Global default array to be used for trial division
int testList[] = {3,5,7,11,13,17,19,23,29,31};

/*
*   It is a function that is used to set new testList for the trial division
*   input: int array of size 10
*   output: none
*/
void setDNum(int newTestList[]){
    int i;
    for(i=0; i<10; i++){
        testList[i] = newTestList[i];
    }
}

/*
*   It is a function that is used to get the list of integer used for the trial division
*   input: int array of size 10
*   output: int array of the input
*/
void getDNum(int getList[]){
    int i;
    for(i=0; i<10; i++){
        getList[i] = testList[i];
    }
}

/*
*   It is a function that does trial division on the number
*   input: number(mpz_t) to be tested
*   output: int 1 and 0 which represents pass and fail the test respectively
*   The array of trial division number can be change as by overriding the global variable,
*   testList, which is {3,5,7,11,13,17,19,23,29,31} by default
*/
int TrialDivision(mpz_t num){
    mpz_t check;
    mpz_init(check);
    int arraysize = sizeof(testList) / sizeof(testList[0]);
    int i;
    for(i = 0; i < arraysize; i++){
        mpz_cdiv_r_ui(check, num, testList[i]);
        if(mpz_cmp_ui(check, 0) == 0) return 0;
    }
    return 1;
}

/*
*   It is a function that performs Miller Rabin Primality test
*   input: number(mpz_t) to be tested and number of iterations(int) to run the test
*   output: int 1 and 0 which represents pass and fail the test respectively
*/
int Miller(mpz_t p, int iterations){
    mpz_t a;
    mpz_init(a);
    mpz_t p4;
    mpz_init(p4);
    mpz_sub_ui(p4, p, 4);
    mpz_t p1;
    mpz_init(p1);
    mpz_sub_ui(p1, p, 1);
    mpz_t x;
    mpz_init(x);
    mpz_t m;
    mpz_init_set_str(m, "1", 10);
    mpz_t val;
    mpz_init(val);

    gmp_randstate_t state;
    srand(time(NULL));
    int seed = rand();
    gmp_randinit_default (state);
    gmp_randseed_ui(state, seed);

    //find t and m
    int check = 0;
    int t = 1;
    while(check != 1){
        mpz_ui_pow_ui(val, 2, t);
        mpz_cdiv_r(m,p1,val);
        if(mpz_cmp_ui(m,0) == 0){
            mpz_cdiv_q(m,p1,val);
            check = 1;
        }
        else{
            t++;
        }
    }

    //step 1
    mpz_urandomb(a,state,64);
    mpz_mod(a, a, p4);
    mpz_add_ui(a, a, 2); //base must not be bigger than p - 2
    //gmp_printf("a = %Zd\n", a);

    //step 2
    mpz_powm(x, a, m, p);
    //gmp_printf("x = %Zd\n", x);
    if(mpz_cmp_ui(x, 1) == 0 || mpz_cmp(x, p1) == 0) return 1;
    else if(t == 1) return 0;
    int j = 1;

    //step 3 & 4
    int capture;
    do
    {
        printf("what is t, %d\n", t);
        capture = step3(j,m,x,a,p,p1);
        printf("capture = %d\n", capture);
        if(capture == 0) return 0;
        else if(capture == 1) return 1;
        else{
            j = capture;
            printf("here when j = %d\n", j);
        }
    }while(j < (t-1));

    //step 5
    capture = step3(j+1, m, x, a, p, p1);
    if(capture == 1) return 1;
    else return 0;

}

/* This function is used by Miller */
int step3(int j, mpz_t m, mpz_t x, mpz_t a, mpz_t p, mpz_t p1){
    int newm;
    int somethi = (int) pow(2.0,(double)j);
    printf("something = %d\n", somethi);
    if(mpz_fits_slong_p(m) == 0){
        printf("this is problematic cause m is too huge for gmp to handle");
        return 0;
    }
    else{
        newm = mpz_get_si(m);

    }
    somethi = somethi*newm;
    printf("something (after new M) = %d\n", somethi);
    gmp_printf("x (before mod): %Zd\t & p1: %Zd\n", x, p1);
    mpz_powm_ui(x, a, somethi, p);
    gmp_printf("x (step 3) = %Zd\n", x);
    if(mpz_cmp_ui(x, 1) == 0){
        printf("entered if\n");
        return 0;
    }
    else if(mpz_cmp(x, p1) == 0){
        printf("enter else if\n");
        return 1;
    }
    else{
        printf("enter else, let's see j's value, %d\n", j);
        return ++j;
    }
}

/*
*   It is a functions that performs Fermat Little Theoem
*   input: number(mpz_t) to be tested and number of iterations(int) to run the test
*   output: int 1 and 0 which represents pass and fail the test respectively
*/
int Fermat(mpz_t p, int iterations){
    mpz_t rand_Num; //use as base, a
    mpz_init(rand_Num);
    mpz_t ans; //ans = a^(p-1) mod p
    mpz_init(ans);
    mpz_t p2; //p2 = p - 1
    mpz_init(p2);
    mpz_sub_ui(p2, p, 1);
    if(mpz_cmp_ui(p,1)== 0) return 0;
    gmp_randstate_t state;
    srand(time(NULL));
    int seed = rand();
    gmp_randinit_default (state);
    gmp_randseed_ui(state, seed); //prepare random seed
    int i;
    for(i =0; i<iterations; i++){
        mpz_urandomb(rand_Num,state,64);
        mpz_mod(rand_Num, rand_Num, p2); //base must not be bigger than p
        mpz_add_ui(rand_Num, rand_Num, 1); //must be at least 1
        mpz_powm(ans, rand_Num, p2, p);
        if(mpz_cmp_ui(ans, 1) != 0) return 0;
    }
    return 1;
}

/*
*   It is a function that generates random prime numbers
*   input: prime (mpz_t) for retrieving the prime and bitUsed for determining the bits size
*   output: mpz_t prime will be captured in host if parameter of main is void, e.g. main(void)
*/
void generatePrime(mpz_t prime, int bitUsed){
    int check;
    mpz_t rop;
    unsigned long int seed;
    gmp_randstate_t state;
    srand(time(NULL));
    seed = rand();
    gmp_randinit_default (state);
    gmp_randseed_ui(state, seed);
    mpz_init(rop);
    int cond = 1;
    for(check = 0; check != 1;)
    {
        switch(cond){
        case 1:
            mpz_urandomb(rop, state, bitUsed);
            printf(".");
            if(CheckOdd(rop) == 1){
                cond = 2;
            }else{
                cond = 1;
            }
            break;
        case 2:
            if(TrialDivision(rop) == 1){
                cond = 3;
            }else{
                cond = 1;
            }
            break;
        case 3:
            if(Miller(rop,10) == 1){
                check = 1;
            }else{
                cond = 1;
            }
            break;
        }
    }
    printf("\n");
    mpz_set(prime, rop);
    gmp_randclear(state);
    mpz_clear(rop);
    return 0;
}

/*
*   It is a function that performs binomial theorem/expansion
*   (a+b)^n = {n "summation" k} [n k]*a^(n-k)b*^k
*   input: mpz_t n, mpz_t k and mpz_t *bin(pointer) to retrieve the output
*   output: mpz_t *bin as a pointer
*/
void binomial(mpz_t n, mpz_t k, mpz_t *bin){
    mpz_t f1;
    mpz_t f2;
    mpz_t f3;
    mpz_t temp;
    mpz_t denominator;
    mpz_init(f1);
    mpz_init(f2);
    mpz_init(f3);
    mpz_init(temp);
    mpz_init(denominator);
    factorial(k, &f1);
    mpz_sub(temp, n, k);
    factorial(temp, &f2);
    mpz_mul(denominator, f1, f2);
    //gmp_printf("%Zd\n", denominator);
    factorial(n, &f3);
    //gmp_printf("%Zd\n", f3);
    mpz_cdiv_q(*bin, f3, denominator);
}

/*
*   It is a function that performs simple factorization, n!
*   input: mpz_t n and mpz_t *bin(piinter) to retrieve the output
*   output: mpz_t *bin as a pointer
*/
void factorial(mpz_t n, mpz_t *f){
    mpz_t c;
    mpz_init(c);
    mpz_set_ui(f, 1);
    for(mpz_set_ui(c, 1); mpz_cmp(c, n) <= 0; mpz_add_ui(c,c, 1)){
        mpz_mul(*f, *f, c);
    }
    mpz_clear(c);
}

/*
*   It is a function that find small value of e(private key) for RSA algorithm
*   input: mpz_t e and mpz_t m
*   output: mpz_t e
*/
void findESimple(mpz_t e, mpz_t m){
    int checklist[] = {71,67,61,59,53,47,43,41,37,31,29,23,19,17,13,11,7,5,3,2};
    mpz_t storedValue;
    mpz_t copyM; //so that GCD would not change n's value
    mpz_init(copyM);
    mpz_set(copyM, m);
    mpz_init(storedValue);
    int i;
    for(i = 0; i < 20; i++)
    {
        mpz_set_ui(storedValue, checklist[i]);
        GCDlarge(storedValue, copyM);
        if(mpz_cmp_ui(storedValue, 1) == 0){
            mpz_set_ui(e, checklist[i]);
            break;
        }
    }
}

/*
*   It is a function that generates the keys for RSA algorithm
*   input: mpz_t e, mpz_t, d, mpz_t n and int bitUsed;
*   output: mpz_t e, mpz_t d, mpz_t n
*/
void findRSAKeys(mpz_t e, mpz_t d, mpz_t n, int bitUsed){
    mpz_t p;
    mpz_init(p);
    mpz_t q;
    mpz_init(q);
    //mpz_t n;
    //mpz_init(n);
    mpz_t p1;
    mpz_init(p1);
    mpz_t q1;
    mpz_init(q1);
    mpz_t eulerN;
    mpz_init(eulerN);
    //steps according to wikipedia
    //step 1
    generatePrime(p, bitUsed);
    while(mpz_cmp_ui(q, 0) == 0 || mpz_cmp(p, q) == 0){
        generatePrime(q, bitUsed);
    }
    //step 2
    mpz_mul(n,p,q);
    //step 3
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(eulerN, p1, q1);
    //step 4
    //mpz_t e;
    //mpz_init(e);
    findESimple(e, eulerN);
    //gmp_printf("e is %Zd\n", e);
    //step 5
    mpz_t g, s;
    mpz_init(g);
    mpz_init(s);
    mpz_gcdext(g, s, d, eulerN, e);
}

/*
*   It is a function that to perform GCD for large numbers
*   input: mpz_t a, mpz_t b
*   output: mpz_t a
*/
void GCDlarge(mpz_t a, mpz_t b){
    mpz_t c;
    mpz_t neg;
    mpz_init(c);
    mpz_init_set_str(neg, "-1", 10);
    while(mpz_cmp_ui(b,0) != 0){
        mpz_cdiv_r(c, a, b);
        mpz_set(a, b);
        mpz_set(b, c);
    }
    if(mpz_cmp_ui(a, 0) < 0){
        mpz_mul(a, a, neg);
    }
}

/*
*   It is a function that performs Extended Euclid
*   input: mpz_t of a, b, s and t
*   output: variable a as the GCD and give s and t values
*/
void extendedEuclid(mpz_t a, mpz_t b, mpz_t s, mpz_t t){
    mpz_t q, r, r1, r2,s1,s2,t1,t2, temp;
    mpz_init(q);
    mpz_init(r1);
    mpz_init(r2);
    mpz_init(temp);
    mpz_set(r1, a);
    mpz_set(r2, b);
    mpz_init_set_str(r, "1", 10); //to prevent r from inheriting other value
    mpz_init_set_str(s1, "1", 10);
    mpz_init_set_str(s2, "0", 10);
    mpz_init_set_str(t1, "0", 10);
    mpz_init_set_str(t2, "1", 10);
    while(mpz_cmp_ui(r2,0) != 0){
        mpz_cdiv_q(q, r1, r2);
        mpz_cdiv_r(r, r1, r2);
        mpz_mul(temp, q, s2);
        mpz_sub(s, s1, temp);
        mpz_mul(temp, q, t2);
        mpz_sub(t, t1, temp);
        mpz_set(r1, r2);
        mpz_set(r2, r);
        mpz_set(s1, s2);
        mpz_set(s2, s);
        mpz_set(t1, t2);
        mpz_set(t2, t);
    }
    mpz_set(s, s1);
    mpz_set(t, t1);
    mpz_set(a, r1);
}
