#include "ram_storage.h"

#include <stdio.h>

#include <string.h>

static RAM_STORAGE__T_ELEMENT  RAM_STORAGE_G_MEMORY_HEAP[RAM_STORAGE__C_MAX_ELEMENTS_TO_STORE];

static RAM_STORAGE__T_MEMORY_STORAGE RAM_STORAGE_G_ram_handler;

static RAM_STORAGE__T_ELEMENT test_sample[] = 
		{
			{
				.time = 15, .buffer_length = 4, .buffer = {0x32,13,14,15}
			},
			{
				.time = 16, .buffer_length = 4, .buffer = {0x42,13,14,15}
			},
			{
				.time = 18, .buffer_length = 4, .buffer = {0x22,13,14,15}
			}

		};

#define START_READ_TIME (15)
#define END_READ_TIME (18)
void test_ram_storage(void)
{
	int ret_value = 0;
	unsigned int i = 0;
	unsigned int read_elements = 0;
	RAM_STORAGE__T_ELEMENT read_list[5] = { 
											{.time = 0,.buffer_length = 0,.buffer = {0} },
											{.time = 0,.buffer_length = 0,.buffer = {0} },
											{.time = 0,.buffer_length = 0,.buffer = {0} },
											{.time = 0,.buffer_length = 0,.buffer = {0} },
											{.time = 0,.buffer_length = 0,.buffer = {0} }
										  };


	printf("RAM STORAGE TEST\n\r");

	ret_value = RAM_STORAGE_init_ram_memory_storage(3);
	if(ret_value)
	{
		printf("ERROR : RAM_STORAGE_init_ram_memory_storage code %d \n\r",ret_value);
	}

	/* Send */
	for( i = 0 ; i < 3 ; i++)
	{
		ret_value = RAM_STORAGE_write_element(&test_sample[i]);
		if(ret_value)
		{
			printf("ERROR : RAM_STORAGE_write_element code %d (adding step i : %d) \n\r",ret_value,i);	
		}
	}

	// RAM_STORAGE_print_memory_heap();


	ret_value = RAM_STORAGE_read_elements_between_two_dates(read_list,5,&read_elements,START_READ_TIME,END_READ_TIME );
	if(ret_value)
	{
		printf("ERROR : RAM_STORAGE_read_elements_between_two_dates code %d\n\r",ret_value);
	}
	else
	{
		printf("Read %d elements from %d to %d\n\r",read_elements,START_READ_TIME,END_READ_TIME);
		for(i = 0 ; i < read_elements;i++)
		{
			print_element(read_list[i]);
		}
	}
	// RAM_STORAGE_delete_up_to_time(20);
}

static void clean_heap(void)
{
	unsigned int i = 0;
	for(i = 0 ; i < RAM_STORAGE__C_MAX_ELEMENTS_TO_STORE ; i++)
	{
		memset(&RAM_STORAGE_G_MEMORY_HEAP[i],0,sizeof(RAM_STORAGE_G_MEMORY_HEAP[i]));
	}
}

static int create_memory_storage(unsigned int length)
{
	if(length <= RAM_STORAGE__C_MAX_ELEMENTS_TO_STORE)
	{
		RAM_STORAGE_G_ram_handler.head = (RAM_STORAGE__T_ADDRESS_TYPE) &RAM_STORAGE_G_MEMORY_HEAP[0];
		RAM_STORAGE_G_ram_handler.tail = (RAM_STORAGE__T_ADDRESS_TYPE) &RAM_STORAGE_G_MEMORY_HEAP[length-1];

		RAM_STORAGE_G_ram_handler.write_to = RAM_STORAGE_G_ram_handler.head;
		RAM_STORAGE_G_ram_handler.read_from = RAM_STORAGE_G_ram_handler.head;

		RAM_STORAGE_G_ram_handler.number_of_elements = 0;
		RAM_STORAGE_G_ram_handler.max_number_of_elements = length;
		return 0;
	}
	else
	{
		return 1;
	}
}

int RAM_STORAGE_init_ram_memory_storage(unsigned int length)
{
	clean_heap();
	return create_memory_storage(length);
}

