#define _CRT_SECURE_NO_WARNINGS

/***** INCLUDES *****/
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "PGM.h"
#include "general.h"

/***** DEFINES *****/
#define ERROR_OPENING_FILE 1
#define FILE_INTRO_ERROR 1
#define NOT_VALID_PIXEL 1
#define MAX_LINE_LENGTH  100 /* TODO change this */

/**************************** STATIC FUNCTION PROTOTYPES *****************************/
static void checkPGMFileIntro(FILE *fname, int *cols, int *rows, int *maxGrey);
static unsigned char **createPixelsArr(int rows, int cols);
static void readImageFromFile(FILE *fPtr, greyImage *img, int greyMax);
static void skipComments(FILE *PGMFilePtr);
static void exitAndPrintIntroError(void);
static char checkIfPixelValid(int pixel, int greyMax);
static void readRowsColsAndZFromBinaryFile(unsigned short *rows, unsigned short *cols, unsigned char *Z, FILE *binaryFile);
static void writeIntroToPGM(unsigned short rows, unsigned short cols, FILE *PGMFilePtr);
static void closeTwoFiles(FILE *file1, FILE *file2);
static void openBinaryFileAndPGMFile(char  *compressed_file_name, char *pgm_file_name, FILE **binaryFilePtr, FILE **PGMFilePtr);
static void resetMaskAndCounterAndReadAnotherByte(unsigned char *mask, unsigned char *oneByteFromBinaryFile, unsigned int *counter, FILE *binaryFilePtr);

/**************************** FUNCTION IMPLEMENTATIOS *****************************/

