	#include "llclose.h"

int llclose(int fd){

	unsigned char DISC[5], UA[5] c;
	int res, res2, estado = 0, stop = 1;

	DISC[0] = F;
	DISC[1] = A;
	DISC[2] = CD;
	DISC[3] = (DISC[1] ^ DISC[2]);
	DISC[4] = F;

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
				if (c == (DISC[1]^DISC[2]){
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

	stop = 1;
	estado = 0;

	while(stop){
	    	res = read(fd,&c,1);
		switch(estado){
			case 0: {
				printf("estado 0\n");
				if (c == F){
					UA[0] = c;
					estado = 1;
				}
			break;
			}
			case 1:{
				printf("estado 1\n");
				if (c == A){
					UA[1] = c;
					estado = 2;
				}
				else if (c == F){
					UA[0] = c;
					estado = 1;
				}
				else 
					estado = 0;
				break;	
			}
			case 2:{				
				printf("estado 2\n");
				if (c == C){
					UA[2] = c;
					estado = 3;
				}
				else if (c == F){
					UA[0] = c;
					estado = 1;
				}
				else
					estado = 0; 
				break;
			}
			case 3:{
				printf("estado 3\n");
				if (c == (UA[1]^UA[2]){
					UA[3] = c;
					estado = 4;
				}
				else if (c == F){
					UA[0] = c;
					estado = 1;
				}
				else 
					estado = 0;
				break;
			}
			case 4:{
				printf("estado 4\n");
				if (c == F){
					UA[4] = c;
					if (UA[0] == F && UA[1] == A && UA[2] == CS && UA[3] == (UA[1]^UA[2]) && UA[4] == F){
						printf("UA received",res2);
						stop = 0;
						if (close(fd) < 0)
							return -1;
						return 0;
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
	return 0;
}
