#define _CRT_SECURE_NO_WARNINGS

/***** INCLUDES *****/
#include <string.h>
#include "general.h"

/****** DEFINES ******/
#define FILE_ENDING_ERROR 1
#define MEMORY_ALLOCATION_ERROR 1

/**************************** FUNCTION IMPLEMENTATIOS *****************************/
/* This function check for allocation memory errors */
void* my_malloc(int amount_of_bytes)
{
	void* new_arr;
	new_arr = malloc(amount_of_bytes);
	if (new_arr == NULL)
	{
		printf("ERROR\n");
		exit(1);
	}
	return new_arr;
}
/* This function checks for realloc allocation memory error */
void checkReallocAllocation(void *p)
{
	if (p == NULL)
	{
		printf("Memory allocation error\n");
		exit(MEMORY_ALLOCATION_ERROR);
	}
}
/* This function checks for opening files errors */
void checkFileOpening(FILE *check)
{
	if (check == NULL)
	{
		printf("Error opening file\n");
		exit(ERROR_OPENING_FILE);
	}
}
/* This function checks if a file name ending with the valid ending */
void checkFileEnding(char *fileName, char *ending)
{
	if (strcmp(fileName + (strlen(fileName) - 4), ending) != 0)
	{
		printf("File name ending is not valid\n");
		exit(FILE_ENDING_ERROR);
	}
}