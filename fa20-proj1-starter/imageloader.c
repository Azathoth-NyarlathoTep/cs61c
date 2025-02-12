/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	FILE *fp = fopen(filename, "r");
	if (!fp)
	{
		exit(-1);
	}
	char buff[20];	//for P3
	uint32_t r, c;
	int maxn = 0;
	fscanf(fp, "%s %u %u", buff, &c, &r);
	Image *img = (Image *)malloc(sizeof(Image));
	img->rows = r;
	img->cols = c;
	img->image = (Color **)malloc(img->rows * sizeof(Color *));
	for(uint32_t i = 0; i < img->rows; i++) img->image[i] = (Color *)malloc(img->cols * sizeof(Color));
	fscanf(fp, "%d", &maxn);
	for(uint32_t i = 0; i < img->rows; i++)
	{
		for(uint32_t j = 0; j < img->cols; j++)
		{
			uint8_t r, g, b;
			fscanf(fp, "%hhu %hhu %hhu", &r, &g, &b);
			img->image[i][j].R = r;
			img->image[i][j].G = g;
			img->image[i][j].B = b;
		}
	}
	fclose(fp);
	return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	uint32_t rows = image->rows;
	uint32_t cols = image->cols;
	printf("P3\n%u %u\n255\n", cols, rows);
	for(uint32_t i = 0; i < rows; i++)
	{
		for(uint32_t j = 0; j < cols - 1; j++)
		{
			uint8_t r = image->image[i][j].R;
			uint8_t g = image->image[i][j].G;
			uint8_t b = image->image[i][j].B;

			printf("%3hhu %3hhu %3hhu   ", r, g, b);
		}
		uint8_t r = image->image[i][cols - 1].R;
		uint8_t g = image->image[i][cols - 1].G;
		uint8_t b = image->image[i][cols - 1].B;

		printf("%3hhu %3hhu %3hhu\n", r, g, b);
	}
}

//Frees an image
void freeImage(Image *image)
{
	uint32_t rows = image->rows;
	for(uint32_t i = 0; i < rows; i++) free(image->image[i]);
	free(image->image);
	free(image);
}
//逆天格式问题，比PTA还tm气人，每行最后一定不能有多余空格
