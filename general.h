#define _CRT_SECURE_NO_WARNINGS

#ifndef GREYIMAGEPROJECT_GENERAL_H
#define GREYIMAGEPROJECT_GENERAL_H

/***** INCLUDES *****/
#include <stdlib.h>
#include <stdio.h>

/****** DEFINES ******/
#define ERROR_OPENING_FILE 1

/********************** FUNCTION PROTOTYPES ************************/
void *my_malloc(int amount_of_bytes);
void checkFileOpening(FILE *check);
void checkFileEnding(char *fileName, char *ending);
void checkReallocAllocation(void *p);

#endif //GREYIMAGEPROJECT_GENERAL_H
