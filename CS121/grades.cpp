//
// Homework 1 Problem 3
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include <math.h>

using namespace std;

int main(void)
{
	bool loop = true;
	bool firstRun = true;
	float tempIn = -1;
	float grades[50];
	int pos = 0;
	float droped = 100;
	float average = 0;

	while (loop)
	{
		cout << "Enter the next grade (-1 to exit): ";
		cin >> tempIn;
		if (tempIn == -1)
		{
			loop = false;
		}
		else
		{
			if (firstRun)
			{
				droped = tempIn;
				cout << setw(15) << "All Grades: " << droped << endl;
				cout << setw(15) << "Droped Grade: " << "n/a" << endl;
				cout << setw(15) << "Total grade: " << droped << endl<<endl<<endl;
				firstRun = false;
			}
			else
			{
				if (droped > tempIn)
				{
					grades[pos++] = droped;
					droped = tempIn;
				}
				else
				{
					grades[pos++] = tempIn;
				}

				cout << setw(15) << "All Grades: " << grades[0] << endl;
				average = grades[0];
				for (int i = 1; i < pos; i++)
				{
					average += grades[i];
					cout << setw(15) << " " << grades[i] << endl;
				}
				average /= (pos);
				cout << setw(15) << "Droped Grade: " << droped << endl;
				cout << setw(15) << "Total grade: " << average << endl << endl << endl;
			}
		}
	}
}
