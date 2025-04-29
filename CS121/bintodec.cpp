#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <math.h>

#define WIDTH 10

using namespace std;

int main(void)
{
	string in;
	int bi;
	int dec = 0;
	bool loop = true;

	cout << "Enter a number in binary (up to 16bit) or q to quit:> \n\n ";
	cout << "Binary" << setw(WIDTH) << "Decimal" << endl;
	while (loop)
	{
		cin >> in;

		if (in == "q")
		{
			loop = false;
		}
		else
		{
			for (int i = 0; i < in.length(); i++)
			{
				int power = pow(2, i);
				int binary = (in[in.length() - 1 - i] - 48);
				dec += power * binary;
				//dec += (pow(2, i) * (in[in.length()-1-i] - 48));
			}
			cout << setw(WIDTH) << dec << endl;
			dec = 0;
		}
	}
	system("pause");
	return 0;
}
