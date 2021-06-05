#ifndef __SERVER_H
#define __SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <sys/types.h>


//linux
#define SERVER_PORT 5000

#define BUFLEN 4096

#define MEM_LEN 0x400000 // 4MiB


int get( void *buf, void *receive, int count);


//linux
char *itoa(int _Val,char *_DstBuf,int _Radix);

#endif
