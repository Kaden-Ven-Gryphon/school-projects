/***************************************************************************
*
* Module: typesTable.c
*
* Author: Kaden Gryphon (kb0125Auah.edu)
*
* Description: 1. Create a table that contains each integer or rational
* number in the C language, separating the unsigned and
* signed numbers each on a separte row. Use the '*'
* character for your table header/footer and line separ-
* ters
* 2. The first column should contain the text name of the
* data type being displayed, like "unsigned char"
* 3. The second column should contain the MIN value.
* 4. The third column should contain the MAX value.
* HINT. Use limits.h to help resolve these values, except
* for the MIN on unsigned values which is always 0
* 5. The last column should be a representative value that
* would not fit into the MIN-MAX span of the previous
* row's data type.
*
* Dependencies: stdio.h, limits.h
*
* Usage: typesTable.exe
*
* Revision: 1.0
*
* ***************************************************************************/

#include <stdio.h>  //libaries
#include <limits.h>

//table parts
#define LINE   "******************************************************************************************************************"
#define HEADER "* DATA TYPE                 * MINIMUS VALUE             * MAXIMUM VALUE             * REPRESENTATIVE VALUE      *"
#define CHAR_ROW "* %25s * %25d * %25d * %25d *"

//q to quit
#define HOLD_SCREEN while(getchar() != 'q')


int main(void)
{
	header();
	printf(CHAR_ROW, "char", SCHAR_MIN, SCHAR_MAX, 100);
	printf("\n%s\n", LINE);
	printf(CHAR_ROW, "unsigned char", 0, UCHAR_MAX, 200);
	printf("\n%s\n", LINE);

	HOLD_SCREEN;
	return(0);
}

int header(void)  //print header
{
	printf("%s\n", LINE);
	printf("%s\n", HEADER);
	printf("%s\n", LINE);
	
	return(0);
}
/*
int entry()
{


	return(0);
}
*/

