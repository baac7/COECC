#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#include "cryptoEngine.h"

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

	mpz_t p;
	mpz_init(p);
	mpz_set_ui(p, 7);

	mpz_add(dest, src1, src2);
	mpz_mod(dest, dest, p);
}

void coecc_sub
	(
		mpz_t dest,
		mpz_t src1,
		mpz_t src2
	){

	mpz_t p;
	mpz_init(p);
	mpz_set_ui(p, 7);

	mpz_sub(dest, src1, src2);
	mpz_mod(dest, dest, p);
}

void coecc_mul
	(
		mpz_t dest,
		mpz_t src1,
		mpz_t src2
	){

	mpz_t p;
	mpz_init(p);
	mpz_set_ui(p, 7);

	mpz_mul(dest, src1, src2);
	mpz_mod(dest, dest, p);
}

void coecc_div
	(
		mpz_t dest,
		mpz_t src1,
		mpz_t src2
	){

	mpz_t p;
	mpz_init(p);
	mpz_set_ui(p, 7);

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
	//C*=4
	coecc_mul(D, point1->z, point2->z);
	coecc_add(temp1, point1->x, point1->y);
	coecc_add(temp2, point2->x, point2->y);
	coecc_mul(E, temp1, temp2);
	coecc_sub(E, temp1, A);
	coecc_sub(E, temp2, B);
	coecc_sub(F, D, C);
	coecc_add(G, D, C);
	//temp1 = a*A
	coecc_sub(H, B, temp1);
	coecc_mul(dest->x, E, F);
	coecc_mul(dest->y, G, H);
	coecc_mul(dest->t, E, H);
	coecc_mul(dest->z, F, G);
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

		//coecc_tecPointAddition(&Q, &Q, &Q);

		if(*(bin+i)){

			//coecc_tecPointAddition(&Q, &Q, &P);
		}
	}

	free(bin);

	//return Q(X)/Q(Z) as x, Q(Y)/Q(Z) as y
}
