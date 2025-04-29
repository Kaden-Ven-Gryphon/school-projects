//
//Homework 2 Problem 2
//
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#define SIZE 64
#define WIDTH 20

using namespace std;

int main(void)
{
	long long unsigned int big = 0;

	cout << setw(4) << "n" << setw(WIDTH + 5) << "VALUE of 2^n" << endl;
	cout << setw(4) << "0" << setw(WIDTH) << "1" << endl;
	for (int i = 1; i < SIZE; i++)
	{
		big = 1;
		for (int j = 0; j < i; j++) big *= 2;
		cout << setw(4) << i << setw(WIDTH) << big << endl;
	}
	system("pause");
	return 0;
}
