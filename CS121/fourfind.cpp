#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <math.h>

#define WIDTH 10
#define NUM_WORDS 4030

using namespace std;

int searchDictionary(string in);

string words[NUM_WORDS];									//array of all word global so that the function can use it

int main(void)
{
	
	string puzzleLine;										//strings for manipulation the puzzle
	string substr;
	string lineR;

	unsigned int count = 0;
	ifstream infile;										// Declare file pointer for input
	
	int length = 0;


	infile.open("dictionary_four_letter_words.txt");		//open dictionary
	if (!infile)
	{
		cout << "ERROR\n\n";
	}

	cout << endl << endl;

	while (!infile.eof())									//load dictionary into array
	{
		infile >> words[count];
		//cout << words[count] << endl;
		count++;
	}
	infile.close();								//*****************CLOSE DICTIONARY*****************

	infile.open("word_puzzle.txt");							//open puzzle
	if (!infile)
	{
		cout << "ERROR\n\n";
	}


	cout << endl << endl;

	count = 0;
	while (!infile.eof())									//loop through the puzzle
	{
		getline(infile, puzzleLine);
		cout << endl << "Line: " << puzzleLine << endl;
		lineR = "";											//reset the reverse line
		for (int i = 0; i < puzzleLine.length(); i++)		//reverse the line to look for backwards words
		{
			int temp = (puzzleLine.length() - 1) - i;
			lineR += puzzleLine[temp];
		}
		for (int i = 0; i < puzzleLine.length() - 3; i++)	//loop through and pull out 4 letters at a time
		{
			substr = puzzleLine.substr(i, 4);
			if (1==searchDictionary(substr))				//search the dictionary for word
			{
				cout << substr << endl;						//print if exists
				count++;
			}
			substr = lineR.substr(i, 4);					//search and print reverse words
			if (searchDictionary(substr))
			{
				count++;
				cout << substr << endl;
			}
		}//line search loop
	}//puzzle loop
	infile.close();											//close file


	cout << endl << "number of words found: " << count << endl;	//print number of words
	system("pause");
}//main

int searchDictionary(string in)
{
	int retval = 0;
	for (int i = 0; i < NUM_WORDS; i++)				//compare in word with dictionary
	{
		if (in == words[i])
		{
			retval = 1;
		}
	}
	
	return retval;
}
