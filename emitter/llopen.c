#include <stdio.h>

#include "llopen.h"

void set() {
	retry_count++;
	connecting = 1;
}

int llopen(int fd) {
	retry_count = 0;
	connecting = 1;

	int res;

	//frame creation
	unsigned char SET[5], UA[5];
	SET[0] = F;
	SET[1] = A;
	SET[2] = C;
	SET[3] = SET[1] ^ SET[2];
	SET[4] = F;

	puts("Setting alarm signal...");
	signal(SIGALRM, set); //install set routine
	puts("Alarm signal set.");

	//machine state perfieral vars
	int stop = 1, estado = 0, res2;
	unsigned char c;

	while(retry_count <= 3) {
		if(connecting) {
			connecting = 0;

			//sending SET frame
			puts("Sending SET frame...");
			alarm(3);
    		res = write(fd,SET,5);
			sleep(5); 
			if(res != -1) {
    			printf("%d bytes written\n", res);
			} else {
				puts("Connection establishment failed.");
				return 1;
			}

			//receiving UA frame
			puts("Reading UA frame...");
			while(stop){
				res = read(fd,&c,1);
				if(res != -1)
					alarm(0);
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
						} else if (c == F){
							estado = 1;
							UA[0] = c;
						} else 
							estado = 0;
						break;	
					}
					case 2:{				
						printf("estado 2\n");
						if (c == C){
							UA[2] = c;
							estado = 3;
						} else if (c == F){
							UA[0] = c;
							estado = 1;
						} else
							estado = 0; 
						break;
					}
					case 3:{
						printf("estado 3\n");
						if (c == (SET[1] ^ SET[2])){
							UA[3] = c;
							estado = 4;
						} else if (c == F){
							UA[0] = c;
							estado = 1;
						} else 
							estado = 0;
						break;
					}
					case 4:{
						printf("estado 4\n");
						if (c == F){
							UA[4] = c;
							if (UA[0] == F && UA[1] == A && UA[2] == C && UA[3] == (UA[1] ^ UA[2]) && UA[4] == F){
								res2 = write(fd,UA,5);
								printf("Enviou %d bytes \n",res2);
								stop = 0;
								return fd;
								break;
							} else
								estado = 0;
						} else
							estado = 0;
						break;
					}
				}
			}
		}
	}

	if(retry_count >= 3) {
		puts("Connecting try count reached limit. Buy a better cable.");
		return 1;
	}

	return 0;
}
