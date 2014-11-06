#include "llread.h"

int llread(int fd, int file){
	unsigned char Iheader[3], Itail[2], RR[5], REJ[5], c;
	int res, res2, estado = 0, stop = 1, dataIndex = 0;

	Iheader[0] = F;
	Iheader[1] = A;
	Iheader[2] = CS;
	Itail[0] = (Iheader[1] ^ Iheader[2]);
	Itail[1] = F;

	RR[0] = F;
	RR[1] = A;
	RR[2] = C;
	RR[3] = (RR[1] ^ RR[2]);
	RR[4] = F;

	REJ[0] = F;
	REJ[1] = A;
	REJ[2] = CSR;
	REJ[3] = (REJ[1] ^ REJ[2]);
	REJ[4] = F;

	unsigned char data[12];

	puts("Receiving I frame...");
	while(stop){
	    	res = read(fd,&c,1);
		switch(estado){
			case 0: {
				printf("estado 0\n");
				if (c == F){
					estado = 1;
				}
			break;
			}
			case 1:{
				printf("estado 1\n");
				if (c == A){
					estado = 2;
				}
				else if (c == F){
					estado = 1;
				}
				else 
					estado = 0;
				break;	
			}
			case 2:{				
				printf("estado 2\n");
				if (c == CS){
					estado = 5;
				}
				else if (c == F){
					estado = 1;
				}
				else
					estado = 0; 
				break;
			}
			case 3:{
				printf("estado 3\n");
				printf("%x\n", c);
				if (c == (Iheader[1]^Iheader[2])){
					estado = 4;
				}
				else if (c == F){
					estado = 1;
				}
				else 
					estado = 0;
				break;
			}
			case 5:{
				printf("estado 5 : %x || %c - index: %d\n", c, c, dataIndex);
				data[dataIndex] = c;
				if(c == F) { //dataIndex == 9
					puts("End of frame.");
					stop = 0;
					puts("Changed state to 4.");
					break;
				}
				dataIndex++;
				break;
			}
		}
	}

	puts("Sending RR frame...");
	res2 = write(fd,RR,5);
	printf("Enviou %d bytes \n",res2);
	stop = 0;

	//unstuffing
	unsigned char finalData[100];
	int i, j;
	for(i = 0, j = 0; i < dataIndex; i++, j++) {
		if(data[i] == 0x7D) {
			if(data[i+1] == 0x5E)
				finalData[j] = F;
			else if(data[i+1] == 0x5D)
				finalData[j] = 0x7D;
		} else
			finalData[j] = data[i];
	}

	int thing = write(file, data, sizeof(unsigned char) * (dataIndex - 1));
	if(thing == 0 || thing == -1){
		puts("omgwritefailed");
		return 1;
	} else
		printf("%d bytes written to file (supposedly).", thing);

	printf("Received data: %s\n", data);

	return 0;
}
