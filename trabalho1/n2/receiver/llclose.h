#ifndef LLCLOSE_H
#define LLCLOSE_H

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define F 0x7E
#define A 0x03
#define CD 0x0B
#define CS 0x07

int llclose(int fd);

#endif
