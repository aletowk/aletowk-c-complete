#include "packet_store_sdcard.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

#define PACKET_STORE_SDCARD__C_INFO_FILENAME "info.bin"

#define PACKET_STORE_SDCARD__M_GET_1D_ARRAY_LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))

static PACKET_STORE_SDCARD__T_FILE_HANDLER    PACKET_STORE_SDCARD_G_file_handler;
static const PACKET_STORE_SDCARD__T_ELEMENT null_element;


static int update_info_file(void);
static void update_browser_ascending(long * browser);
static void update_browser_descending(long * browser);

void get_time(unsigned char *time_array)
{
	static unsigned int time = 0;

	time++;
	memcpy(time_array,&time,4);
}

static unsigned char test_sample[PACKET_STORE_SDCARD__C_MAX_TM_LENGTH] = {0xAA,0x44,0x12};

void test_packet_store_sdcard(void)
{
	int ret = 0;
	unsigned int i = 0;
	unsigned int start_read = 0;
	unsigned int end_read = 0;
	unsigned char storage_time_array[5] = {0};
	PACKET_STORE_SDCARD__T_ELEMENT readable_list[5];
	unsigned int read_number = 0;

	printf("TEST PACKET STORE SDCard\n\r");

	(void)read_number;
	(void)readable_list;
	(void)start_read;

	ret = PACKET_STORE_SDCARD_init();
	if(ret)
	{
		printf("[PACKET STORE SDCARD] Error while PACKET_STORE_SDCARD_init code : %d\n\r",ret);
	}
	else
	{
		printf("[PACKET STORE SDCARD] Successfully initialized, file info are :\n\r");
		PACKET_STORE_SDCARD_get_info();
	}

	for(i = 0; i < 4; i++)
	{
		get_time(storage_time_array);
		ret = PACKET_STORE_SDCARD_store_callback(NULL,test_sample,sizeof(test_sample),storage_time_array);
		if(ret != 0)
		{
			printf("[PACKET STORE SDCARD] PACKET_STORE_SDCARD_store_callback error code : %d\n\r",ret);
		}
	}

	PACKET_STORE_SDCARD_get_info();


	// /* Read :  */
	printf("Type start_read : \n");
	start_read = getchar();
	getchar();
	start_read = atoi((char*)&start_read);
	printf("Type end_read : \n");
	end_read = getchar();
	getchar();
	end_read = atoi((char*)&end_read);

	ret = PACKET_STORE_SDCARD_read_elements_between_two_dates(readable_list,PACKET_STORE_SDCARD__M_GET_1D_ARRAY_LENGTH(readable_list),
															  &read_number, start_read,end_read);
	if(ret)
	{
		printf("[PACKET STORE SDCARD] PACKET_STORE_SDCARD_read_elements_between_two_dates failed code : %d\n\r",ret);
	}
	else
	{
		print_element_list(readable_list,read_number);
	}


	/* Delete */
	// printf("Type up time to delete :\n");
	// end_read = getchar();
	// getchar();
	// end_read = atoi((char*)&end_read);
	// ret = PACKET_STORE_SDCARD_delete_up_to_time(end_read);
	// if(ret)
	// {
	// 	printf("Delete up to %d failed code : %d\n\r",end_read,ret);
	// }

}

