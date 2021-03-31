/*
 * This header file declares the functions and structs used in the Crypto System implementation
 *
 * */

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
		unsigned long long,
		tecPoint*
	);

void keyPairGeneration
	(
		tecPoint*
	);

void coecc_encryption
	(
		char*,
		tecPoint*
	);

#endif /* CRYPTOENGINE_H_ */
