#include "cut_tm.h"

#include <sys/stat.h>
#include "common.h"

// #define C__GPSRESULT_FILENAME "./src/cut_tm/GPSRES2_TEC_600.BIN"
#define C__GPSRESULT_FILENAME "./src/cut_tm/GPSRES.BIN"
// each part will have :
// total number | actual part number | <part>
//      1B      |        1B          |   deduced bytes
static unsigned char output_cut_tm[C__OUTPUT_TMS_ARRAY_SIZE];
static unsigned int output_size = 0;

static unsigned int get_file_octet_size(const char * filename);
static void save_to_new_file(void);

void test_cut_tm(void)
{
	TOTAL_PART_NUMBER__T number_of_parts = 0;
	unsigned int octet_size = 0;
	FILE* fp = NULL;

	octet_size = get_file_octet_size(C__GPSRESULT_FILENAME);
	printf("[CUT TM] Size of %s is : %dB\n\r",C__GPSRESULT_FILENAME,octet_size);

	// Open file : 
	fp = fopen(C__GPSRESULT_FILENAME,"r");
	if(!fp)
	{
		printf("[CUT TM] Error while opening %s in read mode\n\r",C__GPSRESULT_FILENAME);
		return;
	}

	cut_the_tm(fp,octet_size,&number_of_parts);

	save_to_new_file();

	reconstruct();

	fclose(fp);
}



void cut_the_tm(FILE* fp, unsigned int base_size, TOTAL_PART_NUMBER__T * _number_of_parts)
{
	ACTUAL_PART_NUMBER__T i = 0;
	unsigned int j = 0;
	unsigned int file_offset = 0;
	unsigned int output_offset = 0;
	unsigned int part_size = 0;
	unsigned char partFileBuff[C__MAX_PART_SIZE] = {0};

	// Get number of parts:
	*_number_of_parts = (base_size / C__MAX_PART_SIZE);
	if((base_size%C__MAX_PART_SIZE) != 0)
	{
		*(_number_of_parts) += 1;
	}
	printf("[CUT TM] Base TM will be cut in %d parts\n\r",*_number_of_parts);

	for(i = 0 ; i < *_number_of_parts ; i++)
	{
		output_offset = i*LPUS_TM_CONFIGURATION__C_MAX_TOTAL_LENGTH;
		file_offset = i*C__MAX_PART_SIZE;

		output_cut_tm[output_offset] = *_number_of_parts;
		output_offset += C__SIZEOF_TOTAL_NUMBER_FIELD;
	
		output_cut_tm[output_offset] = i;
		output_offset += C__SIZEOF_ACTUAL_PART_FIELD;

		// Warning, last part could be not full
		if(i != (*(_number_of_parts) - 1) )
		{
			part_size = C__MAX_PART_SIZE;
		}
		else
		{
			part_size = (base_size % C__MAX_PART_SIZE);
		}

		fseek(fp,file_offset,SEEK_SET);
		fread(partFileBuff,1,part_size,fp);
		for(j = 0 ; j < part_size ; j++)
		{
			output_cut_tm[output_offset + j] = partFileBuff[j];
		}
		output_size += C__SIZEOF_TOTAL_NUMBER_FIELD + C__SIZEOF_ACTUAL_PART_FIELD + part_size;
	}
	printf("Written %d bytes !\n\r",output_size);
}

static void save_to_new_file(void)
{
	FILE* fp = NULL;
	const char filename[] = "./src/cut_tm/out_file.bin";

	fp = fopen(filename,"w");
	if(!fp)
	{
		printf("Error while save_to_new_file opening %s in w mode\n\r",filename);
		return;
	}

	fwrite(output_cut_tm,1,output_size,fp);

	fclose(fp);
}

static unsigned int get_file_octet_size(const char * filename)
{
	struct stat file_stats;
	int ret = 0;

	ret = stat(filename,&file_stats);
	if(ret)
	{
		printf("Error while get_file_octet_size of file %s\n\r",filename);
		return 0;
	}
	return file_stats.st_size;
}



void reconstruct(void)
{
	FILE * fp_out = NULL;
	FILE*  fp_in = NULL;
	const char out_filename[] = "./src/cut_tm/out_reconstructed.bin";
	const char in_filename[]  = "./src/cut_tm/out_file.bin";

	unsigned char tmp_buffer[C__MAX_PART_SIZE] = {0};
	TOTAL_PART_NUMBER__T tmp_total_size = 0;
	unsigned int offset = 0;
	unsigned int last_part = 0;
	TOTAL_PART_NUMBER__T i = 0; 

	fp_in = fopen(in_filename,"r");
	fp_out = fopen(out_filename,"w");
	if( (!fp_in) || (!fp_out) )
	{
		printf("[RECONSTRUCT] Error while opening files\n\r");
		return;
	}

	fseek(fp_in,0,SEEK_SET);
	fread(&tmp_total_size, C__SIZEOF_TOTAL_NUMBER_FIELD, 1, fp_in);
	printf("In input file, there are %d parts\n\r",tmp_total_size);

	fseek(fp_in,C__SIZEOF_TOTAL_NUMBER_FIELD + C__SIZEOF_ACTUAL_PART_FIELD,SEEK_SET);
	for( i = 0 ; i < tmp_total_size ; i++)
	{
		fseek(fp_in,offset,SEEK_SET);
		// Read part per part
		if( i < tmp_total_size - 1)
		{
			fread(tmp_buffer,LPUS_TM_CONFIGURATION__C_MAX_TOTAL_LENGTH,1,fp_in);
			fwrite(&tmp_buffer[2],C__MAX_PART_SIZE,1,fp_out);
		}
		else
		{
			// It could remains less then LPUS_TM_CONFIGURATION__C_MAX_TOTAL_LENGTH TO READ:
			last_part = output_size - offset;
			fread(tmp_buffer,last_part,1,fp_in);
			fwrite(&tmp_buffer[2],last_part - C__SIZEOF_TOTAL_NUMBER_FIELD - C__SIZEOF_ACTUAL_PART_FIELD,1,fp_out);
		}
		offset += LPUS_TM_CONFIGURATION__C_MAX_TOTAL_LENGTH;
	}
	fclose(fp_in);
	fclose(fp_out);
}