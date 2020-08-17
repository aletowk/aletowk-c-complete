#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "packet_store_sdcard.h"

/**
 * @brief      Main function of the API
 *
 * @return     0 if no errors
 */
int main(void)
{
	test_packet_store_sdcard();
	return 0;
}