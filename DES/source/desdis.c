/****************************************************************************************
*
*	File:		desdis.c
*	Author:	Kaden Gryphon
*	Class:	CS 581-01
*	Program:	Program 1 Future Event List FEL
*	Due:		10.05.2022
*	Souces:	An Improved Ziggurat Method to Generate Normal Random Samples
*			https://www.doornik.com/research/ziggurat.pdf
*
*			Getting a better uniform distribution
*			https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Randomization.html
*
*			Probabilty and Statistical Inference 10th ed Hogg Tanis Zimmerman
*
*
*****************************************************************************************/

#include "desdis.h"

#define _DEBUG_MESSAGES
#ifdef _DEBUG_MESSAGES
	#include <stdio.h>
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...) {}
#endif


//look up table for ziggurat method
static double _desdis_s_zigX[DESDIS_NORMAL_ZIG_BLOCKS+1], _desdis_s_zigR[DESDIS_NORMAL_ZIG_BLOCKS];

unsigned int desdis_ui32_rand()
{
	unsigned int A = (unsigned int) rand(); //15bits each
	unsigned int B = (unsigned int) rand();
	unsigned int C = (unsigned int) rand();
	A = (A << 15) + B;	//30 bits
	A = (A << 2) + (C & 0x3);	//32 bits
	return A;
}

double desdis_double_rand()
{
	return (double)rand() / (double)RAND_MAX;
}


double desdis_ranNormalTail(double min, int isNegative)
{
	double x, y;
	do
	{
		x = log(desdis_double_rand()) / min;
		y = log(desdis_double_rand());
	} while (-2 * y < x * x);
	
	if(isNegative) return x-min;
	else return min - x;
}


void desdis_normal_zig_init(int blocks, double rightTail, double v)
{
	int i;
	double f;
	
	f = exp(-0.5 * rightTail * rightTail);
	_desdis_s_zigX[0] = v / f;
	_desdis_s_zigX[1] = rightTail;
	_desdis_s_zigX[blocks] = 0;
	
	for (i = 2; i < blocks; i++)
	{
		_desdis_s_zigX[i] = sqrt(-2 * log(v / _desdis_s_zigX[i-1] + f));
		f = exp(-0.5 * _desdis_s_zigX[i] * _desdis_s_zigX[i]);
	}
	for (i = 0; i < blocks; i++)
	{
		_desdis_s_zigR[i] = _desdis_s_zigX[i + 1] / _desdis_s_zigX[i];
	}
}


double desdis_normal_zig(double mean, double sd)
{
	unsigned int i;
	double x, u, f0, f1;
	
	while(1)
	{
		u = 2 * desdis_double_rand() -1;
		i = desdis_ui32_rand() & 0x7F;
		if(fabs(u) < _desdis_s_zigR[i]) return (u * _desdis_s_zigX[i]*sd)+mean;
		if(i == 0) return (desdis_ranNormalTail(DESDIS_NORMAL_ZIG_RIGHTTAIL, u < 0)*sd) + mean;
		x = u * _desdis_s_zigX[i];
		f0 = exp(-0.5 * (_desdis_s_zigX[i] * _desdis_s_zigX[i] - x * x));
		f1 = exp(-0.5 * (_desdis_s_zigX[i+1] * _desdis_s_zigX[i+1] - x * x));
		if(f1 + desdis_double_rand() * (f0 - f1) < 1.0) return (x*sd)+mean;
	}
}


double desdis_expo(double lambda)
{
	double retVal = 0;
	while(retVal == 0)
	{
		retVal = desdis_double_rand();
	}
	//cant have log(0)
	return -log(retVal) / lambda;
}


double desdis_empDiscrete(struct desdis_empDiscreteTable_s* table)
{
	double retVal = desdis_double_rand();
	for(int i = table->length-2; i >= 0; i--)
	{
		if(retVal > table->resultsAndProbs[i])
		{
			retVal = table->resultsAndProbs[i+table->length+1];
			break;
		}
		else if (i == 0)
		{
			retVal = table->resultsAndProbs[i+table->length];
		} 
	}
	return retVal;
}


#ifdef DESDIS_TEST_AND_VERIFY_DIS

