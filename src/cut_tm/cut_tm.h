#ifndef _CUT_TM_H_
#define _CUT_TM_H_ 

#include <stdio.h>

// TM Format :
// | N_part | part_id | < the part >
// 

#define LPUS_TM_CONFIGURATION__C_MAX_TOTAL_LENGTH (240)

#define C__MAX_PARTS_NUMBER (250)

typedef unsigned char TOTAL_PART_NUMBER__T;
#define C__SIZEOF_TOTAL_NUMBER_FIELD (sizeof(TOTAL_PART_NUMBER__T))
typedef unsigned char ACTUAL_PART_NUMBER__T;
#define C__SIZEOF_ACTUAL_PART_FIELD (sizeof(ACTUAL_PART_NUMBER__T))

#define C__OUTPUT_TMS_ARRAY_SIZE (C__MAX_PARTS_NUMBER*LPUS_TM_CONFIGURATION__C_MAX_TOTAL_LENGTH)

#define C__MAX_PART_SIZE (LPUS_TM_CONFIGURATION__C_MAX_TOTAL_LENGTH - C__SIZEOF_TOTAL_NUMBER_FIELD - C__SIZEOF_ACTUAL_PART_FIELD)

void test_cut_tm(void);

void cut_the_tm(FILE* fp, unsigned int base_size, TOTAL_PART_NUMBER__T * _number_of_parts);

void reconstruct(void);

#endif