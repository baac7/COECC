#include <stdio.h>
#include <stdlib.h>
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

	gmp_printf("p is equal to %Zd\na is equal to %Zd\nd is equal to %Zd\n", p, a, d);

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

void demet
	(
		int k,
		tecPoint *point
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

	//return Q(X)/Q(Z) as x, Q(Y)/Q(Z) as y
}
