#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <gmp.h>

#include "cryptoEngine.h"

mpz_t p, a, d;

void setup(){

	mpz_init(p);
	mpz_init(a);
	mpz_init(d);

	mpz_t base, nineteen, minusOne, nominator, denominator;
	unsigned int exp = 255;

	mpz_init(base);
	mpz_init(nineteen);
	mpz_set_ui(base, 2);
	mpz_set_ui(nineteen, 19);
	mpz_pow_ui(p, base, exp);
	mpz_sub(p, p, nineteen);

	mpz_init(minusOne);
	mpz_set_str(minusOne, "-1", 10);
	mpz_mod(a, minusOne, p);

	mpz_init(nominator);
	mpz_init(denominator);
	mpz_set_ui(nominator, 121665);
	mpz_set_ui(denominator, 121666);
	coecc_div(d, nominator, denominator);

	mpz_clear(base);
	mpz_clear(nineteen);
	mpz_clear(minusOne);
	mpz_clear(nominator);
	mpz_clear(denominator);
}

void init_normalCoord
	(
		tecPoint *point,
		char *xCoord,
		char *yCoord
	){

	mpz_init(point->x);
	mpz_init(point->y);

	mpz_set_str(point->x, xCoord, 10);
	mpz_set_str(point->y, yCoord, 10);
}

void init_extendedCoord
	(
		extendedTecPoint *point,
		char *xCoord,
		char *yCoord,
		char *tCoord,
		char *zCoord
	){

	mpz_init(point->x);
	mpz_init(point->y);
	mpz_init(point->t);
	mpz_init(point->z);

	mpz_set_str(point->x, xCoord, 10);
	mpz_set_str(point->y, yCoord, 10);
	mpz_set_str(point->t, tCoord, 10);
	mpz_set_str(point->z, zCoord, 10);
}

void coecc_add
	(
		mpz_t dest,
		mpz_t src1,
		mpz_t src2
	){

	mpz_add(dest, src1, src2);
	mpz_mod(dest, dest, p);
}

void coecc_sub
	(
		mpz_t dest,
		mpz_t src1,
		mpz_t src2
	){

	mpz_sub(dest, src1, src2);
	mpz_mod(dest, dest, p);
}

void coecc_mul
	(
		mpz_t dest,
		mpz_t src1,
		mpz_t src2
	){

	mpz_mul(dest, src1, src2);
	mpz_mod(dest, dest, p);
}

void coecc_div
	(
		mpz_t dest,
		mpz_t src1,
		mpz_t src2
	){

	mpz_t temp;
	mpz_init(temp);

	mpz_invert(temp, src2, p);
	mpz_mul(dest, src1, temp);
	mpz_mod(dest, dest, p);
}

void coecc_tecPointAddition
	(
		extendedTecPoint *dest,
		extendedTecPoint *point1,
		extendedTecPoint *point2
	){

	mpz_t A, B, C, D, E, F, G, H, temp1, temp2;

	mpz_init(A);
	mpz_init(B);
	mpz_init(C);
	mpz_init(D);
	mpz_init(E);
	mpz_init(F);
	mpz_init(G);
	mpz_init(H);
	mpz_init(temp1);
	mpz_init(temp2);

	coecc_mul(A, point1->x, point2->x);
	coecc_mul(B, point1->y, point2->y);
	coecc_mul(C, point1->t, point2->t);
	coecc_mul(C, C, d);
	coecc_mul(D, point1->z, point2->z);
	coecc_add(temp1, point1->x, point1->y);
	coecc_add(temp2, point2->x, point2->y);
	coecc_mul(E, temp1, temp2);
	coecc_sub(E, temp1, A);
	coecc_sub(E, temp2, B);
	coecc_sub(F, D, C);
	coecc_add(G, D, C);
	coecc_mul(temp1, a, A);
	coecc_sub(H, B, temp1);
	coecc_mul(dest->x, E, F);
	coecc_mul(dest->y, G, H);
	coecc_mul(dest->t, E, H);
	coecc_mul(dest->z, F, G);

	mpz_clear(A);
	mpz_clear(B);
	mpz_clear(C);
	mpz_clear(D);
	mpz_clear(E);
	mpz_clear(F);
	mpz_clear(G);
	mpz_clear(H);
	mpz_clear(temp1);
	mpz_clear(temp2);
}

