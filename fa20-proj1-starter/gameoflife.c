/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

int direction[8][2] = {{-1, -1}, {-1, 0}, {0, -1}, {1, -1}, {-1, 1}, {1, 0}, {0, 1}, {1, 1}};

int check_alive(Image *image, int row, int col)
{
	if (image->image[row][col].R | image->image[row][col].G | image->image[row][col].B)
	{
		return 1;
	}
	return 0;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	Color *clr = (Color *)malloc(sizeof(Color));
	uint32_t rows = image->rows;
	uint32_t cols = image->cols;
	int max_RGB = 0;
	int live = check_alive(image, row, col);
	uint8_t RGB[3] = {0, 0, 0};
	int numbers_of_neighbers = 0;
	for(int i = 0; i < 8; i++)
	{
		int x = (row + direction[i][0] + rows) % rows;	//最左与最右，最上和最下都可以算邻居
		int y = (col + direction[i][1] + cols) % cols;
		if(check_alive(image, x, y))
			numbers_of_neighbers++;
		uint8_t R = image->image[x][y].R;
		uint8_t G = image->image[x][y].G;
		uint8_t B = image->image[x][y].B;
		int sum = R + G + B;
		if(sum > max_RGB)
		{
			RGB[0] = R;
			RGB[1] = G;
			RGB[2] = B;
			// max_RGB = sum;
		}
	}

	int _rule = rule;
	if(live)
		_rule >>= 9;
	_rule >>= numbers_of_neighbers;
	if(_rule & 1)
	{
		clr->R = RGB[0];
		clr->G = RGB[1];
		clr->B = RGB[2];
	}
	else 
	{
		clr->R = 0;
		clr->G = 0;
		clr->B = 0;
	}

	return clr;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *new_Img = (Image *)malloc(sizeof(Image));
	uint32_t r = image->rows;
	uint32_t c = image->cols;
	new_Img->rows = r;
	new_Img->cols = c;

	Color **clr = (Color **)malloc(r * sizeof(Color *));
	new_Img->image = clr;
	for (uint32_t i = 0; i < r; i++)
	{
		clr[i] = (Color *)malloc(c * sizeof(Color));
		for (uint32_t j = 0; j < c; j++)
		{
			Color *c = evaluateOneCell(image, i, j, rule);
			clr[i][j] = *c;
			free(c);
		}
	}

	return new_Img;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if(argc != 3) return -1;
	Image *image = readData(argv[1]);
	uint32_t rule = strtol(argv[2], NULL, 16);
	Image *next_Image = life(image, rule);
	writeData(next_Image);
	freeImage(image);
	freeImage(next_Image);
	return 0;
}
