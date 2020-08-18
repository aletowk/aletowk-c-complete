#ifndef _PACKET_STORE_SDCARD_H_INCLUDED
#define _PACKET_STORE_SDCARD_H_INCLUDED 


#define PACKET_STORE_SDCARD__C_FILENAME "packet_store.bin"

#define PACKET_STORE_SDCARD__C_MAX_TM_LENGTH (10)

#define PACKET_STORE_SDCARD__C_MAX_TM_NUMBER (5)

typedef struct _PACKET_STORE_SDCARD__T_FILE_HANDLER
{
	long head;
	long tail;

	long write_to;

	unsigned int tm_number;

	long oldest_tm_position;
	unsigned int oldest_time;
	
	// long newest_tm_position; // shoudl be write_to minus 1 element
	unsigned int newest_time;
} __attribute__((packed)) PACKET_STORE_SDCARD__T_FILE_HANDLER;

typedef struct _PACKET_STORE_SDCARD__T_ELEMENT
{
	unsigned int coarse_time;
	unsigned int tm_size;
	unsigned char tm_buffer[PACKET_STORE_SDCARD__C_MAX_TM_LENGTH];
}__attribute__((packed)) PACKET_STORE_SDCARD__T_ELEMENT;

void test_packet_store_sdcard(void);


int PACKET_STORE_SDCARD_init(void);

int PACKET_STORE_SDCARD_store_callback(const unsigned char *packet_store_id, const unsigned char *data_ptr,
								 	   const unsigned int data_size, const unsigned char *storage_time);

int PACKET_STORE_SDCARD_read_elements_between_two_dates(PACKET_STORE_SDCARD__T_ELEMENT * elements_to_read,
														unsigned int max_element_to_read,
	 													unsigned int *read_number, 
	 													unsigned int start_time, 
	 													unsigned int end_time );

int PACKET_STORE_SDCARD_delete_up_to_time(unsigned int time);

void PACKET_STORE_SDCARD_get_info(void);

unsigned char* _memcpy8(unsigned char * dest, const unsigned char * src,unsigned int n);

void print_sdcard_element(PACKET_STORE_SDCARD__T_ELEMENT elem);

void print_element_list(PACKET_STORE_SDCARD__T_ELEMENT * buf,unsigned int size);

int get_element_in_file(long position,PACKET_STORE_SDCARD__T_ELEMENT * elem);


#endif

