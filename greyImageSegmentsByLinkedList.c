#define _CRT_SECURE_NO_WARNINGS

/****** INCLUDES *****/
#include <stdlib.h>
#include "greyImageSegmentsByLinkedList.h"
#include "general.h"
#include "greyImageSegmentsByNeighbors.h"

/*********************** STATIC FUNCTION PROTOTYPES ************************/
static void swapForBubbleSortSegmentP(imgPosCell **cell1, imgPosCell **cell2);
static void swapTwoCellsData(imgPosCell *cell1, imgPosCell *cell2);
static imgPosCell *createNewCell(void);
static imgPosCell* findMin(greyImage *img, unsigned int **imgCopy);
static void bubbleSortForSegmentsArr(unsigned int *arrForEveryListLength, imgPosCell **segments, unsigned int size);
static void swapForIntArrForLengthsP(unsigned int *one, unsigned int *two);
static void increaseSizeOfArrs(unsigned int *segmentsArrSize, imgPosCell ***segments, unsigned int **arrForEveryListLength);
static void freeList(imgPosCell *cell);
static void convertTreeToList(treeNode *root, imgPosCell **tailKernel, unsigned int **imgCopy);
static void changeSegmentsArrAndArrForEveryListLengthToRealSize(unsigned int **arrForEveryListLength, imgPosCell ***segments, unsigned int size);
static void printList(imgPosCell *currCell, greyImage *img);

/*********************** FUNCTION IMPLEMENTATIONS ************************/
unsigned int findAllSegments(greyImage *img, unsigned int treshHold, imgPosCell ***segments)
{
	/* arrForEveryListLength is an array used to save every size of linked list ( size of one segment ): */
	*segments = (imgPosCell **)my_malloc(BASE_SIZE * sizeof(imgPosCell *));    /* allocate memory for segments array */
	unsigned int **imgCopy = createImgCopy(img), *arrForEveryListLength = (unsigned int *)my_malloc(BASE_SIZE * sizeof(unsigned int));
	unsigned int i = 0, segmentsArrSize = BASE_SIZE;
	Segment *tree;
	imgPosCell *currKernel = findMin(img, imgCopy), *tailKernel = NULL;    /* finds the first kernel */
	for (i = 0; currKernel != NULL; i++)
	{
		if (i == segmentsArrSize)   /* if we need to increase segments arr size and arrForEveryListLength */
			increaseSizeOfArrs(&segmentsArrSize, segments, &arrForEveryListLength);

		(*segments)[i] = currKernel;
		tree = findSingleSegment(img, currKernel->position, treshHold);
		arrForEveryListLength[i] = tree->size;  /* save the size of the current linked list (to be) */
		tailKernel = currKernel;
		imgCopy[currKernel->position[0]][currKernel->position[1]] = 1;
		convertTreeToList(tree->root, &tailKernel, imgCopy);    /* convert the segment tree to a linked list */
		bubbleSortForList(currKernel);  /* sort the current segment linked list */
		currKernel = findMin(img, imgCopy); /* finds the next kernel */
	}
	changeSegmentsArrAndArrForEveryListLengthToRealSize(&arrForEveryListLength, segments, i);   /* change to the real size */
	bubbleSortForSegmentsArr(arrForEveryListLength, *segments, i);  /* sorts segments arr */
	free(arrForEveryListLength);
	return i;
}
/* This function allocate memory for segments arr and the arr of lists length to the real size */
static void changeSegmentsArrAndArrForEveryListLengthToRealSize(unsigned int **arrForEveryListLength, imgPosCell ***segments, unsigned int size)
{
	*segments = realloc(*segments, size * sizeof(imgPosCell *));
	*arrForEveryListLength = realloc(*arrForEveryListLength, size * sizeof(unsigned int));
	checkReallocAllocation(*segments);  /* check for realloc allocation errors */
	checkReallocAllocation(arrForEveryListLength);
}
/* This function converts a tree to a linked list */
static void convertTreeToList(treeNode *root, imgPosCell **tailKernel, unsigned int **imgCopy)
{
	unsigned int i;
	imgPosCell *newCell = NULL, *prev = *tailKernel;

	for (i = 0; root->similar_neighbors[i] != NULL; i++)
	{
		imgCopy[root->similar_neighbors[i]->position[0]][root->similar_neighbors[i]->position[1]] = 1;
		newCell = createNewCell();
		newCell->position[0] = root->similar_neighbors[i]->position[0];
		newCell->position[1] = root->similar_neighbors[i]->position[1];
		prev->next = newCell;
		newCell->prev = prev;
		prev = newCell;
	}
	if (newCell != NULL)
		*tailKernel = newCell;

	for (i = 0; root->similar_neighbors[i] != NULL; i++)
		convertTreeToList(root->similar_neighbors[i], tailKernel, imgCopy);
}
/* This function doubles the size of segments array
 * in addition also doubles the array used to save the length of every list in segments */
