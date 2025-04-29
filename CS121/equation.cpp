//
// Homework 1 Problem 1
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include <math.h>

using namespace std;

int main(void)
{
	float output = 0;
	float x = 3.73;
	float y = 3.50;
	output = (3 * (pow(x,2)) + (5 * x) + 1);
	cout << "Equation 1: R = " << output << endl;
	x = 9.385;
	output = (((x*x) - (11 * x) + 27) / (pow((x - 6), 2)));
	cout << "Equation 2: S = " << output << endl;
	x = 2.25;
	output = (((pow(x,2) + 3)/pow(y,3)) + ((x = 3) / (pow(y,3)) + (1/y)));
	cout << "Equation 3: T = " << output << endl;

	system("pause"); // hold output window on screen
}
