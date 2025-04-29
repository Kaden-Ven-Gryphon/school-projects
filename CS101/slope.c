/******************************************************************************************************************************
* Modual: Homework 1

* Author: Kaden Gryphon

* Date: 08.31.18

* Version: 1.0

* Usage: calculate slope and find y

********************************************************************************************************************************/


#include <stdio.h> //libaries



#define HOLD_LINE while(getchar() != 'q')  // q to quit

//table parts
#define LINE "*"
#define HEADER_ROW "* %-*s * %-*s * %-*s * %-*s * %-*s * %-*s *\n"
#define VAL_ROW "* %-*g * %-*g * %-*g * %-*g * %-*g * %-*g *\n"

#define TABLE_SIZE 5 //used to agust the width of the colems



//variables
float x;
float y;
float rise;
float run;
float b;
float slope;

//main     **********************************************************************************
int main(void)
{
	collectInputs();  //propts the user for the inputs and records their awnsers

	slope = rise/run;   //claculate the slope
	y = x * slope + b;  //find y


	printTable();  //print and fill the table in

	HOLD_LINE;  // q to quit
	return(0);
}



//functions    ************************************************************
int collectInputs()  //colect 4 floats
{
	printf("Enter x: ");
	if (scanf_s("%g", &x) != 1)  //checking for failed scans
	{
		printf("error on input\n");
	}
	printf("Enter b: ");
	if (scanf_s("%g", &b) != 1)
	{
		printf("error on input\n");
	}
	printf("Enter rise: ");
	if (scanf_s("%g", &rise) != 1)
	{
		printf("error on input\n");
	}
	printf("Enter run: ");
	if (scanf_s("%g", &run) != 1)
	{
		printf("error on input\n");
	}


	return(0);
}


int printTable()
{
	printLine(TABLE_SIZE, 6);
	printf(HEADER_ROW, TABLE_SIZE, "Y", TABLE_SIZE, "X", TABLE_SIZE, "Slope", TABLE_SIZE, "Rise", TABLE_SIZE, "Run", TABLE_SIZE, "B");
	printLine(TABLE_SIZE, 6);
	printf(VAL_ROW, TABLE_SIZE, y, TABLE_SIZE, x, TABLE_SIZE, slope, TABLE_SIZE, rise, TABLE_SIZE, run, TABLE_SIZE, b);
	printLine(TABLE_SIZE, 6);
}








int printLine(int size, int numOfCol)  //print a horizontal line based on the size and number of colums 
{
	printf(LINE);
	

	for (int i = 0; i < numOfCol; i++)  // the space between the colems
	{
		printf("***");
		for (int i = 0; i < (size); i++)  //the space inside the colems
		{
			printf("*");
		}
	}
	printf("\n");

	return(0);
}
