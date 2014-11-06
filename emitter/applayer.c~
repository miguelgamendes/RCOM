#include "applayer.h"

int sendFile(int conn, int file) {
	unsigned char buf[15000];
	unsigned char data[10];
	int currentBufSize = 1;

	if(llopen(conn))
		return 1;
	
	int i, j;
	while(1) {
		//for(j = 0; j < 10; j++)
			//data[j] = buf[i + j];
		currentBufSize = read(file, data, sizeof(data));
		if(currentBufSize == 0)
			break;
		printf("Contents read from file: %s | BufSize: %d\n", data, currentBufSize);
		if(llwrite(conn, data, currentBufSize))
			return 1;
	}

	llclose(conn);

	return 0;
}
