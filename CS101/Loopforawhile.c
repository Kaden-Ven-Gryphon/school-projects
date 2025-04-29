/*****************************************************************************************************************************
* Modual: Loop for a while

* Author: Kaden Gryphon

* Date:  09.11.18

* Version: 1.0

* Usage: n/a

*

* 1) ask for velocity and check if viable  also check for negative value

* 2) ask for angle and check if viable

* 3) build table based on inputs

* 4) loop back to step 1

*

*
********************************************************************************************************************************/


//libaries   ************************************************************************************************************
#include<stdio.h>




//variables   **********************************************************************************************************
float v=0.0;
int deltaTheta;
int theta = 0;
int run = 1;

//main ********************************************************************************************************************
int main(void)
{

	while (run)
	{
		printf("\nEnter a velocity as a positive value or -1 to terminate:");		// 1.)ask for velocity and check if viable 
		scanf_s("%g", &v);
		if (v == -1)																// also check for negative value
		{
			run = 0;
			break;
		}
		printf("\nEnter an angle step:");											// 2.)ask for angle and check if viable
		scanf_s("%i", &deltaTheta);
		if (deltaTheta <= 0)														//and check if viable (force postive)
		{
			deltaTheta *= -1;
		}
		
		printf("\nAngle\t\tVelocity");												//3.) print table (the header line first
		for (int i = 0; i < (360 / deltaTheta)+1; i++)								// iterrate through the lines of the table based on the deltaTheta
		{
			printf("\n%i\t\t%3g", theta, v);
			theta += deltaTheta;
		}
	}
	


	return(0);
}
