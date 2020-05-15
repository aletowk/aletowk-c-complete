#include "tcp_server.h"
#include <sys/socket.h>
#include <netinet/in.h>

#include "common.h"

#include <unistd.h>

#include <stdio.h>

static TPC_SERVER__T_HANDLER G_TCP_SERVER_handler = 
					{
						.ip = {0},
						.port  = 0,
						.output_queue = NULL
					};

static pthread_t G_TCP_SERVER_socket_thread_hanldler = 0;

pthread_t* TCP_SERVER_get_socket_thread_hanldler(void)
{
	return &G_TCP_SERVER_socket_thread_hanldler;
}

int TCP_SERVER__Init(int port, QUEUE* queue)
{
	G_TCP_SERVER_socket_thread_hanldler = 0;
	G_TCP_SERVER_handler.port = port;
	G_TCP_SERVER_handler.output_queue = queue;
	return 0;
}

void* TPC_SERVER__Thread(void* args)
{
	int server_fd = 0, new_socket = 0, valread = 0;
	int ret = 0;
	struct sockaddr_in addr;
	int addrlen = sizeof(addr);

	char buffer[C_TCP_SERVER__MAX_STREAM_BUFFER_SIZE] = {0};
	char ret_queue = 0;
	
	(void)args;

	server_fd = socket(AF_INET,SOCK_STREAM,0);
	if(server_fd == 0)
	{
		printf("[SOCKET THREAD] Error while creating socket !");
		pthread_cancel(pthread_self());
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(G_TCP_SERVER_handler.port);

	ret = bind(server_fd,(struct sockaddr*)&addr,sizeof(addr));
	if(ret < 0)
	{
		printf("[SOCKET THREAD] Error while bind code %d\n",ret );
		pthread_cancel(pthread_self());
	}

	ret = listen(server_fd,3);
	if(ret < 0)
	{
		printf("[SOCKET THREAD] Error while listen code %d\n",ret );
		pthread_cancel(pthread_self());
	}

	new_socket = accept(server_fd,(struct sockaddr*)&addr,(socklen_t*)&addrlen);
	if(new_socket < 0)
	{
		printf("[SOCKET THREAD] Error while accept code %d\n",new_socket );
		pthread_cancel(pthread_self());
	}

	while(1)
	{
		valread = read(new_socket,buffer,C_TCP_SERVER__MAX_STREAM_BUFFER_SIZE);
		if(valread == 0)
		{
			printf("[SOCKET THREAD] Error while read may be du to client lost. Stop thread !\n\r");
			pthread_cancel(pthread_self());
		}
		else
		{
			printf("[SOCKET THREAD] received buffer :\n\r");
			print_array_bytes(buffer,valread);	
			ret_queue = queue_send(G_TCP_SERVER_handler.output_queue,buffer);
			if(ret_queue != 0)
			{
				printf("[SOCKET THREAD] Error while queue_send code %d\n\r",ret_queue);
			}
		}
	}
}