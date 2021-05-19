#ifndef CRYPTOENGINE_H_
#define CRYPTOENGINE_H_

#include <gmp.h>

typedef struct normalCoord{
	mpz_t x;
	mpz_t y;
}tecPoint;

typedef struct extendedCoord{
	mpz_t x;
	mpz_t y;
	mpz_t t;
	mpz_t z;
}extendedTecPoint;

typedef struct keyPair{
	unsigned long long d;	//private key
	tecPoint Q;				//public key
}keyPair;

void setup();

void init_normalCoord
	(
		tecPoint*,
		char*,
		char*
	);

void init_extendedCoord
	(
		extendedTecPoint*,
		char*,
		char*,
		char*,
		char *
	);

void coecc_add
	(
		mpz_t,
		mpz_t,
		mpz_t
	);

void coecc_sub
	(
		mpz_t,
		mpz_t,
		mpz_t
	);

void coecc_mul
	(
		mpz_t,
		mpz_t,
		mpz_t
	);

void coecc_div
	(
		mpz_t,
		mpz_t,
		mpz_t
	);

void coecc_tecPointAddition
	(
		extendedTecPoint*,
		extendedTecPoint*,
		extendedTecPoint*
	);

tecPoint coecc_scalarMul
	(
		int,
		tecPoint*
	);

keyPair keyPairGeneration
	(
		unsigned long long,
		tecPoint*
	);

tecPoint* coecc_enc
	(
		tecPoint*,
		tecPoint
	);

void coecc_dec
	(
		tecPoint*,
		unsigned long long
	);

#endif /* CRYPTOENGINE_H_ */
