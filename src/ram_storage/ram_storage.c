#include "ram_storage.h"

#include <stdio.h>

#include <string.h>

static RAM_STORAGE__T_ELEMENT  RAM_STORAGE_G_MEMORY_HEAP[RAM_STORAGE__C_MAX_ELEMENTS_TO_STORE];

static RAM_STORAGE__T_MEMORY_STORAGE RAM_STORAGE_G_ram_handler;

static RAM_STORAGE__T_ELEMENT test_sample[] = 
		{
			{
				.time = 0, .buffer_length = 4, .buffer = {0x32,13,14,15}
			},
			{
				.time = 0, .buffer_length = 4, .buffer = {0x42,13,14,15}
			},
			{
				.time = 0, .buffer_length = 4, .buffer = {0x22,13,14,15}
			}

		};

static unsigned int get_time(void)
{
	static unsigned int default_time = 10;
	default_time++;
	return default_time;
}

#define START_READ_TIME (9)
#define END_READ_TIME (15)
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


	// /* ************************** */
	// /* *** Read Test Scenario *** */
	// printf("==> Read Test Scenario\n\r");
	// /* Creates a memory storage of 5 elemen and add 5 element time from 10 to 15 */
	// ret_value = RAM_STORAGE_init_ram_memory_storage(5);
	// if(ret_value)
	// {
	// 	printf("ERROR : RAM_STORAGE_init_ram_memory_storage code %d \n\r",ret_value);
	// }
	// /* Send */
	// for( i = 0 ; i < 3 ; i++)
	// {
	// 	test_sample[i].time = get_time();
	// 	ret_value = RAM_STORAGE_write_element(&test_sample[i]);
	// 	if(ret_value)
	// 	{
	// 		printf("ERROR : RAM_STORAGE_write_element code %d (adding step i : %d) \n\r",ret_value,i);	
	// 	}
	// }
	// /* Send */
	// for( i = 0 ; i < 2 ; i++)
	// {
	// 	test_sample[i].time = get_time();
	// 	ret_value = RAM_STORAGE_write_element(&test_sample[i]);
	// 	if(ret_value)
	// 	{
	// 		printf("ERROR : RAM_STORAGE_write_element code %d (adding step i : %d) \n\r",ret_value,i);	
	// 	}
	// }

	// RAM_STORAGE_print_memory_heap();
	// /* Ask to read up to 14 */
	// ret_value = RAM_STORAGE_read_elements_between_two_dates(read_list,5,&read_elements,0,14 );
	// if(ret_value)
	// {
	// 	printf("ERROR : RAM_STORAGE_read_elements_between_two_dates code %d\n\r",ret_value);
	// }
	// else
	// {
	// 	printf("Read %d elements from %d to %d\n\r",read_elements,0,14);
	// 	for(i = 0 ; i < read_elements;i++)
	// 	{
	// 		print_element(read_list[i]);
	// 	}
	// }
	// /* *** End of read scenario *** */
	// /* **************************** */



 //    /* **************************** */
	// /* *** Delete Test Scenario *** */
	// printf("==> Delete Test Scenario\n\r");
	// /* Create a 5 length storage then send 5 packets (time from 10 to 15) */
	// ret_value = RAM_STORAGE_init_ram_memory_storage(5);
	// if(ret_value)
	// {
	// 	printf("ERROR : RAM_STORAGE_init_ram_memory_storage code %d \n\r",ret_value);
	// }
	// /* Send */
	// for( i = 0 ; i < 3 ; i++)
	// {
	// 	test_sample[i].time = get_time();
	// 	ret_value = RAM_STORAGE_write_element(&test_sample[i]);
	// 	if(ret_value)
	// 	{
	// 		printf("ERROR : RAM_STORAGE_write_element code %d (adding step i : %d) \n\r",ret_value,i);	
	// 	}
	// }
	// /* Send */
	// for( i = 0 ; i < 2 ; i++)
	// {
	// 	test_sample[i].time = get_time();
	// 	ret_value = RAM_STORAGE_write_element(&test_sample[i]);
	// 	if(ret_value)
	// 	{
	// 		printf("ERROR : RAM_STORAGE_write_element code %d (adding step i : %d) \n\r",ret_value,i);	
	// 	}
	// }
	// /* Delete All packets up to time = 14 (up to the previous last added) */
	// RAM_STORAGE_delete_up_to_time(14);
	// RAM_STORAGE_print_memory_heap();
	// /* Add 3 new elements (from 15 to 17) (loop from tail to head of memory space) */
	// for( i = 0 ; i < 3 ; i++)
	// {
	// 	test_sample[i].time = get_time();
	// 	ret_value = RAM_STORAGE_write_element(&test_sample[i]);
	// 	if(ret_value)
	// 	{
	// 		printf("ERROR : RAM_STORAGE_write_element code %d (adding step i : %d) \n\r",ret_value,i);	
	// 	}
	// }
	// /* Prints how it added the last 3 elements */
	// RAM_STORAGE_print_memory_heap();
	// /* Delete Up to time = 17 (it should only have  the last added element timetag to 17) */
	// RAM_STORAGE_delete_up_to_time(16);
	// RAM_STORAGE_print_memory_heap();

	// /* *** End of delete scenario *** */
	// /* ****************************** */
	
	/* ************************************* */
	/* *** Read and delete Test Scenario *** */
	/*     (complete the read test)          */
	printf("==> Read Test Scenario\n\r");
	/* Creates a memory storage of 5 elemen and add 5 element time from 10 to 15 */
	ret_value = RAM_STORAGE_init_ram_memory_storage(5);
	if(ret_value)
	{
		printf("ERROR : RAM_STORAGE_init_ram_memory_storage code %d \n\r",ret_value);
	}
	/* Send */
	for( i = 0 ; i < 3 ; i++)
	{
		test_sample[i].time = get_time();
		ret_value = RAM_STORAGE_write_element(&test_sample[i]);
		if(ret_value)
		{
			printf("ERROR : RAM_STORAGE_write_element code %d (adding step i : %d) \n\r",ret_value,i);	
		}
	}
	/* Send */
	for( i = 0 ; i < 2 ; i++)
	{
		test_sample[i].time = get_time();
		ret_value = RAM_STORAGE_write_element(&test_sample[i]);
		if(ret_value)
		{
			printf("ERROR : RAM_STORAGE_write_element code %d (adding step i : %d) \n\r",ret_value,i);	
		}
	}

	RAM_STORAGE_print_memory_heap();
	/* Ask to read up to 14 */
	ret_value = RAM_STORAGE_read_elements_between_two_dates(read_list,5,&read_elements,0,14 );
	if(ret_value)
	{
		printf("ERROR : RAM_STORAGE_read_elements_between_two_dates code %d\n\r",ret_value);
	}
	else
	{
		printf("Read %d elements from %d to %d\n\r",read_elements,0,14);
		for(i = 0 ; i < read_elements;i++)
		{
			print_element(read_list[i]);
		}
	}

	/* Delete Up to time = 14 */
	RAM_STORAGE_delete_up_to_time(14);
	/* And add 2 elements (so newest time is 17) */
	for( i = 0 ; i < 2 ; i++)
	{
		test_sample[i].time = get_time();
		ret_value = RAM_STORAGE_write_element(&test_sample[i]);
		if(ret_value)
		{
			printf("ERROR : RAM_STORAGE_write_element code %d (adding step i : %d) \n\r",ret_value,i);	
		}
	}
	RAM_STORAGE_print_memory_heap();
	/* Ask to read up to 16 */
	ret_value = RAM_STORAGE_read_elements_between_two_dates(read_list,5,&read_elements,0,16 );
	if(ret_value)
	{
		printf("ERROR : RAM_STORAGE_read_elements_between_two_dates code %d\n\r",ret_value);
	}
	else
	{
		printf("Read %d elements from %d to %d\n\r",read_elements,0,16);
		for(i = 0 ; i < read_elements;i++)
		{
			print_element(read_list[i]);
		}
	}
	/* *** End of read and delete scenario *** */
	/* *************************************** */


	/* *********************** */
	/* *** Test Get oldest *** */
	printf("\n\r***********************\n\r");
	printf("*** Test Get Oldest And Newest ***\n\r");
	RAM_STORAGE_print_memory_heap();

	unsigned int oldest_time  = 0 ; 
	unsigned int newest_time = 0;

	RAM_STORAGE_update_with_oldest_time(&oldest_time);
	printf("The oldest time is : %d\n\r",oldest_time);
	RAM_STORAGE_update_with_newest_time(&newest_time);
	printf("The newest time is : %d\n\r",newest_time);
	printf("*** End Test Get Oldest and Newest***\n\r");
	printf("***************************\n\r");

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

