/****************************************************************************************
*
*	File:		desdis.h
*	Author:	Kaden Gryphon
*	Class:	CS 581-01
*	Program:	Program 1 Future Event List FEL
*	Due:		10.05.2022
*	
*	Use:		disdis.h is the collection of functions to generate randome
*			numbers of diffrent distributions
*
*****************************************************************************************/

#ifndef DESDIS_H
#define DESDIS_H


#include <stdlib.h>
#include <math.h>

//constants for ziggurat method
#define DESDIS_NORMAL_ZIG_BLOCKS 128
#define DESDIS_NORMAL_ZIG_RIGHTTAIL 3.442619855899
#define DESDIS_NORMAL_ZIG_V 9.91256303526217e-3



//control to include and compile the testing methods
#define DESDIS_TEST_AND_VERIFY_DIS


//control of which method of normal distribution to use
#define DESDIS_NORMAL_ZIG

#ifdef DESDIS_NORMAL_ZIG
	#define DESDIS_NORMAL desdis_normal_zig
	#define DESDIS_NORMAL_INIT desdis_normal_zig_init(DESDIS_NORMAL_ZIG_BLOCKS, DESDIS_NORMAL_ZIG_RIGHTTAIL, DESDIS_NORMAL_ZIG_V)
#endif

//return a 32bit random number
unsigned int desdis_ui32_rand();

//returns a uniform 0 to 1 number
double desdis_double_rand();

//functions for ziggurat method normal distribution
double desdis_ranNormalTail(double min, int isNegative);
void desdis_normal_zig_init(int blocks, double rightTail, double v);
double desdis_normal_zig(double mean, double sd);

//functions for exponetial distribution
double desdis_expo(double lambda);

//functions for empirical discreate distributions
struct desdis_empDiscreteTable_s
{
	int length;
	double resultsAndProbs[];
};
double desdis_empDiscrete(struct desdis_empDiscreteTable_s* table);


#ifdef DESDIS_TEST_AND_VERIFY_DIS
	#define DESDIS_TAV_SAMPLESIZE 10000
	
	int desdis_tav_all();
	//returns true (1) if the expected and sample at within 0.5 of each other
	int desdis_tav_normal();
	int desdis_tav_expo();
	int desdis_tav_empDis();
#endif

#endif
