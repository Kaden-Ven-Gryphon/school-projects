
/******************************************************************************************************************************
* Modual: F to C

* Author: Kaden Gryphon

* Date: 08.31.18

* Version: 1.0

* Usage: convert tempatures

********************************************************************************************************************************/


#include <stdio.h>   //libaries


#define HOLD_SCREEN while(getchar() != 'q')  //q to quit

//table parts
#define LINE "*******"
#define HEADER_ROW "* %-*s * %-*s *\n"
#define TEMPS_ROW "* %-*g * %-*g *\n"

#define TABLE_SIZE 10 //used to agust the width of the colems


//inizilize varriables
float input;
float fahrenheit;
float celsius;



//main     **************************************************************************

int main(void)
{
	takeInputs();  //display promt and collect input
	calcValues();  // set fahrenheit and celsius variables based on input
	printTable();  // print table based of the defined size and colums with the calculated values

	HOLD_SCREEN;   //q to quit

	return(0);
}


//Functions   ***********************************************************************************

int takeInputs(void)
{
	printf("Enter a tempature in fahrenheit:  ");  
	scanf_s("%g", &input);              //colect a float input

	return(0);
}

int calcValues()
{
	fahrenheit = input;   // set variables
	ftoC();

	return (0);
}



int ftoC()   //F to C
{
	celsius = (fahrenheit - 32) * 5 / 9;    //set celsius as a function of fahrenheit
	return(0);
}

/*
int testing(void)
{
	printf("%g", fahrenheit);
	printf("%g", celsius);
	
	return(0);
}
*/


int printTable(void)   //print out the table
{
	printLine(TABLE_SIZE, 2);
	printf(HEADER_ROW, TABLE_SIZE, "Fahrenheit", TABLE_SIZE, "Celsius");
	printLine(TABLE_SIZE, 2);
	printf(TEMPS_ROW, TABLE_SIZE, fahrenheit, TABLE_SIZE, celsius);
	printLine(TABLE_SIZE, 2);
	return(0);

}



int printLine(int size, int numOfCol)  //print a horizontal line based on the size and number of colums 
{
	printf(LINE);
	for (int i = 0; i < (size*numOfCol); i++)
	{
		printf("*");
	}
	printf("\n");


	return(0);
}

