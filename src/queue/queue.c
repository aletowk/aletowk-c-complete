#include "queue.h"

#include "common.h"

#include <stdio.h>
#include <string.h>


static unsigned char queue_memory_area[MAX_QUEUE_LENGTH];

static QUEUE my_queue;

QUEUE* QUEUE_get_my_queue(void)
{
	return &my_queue;
}


void test_one_byte_item_size(unsigned int nb_items)
{
	unsigned int i = 0;
	int test_val = 10;
	char ret_val = 0;
	unsigned char receivedBuffer = 0;

	ret_val = create_queue(&my_queue,nb_items,1);
	printf("create_queue returned : %d\n\r",ret_val);

	for( i = 0 ; i  < nb_items+1 ; i++)
	{
		ret_val = queue_send(&my_queue,&test_val);
		ASSERT(ret_val==0);
		test_val++;	
	}

	print_queue(my_queue);
	printf("\n\r");

	for(i = 0 ; i < nb_items + 1 ; i++)
	{
		ret_val = queue_receive(&my_queue,(void*)&receivedBuffer);
		ASSERT(ret_val == 0);
		printf("Received : 0x%X\n\r",receivedBuffer);
		print_queue(my_queue);
	}
	printf("\n\r");
	print_queue(my_queue);
}


void test_two_bytes_item_size(unsigned int nb_items)
{
	unsigned int i = 0;
	unsigned char test_array[2] = {42,1};
	unsigned char receivedBuffer[2] = {0};

	char ret_val = 0;

	ret_val = create_queue(&my_queue,nb_items,2);
	printf("create_queue returned : %d\n\r",ret_val);

	for( i = 0 ; i  < nb_items+1 ; i++)
	{
		ret_val = queue_send(&my_queue,test_array);
		ASSERT(ret_val==0);
		test_array[1]++;	
	}

	print_queue(my_queue);

	for(i = 0 ; i < nb_items + 1 ; i++)
	{
		ret_val = queue_receive(&my_queue,receivedBuffer);
		ASSERT(ret_val == 0);
		printf("Received : ");
		print_array_bytes(receivedBuffer,2);
		printf("\n\r");
		print_queue(my_queue);
	}
	printf("\n\r");
	print_queue(my_queue);
}

void test_4_bytes_item_size(unsigned int nb_items)
{
	unsigned int i = 0;
	unsigned char test_array[4] = {42,1,42,41};
	unsigned char receivedBuffer[4] = {0};

	char ret_val = 0;

	ret_val = create_queue(&my_queue,nb_items,4);
	printf("create_queue returned : %d\n\r",ret_val);

	for( i = 0 ; i  < nb_items+1 ; i++)
	{
		ret_val = queue_send(&my_queue,test_array);
		ASSERT(ret_val==0);
		test_array[3]++;	
	}

	print_queue(my_queue);

	for(i = 0 ; i < nb_items + 1 ; i++)
	{
		ret_val = queue_receive(&my_queue,receivedBuffer);
		ASSERT(ret_val == 0);
		printf("Received : ");
		print_array_bytes(receivedBuffer,4);
		printf("\n\r");
		print_queue(my_queue);
	}
	printf("\n\r");
	print_queue(my_queue);
}


void test_queue(void)
{

	printf("This is Test\n\r");

	// test_one_byte_item_size(5);
	// test_two_bytes_item_size(5);
	test_4_bytes_item_size(5);

}



char create_queue(QUEUE* queue_to_create, unsigned int length, unsigned int item_size)
{
	unsigned int size_in_bytes = 0;

	if(queue_to_create->length != 0)
	{
		return 1; /* This queue was already created */
	}

	if(length > 0)
	{
		size_in_bytes = length*item_size;

		if( size_in_bytes < MAX_QUEUE_LENGTH)
		{
			if(size_in_bytes != 0)
			{

				queue_to_create->head 			= queue_memory_area;
				queue_to_create->length 	= length;
				queue_to_create->item_size 		= item_size;

				queue_to_create->tail	 		= queue_to_create->head + queue_to_create->length*queue_to_create->item_size;

				queue_to_create->write_to		= queue_to_create->head;
				queue_to_create->read_from		= queue_to_create->head + (queue_to_create->length - 1)*queue_to_create->item_size;

				queue_to_create->waiting_messages = 0;

				return 0;
			}
			else
			{
				return -3;
			}

		}
		else
		{
			return -2;
		}
	}
	else
	{
		return -1;
	}
}


char queue_send(QUEUE* pQueue, const void* const pItem)
{
	ASSERT(pQueue);
	ASSERT(!(pItem == NULL));

	if(pQueue->waiting_messages < pQueue->length)
	{
		memcpy( (void*) pQueue->write_to, pItem,pQueue->item_size);
		pQueue->write_to += pQueue->item_size;

		if(pQueue->write_to >= pQueue->tail)
		{
			pQueue->write_to = pQueue->head;
		}
		++(pQueue->waiting_messages);
		return 0;
	}
	else
		return - 1; // Queue full
}

char queue_receive(QUEUE * pQueue, const void* const pBuffer)
{
	ASSERT(pQueue);
	ASSERT(!(pBuffer==NULL));

	if(pQueue->waiting_messages > 0)
	{
		pQueue->read_from += pQueue->item_size;
		if(pQueue->read_from >= pQueue->tail)
		{
			pQueue->read_from = pQueue->head;
		}
		memcpy((void * ) pBuffer, (void * )pQueue->read_from,pQueue->item_size);

		(pQueue->waiting_messages)--;
		return 0;
	}
	else
	{
		return -1;//EmptyQUEUE
	}
}

void print_queue(const QUEUE queue)
{
	unsigned int i = 0;
	unsigned char * address;

	if(queue.waiting_messages != 0)
	{
		address = queue.read_from+queue.item_size;
		for( i = 0 ; i < queue.waiting_messages;i++)
		{
			if(address >= queue.tail)
			{
				address = queue.head;
			}
			print_array_bytes(address,queue.item_size);
			address += queue.item_size;
			printf("\n\r");
		}	
	}
	else
	{
		printf("Queue is empty\n\r");
	}
}