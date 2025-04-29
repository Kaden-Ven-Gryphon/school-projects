/******************************************************************************************************************************
* Modual: Simple Missiles

* Author: Kaden Gryphon

* Date: 09.20.18

* Version: 1.0

* Usage: detirmin the distance and hight a simple projectile will go based on a starting velocity and theta

* 1. prompt the user for the Velocity of the projectile.
* 2. prompt the user for the Angle step (between 0-90 degrees report an error if not)
* 3. have a function for height, distance, and time
* 4. build a table that cycles through the angle values and prints the deciered values
*
*
*
* ERRORS:
*
*	1: the angle entered was not beetween 0 and 90
*


********************************************************************************************************************************/


// Libaries *********************************************************************************************************************
#include <stdio.h>
#include <math.h>


// Defines  *********************************************************************************************************************
#define HOLD_SCREEN while(getchar() != 'q')


#define PI 3.14159265359
#define G 9.81
#define RADS PI/180

#define WIDTH 71
#define NEW_LINE printf("\n\t");


#define DEBUG 0

//function decurations **********************************************************************************************************

float distance(float v, float theta);
float time(float v, float theta);
float height(float v, float theta);


// variables ********************************************************************************************************************
float velocity = 0;
float thetaInc = 0;
float d = 0;
float h = 0;
float t = 0;


// Main  ************************************************************************************************************************
int main(void)
{
	if (DEBUG)														// DEBUG
	{
		velocity = 250;
		thetaInc = 45;
		float test1 = velocity * velocity;
		float test2 = sin(2 * thetaInc*RADS);
		float test3 = test1 / G;
		float test4 = test3 * test2;
		printf("\n\t%10.2f", test1);
		printf("\n\t%10.2f", test2);
		printf("\n\t%10.2f", test3);
		printf("\n\t%10.2f", test4);
		d = distance(velocity, thetaInc);
		t = time(velocity, thetaInc);
		h = height(velocity, thetaInc);
		printf("\n\t%10.2f", d);
		printf("\n\t%10.2f", t);
		printf("\n\t%10.2f", h);
		
	}


	printf("\n\tEnter a Velocity: ");								// 1. prompt the user for the Velocity of the projectile.
	scanf_s("%f", &velocity);
	if (velocity < 0) velocity *= -1;
	printf("\n\tEnter a angle step: ");								// 2. prompt the user for the Angle step (between 0-90 degrees )
	scanf_s("%f", &thetaInc);

	if (DEBUG)														//DEBUG
	{
		printf("%f", velocity);
		printf("%f", thetaInc);
	}


	if (thetaInc < 0 || thetaInc > 90)								// report an error if not
	{
		printf("\nError 1: non valid angle input");					//error 1 
		printf("\nq to quit");
		HOLD_SCREEN;
		return(1);
	}


	NEW_LINE
	for (int i = 0; i < WIDTH; i++)									//start table
	{
		printf("*");
	}
																	//table header
	printf("\n\t* %-10s * %-10s * %-10s * %-10s * %-15s *", "Velocity", "Angle", "Distance", "Height", "Time of Flight");

	NEW_LINE
	for (int i = 0; i < WIDTH; i++)
	{
		printf("*");
	}

	for (float i = 0; i < 91; i += thetaInc)						//print out the values for each angle
	{
		d = distance(velocity, i);									//calculate distance
		t = time(velocity, i);										//calculate time
		h = height(velocity, i);									//calculate height

		if (DEBUG) printf("%f", i);									//DEBUG

		printf("\n\t* %-10.2f * %-10.2f * %-10.2f * %-10.2f * %-15.2f *", velocity, i, d, h, t);
		NEW_LINE
		for (int j = 0; j < WIDTH; j++)
		{
			printf("*");
		}

	}

	printf("\n\n'q' to quit: ");
	HOLD_SCREEN;

	return(1);
}// int ma...


// functions definions  *********************************************************************************************************

float distance(float v, float theta)								//distance funciton
{
	return(v*v * sin(2 * theta * RADS) / G);
}//float dista... 

float time(float v, float theta)									//time function
{
	return(v*2 * sin(theta * RADS) / G);	
}//float ti...

float height(float v, float theta)									//height function
{
	return(v*v * sin(theta * RADS) * sin(theta * RADS) / (2*G));
}//float heigh...
