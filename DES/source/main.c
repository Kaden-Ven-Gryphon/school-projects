/****************************************************************************************
*
*	File:		main.h
*	Author:	Kaden Gryphon
*	Class:	CS 581-01
*	Program:	Program 1 Future Event List FEL
*	Due:		10.31.2022
*
*	Use:		call the test functions
*****************************************************************************************/


#include <stdio.h>
#include <math.h>
#include "desfel.h"
#include "desdis.h"
#include "dessupercomp.h"

int main()
{
	static char processorChars[6] = {'S', 'M', 'N', 'V', 'G', 'F'};
	double ttable95con49df = 2.009575;
	
	double sd[3] = {0,0,0}; //standard deviation I am using the expanded formula: sum(x_i^2) -2xBar*sum(x_i) + n*xBar^2
			//this way I can use running totals and to not have to keep a list of every value from the sample
	double sdsum1[3] = {0,0,0}; //sum(x_i^2)
	double sdsum2[3] = {0,0,0}; //sum(X_i)
	
	double moe[3] = {0,0,0};
	
	struct des_supercompResults_s retBuf;
	struct des_supercompResults_s meanSum;
	
	for(int i = 0; i < 3; i++)
	{
		meanSum.submitted[i] = 0;
		meanSum.completed[i] = 0;
		meanSum.meanResponse[i] = 0;
	}
	for(int i = 0; i < 6; i++)
	{
		meanSum.utilization[i] = 0;
		meanSum.maxQueLength[i] = 0;
	}
	
	
	srand((unsigned int)time(NULL));
	desdis_tav_all();
	desfel_tav_all();
	for(int i = 0; i < 50; i++)
	{
		printf("%d\n",i);
		retBuf = des_supercomp1(0);
		if(retBuf.success)
		{
	
			for(int j = 0; j < 3; j++)
			{
				meanSum.submitted[j] += retBuf.submitted[j];
				meanSum.completed[j] += retBuf.completed[j];
				meanSum.meanResponse[j] += retBuf.meanResponse[j];
				sdsum1[j] += retBuf.meanResponse[j] * retBuf.meanResponse[j];
				sdsum2[j] += retBuf.meanResponse[j];
			}
			for(int j = 0; j < 6; j++)
			{
				meanSum.utilization[j] += retBuf.utilization[j];
				meanSum.maxQueLength[j] += retBuf.maxQueLength[j];
			}
		}
		else
		{
			printf("ERROR sim failed");
			return 1;
		}
	}
	
	
	//get means
	for(int i = 0; i < 3; i++)
	{
		meanSum.submitted[i] /= 50;
		meanSum.completed[i] /= 50;
		meanSum.meanResponse[i] /= 50;
	}
	for(int i = 0; i < 6; i++)
	{
		meanSum.utilization[i] /= 50;
		meanSum.maxQueLength[i] /= 50;
	}
	//get standard devs
	for(int i = 0; i < 3; i++)
	{
		sd[i] = sdsum1[i] - (2*meanSum.meanResponse[i]*sdsum2[i]) + (50*meanSum.meanResponse[i]*meanSum.meanResponse[i]);
		sd[i] /= 49;
		sd[i] = sqrt(sd[i]);
		//cal margin of error
		moe[i] = ttable95con49df * sd[i] / sqrt(50);
	}
	
	printf("\n\n 50 trails\n");
	printf("\n\nMean Calculated Statistics\n");
	printf("\tJobs\n");
	for(int i = 0; i < 3; i++)
	{
		printf("\t\tType %d mean submitted\t\t %d\n", i+1, meanSum.submitted[i]);
		printf("\t\t       mean completed\t\t %d\n", meanSum.completed[i]);
		printf("\t\t       mean mean response\t %0.2f\n", meanSum.meanResponse[i]);
		printf("\t\t       mean response c.i.  [\t %0.2f, %0.2f\t]\n", meanSum.meanResponse[i] - moe[i], meanSum.meanResponse[i] + moe[i]);
	}
	
	printf("\tProcessors\n");
	for(int i = 0; i < 6; i++)
	{
		printf("\t\tType %c mean utilization\t\t %0.2f\n", processorChars[i], meanSum.utilization[i]);
		printf("\t\t       mean max queue length\t %d\n", meanSum.maxQueLength[i]);
	}
	
	
	return 0;
}