int PACKET_STORE_SDCARD_init(void)
{
	struct stat statbuf;
	FILE* fp = NULL;
	// Perform task register and volume initialization here

	// Initializes handler (not regarding info file)
	// Head of SDCard memory area is the start of the file;
	PACKET_STORE_SDCARD_G_file_handler.head = 0;
	// Tail is the last storable element (inclusive)
	PACKET_STORE_SDCARD_G_file_handler.tail = PACKET_STORE_SDCARD_G_file_handler.head 
										    + ((PACKET_STORE_SDCARD__C_MAX_TM_NUMBER-1)*sizeof(PACKET_STORE_SDCARD__T_ELEMENT));

	// Depending on last usage of file, we update the others infos
	// Create a file to hanlde information about the effective file used to store TM
	if( stat(PACKET_STORE_SDCARD__C_INFO_FILENAME,&statbuf) != 0)
	{
		// File not yet created. We initializes the others fields as if it was the first time we use it.
		PACKET_STORE_SDCARD_G_file_handler.write_to = PACKET_STORE_SDCARD_G_file_handler.head;
		PACKET_STORE_SDCARD_G_file_handler.tm_number = 0;
		PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position = 0;
		PACKET_STORE_SDCARD_G_file_handler.oldest_time = 0;

		// Create the file :
		fp = fopen(PACKET_STORE_SDCARD__C_INFO_FILENAME,"w");
		if(!fp)
		{
			return -1;
		}
		fwrite(&PACKET_STORE_SDCARD_G_file_handler,1,sizeof(PACKET_STORE_SDCARD_G_file_handler),fp);
		fclose(fp);
	}
	else
	{
		// File already created, we actualize our hanlder with its infos
		fp = fopen(PACKET_STORE_SDCARD__C_INFO_FILENAME,"r");
		if(!fp)
		{
			return -1;
		}
		fread(&PACKET_STORE_SDCARD_G_file_handler,sizeof(PACKET_STORE_SDCARD_G_file_handler),1,fp);
		fclose(fp);
	}

	// Init null element:
	memset((void*)&null_element,0,sizeof(null_element));
	return 0;
}



int PACKET_STORE_SDCARD_store_callback(const unsigned char *packet_store_id, const unsigned char *data_ptr,
								 	   const unsigned int data_size, const unsigned char *storage_time)
{
	FILE* fp = NULL;
	int ret = 0;
	PACKET_STORE_SDCARD__T_ELEMENT element_to_write = {.coarse_time = 0, .tm_size = 0,.tm_buffer = {0}};

	(void)packet_store_id;

	if(PACKET_STORE_SDCARD_G_file_handler.tm_number >= PACKET_STORE_SDCARD__C_MAX_TM_NUMBER)
	{
		return -1;
	}
	element_to_write.coarse_time = (storage_time[0]) + (storage_time[1]<<8) + (storage_time[2] << 16) + (storage_time[3]<<24);
	element_to_write.tm_size = data_size;
	_memcpy8(element_to_write.tm_buffer,data_ptr,PACKET_STORE_SDCARD__C_MAX_TM_LENGTH);
	
	if(PACKET_STORE_SDCARD_G_file_handler.tm_number == 0)
	{
		// If first element, it is necesserarly the oldest. at this point we did
		// not write the elemeent so the oldest position must be set to the 'write_to' position
		PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position = PACKET_STORE_SDCARD_G_file_handler.write_to;
		PACKET_STORE_SDCARD_G_file_handler.oldest_time = element_to_write.coarse_time;
	}
	else if(element_to_write.coarse_time < PACKET_STORE_SDCARD_G_file_handler.newest_time)
	{
		// It must be impossible to append a packet with a time tag less older than the actual newest time;
		return -2;
	}

	// Try to open as r+. If failed it means failed does not exists so open it as w to create it.
	fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"r+");
	if(!fp)
	{
		fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"w");
		if(!fp)
		{
			return 1;
		}
	}
	fseek(fp,PACKET_STORE_SDCARD_G_file_handler.write_to,SEEK_SET);
	fwrite(&element_to_write,1,sizeof(element_to_write),fp);
	fclose(fp);

	update_browser_ascending(&PACKET_STORE_SDCARD_G_file_handler.write_to);

	PACKET_STORE_SDCARD_G_file_handler.tm_number++;
	PACKET_STORE_SDCARD_G_file_handler.newest_time = element_to_write.coarse_time;

	ret = update_info_file();
	if(ret)
	{
		return -2;
	}
	return 0;
}


int search_for_start_time_position_in_file(long * position_of_oldest_tm,unsigned int start_time)
{
	FILE* fp;
	unsigned int i = 0;
	unsigned int tmp_time = 0;
	long browser = 0;
	fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"r");
	if(!fp)
	{
		return -1;
	}

	// start read from the oldest
	browser = PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position;
	for(i = 0 ; i < PACKET_STORE_SDCARD_G_file_handler.tm_number ; i++)
	{
		fseek(fp,browser,SEEK_SET);
		fread(&tmp_time,sizeof(unsigned int),1, fp);
		
		if(tmp_time >= start_time)
		{
			// We found it.
			*position_of_oldest_tm = PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position + (i*sizeof(PACKET_STORE_SDCARD__T_ELEMENT));
			fclose(fp);
			return 0;
		}
		update_browser_ascending(&browser);
	}
	fclose(fp);
	return 1;
}

