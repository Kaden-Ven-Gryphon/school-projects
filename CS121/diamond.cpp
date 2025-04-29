//
//Homework 2 Problem 1
//
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

int main(void)
{
	char star = '*';
	char blank = ' ';
	int size = 9;
	int step = 0;
	while (step < 2)
	{
		switch (step)
		{
		case 0:
			cout << "Please enter an odd number for the size:> ";
			cin >> size;
			if (size % 2) step++;
			cout << "\n\n\n";
			break;
		case 1:
			for (int i = 0; i < ((size + 1) / 2); i++)
			{
				cout << setfill(blank) << setw(((size / 2) - i)+1) << blank << setfill(star) << setw((i * 2)+1) << star << endl;
			}
			size -= 2;
			for (int i = 2; i < (((size + 1) / 2) + 2); i++)
			{
				cout << setfill(blank) << setw(i) << blank << setfill(star) << setw(size - ((i-2)*2)) << star << endl;
			}
			step++;
			break;
		}//switch
	}//while
	

	system("pause");
	return 0;
}
