/******************************************************************************************************************************
* Modual:

* Author: Kaden Gryphon

* Date:

* Version: 1.0

* Usage: n/a

********************************************************************************************************************************/


//Libaries**********************************************************************************************************************
#include<stdio.h>
#include<string.h>
#include<ctype.h>


void strFix(char str[]);


//Main**************************************************************************************************************************
int main(void)
{
	char name[81];
	char first[41];
	char last[41];
	name[0] = '\0';

	int run = 1;



	while (run)
	{
		printf("\n\tEnter your first name or 'quit':> ");
		fgets(first, sizeof(name), stdin);
		if (strcmp(first, "quit\n") ==0) break;
		printf("\n\tEnter your last name:> ");
		fgets(last, sizeof(name), stdin);
		strFix(first);
		strFix(last);
		strcpy_s(name, sizeof(name), first);
		strcat_s(name, sizeof(name), " ");
		strcat_s(name, sizeof(name), last);
		printf("\n\tWelcome, %s to the wonderful world of strings in C\n", name);
	}



	return(1);
}//int main(vo...


void strFix(char str[])
{
	int size = strlen(str);
	str[size - 1] = '\0';
	size = strlen(str);
	for (int i = 0; i < size; i++)
	{
		str[i] = (char)toupper(str[i]);
	}
}