int desdis_tav_all()
{
	DEBUG_LOG("\n--- START OF DISTRIBUTION TESTS ---\n");
	int t1 = desdis_tav_normal();
	int t2 = desdis_tav_expo();
	int t3 = desdis_tav_empDis();
	DEBUG_LOG("\n\n\n--- SUMMARY ---\n\n");
	if(t1) DEBUG_LOG("Test 1: OK\n");
	else DEBUG_LOG("Test 1: FAIL\n");
	if(t2) DEBUG_LOG("Test 2: OK\n");
	else DEBUG_LOG("Test 2: FAIL\n");
	if(t3) DEBUG_LOG("Test 3: OK\n");
	else DEBUG_LOG("Test 3: FAIL\n");
	if(t1 && t2 && t3) DEBUG_LOG("All: OK\n");
	else DEBUG_LOG("All: FAIL\n");
	DEBUG_LOG("\n--- END OF DISTRIBUTION TESTS ---\n");
	return t1 & t2;
}


int desdis_tav_normal()
{
	DEBUG_LOG("\n--- START OF NORMAL DISTRIBUTION TEST ---\n");
	//init the lookup tables
	DESDIS_NORMAL_INIT;
	
	//this is the expected mean and standard deviation
	double mean = 7;
	double sd = 4;
	
	DEBUG_LOG("Expected mean and standard deviation: %4lf, %4lf", mean, sd);
	
	//genreate 1000 samples of the distribution
	double sample[DESDIS_TAV_SAMPLESIZE];
	for(int i = 0; i < DESDIS_TAV_SAMPLESIZE; i++) sample[i] = DESDIS_NORMAL(mean, sd);
	
	
	
	//find the mean
	double sMean = 0;
	for(int i = 0; i < DESDIS_TAV_SAMPLESIZE; i++)
	{
		sMean += sample[i];
	}
	sMean /= DESDIS_TAV_SAMPLESIZE;
	
	//fill the histogram
	int counts[30];
	for(int i = 0; i < 30; i++) counts[i] = 0;
	
	for(int i = 0; i < DESDIS_TAV_SAMPLESIZE; i++)
	{
		int j = (int)sample[i]-round(sMean)+15;
		if(j >= 0 && j < 30)
		{
			counts[j]++;
		}
	}
	
	//print the histogram
	DEBUG_LOG("\n\n- histogram -\n");
	for(int i = 0; i < 30; i++)
	{
		DEBUG_LOG("%4d", (int)((i-15)+round(sMean)));
		for(int j = 0; j < counts[i]/(DESDIS_TAV_SAMPLESIZE/100); j++)
		{
			DEBUG_LOG("#");
		}
		DEBUG_LOG("\n");
	}
	DEBUG_LOG("\n\n");
	
	DEBUG_LOG("Expected Mean: %lf\nSample Mean: %lf\n", mean, sMean);
	
	//find the standard deviation
	double sSd = 0;
	for(int i = 0; i < DESDIS_TAV_SAMPLESIZE; i++)
	{
		sSd+= (fabs(sample[i] - sMean))*(fabs(sample[i] - sMean));
	}
	sSd /= DESDIS_TAV_SAMPLESIZE; //dont know if I should have a -1 here, at n 10000 it should not matter though
	sSd = sqrt(sSd);
	DEBUG_LOG("Expected Standard Deviation: %lf\nSample Standard Deviation: %lf\n\n", sd, sSd);
	
	if(round(mean) == round(sMean) && round(sd) == round(sSd))
	{
		DEBUG_LOG("Mean and Standard Diviation: OK\n");
		DEBUG_LOG("\n--- END OF NORMAL DISTRIBUTION TEST ---\n");
		return 1;
	}
	else
	{
		DEBUG_LOG("Mean and Standard Diviation: FAIL\n");
		DEBUG_LOG("\n--- END OF NORMAL DISTRIBUTION TEST ---\n");
		return 0;
	}
}



