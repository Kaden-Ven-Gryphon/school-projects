#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <time.h>

#define DEBUG false													//swaped in a define for all hte debugs to change them together

#define PAUSE cout <<endl; system("Pause"); cout << endl			//system pause
#define Q_TO_QUITE while(getchar() != 'q')
#define PRINT print_slots(slots, pos, block, testCase, nextBlockIndex, cemote, compares)	//things i got tired of typing
#define GRAB_NEXT block = get_block_testcase(testCase, nextBlockIndex++)
#define PUT_BLOCK put_block(block, pos, slots); block = ' '
#define COMPARE comp = robot_ltoreq_slot(block, slots[pos]); compares++

using namespace std;

unsigned int goToPos(unsigned int pos, int target, char slots[], char block, int test, int index, char emote, int count);
int shiftRight(unsigned int pos, int ending, char slots[], char block, int test, int index, char emote, int count);
int shiftLeft(unsigned int pos, int begining, char slots[], char block, int test, int index, char emote, int count);

//robot libary functions
void print_slots(char slots[], unsigned int pos, char block, int testCase, int nextBlockIndex, char emote, int count);
unsigned int put_block(char block, unsigned int position, char array[]);
unsigned int shift_right(unsigned int position);
unsigned int shift_left(unsigned int position);
bool robot_ltoreq_slot(char robot, char in_slot);
char switch_blocks(char robot, unsigned int position, char array[]);
char get_block_testcase(unsigned int testcase, unsigned int index);


int main(void)
{
	string testcases[5] = { "AXFIUTRPQVWSEYJINYTB",
		"ABFGHIJKMOPRSTUVWXYZ",
		"ZYXWVUTSRPOKJIIHGFBA",
		"AAAAAYYYYYQQQQQXXXXX",
		"XXXAAAZZZAAYYVVVVQQQ" };

	char emotes[] = { ' ', '>', '<', '!' };
	char cemote = ' ';

	unsigned int pos = 0;
	char block = ' ';
	char slots[20];
	int compares = 0;

	int begining = 0;
	int ending = 0;
	int testCase = 0;
	int nextBlockIndex = 0;
	int searchPoint = 0;

	

	for (int i = 0; i < 20; i++)																		//inints the blank array
	{
		slots[i] = ' ';
	}

	cout << "1. " << testcases[0] << endl;																//ask which test to do and where to start
	cout << "2. " << testcases[1] << endl;
	cout << "3. " << testcases[2] << endl;
	cout << "4. " << testcases[3] << endl;
	cout << "5. " << testcases[4] << endl;

	cout << "sellect a test case.\n:> ";
	cin >> testCase;
	cout << "sellect a starting pos.\n:> ";
	cin >> begining; begining--;

	ending = begining;

	PRINT;																								//sets up the initial 2 blocks
	GRAB_NEXT;
	pos = goToPos(pos, begining, slots, block, testCase, nextBlockIndex, cemote, compares);
	PUT_BLOCK;
	pos = goToPos(pos, 0, slots, block, testCase, nextBlockIndex, cemote, compares);
	GRAB_NEXT;
	pos = goToPos(pos, begining, slots, block, testCase, nextBlockIndex, cemote, compares);
	if (robot_ltoreq_slot(block, slots[pos]))
	{
		compares++;
		cemote = emotes[2];
		pos = goToPos(pos, --begining, slots, block, testCase, nextBlockIndex, cemote, compares);
		cemote = emotes[0];
		PUT_BLOCK;
	}
	else
	{
		compares++;
		cemote = emotes[1];
		pos = goToPos(pos, ++ending, slots, block, testCase, nextBlockIndex, cemote, compares);
		cemote = emotes[0];
		PUT_BLOCK;
	}
	


	while (nextBlockIndex < 20)																				//loops through the rest of the blocks
	{
		int start = begining;
		int end = ending;
		bool comp = false;

		pos = goToPos(pos, 0, slots, block, testCase, nextBlockIndex, cemote, compares);
		GRAB_NEXT;

		while (true)
		{
			if (start + 1 == end)
			{
				pos = goToPos(pos, end, slots, block, testCase, nextBlockIndex, cemote, compares);
				COMPARE;
				if (!comp)																						//if gerater than end put in end ++
				{
					cemote = emotes[1];
					if (ending < 19)																			//if there is space on the right put h
					{
						pos = goToPos(pos, end+1, slots, block, testCase, nextBlockIndex, cemote, compares); ending++;
						cemote = emotes[0];
						pos = shiftRight(pos, ending, slots, block, testCase, nextBlockIndex, cemote, compares);
						block = ' ';
						break;
					}
					else																						//else put it on the left
					{
						pos = goToPos(pos, end - 1, slots, block, testCase, nextBlockIndex, cemote, compares); begining--;
						cemote = emotes[0];
						pos = shiftLeft(pos, begining, slots, block, testCase, nextBlockIndex, cemote, compares);
						block = ' ';
						break;
					}
				}//if greater than bound
				else																							//if less than end check if greater than start
				{
					cemote = emotes[2];
					pos = goToPos(pos, start, slots, block, testCase, nextBlockIndex, cemote, compares);
					COMPARE;
					if (!comp)																					//greater than start
					{
						cemote = emotes[1];
						if (ending < 19)																		//put on right
						{
							pos = goToPos(pos, end, slots, block, testCase, nextBlockIndex, cemote, compares); ending++;
							cemote = emotes[0];
							pos = shiftRight(pos, ending, slots, block, testCase, nextBlockIndex, cemote, compares);
							block = ' ';
							break;
						}
						else																					//put on left
						{
							pos = goToPos(pos, start, slots, block, testCase, nextBlockIndex, cemote, compares); begining--;
							cemote = emotes[0];
							pos = shiftLeft(pos, begining, slots, block, testCase, nextBlockIndex, cemote, compares);
							block = ' ';
							break;
						}
					}//if greater than bound
					else
					{
						cemote = emotes[2];
						if (begining > 0)
						{
							pos = goToPos(pos, start-1, slots, block, testCase, nextBlockIndex, cemote, compares); begining--;
							cemote = emotes[0];
							pos = shiftLeft(pos, begining, slots, block, testCase, nextBlockIndex, cemote, compares);
							block = ' ';
							break;
						}
						else
						{
							pos = goToPos(pos, start, slots, block, testCase, nextBlockIndex, cemote, compares); ending++;
							cemote = emotes[0];
							pos = shiftRight(pos, ending, slots, block, testCase, nextBlockIndex, cemote, compares);
							block = ' ';
							break;
						}
					}
				}//if less than bounds
			}//if bounds located

          	searchPoint = ((end - start) / 2) + start;
			pos = goToPos(pos, searchPoint, slots, block, testCase, nextBlockIndex, cemote, compares);
			cemote = emotes[0];
			COMPARE;
			if (comp)
			{
				cemote = emotes[2];
				end = pos;
			}
			else
			{
				cemote = emotes[1];
				start = pos;
			}

			//constric bounds


		}//while spot not found
	}//while blocks left
	
	PRINT;
	cout << "press q to quit:> ";
	Q_TO_QUITE;

	return 0;
}

