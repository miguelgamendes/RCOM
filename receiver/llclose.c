	#include "llclose.h"

int llclose(int fd){

	unsigned char DISC[5], c;
	int res, res2, estado = 0, stop = 1;

	DISC[0] = F;
	DISC[1] = A;
	DISC[2] = CD;
	DISC[3] = (DISC[1] ^ DISC[2]);
	DISC[4] = F;

	puts("Receiving DISC frame...");
	while(stop){
	    	res = read(fd,&c,1);
		if(res != -1)
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
				if (c == CD){
					estado = 3;
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
				if (c == (DISC[1]^DISC[2])){
					estado = 4;
				}
				else if (c == F){
					estado = 1;
				}
				else 
					estado = 0;
				break;
			}
			case 4:{
				printf("estado 4\n");
				if (c == F){
					puts("Sending DISC frame...");
					res2 = write(fd,DISC,5);
					printf("Enviou %d bytes \n",res2);
					stop = 0;
					break;
				}
				else
					estado = 0;
				break;
			}
		}
	}

	return 0;
}