int search_for_end_time_position_in_file(long * position_of_end_tm,unsigned int end_tm)
{
	FILE* fp;
	unsigned int i = 0;
	unsigned int tmp_time = 0;
	long browser = 0;

	fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"r");
	if(!fp)
	{
		return -1;
	}

	// start read from the newest (last added)
	browser = PACKET_STORE_SDCARD_G_file_handler.write_to;
	for(i = 0 ; i < PACKET_STORE_SDCARD_G_file_handler.tm_number ; i++)
	{
		update_browser_descending(&browser);
		fseek(fp,browser,SEEK_SET);
		fread(&tmp_time,sizeof(unsigned int),1, fp);
		
		if(tmp_time <= end_tm)
		{
			// We found it.
			*position_of_end_tm = browser;
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;
}

int get_number_of_elements_between_two_position(long start, long end)
{
	if(end >= start)
	{
		return ((end - start)/sizeof(PACKET_STORE_SDCARD__T_ELEMENT)) + 1;
	}
	else
	{
		return ( ( 
					 (PACKET_STORE_SDCARD_G_file_handler.tail - start) 
			       + (end - PACKET_STORE_SDCARD_G_file_handler.head) 
			     )/sizeof(PACKET_STORE_SDCARD__T_ELEMENT) ) + 1;
	}
}

int PACKET_STORE_SDCARD_read_elements_between_two_dates(PACKET_STORE_SDCARD__T_ELEMENT * elements_to_read,
														unsigned int max_element_to_read,
	 													unsigned int *read_number, 
	 													unsigned int start_time, 
	 													unsigned int end_time )
{
	FILE* fp = NULL;
	unsigned int i = 0;
	long start_pos = 0;
	long end_pos = 0;
	long browser = 0;
	int ret = 0;
	// PACKET_STORE_SDCARD__T_ELEMENT tmp_start_elem;

	if(start_time <= PACKET_STORE_SDCARD_G_file_handler.oldest_time )
	{
		start_pos = PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position;
		// Then we ll look for all elements up to end_time
		if(end_time >= PACKET_STORE_SDCARD_G_file_handler.newest_time)
		{
			// Dump all ! 
			if(PACKET_STORE_SDCARD_G_file_handler.tm_number > max_element_to_read)
			{
				return -1;
			}
			fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"r");
			if(!fp)
			{
				return 1;
			}
			
			browser = PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position;
			for(i  = 0 ; i  < PACKET_STORE_SDCARD_G_file_handler.tm_number ; i++)
			{
				fseek(fp,browser,SEEK_SET);
				fread(&elements_to_read[i],sizeof(elements_to_read[i]),1,fp);
				update_browser_ascending(&browser);
			}
			fclose(fp);
			*read_number = PACKET_STORE_SDCARD_G_file_handler.tm_number;
			return 0;
		}
		else
		{
			// Must search for last element
			ret = search_for_end_time_position_in_file(&end_pos,end_time);
			if(ret)
			{
				printf("[PACKET_STORE_SDCARD_read_elements_between_two_dates] End time : %d not found !\n\r",end_time);
				return 2;
			}
			else
			{	
				*read_number = get_number_of_elements_between_two_position(start_pos,end_pos);
				if(*read_number > max_element_to_read)
				{
					return -1;
				}

				fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"r");
				if(!fp)
				{
					return 1;
				}

				browser = start_pos;
				for(i = 0; i  < *read_number ; i++)
				{
					fseek(fp,browser,SEEK_SET);
					fread(&elements_to_read[i],sizeof(elements_to_read[i]),1,fp);
					update_browser_ascending(&browser);
				}
				fclose(fp);
				return 0;
			}
		}
	}
	else
	{
		// Must search for the first element
		ret = search_for_start_time_position_in_file(&start_pos,start_time);
		if(ret)
		{
			printf("[PACKET_STORE_SDCARD_read_elements_between_two_dates] Start time : %d not found !\n\r",start_time);
			return 2;
		}
		else
		{
			// printf("[PACKET_STORE_SDCARD_read_elements_between_two_dates] Start time : %d found ! Start element is:\n\r",start_time);
			// ret = get_element_in_file(start_pos,&tmp_start_elem);
			// if(ret)
			// {
			// 	printf("Error get element in file, code %d\n\r",ret);
			// 	return 1;
			// }
			// print_sdcard_element(tmp_start_elem);

			if(end_time >= PACKET_STORE_SDCARD_G_file_handler.newest_time)
			{
				// Get all elements from the start element previously found and the last element
				end_pos = PACKET_STORE_SDCARD_G_file_handler.write_to;
				update_browser_descending(&end_pos); // access to last added.

				*read_number = get_number_of_elements_between_two_position(start_pos,end_pos);
				if(*read_number > max_element_to_read)
				{
					return -1;
				}

				fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"r");
				if(!fp)
				{
					return 1;
				}
				browser = start_pos;
				
				for(i = 0; i  < *read_number ; i++)
				{
					fseek(fp,browser,SEEK_SET);
					fread(&elements_to_read[i],sizeof(elements_to_read[i]),1,fp);
					update_browser_ascending(&browser);
				}
				fclose(fp);
				return 0;
			}
			else
			{
				// Must search for specific last element
				ret = search_for_end_time_position_in_file(&end_pos,end_time);
				if(ret)
				{
					printf("[PACKET_STORE_SDCARD_read_elements_between_two_dates] End time : %d not found !\n\r",end_time);
					return 2;
				}
				else
				{
					// printf("Found last time %d:\n\r",end_time);
					// ret = get_element_in_file(end_pos,&tmp_start_elem);
					// if(ret)
					// {
					// 	printf("Error get element code %d\n\r",ret);
					// 	return 1;
					// }
					// print_sdcard_element(tmp_start_elem);
					
					*read_number = get_number_of_elements_between_two_position(start_pos,end_pos);
					if(*read_number > max_element_to_read)
					{
						return -1;
					}

					fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"r");
					if(!fp)
					{
						return 1;
					}

					browser = start_pos;
					for(i = 0; i  < *read_number ; i++)
					{
						fseek(fp,browser,SEEK_SET);
						fread(&elements_to_read[i],sizeof(elements_to_read[i]),1,fp);
						update_browser_ascending(&browser);
					}
					fclose(fp);
					return 0;
				}
			}
		}
	}
	return 0;
}


