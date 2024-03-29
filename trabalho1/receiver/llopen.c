#include "llopen.h"

int llopen(int fd){
unsigned char SET[5];
	unsigned char UA[5], c;
	int res, res2, estado = 0, stop = 1;

	UA[0] = F;
	UA[1] = A;
	UA[2] = CS;
	UA[3] = (UA[1] ^ UA[2]);
	UA[4] = F;

	puts("Receiving SET frame...");
	while(stop){
	    	res = read(fd,&c,1);
		switch(estado){
			case 0: {
				printf("estado 0\n");
				if (c == F){
					SET[0] = c;
					estado = 1;
				}
			break;
			}
			case 1:{
				printf("estado 1\n");
				if (c == A){
					SET[1] = c;
					estado = 2;
				}
				else if (c == F){
					estado = 1;
					SET[0] = c;
				}
				else 
					estado = 0;
				break;	
			}
			case 2:{				
				printf("estado 2\n");
				if (c == C){
					SET[2] = c;
					estado = 3;
				}
				else if (c == F){
					SET[0] = c;
					estado = 1;
				}
				else
					estado = 0; 
				break;
			}
			case 3:{
				printf("estado 3\n");
				if (c == (SET[1]^SET[2])){
					SET[3] = c;
					estado = 4;
				}
				else if (c == F){
					SET[0] = c;
					estado = 1;
				}
				else 
					estado = 0;
				break;
			}
			case 4:{
				printf("estado 4\n");
				if (c == F){
					SET[4] = c;
					if (SET[0] == F && SET[1] == A && SET[2] == C && SET[3] == (SET[1]^SET[2]) && SET[4] == F){
						puts("Sending UA frame...");
						res2 = write(fd,UA,5);
						printf("Enviou %d bytes \n",res2);
						stop = 0;
						return fd;
						break;
					}
					else
						estado = 0;
				}
				else
					estado = 0;
				break;
			}
		}
	}

	return -1;
}
