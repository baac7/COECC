#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Interface_GenerateKeyC.h"
#include "cryptoEngine.h"

jstring JNICALL Java_Interface_GenerateKeyC_secretKey (JNIEnv * env, jobject this){

	setup();

	tecPoint P;

	init_normalCoord(&P, "3736021618437571", "5374244857966608");

	keyPair kp = keyPairGeneration(5883, &P);

	mpz_t mul;
	mpz_init(mul);
	coecc_mul(mul, kp.Q.x, kp.Q.y);

	size_t size = 16;

	char* str = malloc(size);
	char* res = malloc(2);
	mpz_get_str(str, 10, mul);

	int i, j = 0, n = strlen(str);

	for(i = size-1; i >= 0; i--){
		res[j++] = str[n-i-1];
	}

	mpz_clear(kp.Q.x);
	mpz_clear(kp.Q.y);
	mpz_clear(mul);

	jstring jkey = (*env)->NewStringUTF(env, res);
	return jkey;
}
