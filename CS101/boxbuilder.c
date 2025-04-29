/******************************************************************************************************************************
* Modual: Box Builder

* Author: Kaden Gryphon

* Date: 09.04.18

* Version: 1.0

* Usage: create a box of the users selected size and character type

*Procedure

* 1. ask user for inputs

* 2. check if they and valid if not re ask

* 3. build a box out of the inputs

*

* Errors:

* error 1 : there was a character that did not match: +, -, *, /, %

* error 2 : there was an input for the width that was out of the range of 10-50

* error 3 : there was an input for the height that was out of the range of 5-15

********************************************************************************************************************************/

#include<stdio.h>  //libaries
#include<stdlib.h> 

#define HOLD_SCREEN printf("\nPress 'n' to continue\n"); while(getchar() != 'n')  // n for next


//   Variables  ****************************************************************
int x = 0;
char block;
int xDim = 0;
int yDim = 0;
int run = 1;


//    Main    ************************************************************************
int main(void)
{
	system("cls");
	while(run)     // main loop for the state machine
	{
		stateControler();
	}
		
	//HOLD_SCREEN;
	

	return(0);
}



//  state machine for swaping between prompts****************************************************
int stateControler(void)
{
	

	switch (x)
	{
		//		case 0 **********************************************************************************************
	case 0:
		
		//system("cls");
		x = 1;

		break;
		//		case 1  *********************			ask the user fo the building block character  and   check if it is valid  ***********************************************************************
	case 1:

		//system("cls");
		printf("Chose a building component [+,-,*,/,%%] : ");
		

		if (scanf_s(" %c", &block) != 1)
		{
			
			printf("\nError 2: input not found!.\n");	// look for errors with scanf
			
			HOLD_SCREEN;
			x = 0;
		}
		
		if (block != '+' && block != '-' && block != '*' && block != '/' && block != '%')  // check if it is a valid char
		{
			system("cls");
			printf("\nError 1: invaled character input!.  Please select anouther.\n");
			//HOLD_SCREEN;
			x = 0;
			break;										// if not jump to case 0
		}
		else
		{
			system("cls");
			printf("\nYou entered %c\n", block);		// report what input the program picked up
			x = 2;
		}
		

		
		break;
		//      case 2   *********************			ask the user for the x dim and check if it was valid     ***********************************************************************
	case 2:
		printf("\nEnter the x conponent [10-50] : ");
		if (scanf_s("%i", &xDim) != 1)
		{
			printf("\nError 2: input not found!.\n");	//checking for error with scanf

			HOLD_SCREEN;
			x = 0;
		}
		if (xDim < 10 || xDim > 50)
		{
			system("cls");
			printf("\nError 2: invaled width input!.\n  Please select anouther.\n");   //chacking if input in bounds
		}
		else
		{
			system("cls");
			printf("You entered: %i\n", xDim);			//report what input the program has
			x = 3;
		}


		break;
		//     case 3   ****************************	ask the user for the y dim and check if it is valid    **********************************************************************
	case 3:
		printf("\nEnter the y conponent [5-15] : ");
		if (scanf_s("%i", &yDim) != 1)
		{
			printf("\nError 2: input not found!.\n");	//check for error with scanf

			HOLD_SCREEN;
			x = 0;
		}
		if (yDim < 5 || yDim > 15)
		{
			system("cls");
			printf("\nError 3: invaled height input!.\n  Please select anouther.\n");  //check for input within bounds
		}
		else
		{
			system("cls");
			printf("You entered: %i\n", yDim);			//reprot what input the program has
			x = 4;
		}

		break;
		//     case 4   ***********************			build box based on users inputs  ***************************************************************************
	case 4:
		printf("Building box now...");
		HOLD_SCREEN;
		system("cls");

		buildBox(block, xDim, yDim);					//build box based on the block, xDim, and yDim
		
		x = 5;


		break;
		//     case 5   ****************************	ask user if they want to start again or quit    **********************************************************************
	case 5:

		printf("Make a new box [y/n] : ");
		if (scanf_s(" %c", &block) != 1)
		{

			printf("\nError 2: input not found!.\n");	//check for errors in scanf

			HOLD_SCREEN;
			x = 0;
		}
		if (block == 'y' || block == 'n')				//check if valid input
		{
			if (block == 'y')
			{
				system("cls");							//back to case 0 to start again
				x = 0;
			}
			if (block == 'n')
			{
				run = 0;								//end the main loop
			}
		}

		break;


		//     case default   **************************************************************************************************
	default:
		x = 1;
		break;

	}
	return (0);
}



int buildBox(char c, int xDims, int yDims) // the box builder function
{
	for (int i = 0; i < xDims; i++)				//write the first line 
	{
		printf("%c", c);
	}
	printf("\n");
	for (int i = 0; i < (yDims - 2); i++)		//write the middle lines
	{
		printf("%c", c);
		for (int j = 0; j < (xDims - 2); j++)	//puting the spaces
		{
			printf(" ");
		}
		printf("%c", c);
		printf("\n");
	}
	for (int i = 0; i < xDims; i++)				// write the last line
	{
		printf("%c", c);
	}
	printf("\n");

	return(0);
}
