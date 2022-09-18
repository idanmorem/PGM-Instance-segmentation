#define _CRT_SECURE_NO_WARNINGS

#ifndef GREYIMAGEPROJECT_GREYIMAGESEGMENTSBYLINKEDLIST_H
#define GREYIMAGEPROJECT_GREYIMAGESEGMENTSBYLINKEDLIST_H

/***** INCLUDES *****/
#include <math.h>
#include "greyImage.h"

/***** DEFINES *****/
#define TRUE 1
#define FALSE 0
#define BASE_SIZE 2
#define REALLOC_ERROR 1

/***** TYPEDEFS *****/
typedef struct _imgPosCell
{
	imgPos              position;
	struct _imgPosCell  *next, *prev;
} imgPosCell;

/******************** FUNCTION PROTOTYPES *******************/
greyImage *colorSegments(greyImage *img, imgPosCell **segments, unsigned int size);
void bubbleSortForList(imgPosCell *currCell);
unsigned int findAllSegments(greyImage *img, unsigned int treshHold, imgPosCell ***segments);
void freeSegmentsLinkedListArr(imgPosCell **segmentsArr, unsigned int size);
void printSegmentArr(imgPosCell **segmentsArr, unsigned int size, greyImage *img);

#endif //GREYIMAGEPROJECT_GREYIMAGESEGMENTSBYLINKEDLIST_H


