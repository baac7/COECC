#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#include "cryptoEngine.h"

int main(){

	setup();

	tecPoint testPoint1;

	init_normalCoord(&testPoint1, "162364632", "27347237423");

	functionToBeNamed1(91, &testPoint1);

	return 0;
}
