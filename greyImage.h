#define _CRT_SECURE_NO_WARNINGS
#ifndef GREYIMAGEPROJECT_GREYIMAGE_H
#define GREYIMAGEPROJECT_GREYIMAGE_H

/***** INCLUDES *****/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/****** TYPEDEFS ******/
typedef struct _greyImage
{
	unsigned short      rows, cols;
	unsigned char       **pixels;
} greyImage;

typedef unsigned short imgPos[2];
/***************** FUNCTION PROTOTYPES *******************/
greyImage *createImage(unsigned int rows, unsigned int cols);
void putRandomPixelsInImage(greyImage *img);
void printImage(greyImage *img);
void freeImage(greyImage *img);

#endif //GREYIMAGEPROJECT_GREYIMAGE_H