int RAM_STORAGE_write_element(const RAM_STORAGE__T_ELEMENT * element)
{
	int ret_value = 0;

	if(RAM_STORAGE_G_ram_handler.number_of_elements < RAM_STORAGE_G_ram_handler.max_number_of_elements)
	{
		memcpy(RAM_STORAGE_G_ram_handler.write_to,element,sizeof(RAM_STORAGE__T_ELEMENT));

		if( (RAM_STORAGE_G_ram_handler.write_to + sizeof(RAM_STORAGE__T_ELEMENT)) > RAM_STORAGE_G_ram_handler.tail )
		{
			RAM_STORAGE_G_ram_handler.write_to = RAM_STORAGE_G_ram_handler.head;
		}
		else
		{
			RAM_STORAGE_G_ram_handler.write_to += sizeof(RAM_STORAGE__T_ELEMENT);
		}
		RAM_STORAGE_G_ram_handler.number_of_elements++;
	}
	else
	{
		ret_value = 1;
	}

	return ret_value;
}

/**
 * @brief      Search for the start element by looking for its "time" value. If
 *             there is no packet with this exact "time" value, then this
 *             function starts again to read the stored packet in order to find the nearest element
 *
 * @param      time  The time
 *
 * @return     The start time element if found otherwise NULL
 */
static RAM_STORAGE__T_ELEMENT * search_element_start_time(unsigned int time)
{
	unsigned int counter = 0;
	unsigned char found = 0;
	RAM_STORAGE__T_ADDRESS_TYPE browser = NULL;

	/* We start searching by the last added element */
	if(RAM_STORAGE_G_ram_handler.write_to - sizeof(RAM_STORAGE__T_ELEMENT) < RAM_STORAGE_G_ram_handler.head)
	{
		browser = RAM_STORAGE_G_ram_handler.tail;
	}
	else
	{
		browser = RAM_STORAGE_G_ram_handler.write_to - sizeof(RAM_STORAGE__T_ELEMENT);
	}

	while( (!found) && (counter < RAM_STORAGE_G_ram_handler.number_of_elements) )
	{
		if( ((RAM_STORAGE__T_ELEMENT*)browser)->time >= time )
		{
			if(counter+1 == RAM_STORAGE_G_ram_handler.number_of_elements)
			{
				return (RAM_STORAGE__T_ELEMENT*)browser;
			}
			/* is it the 'oldest' with this time ? */
			if( (browser - sizeof(RAM_STORAGE__T_ELEMENT)) < RAM_STORAGE_G_ram_handler.head )
			{
				if( ((RAM_STORAGE__T_ELEMENT*)RAM_STORAGE_G_ram_handler.tail)->time >= time)
				{
					found = 0;
				}
				else
				{
					return (RAM_STORAGE__T_ELEMENT*)browser;
				}
			}
			else
			{
				if( ((RAM_STORAGE__T_ELEMENT*)(browser - sizeof(RAM_STORAGE__T_ELEMENT)))->time >= time )
				{
					found = 0;
				}
				else
				{	
					return (RAM_STORAGE__T_ELEMENT*)browser;
				}
			}
		}

		if( (browser - sizeof(RAM_STORAGE__T_ELEMENT)) < RAM_STORAGE_G_ram_handler.head)
		{
			browser = RAM_STORAGE_G_ram_handler.tail;
		}
		else
		{
			browser -= sizeof(RAM_STORAGE__T_ELEMENT);
		}
		counter++;
	}

	counter = 0;
	/* If we reach here then the exact time is not in the store area. We look for the nearest time */
	if(RAM_STORAGE_G_ram_handler.write_to - sizeof(RAM_STORAGE__T_ELEMENT) < RAM_STORAGE_G_ram_handler.head)
	{
		browser = RAM_STORAGE_G_ram_handler.tail;
	}
	else
	{
		browser = RAM_STORAGE_G_ram_handler.write_to - sizeof(RAM_STORAGE__T_ELEMENT);
	}
	return NULL;
}

/**
 * @brief      { function_description }
 *
 * @pre        Start looking for last element to dump starting from the 'start'
 *             element previously found. Do not start this function if null
 *
 * @param      time  The time
 *
 * @return     { description_of_the_return_value }
 */