void convertCompressedToPGM(char *compressed_file_name, char *pgm_file_name)
{
	FILE *binaryFilePtr, *PGMFilePtr;
	unsigned short rows, cols; // 1000 0000
	unsigned char Z, onebyteFromBinaryFile, mask = 128, currPixel;  /* TODO change 128 */
	unsigned int i, j, k, powerOfZ, counter = 0;

	openBinaryFileAndPGMFile(compressed_file_name, pgm_file_name, &binaryFilePtr, &PGMFilePtr); /* open the files */
	readRowsColsAndZFromBinaryFile(&rows, &cols, &Z, binaryFilePtr);    /* read rows cols and Z from the binary file */
	writeIntroToPGM(rows, cols, PGMFilePtr);    /* write in PGM file "P2" and rows and cols and 255 */

	powerOfZ = (unsigned int)log2((double)Z); /* to know how much bits for every pixel */
	fread(&onebyteFromBinaryFile, sizeof(unsigned char), 1, binaryFilePtr); /* reads one byte from the binery file */
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			currPixel = 0;
			for (k = 0; k < powerOfZ; k++)
			{
				if (counter == 8)
					resetMaskAndCounterAndReadAnotherByte(&mask, &onebyteFromBinaryFile, &counter, binaryFilePtr);

				currPixel = (currPixel << 1);

				if ((mask & onebyteFromBinaryFile) != 0)
					currPixel++;

				mask = (mask >> 1);
				counter++;
			}
			fprintf(PGMFilePtr, "%3d ", (int)round(((double)(currPixel) / (Z - 1)) * 255));
		}
		fputc('\n', PGMFilePtr);    /* puts '\n' in PGM file every end of line */
	}
	closeTwoFiles(binaryFilePtr, PGMFilePtr);   /* close the files */
}
/* This function reset mask to 128 and counter to 0 and read from the binary file another byte */
static void resetMaskAndCounterAndReadAnotherByte(unsigned char *mask, unsigned char *oneByteFromBinaryFile, unsigned int *counter, FILE *binaryFilePtr)
{
	*mask = 128;
	fread(oneByteFromBinaryFile, sizeof(unsigned char), 1, binaryFilePtr); /* reads another byte from the binery file */
	*counter = 0;
}
/* This function opens two files (binary and PGM) and check for opening errors */
static void openBinaryFileAndPGMFile(char  *compressed_file_name, char *pgm_file_name, FILE **binaryFilePtr, FILE **PGMFilePtr)
{
	*binaryFilePtr = fopen(compressed_file_name, "rb");
	*PGMFilePtr = fopen(pgm_file_name, "w");

	checkFileOpening(*binaryFilePtr);    /* check for file opening errors */
	checkFileOpening(*PGMFilePtr);
}
/* This function close two files */
static void closeTwoFiles(FILE *file1, FILE *file2)
{
	fclose(file1);
	fclose(file2);
}
/* This function writes to the PGM file the intro */
static void writeIntroToPGM(unsigned short rows, unsigned short cols, FILE *PGMFilePtr)
{
	fprintf(PGMFilePtr, "P2\n");
	fprintf(PGMFilePtr, "%d %d\n", cols, rows);
	fprintf(PGMFilePtr, "%d\n", 255);
}
/* This function read rows and cols from the binary file and the reduced grey level */
static void readRowsColsAndZFromBinaryFile(unsigned short *rows, unsigned short *cols, unsigned char *Z, FILE *binaryFile)
{
	fread(rows, sizeof(unsigned short), 1, binaryFile);
	fread(cols, sizeof(unsigned short), 1, binaryFile);
	fread(Z, sizeof(unsigned char), 1, binaryFile);
}
/* This function convert a PGM file to greyImage according to the data inside */
greyImage *readPGM(char *fname)
{
	FILE *PGMFilePtr;
	greyImage *img;
	int rows, cols, maxGrey;

	PGMFilePtr = fopen(fname, "r"); /* open the PGM file */
	if (PGMFilePtr == NULL) /* checks for open errors */
	{
		fprintf(stderr, "Error opening %s\n", fname);
		exit(ERROR_OPENING_FILE);
	}

	checkPGMFileIntro(PGMFilePtr, &cols, &rows, &maxGrey);    /* first check if the intro of the file is valid and read cols and rows from it */
	/* if the program is still runing it means the intro is ok and rows and cols have values */

	img = createImage(rows, cols);

	readImageFromFile(PGMFilePtr, img, maxGrey);

	fclose(PGMFilePtr); /* close the file */

	return img;
}
/* This function read from the image all the pixels to pixelsArr in the image */
static void readImageFromFile(FILE *fPtr, greyImage *img, int greyMax)
{
	unsigned int i, j;
	int currPixel;

	for (i = 0; i < img->rows; i++)
		for (j = 0; j < img->cols; j++)
		{
			fscanf(fPtr, "%d", &currPixel);
			if (!checkIfPixelValid(currPixel, greyMax))
			{
				printf("Pixel in the position (%d, %d) is not valid\n", i, j);
				exit(NOT_VALID_PIXEL);
			}
			currPixel = round((((float)currPixel) / greyMax) * 255);

			img->pixels[i][j] = (unsigned char)currPixel;
		}

}
/* This function checks if a pixel is between 0 to max grey value */
static char checkIfPixelValid(int pixel, int greyMax)
{
	if (pixel > greyMax || pixel < 0)
		return FALSE;

	return TRUE;
}
/* This function create unsigned char array according to rows and cols to represent the pixels array for the image */
static unsigned char **createPixelsArr(int rows, int cols)
{
	unsigned int i;
	unsigned char **pixelsArr;
	pixelsArr = (unsigned char **)my_malloc(rows * sizeof(unsigned char *));

	for (i = 0; i < rows; i++)
		pixelsArr[i] = (unsigned char *)my_malloc(cols * sizeof(unsigned char));

	return pixelsArr;
}
/* This function check if the intro of the PGM file is valid
 * if the intro is valid - enter cols and rows to variables (by reference) */
static void checkPGMFileIntro(FILE *fname, int *cols, int *rows, int *maxGrey)
{
	char version[3];

	skipComments(fname);  /* checks if the line is comments */
	fgets(version, sizeof(version), fname); /* read the first line (the version) */

	if (strcmp(version, "P2"))    /* check if the first line is "P2" */
	{
		fprintf(stderr, "Wrong file type\n");
		exit(FILE_INTRO_ERROR);
	}
	skipComments(fname);   /* checks if the line is comment */

	fscanf(fname, "%d %d", cols, rows);  /* read the second line (cols and rows) */
	getc(fname);    /* jump over '\n' */

	skipComments(fname);   /* checks if the line is comment */
	fscanf(fname, "%d", maxGrey);  /* read the last line */
	getc(fname);    /* jump over '\n' */

	skipComments(fname);   /* checks if the line is comment */
}
/* This function exit to program in case the file intro is not valid */
static void exitAndPrintIntroError(void)
{
	printf("File intro is not valid\n");
	exit(FILE_INTRO_ERROR);
}
/* This function checks if the current line is comment
 * if the line is comment the function jump to the next line and check again */
static void skipComments(FILE *PGMFilePtr) {
	int ch;
	char line[100];

	while ((ch = fgetc(PGMFilePtr)) != EOF && isspace(ch));

	if (ch == '#')
	{
		fgets(line, sizeof(line), PGMFilePtr);
		skipComments(PGMFilePtr);
	}
	else
		fseek(PGMFilePtr, -1, SEEK_CUR);
}
