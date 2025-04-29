//
// Homework 1 Problem 5
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include <stdlib.h>
#include <time.h>

#define SIZE 50

using namespace std;

int findMin(unsigned int x[], int size);
int findMax(unsigned int x[], int size);


int main(void)
{
	unsigned int mix[SIZE];
	srand(time(NULL));
	int max = 0;
	int min = 100;

	for (int i = 0; i < 50; i++)
	{
		mix[i] = rand() % 100;
		/*
		if (mix[i] < min) min = mix[i];   //this would get every thing done in one loop
		if (mix[i] > max) max = mix[i];
		cout << mix[i] << ", ";
		*/
	}
	//cout << endl << min << endl;
	//cout << max << endl;

	min = findMin(mix, SIZE);
	max = findMax(mix, SIZE);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			cout << setw(3) << mix[(i * 10) + j] << ", ";
		}
		cout << endl << endl;
	}

	cout << setw(15) << "Min value is: " << min << endl;
	cout << setw(15) << "Max value is: " << max << endl;

	system("pause");

}

int findMin(unsigned int x[], int size)
{
	int temp = 100;
	for (int i = 0; i < size; i++)
	{
		if (x[i] < temp) temp = x[i];
	}
	return temp;
}

int findMax(unsigned int x[], int size)
{
	int temp = 0;
	for (int i = 0; i < size; i++)
	{
		if (x[i] > temp) temp = x[i];
	}
	return temp;
}
