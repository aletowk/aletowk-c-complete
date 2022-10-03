#include "binary_search.h"

#include <stdio.h>

#define ARRAY_1D_LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))

typedef unsigned int UINT32_T;

int compare_UINT32(const unsigned char * a, const unsigned char * b)
{
    if( (*(UINT32_T *)a) > (*(UINT32_T *)b)  )
    {
        return 1; // A > B
    }
    else if( (*(UINT32_T *)a) < (*(UINT32_T *)b) )
    {
        return -1; // A > B
    }
    return 0;
}

unsigned char* BIN_SEARCH_search(unsigned char * search_item_address,
                                 unsigned char* array,
                                 unsigned int array_size,
                                 unsigned int elm_size,
                                 BIN__T_COMPARATOR_CALLBACK comparator_callback)
{
    unsigned int low_index = 0;
    unsigned int high_index = array_size;
    unsigned int mid_index = 0;
    int comparison_result = 0; // If neg , search item might be in lower layer of the array,
    unsigned char * return_address = 0;

    while( (high_index > low_index) && (return_address == 0))
    {
        mid_index = low_index + ((high_index - low_index) / 2);
        comparison_result = comparator_callback(search_item_address, array + (mid_index*elm_size));
        if(comparison_result < 0)
        {
            high_index = mid_index; 
        }
        else if(comparison_result > 0)
        {
            low_index = mid_index + 1;
        }
        else
        {
            return_address = array + (mid_index*elm_size);
        }
    }
    return return_address;
}

void BIN_SEARCH_test(void)
{
    UINT32_T tab[] = {1,2,3,4,12,35};
    UINT32_T search_item = -1;
    unsigned char * found_elm = 0;

    found_elm = BIN_SEARCH_search((unsigned char*)&search_item,
                                  (unsigned char*)tab,
                                  ARRAY_1D_LENGTH(tab),
                                  sizeof(UINT32_T),compare_UINT32);
    if(found_elm)
    {
        printf("Found %d in tab at position : %ld\n\r",*(UINT32_T*)(found_elm),
                                                       ((found_elm - (unsigned char*)(tab)))/sizeof(UINT32_T));
    }
    else
    {
        printf("Did not found %d in tab\n\r",search_item);
    }

}