static unsigned int get_abs_distance_between_address(RAM_STORAGE__T_ADDRESS_TYPE start, RAM_STORAGE__T_ADDRESS_TYPE end)
{
	unsigned int distance = 0;
	if(end >= start)
	{
		distance = end - start;
		distance += sizeof(RAM_STORAGE__T_ELEMENT); // must take into account the border and the possibility end == start
	}
	else
	{
		/* Waring about the head-tail junction and the fact that we count head and tail elemens */
		if(start == (RAM_STORAGE__T_ADDRESS_TYPE)RAM_STORAGE_G_ram_handler.head || start == (RAM_STORAGE__T_ADDRESS_TYPE)RAM_STORAGE_G_ram_handler.tail )
		{
			distance += sizeof(RAM_STORAGE__T_ELEMENT);
		}
		if(end == (RAM_STORAGE__T_ADDRESS_TYPE)RAM_STORAGE_G_ram_handler.head || end == (RAM_STORAGE__T_ADDRESS_TYPE)RAM_STORAGE_G_ram_handler.tail )
		{
			distance += sizeof(RAM_STORAGE__T_ELEMENT);
		}
		distance += ((RAM_STORAGE__T_ADDRESS_TYPE)RAM_STORAGE_G_ram_handler.tail - start) + (end - (RAM_STORAGE__T_ADDRESS_TYPE)RAM_STORAGE_G_ram_handler.head);
	}
	return distance;
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
	unsigned int counter = 0, max_readable = 0;
	unsigned char found = 0;
	RAM_STORAGE__T_ADDRESS_TYPE browser = NULL;
	RAM_STORAGE__T_ADDRESS_TYPE last_written = NULL;

	if(start_elem)
	{
		/* MAx readable is between start and last written element so: */
		if(RAM_STORAGE_G_ram_handler.write_to - sizeof(RAM_STORAGE__T_ELEMENT) < RAM_STORAGE_G_ram_handler.head)
		{
			last_written = RAM_STORAGE_G_ram_handler.tail;
		}
		else
		{
			last_written = RAM_STORAGE_G_ram_handler.write_to - sizeof(RAM_STORAGE__T_ELEMENT);
		}
		max_readable = get_abs_distance_between_address((RAM_STORAGE__T_ADDRESS_TYPE)start_elem, last_written);
		max_readable /= sizeof(RAM_STORAGE__T_ELEMENT);
		
		browser = (RAM_STORAGE__T_ADDRESS_TYPE)start_elem;	

		while( (!found) && (counter < max_readable) )
		{

			if( 	((RAM_STORAGE__T_ELEMENT*)browser)->time <= time 
				&&  ((RAM_STORAGE__T_ELEMENT*)browser)->time >= start_elem->time)
			{
				if( (counter + 1) == max_readable)
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
	unsigned int nb = 0;



	if(start_add > end_add)
	{
		if(start_add == RAM_STORAGE_G_ram_handler.head || start_add == RAM_STORAGE_G_ram_handler.tail)
		{
			nb += 1;
		}

		else if(end_add == RAM_STORAGE_G_ram_handler.head || end_add == RAM_STORAGE_G_ram_handler.tail)
		{
			nb += 1;
		}
		nb += (((RAM_STORAGE_G_ram_handler.tail - start_add) + (end_add - RAM_STORAGE_G_ram_handler.head))/sizeof(RAM_STORAGE__T_ELEMENT)) + 1;
		return nb;
	}
	else
	{
		nb += ((end_add - start_add)/sizeof(RAM_STORAGE__T_ELEMENT)) + 1;
		return nb;
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

	printf("There is %d elements to delete up to %d !\n",number_of_elements_to_delete,time);
	for(counter = 0 ; counter < number_of_elements_to_delete ; counter++)
	{
		print_element(*(RAM_STORAGE__T_ELEMENT*)browser);

		memset(browser,0,sizeof(RAM_STORAGE__T_ELEMENT));
		RAM_STORAGE_G_ram_handler.number_of_elements -= 1;

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


static void update_descending_browser(RAM_STORAGE__T_ADDRESS_TYPE* browser)
{
	if( (*browser - sizeof(RAM_STORAGE__T_ELEMENT)) < RAM_STORAGE_G_ram_handler.head )
	{
		*browser = RAM_STORAGE_G_ram_handler.tail;
	}
	else
	{
		*browser -= sizeof(RAM_STORAGE__T_ELEMENT);
	}
}

static unsigned int get_oldest_time(void)
{
	RAM_STORAGE__T_ADDRESS_TYPE browser = NULL;
	unsigned int counter = 0;
	unsigned int oldest_time = 0;
	
	/* Start from last added and look for all elements contained in ram storage */
	browser = RAM_STORAGE_G_ram_handler.write_to;

	update_descending_browser(&browser);
	oldest_time = ((RAM_STORAGE__T_ELEMENT*)browser)->time;
	/* cause of the 2 last lines, it means we already count one element (the last written) so start count from 1 */
	counter = 1;
	while((counter < RAM_STORAGE_G_ram_handler.number_of_elements))
	{
		update_descending_browser(&browser);

		if( ((RAM_STORAGE__T_ELEMENT*)browser)->time <= oldest_time )
		{
			oldest_time = ((RAM_STORAGE__T_ELEMENT*)browser)->time;
		}
		counter++;
	}
	return oldest_time;
}

/**
 * @brief      Gets the newest time.
 *
 * @param      void  The void
 *
 * @return     The newest time.
 */
static unsigned int get_newest_time(void)
{
	RAM_STORAGE__T_ADDRESS_TYPE browser = NULL;
	/* Start from last added and look for all elements contained in ram storage */
	browser = RAM_STORAGE_G_ram_handler.write_to;
	update_descending_browser(&browser);
	/* The newest one is necesserarly the last one added */
	return ((RAM_STORAGE__T_ELEMENT*)browser)->time;
}

void RAM_STORAGE_update_with_oldest_time(unsigned int * oldest_time)
{
	*oldest_time = get_oldest_time();
}

void RAM_STORAGE_update_with_newest_time(unsigned int * newest_time)
{
	*newest_time = get_newest_time();
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
	printf("\n\r*** RAM Storage heap: ***\n\r");
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