tecPoint coecc_scalarMul
	(
		int k,
		tecPoint* point
	){

	extendedTecPoint P, Q;

	init_extendedCoord(&P, "0", "0", "0", "0");
	init_extendedCoord(&Q, "0", "1", "0", "1");

	mpz_set(P.x, point->x);
	mpz_set(P.y, point->y);
	mpz_t temp;
	mpz_init(temp);
	coecc_mul(temp, point->x, point->y);
	mpz_set(P.t, temp);
	mpz_set_ui(P.z, 1);

	int lg2 = 0, n = k, i, j = 0;

	while(n > 0){

		n = n >> 1;
		lg2++;
	}

	int *bin = malloc(sizeof(int)*lg2);

	for(i = lg2 - 1; i >= 0; i--){

		*(bin+j++) = (k >> i) & 1;
	}

	for(i = 0; i < lg2; i++){

		coecc_tecPointAddition(&Q, &Q, &Q);

		if(*(bin+i)){

			coecc_tecPointAddition(&Q, &Q, &P);
		}
	}

	free(bin);
	mpz_clear(temp);

	tecPoint result;
	mpz_t x, y;
	char x_str[128];
	char y_str[128];

	mpz_init(x);
	mpz_init(y);
	coecc_div(x, Q.x, Q.y);
	coecc_div(y, Q.y, Q.z);

	mpz_get_str(x_str, 10, x);
	mpz_get_str(y_str, 10, y);
	init_normalCoord(&result, x_str, y_str);

	return result;
}

keyPair keyPairGeneration
	(
		unsigned long long n,
		tecPoint* P
	){

	srand(time(NULL));
	unsigned long long d = rand()%ULONG_MAX;
	tecPoint Q = coecc_scalarMul(d, P);

	keyPair generatedKP;

	generatedKP.d = d;
	generatedKP.Q = Q;

	return generatedKP;
}

tecPoint coecc_pointAdd
	(
		tecPoint x,
		tecPoint y
	){

	//x1: x.x
	//y1: x.y
	//x2: y.x
	//y2: y.y

	tecPoint z;
	mpz_t temp1, temp2, x3, y3;

	coecc_sub(temp1, y.y, x.y);
	coecc_sub(temp2, y.x, x.x);
	coecc_div(temp1, temp1, temp2);
	coecc_mul(temp2, temp1, temp1);
	coecc_sub(x3, temp2, x.x);
	coecc_sub(x3, x3, y.x);

	coecc_sub(temp2, x.x, x3);
	coecc_mul(y3, temp1, temp2);
	coecc_sub(y3, y3, x.y);

	mpz_clear(temp1);
	mpz_clear(temp2);

	mpz_set(z.x, x3);
	mpz_set(z.y, y3);

	return z;
}

tecPoint* coecc_enc
	(
		tecPoint* P,
		tecPoint Q
	){

	unsigned long long t = rand()%ULONG_MAX;
	tecPoint M = coecc_scalarMul(t, P);

	mpz_t m;
	mpz_set(m, M.x);
	
	//aes_enc(m, plain); //deprecated - migrated to java-aes

	unsigned long long k = rand()%ULONG_MAX;
	tecPoint C1 = coecc_scalarMul(k, P);

	tecPoint temp = coecc_scalarMul(k, &Q);

	tecPoint C2 = coecc_pointAdd(M, temp);

	tecPoint *Cs = malloc(sizeof(tecPoint)*2);
	Cs[0] = C1;
	Cs[1] = C2;
	return Cs;
}

void coecc_dec
	(
		tecPoint* Cs,
		unsigned long long d
	){

	tecPoint C1_negated;

	mpz_t minusOne;
	mpz_init(minusOne);
	mpz_set_str(minusOne, "-1", 10);

	coecc_mul(C1_negated.x, Cs[0].x, minusOne);
	mpz_set(C1_negated.y, Cs[0].y);
	C1_negated = coecc_scalarMul(d, &C1_negated);

	tecPoint M;

	M = coecc_pointAdd(Cs[1], C1_negated);

	mpz_clear(minusOne);

	//cipher = aes_dec(M.x, plain) //deprecated - migrated to java-aes
}

/*
 * 1. Eğrinin üzerinde bir nokta seç: M <- t.P (t random)
 *    1.1. m <- M.x'i anahtar olarak kullanıp aes ile şifreleme yap
 *    1.2. cipher = aes_enc(m, plain)
 * 2.
 * 3.
 * 4.
 * 5. return C1, C2, cipher
 *
 * dec: 1.1. plain = aes_dec(m, cipher)
 * return plain
 *
 * A - B = A + (-B)
 * -(x,y) = (-x,y)
 */
