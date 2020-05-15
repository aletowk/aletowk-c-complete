#ifndef QUEUE_H_
#define QUEUE_H_ 

/**
 * @brief      { function_description }
 *
 * @param      10    { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
#define MAX_QUEUE_LENGTH (100)

typedef struct _QUEUE
{
	unsigned char * head; /*<! */
	unsigned char * tail;
	
	unsigned char * write_to;

	unsigned char *	read_from;

	unsigned int 	length; // in item

	unsigned int 	item_size;

	unsigned int 	waiting_messages; // nb of item currently in queue
}QUEUE;

QUEUE* QUEUE_get_my_queue(void);


/**
 * @brief      Creates a queue.
 *
 * @param      queue_to_create  The queue to create Type : QUEUE
 * @param[in]  length           The length
 * @param[in]  item_size        The item size
 *
 * @return     cf error code
 */
char create_queue(QUEUE* queue_to_create, unsigned int length, unsigned int item_size);

void print_queue(const QUEUE queue);

char queue_send(QUEUE* pQueue, const void* const pItem);

char queue_receive(QUEUE * pQueue, const void* const pBuffer);

/**
 * @brief      Test queue function, just print test
 */
void test_queue(void);

#endif