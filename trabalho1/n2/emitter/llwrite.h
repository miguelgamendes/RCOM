#ifndef LLWRITE_H
#define LLWRITE_H

#define F 0x7E
#define A 0x03
#define C 0x00
#define CS 0x05
#define CSR 0x01

int llwrite(int fd, unsigned char* data, int dataSize);

#endif LLWRITE_H
