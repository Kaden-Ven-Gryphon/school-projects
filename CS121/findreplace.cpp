#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib> 

using namespace std;

int main(void)
{
	ifstream infile;											//decleare file varribles
	ofstream outfile;

	string intxt;

	int numWords = 0;											//word counter and pos varrible for parsing strings
	int pospointer = 0;
	
	infile.open("UAH_sample.txt");								//open and check the input file
	if (!infile)
	{
		cout << "error";
		return -1;
	}

	outfile.open("UAH_sample_fixed.txt");						//open and check the output file
	if (!outfile)
	{
		cout << "error";
		return -1;
	}

	while (!infile.eof())										//count the number of words
	{
		infile >> intxt;
		numWords++;
	}

	infile.clear();												//go back to begining of file
	infile.seekg(0, ios::beg);

	while (!infile.eof())										//replace UAHuntsville
	{
		getline(infile, intxt);									//getline
		pospointer = 0;
		do
		{	
			pospointer = intxt.find("UAHuntsville");			//make sure it is not the end of the line
			if (pospointer != string::npos)
			{
				intxt.replace(pospointer, 12, "UAH");			//replace
			}

		} while (pospointer != string::npos);					//if it is the end of the line move to the next line
		outfile << intxt << endl;								//send the fixed line too the new file
	}

	infile.close();												//close files
	outfile.close();

	cout << "Number of words:> " << numWords << endl;			//cout the number of words
	system("pause");

	return 0;
}
