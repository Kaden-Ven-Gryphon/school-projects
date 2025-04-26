/****************************************************************************************
*
*	File:		dessupercomp.h
*	Author:	Kaden Gryphon
*	Class:	CS 581-01
*	Program:	Program 5 Super Computer
*	Due:		11.30.2022
*
*	Use:		Run trials of the super computer sim and record data
*****************************************************************************************/
#ifndef DESSUPERCOMPUTER_H
#define DESSUPERCOMPUTER_H

#include "desfel.h"
#include "desdis.h"

struct des_supercompResults_s{
	int success;
	int submitted[3];
	int completed[3];
	double meanResponse[3];
	double utilization[6];
	int maxQueLength[6];
};

struct des_process_s{
	int id;
	int type;
	int step;
	int length;
	float enterTime;
	float exitTime;
	struct des_process_s* next;
};

struct des_supercompResults_s des_supercomp1(int verbose);


#endif
