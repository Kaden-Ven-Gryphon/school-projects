#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>

#define PAUSE system("pause")

using namespace std;

bool search(string s, string d[], int n);

int main(void)
{
	ifstream ifp;														//in and out files
	ofstream ofp;

	string *dictionary;													//pointer to the start of the dictionary
	int numWords = 0;													//number of words in the dictionary

	char puzzle[10][10];												//the 10 by 10 puzzle
	string curentLine;													//the curent line the search is testing
	
	int h = 0;															//number of horizantal words
	int v = 0;															//number of vertical words

	ifp.open("dictionary_four_letter_words.txt");						//open the dictionary
	if (!ifp)															//check to make sure file opens
	{
		cout << "ERROR 1: file failed to open\n";
		PAUSE;
		return -1;
	}

	while (!ifp.eof())													//loop through the file to findthe number of words
	{
		ifp >> curentLine;
		numWords++;
	}
	ifp.clear();														//rest the file to be read again
	ifp.seekg(0, ios::beg);

	dictionary = new string[numWords];									//init the array with the number of words found in the file

	numWords = 0;														//reset index to 0
	while (!ifp.eof())													//read through the file again to fill the array
	{
		ifp >> dictionary[numWords++];
	}
	ifp.clear();														//clear the eof and close the file
	ifp.close();

	ifp.open("word_puzzle_2D_problem_six.txt");							//open the puzzle
	if (!ifp)															//check for file fail
	{
		cout << "ERROR 1: file failed to open\n";
		PAUSE;
		return -1;
	}

	for (int i = 0; i < 10; i++)										//loop through to fill the puzzle array with the characters
	{
		for (int j = 0; j < 10; j++)
		{
			ifp.get(puzzle[j][i]);
		}
		ifp.get();														//clear the newlines
	}
	ifp.close();														//close the file

	for (int i = 0; i < 10; i++)										//loop through to print the puzzle from the array
	{
		for (int j = 0; j < 10; j++)
		{
			cout << puzzle[j][i];
		}
		cout << endl;
	}

	PAUSE;

	for (int i = 0; i < 10; i++)										//loop through the 10 rows or collums
	{
		for (int j = 0; j < 7; j++)										// loop through the 6 sets of 4 char
		{
			curentLine = "";											//reset string
			for (int k = 0; k < 4; k++)									//add the 4 letters to the string
			{
				curentLine += puzzle[j + k][i];							//this the horizontal forward
			}
			if (search(curentLine, dictionary, numWords))				//check the dictionary
			{
				cout << curentLine << " HF" << endl;					//print out and mark HF (horizantal forward) and add to h
				h++;
			}

			curentLine = "";											//reset string
			for (int k = 3; k >= 0; k--)
			{
				curentLine += puzzle[j + k][i];							//horizantal backwards
			}
			if (search(curentLine, dictionary, numWords))
			{
				cout << curentLine << " HB" << endl;
				h++;
			}

			curentLine = "";											//reset the string
			for (int k = 0; k < 4; k++)
			{
				curentLine += puzzle[i][j + k];							//vertical down
			}
			if (search(curentLine, dictionary, numWords))
			{
				cout << curentLine << " VD" << endl;
				v++;
			}

			curentLine = "";											//reset the string
			for (int k = 3; k >= 0; k--)
			{
				curentLine += puzzle[i][j + k];							//vertical up
			}
			if (search(curentLine, dictionary, numWords))
			{
				cout << curentLine << " VU" << endl;
				v++;
			}
		}
	}


	cout << "\n\nThe total number of horizantal words: " << h << "\nThe total number of vertical words: " << v << endl << endl;
	PAUSE;
	return 0;
}

bool search(string s, string d[], int n)									//binary search function
{
	int start = 0;															//array markers
	int end = n;
	int i = 0;
	while (true)
	{
		if (start + 1 == end)												//if it is down too two check both
		{
			if (s == d[end])
			{
				return true;
			}
			else if (s == d[start])
			{
				return true;
			}
			else															//if neither equal the word return false
			{
				return false;
			}
		}
		i = ((end - start) / 2) + start;									//find the mid point
		if (s == d[i])														//if it finds the word return true else move the start or end
		{
			return true;
		}
		else if (s > d[i])
		{
			start = i;
		}
		else if (s < d[i])
		{
			end = i;
		}
	}
	return false;
}
