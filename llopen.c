#include "logicallink.h"

void set() {
	puts("Trying again...");
	currentConnStage = CONNECTING;
}

void llopen() {
	unsigned char SET[5], UA[5];
	SET[0] = F;
	SET[1] = A;
	SET[2] = C;
	SET[3] = SET[1] ^ SET[2];
	SET[4] = F;

	puts("Setting alarm signal...");
	signal(SIGALRM, set); //install set routine
	puts("Alarm signal set.");
}
