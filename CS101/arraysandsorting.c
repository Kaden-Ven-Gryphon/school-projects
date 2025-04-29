#include<stdio.h>


#define HOLD_SCREEN while(getchar() != 'q')
#define DEBUG 0


int main(void)
{
	char name[] = "kaden";
	char tmp = 'a';
	int size = sizeof(name);
	int pos = 0;
	/*
	while (name[pos] != '\0')
	{
		putchar(name[])
	}
	*/

	printf("\n\t%s", name);


	for (int i = 0; i < size-1; i++)
	{
		for (int j = 0; j < size - 2; j++)
		{
			
			if (name[j] > name[j + 1])
			{
				if (DEBUG) printf("\n mark");
				tmp = name[j];
				name[j] = name[j + 1];
				name[j+1] = tmp;
			}
		}
	}
	
	printf("\n\t%s", name);

	printf("\n\nq to quit: ");

	HOLD_SCREEN;

}