unsigned int goToPos(unsigned int pos, int target, char slots[], char block, int test, int index, char emote, int count)					//goes to a postion using shitf left and right
{
	while (true)																//loops moves left and right while printing to screen to animate
	{
		int distance = target - pos;
		if (distance == 0)
		{
			print_slots(slots, pos, block, test, index, emote, count);
			emote = ' ';
			return pos;
		}
		else if (distance > 0)
		{
			print_slots(slots, pos, block, test, index, emote, count);
			emote = ' ';
			pos = shift_right(pos);
		}
		else if (distance < 0)
		{
			print_slots(slots, pos, block, test, index, emote, count);
			emote = ' ';
			pos = shift_left(pos);	
		}
	}
}

int shiftRight(unsigned int pos, int ending, char slots[], char block, int test, int index, char emote, int count)			//shift all hte blocks to the right or left to incert a new block
    {
	while (pos < ending)
	{
		block = switch_blocks(block, pos, slots);
		pos = goToPos(pos, pos + 1, slots, block, test, index, emote, count);
	}
	PUT_BLOCK;
	return pos;
}

int shiftLeft(unsigned int pos, int begining, char slots[], char block, int test, int index, char emote, int count)
{
	while (pos > begining)
	{
		block = switch_blocks(block, pos, slots);
		pos = goToPos(pos, pos - 1, slots, block, test, index, emote, count);
	}
	PUT_BLOCK;
	return pos;
}


// ------------------------------------------------------------------------ //
//																			//
//																			//
//				Functions for robot operations								//
//																			//
//																			//
// ------------------------------------------------------------------------ //

