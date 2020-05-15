#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "static_allocation.h"

/**
 * @brief      Main function of the API
 *
 * @return     0 if no errors
 */
int main(void)
{
	test_static_malloc();
	return 0;
}