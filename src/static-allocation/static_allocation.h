#ifndef STATIC_ALLOCATION_H_
#define STATIC_ALLOCATION_H_ 

/**
 * Static pool size in bytes
 */
#define C_STATIC_POOL_SIZE (64)


void test_static_malloc(void);

void STATIC__init_heap(void);

void* STATIC__malloc(int size);


#endif