//
// Function get_block
// Reads in a single character value from the keyboard 
// This is the input from the chute
// Returns: a single character in uppercase
//
// Example function call: 	block = get_block();

char get_block(void)
{
	char block;
	cout << "Enter one block: ";
	cin >> block;
	return toupper(block);
}
//
// Function print_slots
// Prints the contents of the slots array in a well formatted form.
// Input: Array of slots
// Returns: Nothing (void)
//
// Example function call: print_slots(slot_array);

void print_slots(char slots[], unsigned int pos, char block, int testCase, int nextBlockIndex, char emote, int count)
{
	//ADDED BIT TO SHOW WHERE ROBOT IS AND WHAT BLOCK IT IS HOLDING
	system("cls");
	cout << setw(3) << get_block_testcase(testCase, nextBlockIndex)<< " is next block\n\n";
	cout << setw(3 * pos + 3) << emote << endl;
	cout << setw(3 * pos + 3) << "^" <<endl;
	cout << setw(3 * pos + 3) << block << endl;
	//END OF ADDED BIT
	unsigned int j = 1;
	for (j = 1; j <= 20; j++)
	{
		cout << setw(3) << j;
	}
	cout << endl;
	for (j = 0; j < 20; j++)
	{
		cout << setw(3) << slots[j];
	}
	cout << endl;
	cout << "Number of compares: " << count << endl;
	PAUSE;
}

// Function put_block
// This function stores a character into the character array representing the slots
//
// Inputs: 
// block - type char - The character to be inserted into a slot
// position - type unsigned int - index of the slot where the block will go
// array - type char - array of slots containing the blocks
//
// Returns:
// position - type unsigned int - the index of the slot where the block was placed
//
// Example function call: 	put_block(block, position, slots);

unsigned int put_block(char block, unsigned int position, char array[])
{
	bool debug = true;
	array[position] = block;
	if (DEBUG)
		cout << "Block " << block << " inserted into slot " << position << endl;
	return position;
}

// Function remove_block
// This function removes a block from the slot array
// The slot where the block is removed is then set to a space
//
// Inputs: 
// position - type unsigned int - index of the slot where block is located
// array - type char - array of slots containing the blocks
//
// Returns:
// block - type char - the block removed from the slot
//
// Example function call: 	remove_block(position, slots);

unsigned int remove_block(unsigned int position, char array[])
{
	bool debug = true;
	char block = ' ';
	block = array[position];
	array[position] = ' ';  // Reset slot to blank after block removed
	if (DEBUG)
		cout << "Block " << block << " removed from slot " << position + 1 << endl;
	return block;
}


// Function shift_right
// This function increments the index simulating a movement of the robot 
// to the next higher slot (index) of the array
// 
// Inputs:
// position - type unsigned int - current slot position
//
// Returns:
// position - type unsigned int - The updated position which is input position + 1
//
// Example function call:  position = shift_right(position)
//

unsigned int shift_right(unsigned int position)
{
	bool debug = true;
	position++;
	if (DEBUG)
		cout << "Position right shifted to " << position << endl;
	return position;
}

// Function shift_left
// This function decrements the index simulating a movement of the robot 
// to the next lower slot (index) of the array
// 
// Inputs:
// position - type unsigned int - current slot position
//
// Returns:
// position - type unsigned int - The updated position which is input position - 1
//
// Example function call: position = shift_left(position)
//

unsigned int shift_left(unsigned int position)
{
	bool debug = true;
	position--;
	if (DEBUG)
		cout << "Position left shifted to " << position << endl;
	return position;
}

// Function robot_ltoreq_slot
// This function compares the value of the block held by the robot 
// with the value of the block in a slot 
// 
// Inputs:
// robot - type char - value of block held by robot
// in_slot - type char - value of block in the slot
//
// Returns:
// true or false
// TRUE if block held by robot is LESS than or equal to the block in slot
// FALSE if block held by robot is GREATER than block in slot
//
// Example function call: if ( compare_blocks(robot_block, slot_block) )
//
bool robot_ltoreq_slot(char robot, char in_slot)
{
	bool debug = true;
	if (DEBUG)
		cout << endl << "Comparing robot block " << robot << " with block in slot " << in_slot << endl;
	if (robot <= in_slot)
	{
		if (DEBUG)
			cout << "Returning true. Robot block LESS than or EQUAL to block in slot. " << endl;
		return true;
	}
	else
	{
		if (DEBUG)
			cout << "Returning false. Robot block GREATER than block in slot. " << endl;
		return false;
	}
}
// Function switch_blocks
// This function switches the block held by the robot with a block in a slot. 
// After the switch the robot is holding the block removed from the slot.
// 
// Inputs: 
// robot - type char - The block to be inserted into a slot
// position - type unsigned int - index of the slot where the block will go
// array - type char - array of slots containing the blocks
//
// Returns:
// robot - type char. The value of the block removed from the slot.
//
// Example function call: block = switch_blocks(block,  position, array);
//