int PACKET_STORE_SDCARD_delete_up_to_time(unsigned int time)
{
	unsigned int i = 0;
	unsigned int stop = 0;
	int ret = 0;
	long end_pos = 0;
	long browser = 0;
	FILE* fp = NULL;

	if(time >= PACKET_STORE_SDCARD_G_file_handler.newest_time)
	{
		// Delete all
		fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"r+");
		if(!fp)
		{
			return 1;
		}
		fseek(fp,PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position,SEEK_SET);
		for(i = 0; i < PACKET_STORE_SDCARD_G_file_handler.tm_number ; i++)
		{
			fwrite(&null_element,1,sizeof(null_element),fp);
		}
		fclose(fp);
		PACKET_STORE_SDCARD_G_file_handler.write_to = 0;
		PACKET_STORE_SDCARD_G_file_handler.tm_number = 0;
		PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position = 0;
		PACKET_STORE_SDCARD_G_file_handler.oldest_time = 0;
		PACKET_STORE_SDCARD_G_file_handler.newest_time = 0;
	}
	else
	{
		// Search for end time
		ret = search_for_end_time_position_in_file(&end_pos,time);
		if(ret)
		{
			printf("[PACKET_STORE_SDCARD_delete_up_to_time] End time : %d not found !\n\r",time);
			return 2;
		}
		fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"r+");
		if(!fp)
		{
			return 1;
		}
		
		stop = get_number_of_elements_between_two_position(PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position,end_pos);
		browser = PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position;
		for(i = 0 ; i < stop ; i++)
		{
			fseek(fp,browser,SEEK_SET);
			fwrite(&null_element,1,sizeof(null_element),fp);
			update_browser_ascending(&browser);		
		}
		PACKET_STORE_SDCARD_G_file_handler.tm_number -= stop;
		PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position = browser;

		// Read the new oldest time
		fseek(fp,PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position,SEEK_SET);
		fread(&PACKET_STORE_SDCARD_G_file_handler.oldest_time,1,sizeof(unsigned int),fp );
		fclose(fp);
	}
	
	update_info_file();
	return 0;
}

