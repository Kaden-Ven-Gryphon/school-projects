/*
*	binpypng.c
*	Kaden Gryphon
*	
*
*	This is used write binary img files and convert png
*
*
*/

#include "binpypng.h"

//#define _DEBUG_MESSAGES
#ifdef _DEBUG_MESSAGES
	#include <stdio.h>
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...) {}
#endif
#define _ERROR_MESSAGES
#ifdef _ERROR_MESSAGES
	#include <stdio.h>
	#define ERROR_LOG fprintf
#else
	#define ERROR_LOG(...) {}
#endif



int binpypng_writebin(char* fileName, struct binimg_image_s* image)
{
	DEBUG_LOG("DEBUG: start writebin\n");
	DEBUG_LOG("fileName: %s\n",fileName);
	int w = image->width;
	int h = image->hight;
	
	FILE* fp;
	fp = fopen(fileName, "wb");
	if(fp == NULL)
	{
		ERROR_LOG(stderr, "ERROR: binpypng_writebin fopen failed\n");
		return 1;
	}
	
	fwrite(&w, sizeof(int), 1, fp);
	fwrite(&h, sizeof(int), 1, fp);
	
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			fwrite(&image->rawimg[i][j].r, sizeof(char), 1, fp);
			fwrite(&image->rawimg[i][j].g, sizeof(char), 1, fp);
			fwrite(&image->rawimg[i][j].b, sizeof(char), 1, fp);
			fwrite(&image->rawimg[i][j].a, sizeof(char), 1, fp);
		}
	}
	
	
	
	
	fclose(fp);
	DEBUG_LOG("DEBUG: end writebin\n");
	return 0;
}


#ifndef WINDOWS_F
int binpypng_convert(char* fileName)
{
	DEBUG_LOG("DEBUG: start binpypng_convert\n");
	
	char* eargs[4] = {"python3", "./convert-bin-to-png.py", fileName, NULL};
	
	
	int pid = fork();
	if(pid == 0)
	{
		DEBUG_LOG("DEBUG: end binpypng_convert child\n");
		execvp(eargs[0], eargs);
	}
	
	wait(NULL);
	DEBUG_LOG("DEBUG: end binpypng_convert\n");
	return 0;
}	
#endif


