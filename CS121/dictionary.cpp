#include <iostream>
#include <fstream>
#include <string>

int main(void)
{
	std::ifstream infp;
	std::ofstream outfp;
	std::string dictionary[4100];
	int count = 0;
	bool loop = true;
	std::string fileName;
	char input;
	int test = 0;

	int Scount = 0;
	std::string *results;

	infp.open("dictionary.txt");
	if (!infp)
	{
		std::cout << "ERROR FILE FAIL\n\n";
		system("pause");
	}

	while (!infp.eof())
	{

		infp >> dictionary[count++];
	}
	infp.close();

	while (loop)
	{
		std::cout << "enter a character (lowercase) to search or Q (upercase) to quit\n:> ";
		std::cin >> input;
		if (input == 'Q')
		{
			loop = false;
		}
		else
		{
			for (int i = 0; i < count; i++)
			{
				if (dictionary[i][0] == input)
				{
					//std::cout << dictionary[i];
					Scount++;
				}
			}

			results = new std::string[Scount];
			Scount = 0;

			for (int i = 0; i < count; i++)
			{
				if (dictionary[i][0] == input)
				{
					results[Scount++]=dictionary[i];
				}
			}

			std::cout << "Enter file name.\n:> ";
			std::cin >> fileName;
			fileName += ".txt";
			outfp.open(fileName);
			if (!outfp)
			{
				std::cout << "ERROR FILE FAIL\n\n";
				system("pause");
			}

			int wordCount = 0;
			for (int i = 0; i < Scount; i++)
			{
				if (wordCount == 10)
				{
					outfp << "\n";
					std::cout << "\n";
					wordCount = 0;
				}
				outfp << results[i] << " ";
				std::cout << results[i] << " ";
				wordCount++;
			}
			std::cout << std::endl;

			outfp.close();
			//delete results;
		}
	}

}