unsigned char* _memcpy8(unsigned char * dest, const unsigned char * src,unsigned int n)
{
	unsigned int i = 0;
	for(i = 0 ; i < n ; i++)
	{
		dest[i] = src[i];
	}
	return dest;
}


void PACKET_STORE_SDCARD_get_info(void)
{
	printf("[PACKET_STORE_SDCARD] Infos File  : %s\n\r",PACKET_STORE_SDCARD__C_FILENAME);
	printf("                      head pos    : %ld\n\r",PACKET_STORE_SDCARD_G_file_handler.head);
	printf("                      tail pos    : %ld\n\r",PACKET_STORE_SDCARD_G_file_handler.tail);
	printf("                      write_to pos: %ld\n\r",PACKET_STORE_SDCARD_G_file_handler.write_to);

	printf("                      TM Number   : %d\n\r",PACKET_STORE_SDCARD_G_file_handler.tm_number);
	printf("                      Oldest pos  : %ld\n\r",PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position);
	printf("                      Oldest time : %d\n\r",PACKET_STORE_SDCARD_G_file_handler.oldest_time);
	printf("                      Newest time : %d\n\r",PACKET_STORE_SDCARD_G_file_handler.newest_time);
}

static int update_info_file(void)
{
	FILE* fp;
	// File not yest created. We do create it:
	fp = fopen(PACKET_STORE_SDCARD__C_INFO_FILENAME,"r+");
	if(!fp)
	{
		return -1;
	}
	rewind(fp);
	fwrite(&PACKET_STORE_SDCARD_G_file_handler,1,sizeof(PACKET_STORE_SDCARD_G_file_handler),fp);
	fclose(fp);	
	return 0;
}

void print_hexa_line(unsigned char * buff, unsigned int size)
{
	unsigned int i = 0;
	for(i = 0 ; i < size ; i++)
	{
		printf("0x%02X ",buff[i]);
	}
	printf("\n\r");
}

void print_sdcard_element(PACKET_STORE_SDCARD__T_ELEMENT elem)
{
	printf("Time : %d\n\r",elem.coarse_time);
	printf("Size : %d\n\r",elem.tm_size);
	printf("TM : ");
	print_hexa_line(elem.tm_buffer,elem.tm_size);
}

void print_element_list(PACKET_STORE_SDCARD__T_ELEMENT * buf,unsigned int size)
{
	unsigned int i = 0;
	printf("[PRINT ELEM LIST] Showing %d elements:\n\r",size);
	for(i = 0 ; i < size ; i++)
	{
		print_sdcard_element(buf[i]);
	}
}
int get_element_in_file(long position,PACKET_STORE_SDCARD__T_ELEMENT * elem)
{
	FILE * fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"r");
	if(!fp)
	{
		return -1;
	}

	fseek(fp,position,SEEK_SET);
	fread(elem,sizeof(PACKET_STORE_SDCARD__T_ELEMENT),1,fp);

	fclose(fp);
	return 0;
}

static void update_browser_ascending(long * browser)
{
	if( ((*browser) + (long)sizeof(PACKET_STORE_SDCARD__T_ELEMENT)) > PACKET_STORE_SDCARD_G_file_handler.tail)
	{
		*browser = PACKET_STORE_SDCARD_G_file_handler.head;
	}
	else
	{
		*browser += sizeof(PACKET_STORE_SDCARD__T_ELEMENT);	
	}
}

static void update_browser_descending(long * browser)
{
	if( (((*browser) - (long)sizeof(PACKET_STORE_SDCARD__T_ELEMENT)) < PACKET_STORE_SDCARD_G_file_handler.head)  )
	{
		*browser = PACKET_STORE_SDCARD_G_file_handler.tail;
	}
	else
	{
		*browser -= sizeof(PACKET_STORE_SDCARD__T_ELEMENT);	
	}
}