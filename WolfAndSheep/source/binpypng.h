/*
*	binpypng.h
*	Kaden Gryphon
*	
*
*	This is used write binary img files and convert png
*
*
*/


#ifndef BINPYPNG_H
#define BINPYPNG_H

#include <stdio.h>
#include "binimg.h"


int binpypng_writebin(char* fileName, struct binimg_image_s* image);


#ifndef WINDOWS_F
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
	int binpypng_convert(char* fileName);
#endif




#endif