static RAM_STORAGE__T_ELEMENT * search_element_end_time(unsigned int time,const RAM_STORAGE__T_ELEMENT * start_elem)
{
	unsigned int counter = 0;
	unsigned char found = 0;
	RAM_STORAGE__T_ADDRESS_TYPE browser = NULL;

	if(start_elem)
	{
		counter = ((RAM_STORAGE__T_ADDRESS_TYPE)start_elem) - RAM_STORAGE_G_ram_handler.write_to;
		counter /= sizeof(RAM_STORAGE__T_ELEMENT);
		browser = (RAM_STORAGE__T_ADDRESS_TYPE)start_elem;	

		while( (!found) && (counter < RAM_STORAGE_G_ram_handler.number_of_elements) )
		{

			if( 	((RAM_STORAGE__T_ELEMENT*)browser)->time <= time 
				&&  ((RAM_STORAGE__T_ELEMENT*)browser)->time >= start_elem->time)
			{
				if( (counter + 1) == RAM_STORAGE_G_ram_handler.number_of_elements)
				{
					/* it s the last element so cause its valid we return it */
					return ((RAM_STORAGE__T_ELEMENT*)browser);
				}


				/* Does the next one also ? */
				if( (browser + sizeof(RAM_STORAGE__T_ELEMENT) > RAM_STORAGE_G_ram_handler.tail) )
				{
					if(  ((RAM_STORAGE__T_ELEMENT*)RAM_STORAGE_G_ram_handler.head)->time <= time )
					{
						found = 0;
					}
					else
					{
						return ((RAM_STORAGE__T_ELEMENT*)browser);
					}
				}
				else
				{
					if(  ((RAM_STORAGE__T_ELEMENT*)(browser + sizeof(RAM_STORAGE__T_ELEMENT)))->time <= time )
					{
						found = 0;
					}
					else
					{
						return ((RAM_STORAGE__T_ELEMENT*)browser);
					}
				}
			}

			if((browser + sizeof(RAM_STORAGE__T_ELEMENT) > RAM_STORAGE_G_ram_handler.tail))
			{
				browser = RAM_STORAGE_G_ram_handler.head;
			}
			else
			{
				browser += sizeof(RAM_STORAGE__T_ELEMENT);
			}
			counter++;
		}
	}
	return NULL;
}


// /**
//  * @brief      { function_description }
//  *
//  * @param      time  The time
//  *
//  * @return     { description_of_the_return_value }
//  */
// RAM_STORAGE__T_ELEMENT * search_element_end_time(unsigned int time)
// {
// 	unsigned int i = 0;

// 	for(i = 0 ; i < RAM_STORAGE_G_ram_handler.number_of_elements ; i++)
// 	{
// 		if(RAM_STORAGE_G_ram_handler.head[i].time == time)
// 		{ 
// 			if(i < RAM_STORAGE_G_ram_handler.number_of_elements )
// 		}
// 	}
// }

static unsigned int get_number_of_elements_between_positions(RAM_STORAGE__T_ELEMENT const * start,RAM_STORAGE__T_ELEMENT const * end)
{
	RAM_STORAGE__T_ADDRESS_TYPE start_add = (RAM_STORAGE__T_ADDRESS_TYPE)start;
	RAM_STORAGE__T_ADDRESS_TYPE end_add = (RAM_STORAGE__T_ADDRESS_TYPE)end;
	
	if(start_add > end_add)
	{
		return (((RAM_STORAGE_G_ram_handler.tail - start_add) + (end_add - RAM_STORAGE_G_ram_handler.head))/sizeof(RAM_STORAGE__T_ELEMENT)) + 1;
	}
	else
	{
		return ((end_add - start_add)/sizeof(RAM_STORAGE__T_ELEMENT)) + 1;
	}
}

int RAM_STORAGE_read_elements_between_two_dates(RAM_STORAGE__T_ELEMENT * elements_to_read, unsigned int max_element_to_read,
	 											unsigned int *read_number, unsigned int start_time, unsigned int end_time )
{
	unsigned int i = 0;
	RAM_STORAGE__T_ELEMENT * first_element = NULL;
	RAM_STORAGE__T_ELEMENT * last_element = NULL;
	RAM_STORAGE__T_ADDRESS_TYPE browser = NULL;

	/* First search if one element */
	first_element = search_element_start_time(start_time);
	if(first_element == NULL)
	{
		return -1;
	}
	else
	{
		// printf("First element is : \n\r");
		// print_element(*first_element);
		last_element = search_element_end_time(end_time,first_element);
		if(last_element == NULL)
		{
			return -2;
		}
		else
		{
			// printf("Last element is :\n\r");
			// print_element(*last_element);
			*read_number = get_number_of_elements_between_positions(first_element,last_element);
			// printf("Number of elements to read : %d\n\r", *read_number);
		}
	}

	if( (*read_number) == 0)
	{
		return 1;
	}

	if( (*read_number) > max_element_to_read )
	{
		return -3;
	}

	browser = (RAM_STORAGE__T_ADDRESS_TYPE)first_element;
	for( i = 0 ; i < (*read_number) ; i++)
	{
		memcpy(&elements_to_read[i],browser,sizeof(RAM_STORAGE__T_ELEMENT));
		if(  (browser + sizeof(RAM_STORAGE__T_ELEMENT)) > RAM_STORAGE_G_ram_handler.tail)
		{
			browser = RAM_STORAGE_G_ram_handler.head;
		}
		else
		{
			browser += sizeof(RAM_STORAGE__T_ELEMENT);
		}
	}
	return 0;
}

