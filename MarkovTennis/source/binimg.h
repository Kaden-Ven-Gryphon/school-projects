/*
*	binimg.c
*	Kaden Gryphon
*	
*
*	This is a set of functions for creating and manipulate raw binary image data
*
*
*/


#ifndef BINIMG_H
#define BINIMG_H

#include <stdlib.h>
#include <math.h>



struct pixel_s
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};


struct binimg_image_s
{
	int width;
	int hight;
	struct pixel_s** rawimg;
};






int binimg_initImage(int width, int hight, struct binimg_image_s* image);

struct pixel_s binimg_colorNormal(struct pixel_s old, struct pixel_s new);
int binimg_drawRectangle(int x, int y, int width, int hight, int fill, struct pixel_s color, struct binimg_image_s* image);
int binimg_drawCircle(int x, int y, int radius, int fill, struct pixel_s color, struct binimg_image_s* image);
int binimg_drawBitmapText(int x, int y, struct pixel_s color, char *text, int font[37][7][8], int size, struct binimg_image_s* image);
int binimg_testGradiant(struct binimg_image_s* image);



#endif
