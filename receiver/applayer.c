#include "applayer.h"

int receiveFile(int conn, int file) {
	llopen(conn);

	int i;
	for(i = 0; i < 15000; i = i + 10)
		llread(conn, file);

	llclose(conn);
	
	return 0;
}
