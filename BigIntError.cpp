#include<stdio.h>
#include<stdlib.h>
void PrintAndAbort(const char*g,int state){
	fputs(g,stderr);fflush(stderr);
	exit(state);
}
char errmsg[1<<14];
#define PError(errno,...) sprintf(errmsg,##__VA_ARGS__),PrintAndAbort(errmsg,errno);
