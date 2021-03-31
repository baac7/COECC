#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#include "cryptoEngine.h"

int main(){

	setup();

	tecPoint testPoint1, testPoint2;

	init_normalCoord(&testPoint1, "162364632", "27347237423");

	//testPoint2 = coecc_scalarMul(91, &testPoint1);
	//gmp_printf("%Zd\n%Zd", testPoint2.x, testPoint2.y);

	keyPairGeneration(&testPoint1);

	return 0;
}

