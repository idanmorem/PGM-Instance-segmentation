#define _CRT_SECURE_NO_WARNINGS
#ifndef GREYIMAGEPROJECT_GREYIMAGESEGMENTSBYNEIGHBORS_H
#define GREYIMAGEPROJECT_GREYIMAGESEGMENTSBYNEIGHBORS_H

/***** INCLUDES *****/
#include "greyImage.h"

/***** DEFINES *****/
#define MAX_NUM_OF_NEIGHBORS 8
#define TRUE 1
#define FALSE 0

/***** TYPEDEFS *****/
typedef struct _treeNode
{
	imgPos           position;
	struct _treeNode **similar_neighbors;
} treeNode;

typedef struct _segment
{
	treeNode        *root;
	unsigned int    size;
} Segment;
/******************** FUNCTION PROTOTYPES *********************/
Segment *findSingleSegment(greyImage *img, imgPos kernel, unsigned char threshold);
unsigned int **createImgCopy(greyImage *img);

#endif //GREYIMAGEPROJECT_GREYIMAGESEGMENTSBYNEIGHBORS_H
