#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <time.h>
#include <random>

#define PAUSE system("pause")

using namespace std;

void initMatrix(int **m, int s);
void sortMatrix(int **m, int s);
void printMatrix(int **m, int s);
void transposeMatrix(int **m, int **t, int s);

int main(void)
{
	int **matrix;
	int **matrixT;
	int size;



	srand(time(0));													//init the rand

	cout << "Enter the size of the matrix.\n:>";
	cin >> size;

	matrix = new int*[size];										//point at an array of pointers
	for (int j = 0; j < size; j++)
		matrix[j] = new int[size];									//point each of those pointers at an array

	matrixT = new int*[size];										//point at an array of pointers
	for (int j = 0; j < size; j++)
		matrixT[j] = new int[size];									//point each of those pointers at an array


	initMatrix(matrix, size);

	printMatrix(matrix, size);

	sortMatrix(matrix, size);

	cout << endl << endl << "The sorted matrix: " << endl << endl;

	printMatrix(matrix, size);

	cout << "\n\nThe transposed matrix:\n\n";

	transposeMatrix(matrix, matrixT, size);

	printMatrix(matrixT, size);

	PAUSE;
}//main

void initMatrix(int ** m, int s)
{
	bool repeat = false;
	int curent = 0;
	unsigned int test = 0;

	for (int i = 0; i < s; i++)
	{
		for (int j = 0; j < s; j++)
		{
			m[i][j] = 4001;										//init the array to a value outside of the range so the check for repeats latter is possible
		}
	}

	for (int i = 0; i < s; i++)									//loop through the matrix
	{
		for (int j = 0; j < s; j++)
		{
			repeat = false;
			while (true)										//loop till a new number is found
			{
				repeat = false;									//resesting this flag is important

				curent = rand() % 4001;							//make a random number from 0 to 4000
				for (int k = 0; k < s; k++)						//loop through the matix again and
				{
					for (int l = 0; l < s; l++)
					{
						if (m[k][l] == curent)					//if the random is already in the matix loop and make another
						{
							repeat = true;
						}
					}
				}
				if (!repeat) break;
			}//while there is a repeat
			m[i][j] = curent;									//put the rand into the matrix
		}//fill x
	}//fill y
}

void sortMatrix(int ** m, int s)
{
	for (int i = 0; i < (s*s); i++)								//sort the matrix
	{
		int temp = 0;
		for (int j = 0; j < s; j++)								//these two loops act as if it is a singal list
		{
			for (int k = 0; k < s; k++)
			{
				if (k >= s - 1 && j >= s - 1)					//if the last element of the matrix exit
				{
					break;
				}
				else if (k >= s - 1)							//else if the last element of the row then use the next rows first element
				{
					if (m[j][k] > m[j + 1][0])					//swap the vals if needed
					{
						temp = m[j][k];
						m[j][k] = m[j + 1][0];
						m[j + 1][0] = temp;
					}
				}
				else											//else just check the next element of the same row
				{
					if (m[j][k] > m[j][k + 1])					//swap the vals if needed
					{
						temp = m[j][k];
						m[j][k] = m[j][k + 1];
						m[j][k + 1] = temp;
					}
				}
			}
		}
	}
}

void printMatrix(int ** m, int s)
{
	for (int i = 0; i < s; i++)									//print the matrix
	{
		for (int j = 0; j < s; j++)
		{
			cout << setw(6) << m[i][j];
		}
		cout << endl;
	}
}

void transposeMatrix(int ** m, int ** t, int s)
{
	for (int i = 0; i < s; i++)
	{
		for (int j = 0; j < s; j++)
		{
			t[j][i] = m[i][j];
		}
	}
}
