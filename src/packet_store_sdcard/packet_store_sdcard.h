#ifndef _PACKET_STORE_SDCARD_H_INCLUDED
#define _PACKET_STORE_SDCARD_H_INCLUDED 


#define PACKET_STORE_SDCARD__C_FILENAME "packet_store.bin"

#define PACKET_STORE_SDCARD__C_MAX_TM_LENGTH (10)

typedef struct _PACKET_STORE_SDCARD__T_ELEMENT
{
	unsigned int coarse_time;
	unsigned int tm_size;
	unsigned char tm_buffer[PACKET_STORE_SDCARD__C_MAX_TM_LENGTH];
}__attribute__((packed)) PACKET_STORE_SDCARD__T_ELEMENT;



void test_packet_store_sdcard(void);


void PACKET_STORE_SDCARD_init(void);

int PACKET_STORE_SDCARD_store_callback(const unsigned char *packet_store_id, const unsigned char *data_ptr,
								 	   const unsigned int data_size, const unsigned char *storage_time);

int PACKET_STORE_SDCARD_read_elements_between_two_dates(PACKET_STORE_SDCARD__T_ELEMENT * elements_to_read,
														unsigned int max_element_to_read,
	 													unsigned int *read_number, 
	 													unsigned int start_time, 
	 													unsigned int end_time );


unsigned char* _memcpy8(unsigned char * dest, const unsigned char * src,unsigned int n);

#endif
