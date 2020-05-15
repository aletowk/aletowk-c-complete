#include "static_allocation.h"
#include <stdio.h>
#include "common.h"

static unsigned char G_STATIC_pool[C_STATIC_POOL_SIZE] = {0};

static unsigned char * G_STATIC_next_free_space_address;

static int G_STATIC_remaining_space = C_STATIC_POOL_SIZE;


void STATIC__init_heap(void)
{
	G_STATIC_next_free_space_address = &G_STATIC_pool[0];
	G_STATIC_remaining_space = C_STATIC_POOL_SIZE;
}


void* STATIC__malloc(int size)
{
	void * new_address = NULL;
	if( G_STATIC_remaining_space - size < 0 )
	{
		return NULL;
	}
	else
	{
		new_address = G_STATIC_next_free_space_address;
		G_STATIC_next_free_space_address += size;
		G_STATIC_remaining_space -= size;
		return new_address;
	}
}


void test_static_malloc(void)
{
	// unsigned int* val = NULL;
	int nb_cell = C_STATIC_POOL_SIZE;
	unsigned char* array = NULL;

	printf("STATIC Alloc Test !\n\r");

	STATIC__init_heap();

	// val = (unsigned int*) STATIC__malloc(sizeof(unsigned int));
	// *val = 4294967295; // for uint32
	// printf("Val is %u and located at %p\n\r",*val,val);
	
	array = (unsigned char*) STATIC__malloc(nb_cell*sizeof(unsigned char));
	if(array == NULL)
	{
		printf("Arry is NULL !!!!\n\r");
	}
	else
	{
		print_array_bytes(array,nb_cell);		
	}
}