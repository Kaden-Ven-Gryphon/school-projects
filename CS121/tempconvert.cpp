
//
// Homework 2 problem 4
//


#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#define WIDTH 15

using namespace std;


float ctof(float in);

int main(void)
{

	ifstream infile;
	ofstream outfile;
    float temperature_F = 0.0, temperature_C = 0.0;
	unsigned int count = 0;

	infile.open("temps.txt");
	if (!infile)
	{
		cout << "\n\nFile failed on open | ERROR 1\n\n";
		system("pause");
		return -1;
	}
	cout << "File opened for read.\n\n";

	outfile.open("table.txt");
	if (!outfile)
	{
		cout << "\n\nFile failed on open | ERROR 1\n\n";
		system("pause");
		return -1;
	}
	cout << "File opened for write.\n\n";

	cout << left << setw(WIDTH) << "Celsius" << setw(WIDTH) << "Fahrenheit" << setw(WIDTH) << "Kelvin" << endl;
	outfile << left << setw(WIDTH) << "Celsius" << setw(WIDTH) << "Fahrenheit" << setw(WIDTH) << "Kelvin" << endl;

	while (!infile.eof())
	{
		infile >> temperature_C;
		cout << left << setw(WIDTH) << temperature_C << setw(WIDTH) << ctof(temperature_C) << setw(WIDTH) << temperature_C+273 << endl;
		outfile << left << setw(WIDTH) << temperature_C << setw(WIDTH) << ctof(temperature_C) << setw(WIDTH) << temperature_C+273 << endl;
		count++;
	}

	cout << "\n\nThere are " << count << " values in the file.\n\n";

	infile.close();
outfile.close();


	system("pause");
	return 0;
}

float ctof(float in)
{
	return (((9/5)*in)+32);
}
