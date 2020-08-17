#include "packet_store_sdcard.h"

#include <stdio.h>
#include <string.h>

#ifndef NULL
#define NULL ((void*)0)
#endif


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

	unsigned char storage_time_array[4] = {0};
	printf("TEST PACKET STORE SDCard\n\r");


	for(i = 0; i < 3; i++)
	{
		get_time(storage_time_array);
		ret = PACKET_STORE_SDCARD_store_callback(NULL,test_sample,sizeof(test_sample),storage_time_array);
		if(ret != 0)
		{
			printf("[PACKET STORE SDCARD] PACKET_STORE_SDCARD_store_callback error code : %d\n\r",ret);
		}
	}
}



void PACKET_STORE_SDCARD_init(void)
{
	// Perform task register and volume initialization here
}



int PACKET_STORE_SDCARD_store_callback(const unsigned char *packet_store_id, const unsigned char *data_ptr,
								 	   const unsigned int data_size, const unsigned char *storage_time)
{
	FILE* fp = NULL;
	PACKET_STORE_SDCARD__T_ELEMENT element_to_write = {
														.coarse_time = 0, 
														.tm_size = 0,
														.tm_buffer = {0}
													  };
	(void)packet_store_id;

	element_to_write.coarse_time = (storage_time[0]) + (storage_time[1]<<8) + (storage_time[2] << 16) + (storage_time[3]<<24);
	element_to_write.tm_size = data_size;
	_memcpy8(element_to_write.tm_buffer,data_ptr,PACKET_STORE_SDCARD__C_MAX_TM_LENGTH);

	fp = fopen(PACKET_STORE_SDCARD__C_FILENAME,"a");
	if(!fp)
	{
		return 1;
	}

	fwrite(&element_to_write,1,sizeof(element_to_write),fp);

	fclose(fp);
	return 0;
}

int PACKET_STORE_SDCARD_read_elements_between_two_dates(PACKET_STORE_SDCARD__T_ELEMENT * elements_to_read,
														unsigned int max_element_to_read,
	 													unsigned int *read_number, 
	 													unsigned int start_time, 
	 													unsigned int end_time )
{
	(void)elements_to_read;
	(void)max_element_to_read;
	(void)read_number;
	(void)start_time;
	(void)end_time;
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