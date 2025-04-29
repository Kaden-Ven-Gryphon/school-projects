//
// Homework 1 Problem 2
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include <math.h>

using namespace std;

int main(void)
{
	int startTemp = 0;
	int endTemp = 0;
	int increment = 0;

	cout << "Enter the start temperature: ";
	cin >> startTemp;
	cout << endl << "Enter the end tempature: ";
	cin >> endTemp;
	cout << endl << "Enter the increment: ";
	cin >> increment;

	cout << endl << endl << setw(9) << "Celcius" << setw(12) << "Fahrenheit" << setw(8) << "Kelvin" << endl;
	for (int i = startTemp; i <= endTemp; i += increment)
	{
		cout << setw(9) << i << setw(12) << (float)(((9.0/5.0)* i) + 32.0) << setw(8) << (float)(i + 273.15) << endl;
	}



	system("pause"); // hold output window on screen
}
