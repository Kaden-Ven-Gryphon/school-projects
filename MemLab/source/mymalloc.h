/*
*	mymalloc.h
*	Kaden Gryphon
*	
*
*	simple libray for lab 1 that implments mymalloc, and myfree for a mmap heap
*
*
*/

#ifndef MYMALLOC_H
#define MYMALLOC_H

//Need this flag to have MAP_ANON be defined for mmap
#define _GNU_SOURCE
#include <sys/mman.h>



extern void* mymalloc(int size);
extern void myfree(void *ptr);

#endif
