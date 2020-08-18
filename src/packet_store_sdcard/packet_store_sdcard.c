#include "packet_store_sdcard.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

#define PACKET_STORE_SDCARD__C_INFO_FILENAME "info.bin"

static PACKET_STORE_SDCARD__T_FILE_HANDLER    PACKET_STORE_SDCARD_G_file_handler = 
												{
													.tm_number = 0,
													.oldest_tm_position = 0,
													.newest_tm_position = 0
												};


static int update_info_file(void);

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
	unsigned char storage_time_array[4] = {0};
	PACKET_STORE_SDCARD__T_ELEMENT readable_list[4];
	unsigned int read_number = 0;

	printf("TEST PACKET STORE SDCard\n\r");

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


	/* Read :  */
	printf("Type start_read : \n");
	start_read = getchar();
	getchar();
	start_read = atoi((char*)&start_read);
	printf("Type end_read : \n");
	end_read = getchar();
	getchar();
	end_read = atoi((char*)&end_read);

	ret = PACKET_STORE_SDCARD_read_elements_between_two_dates(readable_list,4,&read_number, start_read,end_read);
	if(ret)
	{
		printf("[PACKET STORE SDCARD] PACKET_STORE_SDCARD_read_elements_between_two_dates failed code : %d\n\r",ret);
	}
	else
	{
		print_element_list(readable_list,read_number);
	}
}



int PACKET_STORE_SDCARD_init(void)
{
	struct stat statbuf;
	FILE* fp = NULL;
	// Perform task register and volume initialization here

	// Create a file to hanlde information about the effective file used to store TM

	if( stat(PACKET_STORE_SDCARD__C_INFO_FILENAME,&statbuf) != 0)
	{
		// File not yest created. We do create it:
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
		// File created, we actualize our hanlder with its infos
		fp = fopen(PACKET_STORE_SDCARD__C_INFO_FILENAME,"r");
		if(!fp)
		{
			return -1;
		}
		fread(&PACKET_STORE_SDCARD_G_file_handler,sizeof(PACKET_STORE_SDCARD_G_file_handler),1,fp);
		fclose(fp);
	}
	return 0;
}



int PACKET_STORE_SDCARD_store_callback(const unsigned char *packet_store_id, const unsigned char *data_ptr,
								 	   const unsigned int data_size, const unsigned char *storage_time)
{
	FILE* fp = NULL;
	int ret = 0;
	PACKET_STORE_SDCARD__T_ELEMENT element_to_write = {
														.coarse_time = 0, 
														.tm_size = 0,
														.tm_buffer = {0}
													  };
	(void)packet_store_id;

	if(PACKET_STORE_SDCARD_G_file_handler.tm_number >= PACKET_STORE_SDCARD__C_MAX_TM_NUMBER)
	{
		return -1;
	}

	element_to_write.coarse_time = (storage_time[0]) + (storage_time[1]<<8) + (storage_time[2] << 16) + (storage_time[3]<<24);
	element_to_write.tm_size = data_size;
	_memcpy8(element_to_write.tm_buffer,data_ptr,PACKET_STORE_SDCARD__C_MAX_TM_LENGTH);

	fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"a");
	if(!fp)
	{
		return 1;
	}


	if(PACKET_STORE_SDCARD_G_file_handler.tm_number == 0)
	{
		PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position = ftell(fp);
		PACKET_STORE_SDCARD_G_file_handler.oldest_time = element_to_write.coarse_time;
		PACKET_STORE_SDCARD_G_file_handler.newest_tm_position = ftell(fp);
		PACKET_STORE_SDCARD_G_file_handler.newest_time = element_to_write.coarse_time;
	}
	else
	{
		PACKET_STORE_SDCARD_G_file_handler.newest_tm_position = ftell(fp);
		PACKET_STORE_SDCARD_G_file_handler.newest_time = element_to_write.coarse_time;
	}
	fwrite(&element_to_write,1,sizeof(element_to_write),fp);

	PACKET_STORE_SDCARD_G_file_handler.tm_number++;
	fclose(fp);

	ret = update_info_file();
	if(ret)
	{
		return -2;
	}
	return 0;
}


