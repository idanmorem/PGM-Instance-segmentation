#define _CRT_SECURE_NO_WARNINGS

/************** INCLUDES **************/
#include "saveCompressed.h"
#include "general.h"

/********************** STATIC FUNCTION PROTOTYPES **********************/
static void writeIntroInBinaryFile(unsigned short rows, unsigned short cols, unsigned char reduced_grey_levels, FILE *binaryFilePtr);

/********************** FUNCTION IMPLEMENTATIONS **********************/
/* This function will compress an image according to reduced_grey_levels and insert the compressed image to binary file */
void saveCompressed(char *file_name, greyImage *image, unsigned char reduced_grey_levels)
{
	FILE *out_fPtr;
	unsigned short rows = (unsigned short)image->rows, cols = (unsigned short)image->cols;  /* save rows and cols in unsigned short */
	unsigned char char1 = 0, mask, currPixel;
	unsigned int counter = 0, Z = (unsigned int)reduced_grey_levels, i, j, k;

	out_fPtr = fopen(file_name, "wb");  /* open/create the file and check for errors */
	checkFileOpening(out_fPtr);

	writeIntroInBinaryFile(rows, cols, reduced_grey_levels, out_fPtr);

	for (i = 0; i < rows; i++)
		for (j = 0; j < cols; j++)
		{
			mask = (reduced_grey_levels / 2);
			currPixel = (unsigned char)(round(((double)(image->pixels[i][j]) / 255) * (reduced_grey_levels - 1)));
			for (k = 0; k < round(log2(Z)); k++)
			{
				if (counter == 8)
				{
					fwrite(&char1, sizeof(unsigned char), 1, out_fPtr);
					counter = char1 = 0;
				}
				char1 = (char1 << 1);
				if ((currPixel & mask) != 0)
					char1++;

				counter++;
				mask = (mask >> 1);
			}
		}

	if (counter != 0)
	{
		char1 = (char1 << (8 - counter));
		fwrite(&char1, sizeof(unsigned char), 1, out_fPtr);
	}

	fclose(out_fPtr);
}
/* This function will write in the binary files the rows, cols and reduced grey levels */
static void writeIntroInBinaryFile(unsigned short rows, unsigned short cols, unsigned char reduced_grey_levels, FILE *binaryFilePtr)
{
	fwrite(&rows, sizeof(rows), 1, binaryFilePtr); /* write rows in the binary file */
	fwrite(&cols, sizeof(cols), 1, binaryFilePtr); /* write cols in the binary file */
	fwrite(&reduced_grey_levels, sizeof(reduced_grey_levels), 1, binaryFilePtr);   /* write Z in the binary file */
}