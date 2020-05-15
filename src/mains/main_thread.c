#include <stdio.h>
#include <pthread.h>

#include <stdlib.h>

#include <string.h>

#include "queue.h"
#include "state_manager.h"

#include "tcp_server.h"

#define PORT 8080
#define MAX_BUFF_SIZE (64)


/**
 * @brief      Main function of the API
 *
 * @return     0 if no errors
 */
int main(void)
{
	QUEUE* comm_queue = QUEUE_get_my_queue();
	int port = 8080;
	char ret_queue = 0;
	int ret = 0;
	pthread_t server_thread_handler = *TCP_SERVER_get_socket_thread_hanldler();
	pthread_t state_manager_thread_handler = 0;

	ret_queue = create_queue(comm_queue, 3, 3);
	if(ret_queue != 0)
	{
		printf("[MAIN] Error while create_queue code %d\n\r,",ret_queue);
	}



	TCP_SERVER__Init(port,comm_queue);


	ret = pthread_create(&server_thread_handler, NULL,TPC_SERVER__Thread,NULL);
	if(ret)
	{
		printf("[MAIN] Error while pthread_create TPC_SERVER__Thread code %d\n\r",ret);
	}

	ret = pthread_create(&state_manager_thread_handler, NULL , test_state_manager_thread , NULL);
	if(ret)
	{
		printf("[MAIN] Error while pthread_create test_state_manager_thread code %d\n\r",ret);
	}

	pthread_join(server_thread_handler, NULL);	
	pthread_join(state_manager_thread_handler, NULL);	
	return 0;
}