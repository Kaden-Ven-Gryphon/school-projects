/****************************************************************************************
*
*	File:		dumptruckoperation.h
*	Author:	Kaden Gryphon
*	Class:	CS 581-01
*	Program:	Program 4 dump truck sim
*	Due:		11.09.2022
*
*	Use:		Run trials of the dump truck sim and record data
*****************************************************************************************/
#ifndef DUMPTRUCKOPERATION_H
#define DUMPTRUCKOPERATION_H

#include "desfel.h"
#include "desdis.h"

struct des_dumptruckResults_s
{
	double loaderUtilization;
	double scaleUtilization;
};

struct des_dumptruckResults_s des_dumptruck1(int verbose);
#endif
