/******************************************************************************************************************************
* Modual: Homework 1

* Author: Kaden Gryphon

* Date: 08.31.18

* Version: 1.0

* Usage: calculate the thrid side of a right triangle

********************************************************************************************************************************/


#include <stdio.h> //libaries
#include <math.h>


#define HOLD_LINE while(getchar() != 'q')  // q to quit

//table parts
#define LINE "*"
#define HEADER_ROW "* %-*s * %-*s * %-*s *\n"
#define VAL_ROW "* %-*g * %-*g * %-*g *\n"

#define TABLE_SIZE 10 //used to agust the width of the colems



//variables
float sideOne;
float sideTwo;
float sideThree;

//main     **********************************************************************************
int main(void)
{
	collectInputs();  //propts the user for the sides and records their awnsers

	sideThree = sqrt(pow(sideOne,2) + pow(sideTwo,2));   //claculate the third side

	printTable();  //print and fill the table in

	HOLD_LINE;  // q to quit
	return(0);
}



//functions    ************************************************************
int collectInputs()  //colect two floats
{
	printf("Enter side one: ");
	if (scanf_s("%g", &sideOne) != 1)  //checking for failed scans
	{
		printf("error on input\n");
	}
	printf("Enter side two: ");
	if (scanf_s("%g", &sideTwo) != 1)
	{
		printf("error on input\n");
	}

	return(0);
}


int printTable()
{
	printLine(TABLE_SIZE, 3);
	printf(HEADER_ROW, TABLE_SIZE, "Side One", TABLE_SIZE, "Side Two", TABLE_SIZE, "Side Three");
	printLine(TABLE_SIZE, 3);
	printf(VAL_ROW, TABLE_SIZE, sideOne, TABLE_SIZE, sideTwo, TABLE_SIZE, sideThree );
	printLine(TABLE_SIZE, 3);
}








int printLine(int size, int numOfCol)  //print a horizontal line based on the size and number of colums 
{
	printf(LINE);
	for (int i = 0; i < (size*numOfCol); i++)  //the space inside the colems
	{
		printf("*");
	}
	
	for (int i = 0; i < numOfCol; i++)  // the space between the colems
	{
		printf("***");
	}
	printf("\n");

	return(0);
}