int desdis_tav_expo()
{
	DEBUG_LOG("\n--- START OF EXPONENTIAL DISTRIBUTION TEST ---\n");
	
	//this is the expected lambda
	double lambda = 0.7;
	
	
	DEBUG_LOG("Lambda and Expected mean : %4lf, %4lf", lambda, 1/lambda);
	
	//genreate 1000 samples of the distribution
	double sample[DESDIS_TAV_SAMPLESIZE];
	for(int i = 0; i < DESDIS_TAV_SAMPLESIZE; i++) sample[i] = desdis_expo(lambda);
	
	
	
	//find the mean
	double sMean = 0;
	for(int i = 0; i < DESDIS_TAV_SAMPLESIZE; i++)
	{
		sMean += sample[i];
	}
	sMean /= DESDIS_TAV_SAMPLESIZE;
	
	//fill the histogram
	int counts[30];
	for(int i = 0; i < 30; i++) counts[i] = 0;
	
	for(int i = 0; i < DESDIS_TAV_SAMPLESIZE; i++)
	{
		int j = (int)sample[i];
		if(j >= 0 && j < 30)
		{
			counts[j]++;
		}
	}
	
	//print the histogram
	DEBUG_LOG("\n\n- histogram -\n");
	for(int i = 0; i < 30; i++)
	{
		DEBUG_LOG("%4d", i);
		for(int j = 0; j < counts[i]/(DESDIS_TAV_SAMPLESIZE/100); j++)
		{
			DEBUG_LOG("#");
		}
		DEBUG_LOG("\n");
	}
	DEBUG_LOG("\n\n");
	
	DEBUG_LOG("Expected Mean: %lf\nSample Mean: %lf\n", 1/lambda, sMean);
	
	
	if(round(1/lambda) == round(sMean))
	{
		DEBUG_LOG("Mean/Lambda: OK\n");
		DEBUG_LOG("\n--- END OF EXPONENTIAL DISTRIBUTION TEST ---\n");
		return 1;
	}
	else
	{
		DEBUG_LOG("Mean/Lambda: FAIL\n");
		DEBUG_LOG("\n--- END OF EXPONENTIAL DISTRIBUTION TEST ---\n");
		return 0;
	}
}



int desdis_tav_empDis()
{
	DEBUG_LOG("\n--- START OF EMPIRICAL DISCRETE TEST ---\n");
	int retVal = 1;
	static struct desdis_empDiscreteTable_s testTable = {7, {0.1,0.4,0.45,0.65,0.8,0.81,1,1,2,3,4,5,6,7}};
	DEBUG_LOG("test table:\n");
	DEBUG_LOG("Cumulative Prob, Result\n");
	for(int i = 0; i < 7; i++)
	{
		DEBUG_LOG("%10.2f, %10.2f\n", testTable.resultsAndProbs[i], testTable.resultsAndProbs[i+7]);
	}
	
	double last = 0;
	DEBUG_LOG("Expected Histo\n");
	for(int i = 0; i < 7; i++)
	{
		DEBUG_LOG("%5.2f ", testTable.resultsAndProbs[i+7]);
		int count = (testTable.resultsAndProbs[i] - last) * 100;
		for(int j = 0; j < count; j++)
		{
			DEBUG_LOG("#");
		}
		last = testTable.resultsAndProbs[i];
		DEBUG_LOG("\n");
	}
	
	int counts[7];
	for(int i = 0; i < 7; i++)
	{
		counts[i] = 0;
	}
	for(int i = 0; i < 10000; i++)
	{
		double buf = desdis_empDiscrete(&testTable);
		for(int j = 0; j < 7; j++)
		{
			if(buf == testTable.resultsAndProbs[j+7]) counts[j]++;
		}
	}
	last = 0;
	DEBUG_LOG("Actual Histo\n");
	for(int i = 0; i < 7; i++)
	{
		DEBUG_LOG("%5.2f ", testTable.resultsAndProbs[i+7]);
		int count = counts[i] / 100;
		for(int j = 0; j < count; j++)
		{
			DEBUG_LOG("#");
		}
		DEBUG_LOG("\n");
		
		double buf = (double)counts[i] / 10000;
		double prob = testTable.resultsAndProbs[i] - last;
		double error = prob - buf;
		if(error < 0) error *=-1;
		if(error > 0.1) retVal = 0;
		last = testTable.resultsAndProbs[i];
	}
	if(retVal)
	{
		DEBUG_LOG("EMPIRICAL DISCRETE: OK\n");
	}
	else
	{
		DEBUG_LOG("EMPIRICAL DISCRETE: FAIL\n");
	}
	
	DEBUG_LOG("\n--- END OF EMPIRICAL DISCRETE TEST ---\n");
	return retVal;
}
#endif

