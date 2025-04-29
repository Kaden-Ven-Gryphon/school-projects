//
// Homework 2 Problem 5
// The program requires the file dictionary_four_letter_words.txt in the Temp folder 
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#define NUM_WORDS 4030

using namespace std;

int main(void)
{
    string words[NUM_WORDS];								//array of all word
	string inword;											//input word

	bool spell = false;						
	bool loop = true;
    unsigned int count = 0;
    ifstream infile;										// Declare file pointer for input

 
    //
    // Open the dictionary
    //


	 infile.open("dictionary_four_letter_words.txt");
	 if (!infile)
	 {
		 cout << "ERROR\n\n";
	 }


     cout << endl << endl;

     //
     // Read the file one word at a time and write each word to the screen.
	 //
  
	 while (!infile.eof())									//load dictionary into array
	 {
		 infile >> words[count];
		 cout << words[count] << endl;
		 count++;
	 }

    cout << endl << endl;

    cout << "There are " << count << " words in the dictionary." << endl;
    cout << endl << endl;
	infile.close();

	//
	//Start of the main loop. takes a input compares it to the values of the array and gives an output
	//

	while (loop)
	{
		cout << "\nEnter a four letter word to be spell checked (enter a longer or shorter word to quit) :> ";
		cin >> inword;
		spell = false;


		if (inword.length() == 4)							// check to see if quit
		{
			for (int i = 0; i < NUM_WORDS; i++)				//compare in word with dictionary
			{
				if (inword == words[i])
				{
					spell = true;
				}
			}
			if (spell)										//output results
			{
				cout << "\nWord is spelt corectly\n";
			}
			else cout << "\nWord is spelt incorectly\n";
		}//if not quit
		else loop = false;
	}//while
	

    cout << endl << "good bye..." << endl;
	system("pause");
	return 0;
}
