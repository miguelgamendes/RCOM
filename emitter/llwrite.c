#include "llwrite.h"

int llwrite(int fd, unsigned char** data, int dataSize) {
	int res = 0;
	int resSum = 0;
	int connecting = 1;

	//frame creation
	unsigned char Iheader[3], Itail[2], RR[5];
	Iheader[0] = F;
	Iheader[1] = A;
	Iheader[2] = C;
	Itail[0] = Iheader[1] ^ Iheader[2];
	Itail[1] = F;

	//machine state periferal vars
	int stop = 1, estado = 0, res2;
	unsigned char c;

	*data = malloc(sizeof(char) * 5);
	*data = "aaaaa";
	
	//sending I frame
	puts("Sending I frame...");
	printf("Data contents being sent: %s\n", *data);
	res = write(fd,Iheader,3);
	resSum += res;
	if(res != -1) {
	} else {
		puts("Connection establishment failed.");
		return 1;
	}

	res = write(fd,*data,dataSize);
	resSum += res;
	if(res != -1) {
	} else {
		puts("Connection establishment failed.");
		return 1;
	}

	res = write(fd,Itail,2);
	resSum += res;
	if(res != -1) {
		printf("%d bytes written\n", resSum);
	} else {
		puts("Connection establishment failed.");
		return 1;
	}
			
	//receiving UA frame
	puts("Reading response frame...");
	while(stop){
		res = read(fd,&c,1);
		if(res > 0) {
			alarm(0);
			switch(estado){
				case 0: {
					printf("estado 0\n");
					if (c == F){
						RR[0] = c;
						estado = 1;
					}
					break;
				}
				case 1:{
					printf("estado 1\n");
					if (c == A){
						RR[1] = c;
							estado = 2;
					} else if (c == F){
						estado = 1;
						RR[0] = c;
					} else 
						estado = 0;
					break;	
				}
				case 2:{				
					printf("estado 2\n");
					if (c == CS){
						RR[2] = c;
						estado = 3;
					} else if (c == CSR) {
						RR[2] = c;
						estado = 5;
					} else if (c == F){
						RR[0] = c;
						estado = 1;
					} else
						estado = 0; 
					break;
				}
				case 3:{
					printf("estado 3: %x\n", c);
					if (c == (RR[1] ^ RR[2])){
						RR[3] = c;
						estado = 4;
					} else if (c == F){
						RR[0] = c;
						estado = 1;
					} else 
						estado = 0;
					break;
				}
				case 4:{
					printf("estado 4\n");
					if (c == F){
						RR[4] = c;
						if (RR[0] == F && RR[1] == A && RR[2] == CS && RR[3] == (RR[1] ^ RR[2]) && RR[4] == F){
							puts("Received RR frame successfully.");
							stop = 0;
							puts("Finished reading.");
							break;
						} else {
							puts("REJ frame is wrong, somehow.");
							estado = 0;
						}
					} else
						estado = 0;
					break;
				}
				case 5:{
					puts("estado 5");
					if (c == F){
						RR[4] = c;
						if (RR[0] == F && RR[1] == A && RR[2] == CSR && RR[3] == (RR[1] ^ RR[2]) && RR[4] == F){
							puts("Received REJ frame successfully.");
							stop = 0;
							puts("Finished reading.");
							break;
						} else {
							puts("RR frame is wrong, somehow.");
							estado = 0;
						}
					} else
						estado = 0;
					break;
				}
			}
		}

		if(estado == 4 || estado == 5)
			break;
	}

	return 0;
}