static void increaseSizeOfArrs(unsigned int *segmentsArrSize, imgPosCell ***segments, unsigned int **arrForEveryListLength)
{
	(*segmentsArrSize) *= 2;   /* doubles the size */
	*segments = realloc(*segments, (*segmentsArrSize) * sizeof(imgPosCell *));   /* allocate memory according to the new size */
	*arrForEveryListLength = realloc(*arrForEveryListLength, (*segmentsArrSize) * sizeof(unsigned int));
	if (!(*segments) || !(*arrForEveryListLength))
	{
		printf("Allocation memory error\n");
		exit(REALLOC_ERROR);
	}
}
/* This function sorts the arr of segements according to the size of every element (list nodes) with bubble sort method */
static void bubbleSortForSegmentsArr(unsigned int *arrForEveryListLength, imgPosCell **segments, unsigned int size)
{
	unsigned int i, j;

	for (i = size; i > 0; i--)
		for (j = 0; j < (i - 1); j++)
			if (arrForEveryListLength[j] < arrForEveryListLength[j + 1])
			{
				swapForBubbleSortSegmentP(&segments[j], &segments[j + 1]);
				swapForIntArrForLengthsP(&(arrForEveryListLength[j]), &(arrForEveryListLength[j + 1]));
			}
}
/* This function sorts a linked list with the bubble sort method */
void bubbleSortForList(imgPosCell *currCell)
{
	imgPosCell *start = currCell;

	if (currCell == NULL)   /* check if the list is empty */
		return;

	while (start->next != NULL)
	{
		currCell = start->next;

		while (currCell != NULL)
		{
			if (currCell->position[1] < start->position[1])
				swapTwoCellsData(currCell, start);

			else if (currCell->position[1] == start->position[1])  /* if the columns are equal */
				if (currCell->position[0] < start->position[0]) /* check the rows */
					swapTwoCellsData(currCell, start);

			currCell = currCell->next;  /* jump to the next cell */
		}
		start = start->next;    /* jump the start to the next cell */
	}
}
static void swapTwoCellsData(imgPosCell *cell1, imgPosCell *cell2)
{
	imgPos tmpPos;
	tmpPos[0] = cell1->position[0];
	tmpPos[1] = cell1->position[1];

	cell1->position[0] = cell2->position[0];
	cell1->position[1] = cell2->position[1];

	cell2->position[0] = tmpPos[0];
	cell2->position[1] = tmpPos[1];
}
/* This function is a basic swap function for unsigned interger pointer */
static void swapForIntArrForLengthsP(unsigned int *one, unsigned int *two)
{
	unsigned int tmp = *one;
	*one = *two;
	*two = tmp;
}
/* Function to swap position data for the bubble sort function */
static void swapForBubbleSortSegmentP(imgPosCell **cell1, imgPosCell **cell2)
{
	imgPosCell *tmp = *cell1;
	*cell1 = *cell2;
	*cell2 = tmp;
}
/* This function finds the new kernal of the image depend on check (check = (last kernal + treshHold).
 * the function returns NULL if there is not unused pixels in the image. */
static imgPosCell* findMin(greyImage *img, unsigned int **imgCopy)
{
	imgPosCell *minCell = createNewCell();
	unsigned char data;
	unsigned int i, j;
	char flag = FALSE;

	for (i = 0; i < img->rows; i++)
	{
		for (j = 0; j < img->cols; j++)
			if (!imgCopy[i][j])
			{
				data = img->pixels[i][j];
				flag = TRUE;
				break;
			}
		if (flag)
			break;
	}

	if (!flag)
	{
		free(minCell);
		return NULL;
	}

	minCell->position[0] = i;
	minCell->position[1] = j;

	for (i = minCell->position[0]; i < img->rows; i++)
		for (j = 0; j < img->cols; j++)
			if (!imgCopy[i][j] && img->pixels[i][j] < data)
			{
				data = img->pixels[i][j];
				minCell->position[0] = i;
				minCell->position[1] = j;
			}

	return minCell;
}
/* This function create a new cell for the the linked list of pixels */
static imgPosCell *createNewCell(void)
{
	imgPosCell *new_cell = (imgPosCell *)my_malloc(sizeof(imgPosCell));
	new_cell->prev = new_cell->next = NULL;

	return new_cell;
}
/* This function creates a new image
 * the new image will have the same grey shape for every segment */
greyImage *colorSegments(greyImage *img, imgPosCell **segments, unsigned int size)
{
	unsigned int i;
	unsigned char currGreyShape;
	greyImage *newImage = createImage(img->rows, img->cols);    /* create the new image */
	imgPosCell *currCell;

	for (i = 0; i < size; i++)  /* loop over every list in segments */
	{
		currGreyShape = (unsigned char)floor(i * (255.0 / (size - 1))); /* changes the shape depend on i */
		currCell = segments[i]; /* set the first cell in the current linked list */
		while (currCell) /* loop over the current list cells */
		{
			newImage->pixels[currCell->position[0]][currCell->position[1]] = currGreyShape;
			currCell = currCell->next;
		}
	}

	return newImage;
}
/* This function free segments array ( the array of the linked list that represents segments in the image ) */
void freeSegmentsLinkedListArr(imgPosCell **segmentsArr, unsigned int size)
{
	unsigned int i;

	for (i = 0; i < size; i++)
		freeList(segmentsArr[i]);

	free(segmentsArr);
}
/* This function is a recursive function to free the linked list ( one segment ) */
static void freeList(imgPosCell *cell)
{
	if (cell == NULL)
		return;

	freeList(cell->next);
	free(cell);
}
/* This function prints the array of the linked lists (the segments) */
void printSegmentArr(imgPosCell **segmentsArr, unsigned int size, greyImage *img)
{
	unsigned int i;
	imgPosCell *currCell;

	for (i = 0; i < size; i++)
	{
		printf("segment %d: ", i + 1);
		currCell = segmentsArr[i];
		while (currCell)
		{
			printf("%d->", img->pixels[currCell->position[0]][currCell->position[1]]);
			currCell = currCell->next;
		}
		printf("NULL\n");
	}

}
static void printList(imgPosCell *currCell, greyImage *img)
{
	if (currCell == NULL)
		return;

	printf("%d ", img->pixels[currCell->position[0]][currCell->position[1]]);
	printList(currCell->next, img);
}



