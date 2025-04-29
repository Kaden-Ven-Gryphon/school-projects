//
//Homework 2 Problem 3
//
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#define WIDTH 15

using namespace std;

int main(void)
{
	int hours = 0;
	int mins = 0;
	int secs = 0;
	char hemispere[5];
	char remover;
	bool loop = true;

	cout << "Enter a time in HH:MM:SS PM/AM format (enter -1 to quit) :> "<<endl<<endl;
	cout << setw(5) << "AM/PM" << setw(WIDTH) << "24-Hour" << endl;

	while (loop)
	{
		cin >> hours;
		if (hours < 0) loop = false;
		else 
		{
			cin >> remover >> mins >> remover >> secs >> hemispere;
			switch (hemispere[0])
			{
			case 'p':
				hours += 12;
				if (hours >= 24) hours = 0;
				break;
			case 'a':
			default:
				break;
			}
			cout << setfill(' ') << setw(WIDTH) << ' ' << setfill('0') << setw(2) << hours << ":" << setw(2) << mins << ":" << setw(2) << secs << endl;
			cout << setfill('-') << setw(WIDTH+9) << '-' << setfill(' ') << endl;
		}
	}
	system("pause");
}
