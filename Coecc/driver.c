#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#include "cryptoEngine.h"

int main(){

	tecPoint testPoint1;

	init_normalCoord(&testPoint1, "162364632", "27347237423");

	demet(91, &testPoint1);

	return 0;
}
