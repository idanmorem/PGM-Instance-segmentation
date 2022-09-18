#define _CRT_SECURE_NO_WARNINGS

#ifndef GREYIMAGEPROJECT_PGM_H
#define GREYIMAGEPROJECT_PGM_H

/***** INCLUDES *****/
#include "greyImage.h"

/****** DEFINES ******/
#define TRUE 1
#define FALSE 0

/****** TYPEDEFS ******/

/********************** FUNCTION PROTOTYPES ************************/
greyImage *readPGM(char *fname);
void convertCompressedToPGM(char *compressed_file_name, char *pgm_file_name);

#endif //GREYIMAGEPROJECT_PGM_H
