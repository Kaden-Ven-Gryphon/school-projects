/******************************************************************************************************************************
* Modual: Homework 1

* Author: Kaden Gryphon

* Date: 8.31.18

* Version: 1.0

* Usage: caclulate vloltage from current and resistance

********************************************************************************************************************************/


#include <stdio.h>   //libaries

#define HOLD_SCREEN while(getchar() != 'q')  //q to quit

//table parts
#define LINE "*"
#define HEADER_ROW "* %-*s * %-*s * %-*s *\n"
#define VAL_ROW "* %-*g * %-*g * %-*g *\n"

#define TABLE_SIZE 10 //used to agust the width of the colems


//variables
float volt;
float current;
float resistance;


// main    ****************************************************************************************
int main(void)
{
	colectInputs();   //print promts and collect inputs

	volt = current * resistance;   // calculate volts

	printTable();  //print and fill the table in


	HOLD_SCREEN;  //q to quit
	return(0);
}



//  functions   *************************************************************************************

//search for two values as floats
int colectInputs(void)
{
	printf("Enter Current: ");
	scanf_s("%g", &current);
	printf("\nEnter Resistance:  ");
	scanf_s("%g", &resistance);


	return(0);
}






//print the table
int printTable()
{
	printLine(TABLE_SIZE, 3);
	printf(HEADER_ROW, TABLE_SIZE, "Volts", TABLE_SIZE, "Current", TABLE_SIZE, "Resistance" );
	printLine(TABLE_SIZE, 3);
	printf(VAL_ROW, TABLE_SIZE, volt, TABLE_SIZE, current, TABLE_SIZE, resistance );
	printLine(TABLE_SIZE, 3);
}


int printLine(int size, int numOfCol)  //print a horizontal line based on the size and number of colums 
{
	printf(LINE);
	for (int i = 0; i < (size*numOfCol); i++)
	{
		printf("*");
	}
	for (int i = 0; i < (numOfCol); i++)
	{
		printf("***");
	}
	printf("\n");


	return(0);
}
