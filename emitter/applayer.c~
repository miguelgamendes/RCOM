#include "applayer.h"

int sendFile(int conn, int file) {
	unsigned char buf[15000];
	unsigned char data[10];
	int currentBufSize = 1;

	llopen(conn);
	
	int i, j;
	while(1) {
		//for(j = 0; j < 10; j++)
			//data[j] = buf[i + j];
		currentBufSize = read(file, data, sizeof(data));
		if(currentBufSize == 0)
			break;
		printf("Contents read from file: %s | BufSize: %d\n", data, currentBufSize);
		llwrite(conn, data, currentBufSize);
	}

	llclose(conn);

	return 0;
}