char switch_blocks(char robot, unsigned int position, char array[])
{
	char temp_hold;
	bool debug = true;
	if (DEBUG)
		cout << "Switching blocks " << robot << " with " << array[position] << endl;
	temp_hold = robot;
	robot = array[position];
	array[position] = temp_hold;
	return robot;
}
// Function test_empty
// This function tests the array to determine if a slot is empty (NULL) 
// or if the slot contains a blank. The slot array must be intialized to 
// all NULL or all blanks (spaces) before any blocks are added.
// 
// Inputs:
// position - type unsigned int - index of slot to be tested
//
// Returns:
// true or false as value o function
// TRUE if slot is empty
// FALSE if the slot contains a block
//
// Example function call: if ( test_empty(index, array) )
//
bool test_empty(unsigned int position, char array[])
{
	char blank = ' '; // Blank space
	bool debug = true;
	if (array[position] == NULL || array[position] == blank)
	{
		if (DEBUG)
			cout << "Slot " << position << " empty. " << endl;
		return true;
	}
	else
	{
		if (DEBUG)
			cout << "Slot " << position << " contains a block " << endl;
		return false;
	}

}

// ------------------------------------------------------------------------ //
//																			//
//																			//
//				Functions for robot test cases								//
//																			//
//																			//
// ------------------------------------------------------------------------ //

//
// Function get_block_testcase
// Returns blocks for a given test case one at a time
// This function can be used for all five test cases.
//
// Input: Unsigned int case designating test case 1-5.
// Input: Index of letter desired 0 - 19.
// Returns: a single uppercase character
//
// Example function call: 	block = get_block_testcase(1, 0);
// Returns first letter of test case 1.

char get_block_testcase(unsigned int testcase, unsigned int index)
{
	string testcases[5] = { "AXFIUTRPQVWSEYJINYTB ",					//HAD TO ADD A SPACE TO THE END OF EACH LIST TO MAKE THE "next block is : " work on print_slots after the robot grabs tha last block
		"ABFGHIJKMOPRSTUVWXYZ ",
		"ZYXWVUTSRPOKJIIHGFBA ",
		"AAAAAYYYYYQQQQQXXXXX ",
		"XXXAAAZZZAAYYVVVVQQQ " };

	return testcases[testcase - 1].at(index);
}


//
// Function get_block_testone
// Returns blocks for test case one one at a time
// Returns: a single character
//
// Example function call: 	block = get_block_testone();

char get_block_testone(void)
{
	static unsigned int index = 0;
	char test_case_one[21] = "AXFIUTRPQVWSEYJINYTB";
	return test_case_one[index++];
}



//
// Function get_block_testtwo
// Returns blocks for test case two one at a time
// Returns: a single character
//
// Example function call: 	block = get_block_testtwo();

char get_block_testtwo(void)
{
	static unsigned int index = 0;
	char test_case_two[21] = "ABFGHIJKMOPRSTUVWXYZ";
	return test_case_two[index++];
}

//
// Function get_block_testthree
// Returns blocks for test case three one at a time
// Returns: a single character
//
// Example function call: 	block = get_block_testthree();

char get_block_testthree(void)
{
	static unsigned int index = 0;
	char test_case_three[21] = "ZYXWVUTSRPOKJIIHGFBA";
	return test_case_three[index++];
}
//
// Function get_block_testfour
// Returns blocks for test case four one at a time
// Returns: a single character
//
// Example function call: 	block = get_block_testfour();

char get_block_testfour(void)
{
	static unsigned int index = 0;
	char test_case_four[21] = "AAAAAYYYYYQQQQQXXXXX";
	return test_case_four[index++];
}

//
// Function get_block_testfive
// Returns blocks for test case five one at a time
// Returns: a single character
//
// Example function call: 	block = get_block_testfive();

char get_block_testfive(void)
{
	static unsigned int index = 0;
	char test_case_five[21] = "XXXAAAZZZAAYYVVVVQQQ";
	return test_case_five[index++];
}
