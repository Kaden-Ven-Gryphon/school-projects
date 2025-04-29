#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

void initialize_1D(int array[], int elements);
void initialize_2D(int[][10], int rows, int cols);
void print_array_1D(int array_in[], int elements);
void print_array_2D(int array_in[][10], int rows, int cols);


void selection_sort_1D(int start[], int elements);

void selection_sort_2D(int start[][10], int x, int y);  // Modify for final exam.


int main() {

	int numbers[6][10] = { 0 };
	int values[50] = { 0 };
	int k = 0;
	int *ptr1 = nullptr;

	cout << "INITIALIZING THE 1D ARRAY" << endl << endl;
	initialize_1D(values, 50);

	cout << "ARRAY CONTENTS" << endl;
	print_array_1D(values, 50);

	cout << "INITIALIZING THE 2D ARRAY" << endl << endl;
	initialize_2D(numbers, 6, 10);

	cout << "ARRAY CONTENTS" << endl;
	print_array_2D(numbers, 6, 10);

	cout << "Sort the 1D array." << endl;

	selection_sort_1D(values, 50);

	cout << "SORTED 1D ARRAY CONTENTS" << endl;
	print_array_1D(values, 50);

	cout << "Sort the 2D array." << endl;



	selection_sort_2D(numbers, 6, 10);



	cout << "SORTED 2D ARRAY CONTENTS" << endl;
	print_array_2D(numbers, 6, 10);


	cout << endl << endl << endl;
	system("pause");

}

// Function List

void selection_sort_1D(int start[], int elements)
{
	int index = 0, smallest = 0, hold = 0, count = 0, location = 0;

	for (index = 0; index < elements - 1; index++) // Loop to control number of passes
	{
		smallest = index;
		// Find the index of the smallest element
		for (location = index + 1; location < elements; location++)
		{
			if (start[location] < start[smallest])
			{
				smallest = location;
			} // End If

		} // End Inner for loop

		hold = start[smallest];
		start[smallest] = start[index];
		start[index] = hold;
		count++; // Count number of swaps
	}
	cout << "There were " << count << " element exchanges during the sort. " << endl << endl;
	return;
}

// Modify the following code to sort a two dimensional array

void selection_sort_2D(int start[][10], int x, int y)
{
	int xpos = 0, ypos = 0, smallestx = 0, smallesty = 0, hold = 0, count = 0;
	for(int k = 0; k < x*y; k++)
	{
		smallestx = xpos;						//set the smallest cords to the first of the unsorted
		smallesty = ypos;
		int j = ypos;							//set the starting point of the curent row
		for (int i = xpos; i < x; i++)			//loop through row starting on the current row
		{
			
			while (j < y)						//until end of row
			{
				if (start[i][j] < start[smallestx][smallesty])		//check if this cord is smaller
				{
					smallestx = i;
					smallesty = j;
				}
				j++;							//next collum
			}
			j = 0;								//when moving on to the next row make sure to start at the begining that was causeing issues when i alway started a row at ypos
		}
		hold = start[smallestx][smallesty];							//swap values
		start[smallestx][smallesty] = start[xpos][ypos];
		start[xpos][ypos] = hold;

		ypos++;									//addvance the collum
		if (ypos > 9)							//if over the end of the collum
		{
			xpos++;								//go to next row
			ypos = 0;							//go back to first collom
		}

		count++; // Count number of swaps
	}
	cout << "There were " << count << " element exchanges during the sort. " << endl << endl;
	return;
}

void initialize_1D(int array[], int elements)
{
	unsigned int i, j;
	for (i = 0; i < elements; i++)
	{
		array[i] = rand() % 100;
	}
}


void initialize_2D(int array[][10], int rows, int cols)
{
	unsigned int i, j;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			array[i][j] = rand() % 100;
		}
	}
}
void print_array_1D(int array_in[], int elements)
{
	unsigned int i, j;

	for (j = 0; j < elements; j++)
	{
		cout << setw(3) << array_in[j];
		if ((j + 1) % 10 == 0)
			cout << endl;
	}
	cout << endl;

	return;
}

void print_array_2D(int array_in[][10], int rows, int cols)
{
	unsigned int i, j;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++) {
			cout << setw(3) << array_in[i][j];
		}
		cout << endl;
	}
	cout << endl;
	return;
}
