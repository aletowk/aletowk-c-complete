#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_ 

#include <pthread.h>
#include "queue.h"

#define C_TCP_SERVER__MAX_STREAM_BUFFER_SIZE (1024)

#define C_TCP_SERVER__MAX_IP_STR_LENGTH (16)



typedef struct _TPC_SERVER__T_HANDLER
{
	unsigned char ip[C_TCP_SERVER__MAX_IP_STR_LENGTH];
	int port;
	QUEUE * output_queue;
}TPC_SERVER__T_HANDLER;

pthread_t* TCP_SERVER_get_socket_thread_hanldler(void);

int TCP_SERVER__Init(int port, QUEUE* queue);

void* TPC_SERVER__Thread(void* args);


#endif