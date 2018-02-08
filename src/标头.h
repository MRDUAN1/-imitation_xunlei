#ifndef SPLIT__H
#define SPLIT__H
#include <stdio.h>
#include "include\curl\curl.h"
//#include <pthread.h>
struct split_
{
	FILE *fp;
	CURL *curls;
	long startPos;
	long endPos;
	int num;
	int already_sizes;
	long all_num;
	//pthread_t tid;
};
#endif // CLASS_INTERFACE_H