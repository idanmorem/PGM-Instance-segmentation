#define _CRT_SECURE_NO_WARNINGS

#ifndef GREYIMAGEPROJECT_SAVECOMPRESSED_H
#define GREYIMAGEPROJECT_SAVECOMPRESSED_H


/************** INCLUDES **************/
#include <math.h>
#include "greyImage.h"


/************** DEFINES **************/
#define ERROR_OPENING_OUTPUT_BINARY_FILE 1

/********************** FUNCTION PROTOTYPES **********************/
void saveCompressed(char *file_name, greyImage *image, unsigned char reduced_grey_levels);



#endif //GREYIMAGEPROJECT_SAVECOMPRESSED_H
