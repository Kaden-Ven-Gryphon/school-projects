//
// Homework 1 Problem 5
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include <stdlib.h>
#include <time.h>

using namespace std;

float calBMI(float feet, float in, float lb);

int main(void)
{
	float in = 0;
	float feet = 0;
	float lb = 0;
	float bmi = 0.0f;

	cout << endl << setw(40) << "What is you wheight in lb: ";
	cin >> lb;
	cout << endl << setw(40) << "What is your hieght in ft and in: ";
	cin >> feet;
	cin >> in;

	bmi = calBMI(feet, in, lb);

	cout << endl << setw(40) << "BMI: " << bmi << endl;

	if (bmi < 18.5) cout << setw(40) << " " << "Under Weight" << endl << endl;
	else if (bmi < 25) cout << setw(40) << " " << "Normal Weight" << endl << endl;
	else if (bmi < 30) cout << setw(40) << " " << "Over Weight" << endl << endl;
	else cout << setw(40) << " " << "Obesity" << endl << endl;

	system("pause");
	return 1;
}

float calBMI(float feet, float in, float lb)
{
	float h = in + (feet * 12);
	float retVal = (lb / (h*h)) * 703;
	return retVal;
}
