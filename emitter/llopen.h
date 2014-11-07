#ifndef LLOPEN_H
#define LLOPEN_H

#include "bytevalues.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

//connection establishment variables
int retry_count;
int connecting;

int llopen(int fd);

#endif
