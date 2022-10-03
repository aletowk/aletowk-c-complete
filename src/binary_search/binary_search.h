#ifndef _BINARY_SEARCH_H_INCLUDED
#define _BINARY_SEARCH_H_INCLUDED

typedef int (*BIN__T_COMPARATOR_CALLBACK)( const unsigned char* a, const unsigned char * b);

unsigned char* BIN_SEARCH_search(unsigned char * search_item_address,
                                 unsigned char* array,
                                 unsigned int array_size,
                                 unsigned int elm_size,
                                 BIN__T_COMPARATOR_CALLBACK comparator_callback);

void BIN_SEARCH_test(void);


#endif