#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "greyImage.h"
#include "greyImageSegmentsByLinkedList.h"
#include "greyImageSegmentsByNeighbors.h"
#include "PGM.h"
#include "general.h"
#include "saveCompressed.h"

#define MAX_FILE_NAME_LENGTH 100

int main() {

	unsigned int rows, cols, reducedGreyLevels, size, threshold;
	greyImage *img, *newImg;
	imgPosCell **segmentsArr;
	char fileName1[MAX_FILE_NAME_LENGTH], fileName2[MAX_FILE_NAME_LENGTH];

	/************************ check question 1+2+3 ***************************/
	printf("/******************* Check for question 1+2 *******************/\n");
	printf("Pls enter the amount of rows and cols you want:\n");
	scanf("%d %d", &rows, &cols);
	printf("Pls enter your threshold:\n");
	scanf("%d", &threshold);

	img = createImage(rows, cols);    /* creates the image */
	putRandomPixelsInImage(img);  /* puts random values (pixels) in the image */

	printf("This is your image:\n");
	printImage(img);
	printf("\n");

	size = findAllSegments(img, (unsigned char)threshold, &segmentsArr);

	printSegmentArr(segmentsArr, size, img);
	printf("\n");

	newImg = colorSegments(img, segmentsArr, size);

	printf("Your image after question 3: \n");
	printImage(newImg);
	printf("\n");

	freeImage(img);
	freeImage(newImg);
	freeSegmentsLinkedListArr(segmentsArr, size);

	/************************ check question 4 ***************************/
	printf("/******************* Check for question 4: *******************/\n");
	printf("Pls enter the name of the PGM file:\n");
	scanf("%s", fileName1);

	img = readPGM(fileName1);

	printf("This is the image from file %s:\n", fileName1);
	printImage(img);

	/************************ check question 5 + 6 ***************************/
	printf("/******************* Check for question 5+6: *******************/\n");
	printf("Pls enter your reduced grey levels:\n");
	scanf("%d", &reducedGreyLevels);
	printf("Pls enter your name to the binary file:\n");
	scanf("%s", fileName1);

	checkFileEnding(fileName1, ".bin");

	saveCompressed(fileName1, img, reducedGreyLevels);

	printf("The binary file has been created\n");
	printf("Pls enter the name of the new PGM file to be created:\n");
	scanf("%s", fileName2);

	checkFileEnding(fileName2, ".pgm");

	convertCompressedToPGM(fileName1, fileName2);

	printf("The new PGM file has been created\n");

	freeImage(img);
	return 0;
}