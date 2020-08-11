#ifndef _RAM_STORAGE_INCLUDED_
#define _RAM_STORAGE_INCLUDED_ 

#ifndef NULL
  #define NULL ((void *) 0x00000000000000)
#endif

typedef void* RAM_STORAGE__T_ADDRESS_TYPE;

#define RAM_STORAGE__M_UNUSED(args) ((void) args)

#define RAM_STORAGE__C_MAX_ELEMENTS_TO_STORE (5)

#define RAM_STORAGE__C_MAX_BUFFER_LENGTH (5)

typedef struct __attribute__((packed)) _RAM_STORAGE__T_ELEMENT
{
	unsigned int time;
	unsigned int buffer_length;
	unsigned char buffer[RAM_STORAGE__C_MAX_BUFFER_LENGTH];
}RAM_STORAGE__T_ELEMENT;


typedef struct _RAM_STORAGE__T_MEMORY_STORAGE
{
	RAM_STORAGE__T_ADDRESS_TYPE head;
	RAM_STORAGE__T_ADDRESS_TYPE tail;

	RAM_STORAGE__T_ADDRESS_TYPE write_to;
	RAM_STORAGE__T_ADDRESS_TYPE read_from;

	unsigned int number_of_elements;
	unsigned int max_number_of_elements;
}RAM_STORAGE__T_MEMORY_STORAGE;

void test_ram_storage(void);


int RAM_STORAGE_init_ram_memory_storage(unsigned int length);

int RAM_STORAGE_write_element(const RAM_STORAGE__T_ELEMENT * element);

int RAM_STORAGE_read_elements_between_two_dates(RAM_STORAGE__T_ELEMENT * elements_to_read, unsigned int max_element_to_read,
	 											unsigned int *read_number, unsigned int start_time, unsigned int end_time );

int RAM_STORAGE_delete_up_to_time(unsigned int time);

void RAM_STORAGE_print_memory_heap(void);

void RAM_STORAGE_update_with_oldest_time(unsigned int * oldest_time);

void RAM_STORAGE_update_with_newest_time(unsigned int * newest_time);

void print_element(RAM_STORAGE__T_ELEMENT elem);
#endif
