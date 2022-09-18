#define _CRT_SECURE_NO_WARNINGS

/***** INCLUDES *****/
#include "greyImage.h"
#include "general.h"

/******************** STATIC FUNCTION PROTOTYPES *********************/
static unsigned char **createPixelsArr(unsigned int rows, unsigned int cols);

/******************** FUNCTION IMPLEMENTAION *********************/
/* This function will allocate memory for a new image (by ref), and insert the values of rows and cols */
greyImage *createImage(unsigned int rows, unsigned int cols)
{
	greyImage *img = (greyImage *)my_malloc(sizeof(greyImage));

	img->cols = cols;
	img->rows = rows;

	img->pixels = createPixelsArr(rows, cols);

	return img;
}
/* This function create pixels table for the image */
static unsigned char **createPixelsArr(unsigned int rows, unsigned int cols)
{
	unsigned char **pixels = (unsigned char **)my_malloc(rows * sizeof(unsigned char *));
	unsigned int i;

	for (i = 0; i < rows; i++)
		pixels[i] = (unsigned char *)my_malloc(cols * sizeof(unsigned char));

	return pixels;
}
/* This function prints the pixels table of an image */
void printImage(greyImage *img)
{
	unsigned int i, j;

	for (i = 0; i < img->rows; i++)
	{
		for (j = 0; j < img->cols; j++)
			printf("%3d ", img->pixels[i][j]);

		printf("\n");
	}
}
void putRandomPixelsInImage(greyImage *img)
{
	srand(time(0));

	unsigned int i, j;
	for (i = 0; i < img->rows; i++)
		for (j = 0; j < img->cols; j++)
			img->pixels[i][j] = (rand() % 256);
}
/* This function free all the memory of an image */
void freeImage(greyImage *img)
{
	unsigned int i, j;

	for (i = 0; i < img->rows; i++)
		free(img->pixels[i]);

	free(img->pixels);
	free(img);
}