int RAM_STORAGE_delete_up_to_time(unsigned int time)
{
	unsigned int counter = 0, number_of_elements_to_delete = 0;
	unsigned char found = 0;
	RAM_STORAGE__T_ADDRESS_TYPE browser = NULL;
	RAM_STORAGE__T_ADDRESS_TYPE end_delete = NULL;

	/* Search the elements that have time less or egual to time. Start by looking from last written */
	browser = RAM_STORAGE_G_ram_handler.write_to; /* write_to should be the oldest */
	if(browser - sizeof(RAM_STORAGE__T_ELEMENT) < RAM_STORAGE_G_ram_handler.head)
	{
		browser = RAM_STORAGE_G_ram_handler.tail;
	}
	else
	{
		browser -= sizeof(RAM_STORAGE__T_ELEMENT);
	}

	while(!found && (counter < RAM_STORAGE_G_ram_handler.number_of_elements))
	{
		if( ((RAM_STORAGE__T_ELEMENT*)browser)->time <= time )
		{
			if(!end_delete)
			{
				end_delete = browser;				
			}

			if(counter+1 == RAM_STORAGE_G_ram_handler.number_of_elements)
			{
				found = 1;
			}
			/* is it the 'oldest' time less than "time" ? */
			if(!found)
			{
				if( (browser - sizeof(RAM_STORAGE__T_ELEMENT)) < RAM_STORAGE_G_ram_handler.head )
				{
					if( ((RAM_STORAGE__T_ELEMENT*)RAM_STORAGE_G_ram_handler.tail)->time <= time)
					{
						found = 0;
					}
					else
					{
						found = 1;
					}
				}
				else
				{
					if( ((RAM_STORAGE__T_ELEMENT*)(browser - sizeof(RAM_STORAGE__T_ELEMENT)))->time <= time )
					{
						found = 0;
					}
					else
					{	
						found = 1;
					}
				}
			}
			
		}

		if(!found)
		{
			if( (browser - sizeof(RAM_STORAGE__T_ELEMENT)) < RAM_STORAGE_G_ram_handler.head)
			{
				browser = RAM_STORAGE_G_ram_handler.tail;
			}
			else
			{
				browser -= sizeof(RAM_STORAGE__T_ELEMENT);
			}
			counter++;
		}
	}

	if(!found)
	{
		return -1;
	}

	number_of_elements_to_delete = get_number_of_elements_between_positions(browser,end_delete);

	printf("There is %d elements to delete !\n",number_of_elements_to_delete);
	for(counter = 0 ; counter < number_of_elements_to_delete ; counter++)
	{
		print_element(*(RAM_STORAGE__T_ELEMENT*)browser);
		if(browser + (sizeof(RAM_STORAGE__T_ELEMENT)) > RAM_STORAGE_G_ram_handler.tail)
		{
			browser = RAM_STORAGE_G_ram_handler.head;
		}
		else
		{
			browser += sizeof(RAM_STORAGE__T_ELEMENT); 
		}
	}
	return 0;
}


static void print_line(unsigned char * buffer, unsigned int size)
{
	unsigned int i = 0;
	for(i = 0 ; i < size ; i++)
	{
		printf("0x%02X ",buffer[i]);
	}
}
void RAM_STORAGE_print_memory_heap(void)
{
	unsigned int i = 0;
	printf("RAM Storage heap:\n\r");
	for(i = 0 ; i < RAM_STORAGE__C_MAX_ELEMENTS_TO_STORE ; i++)
	{
		printf("Element %d / %d   : \n\rTime   : %d\n\r",i+1,RAM_STORAGE__C_MAX_ELEMENTS_TO_STORE,RAM_STORAGE_G_MEMORY_HEAP[i].time);
		printf("Length : %d\n\r",RAM_STORAGE_G_MEMORY_HEAP[i].buffer_length);
		print_line(RAM_STORAGE_G_MEMORY_HEAP[i].buffer,RAM_STORAGE_G_MEMORY_HEAP[i].buffer_length);
		printf("\n\r\n\r");
	}	
}

void print_element(RAM_STORAGE__T_ELEMENT elem)
{
	printf("Time   : %d\n\r",elem.time);
	printf("Length : %d\n\r",elem.buffer_length);
	print_line(elem.buffer,elem.buffer_length);
	printf("\n\r");
}
