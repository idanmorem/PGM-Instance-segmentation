#define _CRT_SECURE_NO_WARNINGS

/***** INCLUDES *****/
#include "greyImageSegmentsByNeighbors.h"
#include "greyImage.h"
#include "general.h"

/***** DEFINES *****/
#define ALLOCATION_MEMORY_ERROR 1
/***** TYPEDEFS *****/

/******************** STATIC FUNCTION PROTOTYPES *********************/
static Segment *createTree();
static treeNode* createNodeAndInsertData(unsigned short row, unsigned short col);
static char iAndjLimitCheck(unsigned int i, unsigned int j, unsigned short rows, unsigned short cols);
static char checkIfSimilarNeighbor(greyImage *img, unsigned char toCheck, unsigned char upperLim, unsigned char lowerLim, unsigned int row, unsigned int col, unsigned int **imgCopy);
static void changeSimilarNeighborsToRealSize(unsigned int realSize, treeNode ***similar_neighbors);
static void RecBuildTree(treeNode *node, unsigned char threshold, greyImage *img, unsigned int **imgCopy, Segment *tree);
static void freeImgCopy(unsigned int ** imgCopy, unsigned short rows);

/*********************** FUNCTION IMPLEMENTATIOS ************************/

Segment* findSingleSegment(greyImage *img, imgPos kernel, unsigned char threshold)
{
	/* create a table according to the size of the image:
	 * for start the table will be filled with zeros.
	 * every time we add a pixel to the segment , the 0 in the new table will be switched to 1 */
	unsigned int **imgCopy = createImgCopy(img);

	Segment *result = createTree();  /* creates the tree */

	result->root = createNodeAndInsertData(kernel[0], kernel[1]); /* creates the first root of the tree (kernel) */

	imgCopy[kernel[0]][kernel[1]] = 1;

	RecBuildTree(result->root, threshold, img, imgCopy, result);

	freeImgCopy(imgCopy, img->rows);
	return result;
}
/* This function creates unsigned int two dimensional array according to the size of the image */
unsigned int** createImgCopy(greyImage *img)
{
	unsigned int **imgCopy, i, j;

	imgCopy = (unsigned int**)my_malloc(img->rows * sizeof(unsigned int*));

	for (i = 0; i < img->rows; i++)
		imgCopy[i] = (unsigned int*)my_malloc(img->cols * sizeof(unsigned int));

	for (i = 0; i < img->rows; i++)   /* fills the table with zeros */
		for (j = 0; j < img->cols; j++)
			imgCopy[i][j] = 0;

	return imgCopy;
}
/* This function creates a tree (segment)
 * tree size will be 1 for start (kernel) */
static Segment* createTree()
{
	Segment *newTree;
	newTree = (Segment*)my_malloc(sizeof(Segment));
	newTree->size = 1;
	newTree->root = NULL;
	return newTree;
}
/* This function creates a node for the tree */
static treeNode* createNodeAndInsertData(unsigned short row, unsigned short col)
{
	treeNode *newNode;
	newNode = (treeNode*)my_malloc(sizeof(treeNode));
	newNode->position[0] = row;
	newNode->position[1] = col;
	/* for start allocates maximum memory for similar neighbors (8): */
	newNode->similar_neighbors = (treeNode**)my_malloc(MAX_NUM_OF_NEIGHBORS * sizeof(treeNode*));

	return newNode;
}
/* This recursive function segment in the image and build according to him the tree */
static void RecBuildTree(treeNode *node, unsigned char threshold, greyImage *img, unsigned int **imgCopy, Segment *tree)
{
	unsigned int  segmentSize = 0;
	int i, j, pixelCompare = (int)img->pixels[node->position[0]][node->position[1]];
	unsigned char upperLim = (unsigned char)(pixelCompare + threshold), lowerLim = (unsigned char)(pixelCompare - threshold);
	treeNode *newNode;

	if ((pixelCompare + threshold) > 255)    /* in case curr (kernel + threshold) is above 255 */
		upperLim = 255;

	if ((pixelCompare - threshold) < 0)  /* in case curr (kernel + threshold) is under 0 */
		lowerLim = 0;

	for (i = (node->position[0] - 1); i < (node->position[0] + 2); i++)
		for (j = (node->position[1] - 1); j < (node->position[1] + 2); j++)
			if (iAndjLimitCheck(i, j, img->rows, img->cols))
				if (checkIfSimilarNeighbor(img, img->pixels[i][j], upperLim, lowerLim, i, j, imgCopy))
				{
					tree->size++;
					imgCopy[i][j] = 1;
					newNode = createNodeAndInsertData(i, j);
					node->similar_neighbors[segmentSize] = newNode;
					segmentSize++;
				}

	changeSimilarNeighborsToRealSize(segmentSize, &node->similar_neighbors);

	for (i = 0; i < segmentSize; i++)    /* the recursive calls */
		RecBuildTree(node->similar_neighbors[i], threshold, img, imgCopy, tree);
}
/* This function change similar neighbors to the real size and checks for memory allocation errors */
static void changeSimilarNeighborsToRealSize(unsigned int realSize, treeNode ***similar_neighbors)
{
	*similar_neighbors = realloc(*similar_neighbors, (realSize + 1) * sizeof(treeNode *));
	checkReallocAllocation((*similar_neighbors));
	(*similar_neighbors)[realSize] = NULL;
}
/* This function checks if one neighbor is a similar neighbor */
static char checkIfSimilarNeighbor(greyImage *img, unsigned char toCheck, unsigned char upperLim, unsigned char lowerLim, unsigned int row, unsigned int col, unsigned int **imgCopy)
{
	if ((toCheck >= lowerLim) && (toCheck <= upperLim) && (!imgCopy[row][col]))
		return TRUE;

	return FALSE;
}
/* This function checks if the neighbor to be check is in the range of the image */
static char iAndjLimitCheck(unsigned int i, unsigned int j, unsigned short rows, unsigned short cols)
{
	if ((i < 0 || i >= rows) || (j < 0 || j >= cols))
		return FALSE;

	return TRUE;
}
/* This function free the memory of the unsigned int table of 1 and 0 */
static void freeImgCopy(unsigned int ** imgCopy, unsigned short rows)
{
	int i;

	for (i = 0; i < rows; i++)
		free(imgCopy[i]);

	free(imgCopy);
}
