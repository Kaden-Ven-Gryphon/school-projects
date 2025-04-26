/****************************************************************************************
*
*	File:		desairportcheckout.h
*	Author:	Kaden Gryphon
*	Class:	CS 581-01
*	Program:	Program 2 Grocery Checkout
*	Due:		10.31.2022
*
*	Use:		Run trials of the grocery checkout sim and record data
*****************************************************************************************/
#ifndef DESAIRPORTCHECKOUT_H
#define DESAIRPORTCHECKOUT_H

#include "desfel.h"
#include "desdis.h"

struct des_airportResults_s{
	double meanResponse1;
	double meanResponse2;
	double meanResponseTotal;
};


struct des_airportResults_s airport1(double lambda1, double lambda2, double mu1, double sigma1, double mu2, double sigma2, float endTime, int allowTransfers, int verbose);


#endif
