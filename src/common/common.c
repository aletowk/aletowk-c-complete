#include "common.h"

#include <stdio.h>

void print_array_bytes(void* buffer,unsigned int size)
{
	unsigned char * charBuffer = (unsigned char*)buffer;
	unsigned int i = 0;

	for( i = 0 ; i < size ; i++)
	{
		printf("0x%X ",charBuffer[i]);
	}
	printf("\n\r");
}