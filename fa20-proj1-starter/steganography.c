/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	Color **img = image->image;
	Color clr = img[row][col];
	Color *tmp = (Color *)malloc(sizeof(Color));

	uint8_t Blue = clr.B;
	bool blackOrWhite = Blue & 1;
	if(blackOrWhite)	//白
	{
		tmp->R = 255;
		tmp->G = 255;
		tmp->B = 255;
	} 
	else 
	{
		tmp->R = 0;
		tmp->G = 0;
		tmp->B = 0;
	}

	return tmp;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	Image *new_Img = (Image *)malloc(sizeof(Image));
	uint32_t r = image->rows;
	uint32_t c = image->cols;
	new_Img->rows = r;
	new_Img->cols = c;

	Color **clr = (Color **)malloc(r * sizeof(Color *));
	new_Img->image = clr;
	for(uint32_t i = 0; i < r; i++)
	{
		clr[i] = (Color *)malloc(c * sizeof(Color));
		for(uint32_t j = 0; j < c; j++)
		{
			Color *c = evaluateOnePixel(image, i, j);
			clr[i][j] = *c;
			free(c);
			//clr[i][j] = *evaluateOnePixel(image, i, j);则会有堆上内存未释放
		}
	}

	return new_Img;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	Image *img = readData(argv[1]);
	Image *new_Img = steganography(img);

	writeData(new_Img);

	freeImage(new_Img);
	freeImage(img);

	return 0;
}
