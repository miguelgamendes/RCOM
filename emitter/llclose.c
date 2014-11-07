#include "llclose.h"

int llclose(int fd) {
	//emissor
		int res;
		retry_count = 0;
		connecting = 1;

		//frame creation
		unsigned char DISC[5], DISC2[5];
		DISC[0] = F;
		DISC[1] = A;
		DISC[2] = C_CLOSE;
		DISC[3] = DISC[1] ^ DISC[2];
		DISC[4] = F;

		UA[0] = F;
		UA[1] = A;
		UA[2] = C_UA;
		UA[3] = UA[1] ^ UA[2];
		UA[4] = F;

		puts("Setting alarm signal...");
		signal(SIGALRM, llOpenCloseAlarmHandler); //install alarm routine
		puts("Alarm signal set.");

		//machine state periferal vars
		int stop = 1, estado = 0, res2;
		unsigned char c;

		//sending DISC frame
		while(retry_count <= 3) {
			if(connecting) {
				connecting = 0;

				puts("Sending DISC frame...");
				res = write(fd,DISC,5);
				if(res != -1) {
					printf("%d bytes written\n", res);
				} else {
					puts("Connection establishment failed.");
					return 1;
				}

				alarm(3);
				//receiving DISC frame
				puts("Reading DISC frame...");
				while(stop && !connecting){
					res = read(fd,&c,1);
					if(res > 0) {
						alarm(0);
						switch(estado){
						case 0: {
							printf("estado 0\n");
							if (c == F){
								DISC2[0] = c;
								estado = 1;
							}
							break;
						}
						case 1:{
							printf("estado 1\n");
							if (c == A){
								DISC2[1] = c;
								estado = 2;
							} else if (c == F){
								estado = 1;
								DISC2[0] = c;
							} else 
								estado = 0;
							break;	
							   }
						case 2:{				
							printf("estado 2\n");
							if (c == C_CLOSE){
								DISC2[2] = c;
								estado = 3;
							} else if (c == F){
								DISC2[0] = c;
								estado = 1;
							} else
								estado = 0; 
							break;
							   }
						case 3:{
							printf("estado 3\n");
							if (c == (DISC2[1] ^ DISC2[2])){
								DISC2[3] = c;
								estado = 4;
							} else if (c == F){
								DISC2[0] = c;
								estado = 1;
							} else 
								estado = 0;
							break;
							   }
						case 4:{
							printf("estado 4\n");
							if (c == F){
								DISC2[4] = c;
								if (DISC2[0] == F && DISC2[1] == A && DISC2[2] == C_CLOSE && DISC2[3] == (DISC2[1] ^ DISC2[2]) && DISC2[4] == F){
									puts("Received DISC frame successfully.");
									stop = 0;
									puts("Finished reading.");
									break;
								} else {
									puts("DISC frame is wrong.");
									estado = 0;
								}
							} else
								estado = 0;
							break;
							   }
						}
					}
				}
			}

			if(estado == 4)
				break;
		}
			if(retry_count >= 3) {
				puts("Connecting try count reached limit.");
				return -1;
			}
			write(fd ,UA,5);
		return 0;
}
