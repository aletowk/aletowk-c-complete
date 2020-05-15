#ifndef _COMMON_H_
#define _COMMON_H_ 

#define ASSERT(condition) {\
			if(!(condition)){\
				printf("Assert failed line: %d in file: %s\n\r",__LINE__,__FILE__);\
			}\
		}

void print_array_bytes(void* buffer,unsigned int size);


#endif