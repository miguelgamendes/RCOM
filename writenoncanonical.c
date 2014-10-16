/*Non-Canonical Input Processing*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

#define F 0x7E
#define A 0x03
#define C 0x03

volatile int STOP=FALSE;

int establishConn(int fd, unsigned char* SET, unsigned char* UA) {
	int res;

	//sending SET trama
	puts("Sending SET trama...");
    res = write(fd,SET,5);
	sleep(5); 
	if(res != -1)
    	printf("%d bytes written\n", res);

	alarm(3);

	res = read(fd,UA,5);
	if(res != -1)
		printf("Established. Read %d bytes.");
	else {
		printf("Reading failed.");
		return 1;
	}

	if(UA[0] == F && UA[1] == A && UA[2] == 0x07 && UA[3] == (A ^ 0x07) && UA[4] == F)
		puts("UA trama: valid");
	else {
		puts("UA trama: invalid");
		return 1;
	}

	return 0;
}

int set(int fd, unsigned char* SET, unsigned char* UA) {
	puts("Trying again...");
	establishConn(fd, &SET, &UA);
	return 0;
}

int main(int argc, char** argv)
{
	puts("Beginning.");
    int fd,c;
    struct termios oldtio,newtio;
	unsigned char* SET = malloc(5);
	unsigned char* UA = malloc(5);
    char buf[255];
    int i, sum = 0, speed = 0;
    
    if ( (argc < 2) || 
  	     (strcmp("/dev/ttyS4", argv[1])!=0)) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
      exit(1);
    }

	puts("Setting alarm signal...");
	signal(SIGALRM, set); //install set routine
	puts("Alarm signal set.");

	puts("Opening port...");

    fd = open(argv[1], O_RDWR | O_NOCTTY );
    if (fd <0) {perror(argv[1]); exit(-1); }

    if ( tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
      perror("tcgetattr");
      exit(-1);
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = OPOST;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME]    = 3;   /* inter-character timer */
    newtio.c_cc[VMIN]     = 5;   /* blocking read until 5 chars received */

    tcflush(fd, TCIFLUSH);

    if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

	puts("Creating Set trama...");

	//trama creation
	SET[0] = F;
	SET[1] = A;
	SET[2] = C;
	SET[3] = SET[1] ^ SET[2];
	SET[4] = F;

	if(establishConn(fd, &SET, &UA))
		return 1;
    
    if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }
    close(fd);
    return 0;
}
