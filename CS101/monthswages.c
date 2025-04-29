/******************************************************************************************************************************
* Modual: Month's Wages

* Author: Kaden Gryphon

* Date: 09.27.30

* Version: 1.0

* Usage: calulate a month's wages through user input

********************************************************************************************************************************/

//Libaries ####################################################################################################################
#include<stdio.h>


//defines  ####################################################################################################################
#define HOLD_SCREEN while(getchar() != 'q')
#define HOUR_PAY 8.51

#define DEBUG 0


//main  #######################################################################################################################
int main(void)
{
	int cal[] = {0,0,0,8,4,8,8,0,0,8,8,4,8,8,0,0,8,8,4,8,8,0,0,8,8,4,8,8,0,0};	//array of hours for each day
	int size = sizeof(cal)/sizeof(int);											//find the number of days


	if (DEBUG) printf("\n%i", size);

	float totalWage = 0;														//init the variables of total wage, total hours, and hourly pay
	int totalHours = 0;

	float hourPay = 0;

	printf("\n\tEnter Hourly Pay: $");											//prompt the user for hourly wage
	scanf_s("%f", &hourPay);													//colect their value

	for (int i = 0; i < size; i++)												//cycle through the array to sum up total hours
	{
		totalHours += cal[i];
		if (DEBUG) printf("\n%i", cal[i]);
	}

	totalWage = totalHours * hourPay;											//cal the total wage


	if (DEBUG)
	{
		printf("\n\n\t%i  %f", totalHours, totalWage);
	}

	printf("\n\t##################################");							//print the table
	printf("\n\t# %-18s # %9i #", "#days", size);
	printf("\n\t##################################");
	printf("\n\t# %-18s # %9i #", "#hours worked", totalHours);
	printf("\n\t##################################");
	printf("\n\t# %-18s # $%8.2f #", "Total monthly wage", totalWage);
	printf("\n\t##################################");


	printf("\n\n'q' to quit: ");												//q to quit
	HOLD_SCREEN;	

	return(1);
}//int main(v...
