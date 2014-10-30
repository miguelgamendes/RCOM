#include "applayer.h"

int sendFile(int conn, int file) {
	unsigned char data[11000];

	llopen(conn);
	
	read(file, data, sizeof(data));
	printf("Contents read from file: %s\n", data);
	llwrite(conn, data, 11000);

	llclose(conn);

	return 0;
}
