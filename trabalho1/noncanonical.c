/*Non-Canonical Input Processing*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
#define F 0x7E
#define A 0x03
#define C 0x03

#define CS 0x07


volatile int STOP=FALSE;

int main(int argc, char** argv)
{
    int fd, res, res2;
    struct termios oldtio,newtio;
    char buf[255];

    if ( (argc < 2) ||
  	     ((strcmp("/dev/ttyS0", argv[1])!=0) &&
  	      (strcmp("/dev/ttyS1", argv[1])!=0) )) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
      exit(1);
    }


  /*
    Open serial port device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C.
  */


    fd = open(argv[1], O_RDWR | O_NOCTTY );
    if (fd <0) {perror(argv[1]); exit(-1); }

    if ( tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
      perror("tcgetattr");
      exit(-1);
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 5;   /* blocking read until 5 chars received */



  /* 
    VTIME e VMIN devem ser alterados de forma a proteger com um temporizador a 
    leitura do(s) próximo(s) caracter(es)
  */



    tcflush(fd, TCIOFLUSH);

    if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

	printf("New termios structure set\n");

	unsigned char SET[5];
	unsigned char UA[5];

	UA[0] = F;
	UA[1] = A;
	UA[2] = CS;
	UA[3] = UA[1] ^ UA[2];
	UA[4] = F;

	int estado = 0;
	unsigned char c;
	int stop = 1;


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
				if (c == SET[1]^SET[2]){
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
					if (SET[0] == F && SET[1] == A && SET[2] == C && SET[3] == SET[1]^SET[2] && SET[4] == F){
						res2 = write(fd,UA,5);
						stop = 0;
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

	printf("Enviou %d bytes \n",res2);

    tcsetattr(fd,TCSANOW,&oldtio);
    close(fd);
    return 0;
}