int serch_for_start_time_position_in_file(long * position_of_oldest_tm,unsigned int start_time)
{
	FILE* fp;
	unsigned int i = 0;
	unsigned int tmp_time = 0;

	fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"r");
	if(!fp)
	{
		return -1;
	}

	// start read from the oldest
	for(i = 0 ; i < PACKET_STORE_SDCARD_G_file_handler.tm_number ; i++)
	{
		fseek(fp,PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position + (i*sizeof(PACKET_STORE_SDCARD__T_ELEMENT)),SEEK_SET);
		fread(&tmp_time,sizeof(unsigned int),1, fp);
		
		if(tmp_time >= start_time)
		{
			// We found it.
			*position_of_oldest_tm = PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position + (i*sizeof(PACKET_STORE_SDCARD__T_ELEMENT));
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;
}

int serch_for_end_time_position_in_file(long * position_of_end_tm,unsigned int end_tm)
{
	FILE* fp;
	unsigned int i = 0;
	unsigned int tmp_time = 0;

	fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"r");
	if(!fp)
	{
		return -1;
	}

	// start read from the oldest
	for(i = 0 ; i < PACKET_STORE_SDCARD_G_file_handler.tm_number ; i++)
	{
		fseek(fp,PACKET_STORE_SDCARD_G_file_handler.newest_tm_position - (i*sizeof(PACKET_STORE_SDCARD__T_ELEMENT)),SEEK_SET);
		fread(&tmp_time,sizeof(unsigned int),1, fp);
		
		if(tmp_time <= end_tm)
		{
			// We found it.
			*position_of_end_tm = PACKET_STORE_SDCARD_G_file_handler.newest_tm_position - (i*sizeof(PACKET_STORE_SDCARD__T_ELEMENT));
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;
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
	int ret = 0;
	PACKET_STORE_SDCARD__T_ELEMENT tmp_start_elem;

	if(start_time <= PACKET_STORE_SDCARD_G_file_handler.oldest_time )
	{
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
			for(i  = 0 ; i  < PACKET_STORE_SDCARD_G_file_handler.tm_number ; i++)
			{
				fseek(fp,PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position + (i*sizeof(elements_to_read[i])),SEEK_SET);
				fread(&elements_to_read[i],sizeof(elements_to_read[i]),1,fp);
			}
			fclose(fp);
			*read_number = PACKET_STORE_SDCARD_G_file_handler.tm_number;
			return 0;
		}
		else
		{
			// Must search for last element
			ret = serch_for_end_time_position_in_file(&end_pos,end_time);
			if(ret)
			{
				printf("[PACKET_STORE_SDCARD_read_elements_between_two_dates] End time : %d not found !\n\r",end_time);
				return 2;
			}
			else
			{	
				*read_number = ((end_pos - PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position)/sizeof(PACKET_STORE_SDCARD__T_ELEMENT))  + 1; // +1 to count the last one to read too
				if(*read_number > max_element_to_read)
				{
					return -1;
				}
				fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"r");
				if(!fp)
				{
					return 1;
				}
				fseek(fp,start_pos,SEEK_SET);
				for(i = 0; i  < *read_number ; i++)
				{
					fread(&elements_to_read[i],sizeof(elements_to_read[i]),1,fp);
				}
				fclose(fp);
				return 0;
			}
		}
	}
	else
	{
		// Must search for the first element
		ret = serch_for_start_time_position_in_file(&start_pos,start_time);
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
				*read_number = ((PACKET_STORE_SDCARD_G_file_handler.newest_tm_position - start_pos)/sizeof(PACKET_STORE_SDCARD__T_ELEMENT))  + 1; // +1 to count the last one to read too
				if(*read_number > max_element_to_read)
				{
					return -1;
				}
				fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"r");
				if(!fp)
				{
					return 1;
				}
				fseek(fp,start_pos,SEEK_SET);
				for(i = 0; i  < *read_number ; i++)
				{
					fread(&elements_to_read[i],sizeof(elements_to_read[i]),1,fp);
				}
				fclose(fp);
				return 0;
			}
			else
			{
				// Must search for specific last element
				ret = serch_for_end_time_position_in_file(&end_pos,end_time);
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
					
					*read_number = ((end_pos - start_pos)/sizeof(PACKET_STORE_SDCARD__T_ELEMENT))  + 1; // +1 to count the last one to read too
					if(*read_number > max_element_to_read)
					{
						return -1;
					}
					fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"r");
					if(!fp)
					{
						return 1;
					}
					fseek(fp,start_pos,SEEK_SET);
					for(i = 0; i  < *read_number ; i++)
					{
						fread(&elements_to_read[i],sizeof(elements_to_read[i]),1,fp);
					}
					fclose(fp);
					return 0;
				}
			}
		}
	}
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
	printf("                      TM Number   : %d\n\r",PACKET_STORE_SDCARD_G_file_handler.tm_number);
	printf("                      Oldest pos  : %ld\n\r",PACKET_STORE_SDCARD_G_file_handler.oldest_tm_position);
	printf("                      Oldest time : %d\n\r",PACKET_STORE_SDCARD_G_file_handler.oldest_time);
	printf("                      Newest pos  : %ld\n\r",PACKET_STORE_SDCARD_G_file_handler.newest_tm_position);
	printf("                      Newest time : %d\n\r",PACKET_STORE_SDCARD_G_file_handler.newest_time);
}

static int update_info_file(void)
{
	FILE* fp;
	// File not yest created. We do create it:
	fp = fopen(PACKET_STORE_SDCARD__C_INFO_FILENAME,"w");
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