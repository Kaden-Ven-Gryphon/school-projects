#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib> 

using namespace std;

int main(void)
{
	string line;
	string buf[50];
	string space = " ";
	char endMark = '.';
	int length = 0;
	int pos = 0;
	cout << "Enter a sentence to be reversed:> ";
	getline(cin, line);
	do
	{
		pos = line.find(" ");
		if (pos != string::npos)
		{
			buf[length++] = line.substr(0, pos);
			line.erase(0, pos+1);
			pos = 0;
		}

		
	} while (pos != string::npos);
	buf[length++] = line;
	line = "";

	pos = buf[length - 1].find(".");
	if (pos != string::npos)
	{
		buf[length - 1].erase(pos,1);
		endMark = '.';
	}

	pos = buf[length - 1].find("?");
	if (pos != string::npos)
	{
		buf[length - 1].erase(pos, 1);
		endMark = '?';
	}

	pos = buf[length - 1].find("!");
	if (pos != string::npos)
	{
		buf[length - 1].erase(pos, 1);
		endMark = '!';
	}

	for (length >= 0; length--;)
	{
		line = line + buf[length];
		line.append(space);
	}


	line[line.length() - 1] = endMark;
	
	

	cout << endl << endl << line << endl;
	system("pause");

}
