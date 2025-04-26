/****************************************************
*
*	Kaden Gryphon
*	Lab 1 mymalloc
*	09.15.2023
*	CS 690-01
*	Testing program for mymalloc lib
*
****************************************************/


#include "mymalloc.h"


#include <stdio.h>

int main()
{
	int* a = (int*)mymalloc(sizeof(int));
	*a = 25;
	printf("int a was allocated at %lx and stores %d\n", (long)a, *a);
	int* b = (int*)mymalloc(sizeof(int));
	int* c = (int*)mymalloc(sizeof(int));
	myfree(b);
	myfree(c);
	myfree(a);
	
	int** array = (int**)mymalloc(sizeof(int*)*8);
	for(int i = 0; i < 8; i++)
	{
		array[i] = (int*)mymalloc(sizeof(int)*8);
	}
	
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			array[i][j] = i*8 + j;
		}
	}
	
	printf("ARRAY TEST:\n");
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			printf("%3d", array[i][j]);
		}
		printf("\n");
	}
	
	
	void* spacewaste = mymalloc(3515);
	
	double* noroom = (double*)mymalloc(sizeof(double));
	if(!noroom)
	{
		printf("There was not enough room to allocate a double\n");
	}
	
	printf("freeing array\n");
	for(int i = 0; i < 8; i++)
	{
		myfree(array[i]);	
	}
	myfree(array);
	
	noroom = (double*)mymalloc(sizeof(double));
	if(noroom != NULL)
	{
		printf("There was room to allocate a double\n");
	}
	
	myfree(noroom);
	myfree(spacewaste);
	return 0;
}
