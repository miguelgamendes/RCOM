#ifndef LLREAD_H
#define LLREAD_H

#define F 0x7E
#define A 0x03
#define C 0x05
#define CSR 0x01
#define CS 0x00

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int llread(int conn, int file);

#endif
