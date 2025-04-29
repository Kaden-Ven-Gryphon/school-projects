#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#define PAUSE cout <<endl; system("Pause"); cout << endl			//system pause

using namespace std;

int main(void)
{
	string in;														//strings
	string sub;

	double nums[100];												//to store all the numbers
	int arrayPos = 0;												//number of numbers gotten

	double total = 0;												//total

	int pos = 0;													//varius counters and pos trackers
	int length = 0;
	int l = 0;
	int r = 0;
	int i = 1;

	bool loop = true;												//loop conroler

	cout << "Enter a question for numbers to be added [must have a decimal point and a persision of at least 1]\n:>";
	getline(cin, in);												//get input line

	length = in.length();
	pos = in.find_last_of('.');
	if ( pos == length - 1)											//get rid of the last period if there is one
	{
		in.erase(pos, 1);
	}

	while (in.find('.') != string::npos)							//while there are still decimals in the line look for numbers
	{
		pos = in.find('.');											//find the '.'

		while (loop)
		{								
			if (in[pos + i] != '0' && in[pos + i] != '1' && in[pos + i] != '2' && in[pos + i] != '3' && in[pos + i] != '4' && in[pos + i] != '5' && in[pos + i] != '6' && in[pos + i] != '7' && in[pos + i] != '8' && in[pos + i] != '9')
			{
				l = pos + i;										//find the last char of the number
				loop = false;
			}
			else i++;			
		}
		loop = true;
		i = 1;

		while (loop)
		{
			if (in[pos - i] != '0' && in[pos - i] != '1' && in[pos - i] != '2' && in[pos - i] != '3' && in[pos - i] != '4' && in[pos - i] != '5' && in[pos - i] != '6' && in[pos - i] != '7' && in[pos - i] != '8' && in[pos - i] != '9')
			{
				r = pos - i;										//find the first char of the number
				loop = false;
			}
			else i++;
		}
		loop = true;
		i = 1;

		sub = in.substr(r, (l - r));								//pull out just the number
		nums[arrayPos++] = stod(sub);								//convert it to a double and add it to the array

		in.erase(0, l);												//errase upto and includeing the number it just pulled out
	}

	for (int z = 0; z < arrayPos; z++)								//add them all up to find the total
	{
		total += nums[z];
	}

	cout << "\nYour total is: " << total << endl;					//print the total

	PAUSE;

	return 0;
}
