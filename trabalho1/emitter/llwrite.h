#ifndef LLWRITE_H
#define LLWRITE_H

#include "bytevalues.h"

#include <signal.h>

int retry_count;
int sending;
int successful;

int llwrite(int fd, unsigned char* data, int dataSize);

#endif LLWRITE_H
