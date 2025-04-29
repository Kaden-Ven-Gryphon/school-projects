//
//Homework 4 Problems 5
//
//ERRORS:
//-1: a file was failed to open
//-2: while looking for the header of an element the search turned up empty
//


#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>

#define ERROR_TXT_TEMP "ERROR %d: %s"								//error messages
#define ERROR_FILE_NOT_FOUND -1
#define ERROR_TXT_1 "THE FILE FAILED TO OPEN"
#define ERROR_POS_OF_ELEMENT_NOT_FOUND -2
#define ERROR_TXT_2 "THE FILE DOES NOT CONTAIN THE REQUIRED ELEMENT OR IS NOT FORMATED PROPULY"


#define PAUSE cout <<endl; system("Pause"); cout << endl			//system pause


#define STATION_NAME_WIDTH 120										//define for table formating
#define DATE_WIDTH 11
#define PRCP_WIDTH 6
#define STATION_NAME "STATION NAME"
#define DATE_NAME "DATE"
#define PRCP_NAME "PRCP"
#define TMAX_NAME "TMAX"
#define TMIN_NAME "TMIN"
#define TABLE_LINE "%*s %*s %*s %*s %*s\n"
#define TABLE_LINE_DIV "-"
#define TABLE_LINE_DATA "%*s %5d %02d %02d %*.2f %*.2f %*.2f\n"

#define Q_OPEN "What would you like to do?\n"				//defines for all text.  so that wording can easily be changed
#define OPTION_A "[A] find total PRCP for a single date\n"
#define OPTION_B "[B] find total PRCP for a range of dates\n"
#define OPTION_C "[C] find total PRCP for a station group\n"
#define OPTION_D "[D] find TEMP extremes and averages for a single station\n"
#define OPTION_E "[E] find TEMP extremes and averages for a single station over a range of dates\n"
#define OPTION_Q "[Q] to quit\n"
#define ENTER_INPUT ":> "
#define FAREWELL "Good Bye...\n"

#define Q_SEE_TABLE "Would you like to see the relevant data?\n"
#define Q_EXPORT "Would you like the results Exported?\n"
#define Q_FILE_NAME "Enter a name for the file [xxxx.txt].\n"

#define A_QUESTION "Enter the date that you would like to see the total PRCP for [mm dd yyyy].\n"
#define B_QUESTION "Enter the start date and end date you would like to see the total PRCP of [mm dd yyyy mm dd yyyy].\n"
#define A_TXT_TOTAL "The total PRCP of "
#define A_TXT_AVERAGE "The average PRCP of "
#define A_FILE_OUT "The total PRCP of %d %d %d: %f\nThe average PRCP of %d %d %d: %f\n"
#define B_FILE_OUT "The total PRCP of %d %d %d - %d %d %d: %f\nThe average PRCP of %d %d %d - %d %d %d: %f\n"

#define C_QUESTION "Enter the name of the group of stations that you would like to see the total PRCP of [no spaces].\n"
#define C_FILE_OUT "The total PRCP of group %s: %f\nThe average PRCP of %s: %f\n"

#define D_QUESTION "Enter the exact station name you would like to see the temperture data for.\n"
#define D_EXSTREAM_HIGH "The highest temperture for "
#define D_EXTREAM_LOW "The lowest temperture for "
#define D_AVERAGE_HIGH "The average high for "
#define D_AVERAGE_LOW "The average low for "
#define D_EXSTREAM_HIGH_FILE "The highest temperture for %s: %f\n"
#define D_EXTREAM_LOW_FILE "The lowest temperture for %s: %f\n"
#define D_AVERAGE_HIGH_FILE "The average high for %s: %f\n"
#define D_AVERAGE_LOW_FILE "The average low for %s: %f\n"

#define E_QUESTION "Enter the date range [mm dd yyyy mm dd yyyy].\n"
#define E_EXSTREAM_HIGH_FILE "The highest temperture for %s %d %d %d - %d %d %d: %f\n"
#define E_EXTREAM_LOW_FILE "The lowest temperture for %s %d %d %d - %d %d %d: %f\n"
#define E_AVERAGE_HIGH_FILE "The average high for %s %d %d %d - %d %d %d: %f\n"
#define E_AVERAGE_LOW_FILE "The average low for %s %d %d %d - %d %d %d: %f\n"

using namespace std;

typedef struct {
	int year;
	int month;
	int day;
}date_t;

typedef struct {													//the data from the file broken up into indevidual variables
	string name;
	date_t date;
	float prcp;
	float tempMax;
	float tempMin;
	int alpha;
}dataEntry_t;

void sortNameFirst(dataEntry_t in[], int length);
void sortName(dataEntry_t in[], int length);
void sortDate(dataEntry_t in[], int length);						//sorting functions
void sortHighTemp(dataEntry_t in[], int length);
void sortLowTemp(dataEntry_t in[], int length);

int copyArray(dataEntry_t in[], dataEntry_t out[], int start, int end);	//ultility functions
int dateBeforeOrAfter(date_t date1, date_t date2);
void printTable(dataEntry_t in[], int length);
void exportTable(dataEntry_t in[], int length, FILE *fp);
int orderStrings(string s1, string s2);
																	//search functions
int searchByDate(dataEntry_t in[], dataEntry_t out[], date_t date1, date_t date2, int length);
int searchByName(dataEntry_t in[], dataEntry_t out[], string name, int length);
int searchByGroup(dataEntry_t in[], dataEntry_t out[], string name, int length);

void functionA(dataEntry_t in[], int length, int b);						//the 5 tasks functions
void functionC(dataEntry_t in[], int length);
void functionD(dataEntry_t in[], int length);
void functionE(dataEntry_t in[], int length);


//#############################################################     MAIN    ##################################################

int main(void)
{
	dataEntry_t data[3000];											//array to contain all the data of the file
	dataEntry_t dataLowCopy[3000];

	int prcpPos = 0;												//varibles for the pos of the entry types
	int tmaxPos = 0;
	int tminPos = 0;
	int stationPos = 0;
	int datePos = 0;

	int index = 0;													//variables for keeping track of array indexes and counting lines
	int count = 0;
	
	string buf;														//buffer varriables to manipulate data

	ifstream infp;													//file manager variables
	string inLine;

	bool loop = true;												//variables that io uses
	char input;
	int ioStep = 0;


	infp.open("weather_station_five_column.txt");					//open in file
	if (!infp)														//check for error
	{
		printf(ERROR_TXT_TEMP, ERROR_FILE_NOT_FOUND, ERROR_TXT_1);
		PAUSE;
		return ERROR_FILE_NOT_FOUND;
	}
	cout << "Input file opened\n";

	getline(infp, inLine);											//get the first line and find the pos of the desiered values
	prcpPos = inLine.find("PRCP");
	tmaxPos = inLine.find("TMAX");
	tminPos = inLine.find("TMIN");
	datePos = inLine.find("DATE");
	stationPos = inLine.find("STATION_NAME");

	if (prcpPos == string::npos)									//START check to make sure the pos are found
	{
		printf(ERROR_TXT_TEMP, ERROR_POS_OF_ELEMENT_NOT_FOUND, ERROR_TXT_2);
		PAUSE;
		return ERROR_POS_OF_ELEMENT_NOT_FOUND;
	}

	if (tmaxPos == string::npos)
	{
		printf(ERROR_TXT_TEMP, ERROR_POS_OF_ELEMENT_NOT_FOUND, ERROR_TXT_2);
		PAUSE;
		return ERROR_POS_OF_ELEMENT_NOT_FOUND;
	}

	if (tminPos == string::npos)
	{
		printf(ERROR_TXT_TEMP, ERROR_POS_OF_ELEMENT_NOT_FOUND, ERROR_TXT_2);
		PAUSE;
		return ERROR_POS_OF_ELEMENT_NOT_FOUND;
	}

	if (datePos == string::npos)
	{
		printf(ERROR_TXT_TEMP, ERROR_POS_OF_ELEMENT_NOT_FOUND, ERROR_TXT_2);
		PAUSE;
		return ERROR_POS_OF_ELEMENT_NOT_FOUND;
	}

	if (stationPos == string::npos)
	{
		printf(ERROR_TXT_TEMP, ERROR_POS_OF_ELEMENT_NOT_FOUND, ERROR_TXT_2);
		PAUSE;
		return ERROR_POS_OF_ELEMENT_NOT_FOUND;
	}																//END check
	
	getline(infp, inLine);											//clear devider line

	while (getline(infp, inLine))									//loop through the file and fill in name,tmax,and tmin into the array
	{
		data[index].name = inLine.substr(stationPos, datePos - stationPos);
		data[index].date.year = stof(inLine.substr(datePos, 4));
		data[index].date.month = stof(inLine.substr(datePos + 5, 2));
		data[index].date.day = stof(inLine.substr(datePos + 8, 2));
		data[index].prcp = stof(inLine.substr(prcpPos, tmaxPos - prcpPos));
		data[index].tempMax = stof(inLine.substr(tmaxPos, (tminPos - tmaxPos)));
		data[index].tempMin = stof(inLine.substr(tminPos));
		index++;													//index-1 is the last element/ index it equale to the number of elements
	}
	infp.close();													//close file
	sortNameFirst(data, index);										//asign alpha
	for (int i = 0; i < index; i++)
	{
		data[i].alpha = i;
	}

	PAUSE;
	system("cls");													//start the main loop

	//############################################################	MAIN LOOP  ##############################################

	while (loop)													//main loop
	{
		switch(ioStep)
		{
		case 0:														//ask for desiered action
			system("cls");
			cout << Q_OPEN
				<< OPTION_A
				<< OPTION_B
				<< OPTION_C
				<< OPTION_D
				<< OPTION_E
				<< OPTION_Q
				<< ENTER_INPUT;
			cin >> input;
			input = toupper(input);

			if (input != 'A' && input != 'B' && input != 'C' && input != 'D' && input != 'E' && input != 'Q')
			{
				system("cls");										//input is not a valid letter reset
			}
			else ioStep++;
			break;//case 0
		case 1:
			switch (input)											//swtich to chose function
			{
			case 'A':
				functionA(data, index, -1);
				ioStep = 0;
				break;
			case 'B':
				functionA(data, index, 1);
				ioStep = 0;
				break;
			case 'C':
				functionC(data, index);
				ioStep = 0;
				break;
			case 'D':
				functionD(data, index);
				ioStep = 0;
				break;
			case 'E':
				functionE(data, index);
				ioStep = 0;
				break;
			case 'Q':												//break the loop and end the program
				loop = false;
				break;
			}//switch for the choice of function
			break;//case 1
		default:
			ioStep = 0;
			break;//default
		}//main switch loop
	}//main loop while

	cout << FAREWELL;
	PAUSE;
	return 0;
}//main

//##################################################################   functions    ######################################

void sortNameFirst(dataEntry_t in[], int length)
{
	string lowest;
	int lowestI = 0;
	int sortedIndex = 0;

	dataEntry_t buf;

	while (sortedIndex < length)
	{
		lowest = "zzzzzz";
		for (int i = sortedIndex; i < length; i++)					//find the lowest name
		{
			if (orderStrings(in[i].name, lowest) < 0)
			{
				lowest = in[i].name;
				lowestI = i;
			}
		}
		buf = in[sortedIndex];										//put it in the next unsorted spot
		//in[sortedIndex].alpha = sortedIndex;					Alpha cant be assigned here for some reson had to be assainged in main
		in[sortedIndex++] = in[lowestI];
		in[lowestI] = buf;
	}
}

void sortName(dataEntry_t in[], int length)
{
	int lowest;
	int lowestI = 0;
	int sortedIndex = 0;

	dataEntry_t buf;

	while (sortedIndex < length)
	{
		lowest = 9999;
		for (int i = sortedIndex; i < length; i++)					//find the lowest name
		{
			if (in[i].alpha < lowest)
			{
				lowest = in[i].alpha;
				lowestI = i;
			}
		}
		buf = in[sortedIndex];										//put it in the next unsorted spot
		in[sortedIndex++] = in[lowestI];
		in[lowestI] = buf;
	}
	return;
}//sort by name

void sortDate(dataEntry_t in[], int length)
{
	date_t highest;
	int highestI = 0;
	int sortedIndex = 0;

	dataEntry_t buf;

	while (sortedIndex < length)
	{
		highest.day = 0;
		highest.month = 0;
		highest.year = 0;
		for (int i = sortedIndex; i < length; i++)					//find the most resent date
		{
			if (dateBeforeOrAfter(in[i].date, highest) == 1)
			{
				highest.day = in[i].date.day;
				highest.month = in[i].date.month;
				highest.year = in[i].date.year;
				highestI = i;
			}
		}
		buf = in[sortedIndex];										//place in the next unsorted spot
		in[sortedIndex++] = in[highestI];
		in[highestI] = buf;
	}
	return;
}//sortdate

void sortHighTemp(dataEntry_t in[], int length)
{
	float highest = 0;
	int highestI = 0;
	int sortedIndex = 0;

	dataEntry_t buf;

	while (sortedIndex < length)
	{
		highest = 0;
		for (int i = sortedIndex; i < length; i++)					//find the highest temp
		{
			if (in[i].tempMax > highest)
			{
				highest = in[i].tempMax;
				highestI = i;
			}
		}
		buf = in[sortedIndex];										//put in in the next unsorted spot
		in[sortedIndex++] = in[highestI];
		in[highestI] = buf;
	}
	return;
}//sortHighTemp

void sortLowTemp(dataEntry_t in[], int length)
{
	float lowest = 0;
	int lowestI = 0;
	int sortedIndex = 0;

	dataEntry_t buf;

	while (sortedIndex < length)
	{
		lowest = 9999;
		for (int i = sortedIndex; i < length; i++)					//find the lowest temp
		{
			if (in[i].tempMin < lowest)
			{
				lowest = in[i].tempMin;
				lowestI = i;
			}
		}
		buf = in[sortedIndex];										//put it in the next unsorted spot
		in[sortedIndex++] = in[lowestI];
		in[lowestI] = buf;
	}
	return;
}//sortLowTemp

int copyArray(dataEntry_t in[], dataEntry_t out[],int start, int end)	//copies the selected part of in to out and returns the length of out
{
	int outI = 0;
	for (int i = start; i < end; i++)
	{
		out[outI++] = in[i];
	}
	return outI;
}//copyArray

int dateBeforeOrAfter(date_t date1, date_t date2)					//return 0 if the same, 1 if date one is after date two, -1 if date one is before date two
{
	//RETURN VALUES
	//-1: date1 < date 2
	//0: date1 == date 2
	//1: date 1 > date 2

	int retVal = 0;
	if (date1.day == date2.day && date1.month == date2.month && date1.year == date2.year) retVal = 0;
	else if (date1.year < date2.year || (date1.year == date2.year && date1.month < date2.month) || (date1.year == date2.year && date1.month == date2.month && date1.day < date2.day)) retVal = -1;
	else retVal = 1;
	return retVal;
}//dateBeforeOrAfter

void printTable(dataEntry_t in[], int length)						//print a table from the array
{																	//header
	printf(TABLE_LINE, STATION_NAME_WIDTH, STATION_NAME, DATE_WIDTH, DATE_NAME, PRCP_WIDTH, PRCP_NAME, PRCP_WIDTH, TMAX_NAME, PRCP_WIDTH, TMAX_NAME);
	for (int i = 0; i < STATION_NAME_WIDTH; i++) printf(TABLE_LINE_DIV);	//div of dashes
	printf(" ");
	for (int i = 0; i < DATE_WIDTH; i++) printf(TABLE_LINE_DIV);
	printf(" ");
	for (int i = 0; i < PRCP_WIDTH; i++) printf(TABLE_LINE_DIV);
	printf(" ");
	for (int i = 0; i < PRCP_WIDTH; i++) printf(TABLE_LINE_DIV);
	printf(" ");
	for (int i = 0; i < PRCP_WIDTH; i++) printf(TABLE_LINE_DIV);
	printf("\n");
	for (int i = 0; i < length; i++)								//print data
	{
		printf(TABLE_LINE_DATA, STATION_NAME_WIDTH, in[i].name.c_str(), in[i].date.year, in[i].date.month, in[i].date.day, PRCP_WIDTH, in[i].prcp, PRCP_WIDTH, in[i].tempMax, PRCP_WIDTH, in[i].tempMin);
	}
	printf("\n");
	return;
}//print table

void exportTable(dataEntry_t in[], int length, FILE *fp)
{
	fprintf(fp, TABLE_LINE, STATION_NAME_WIDTH, STATION_NAME, DATE_WIDTH, DATE_NAME, PRCP_WIDTH, PRCP_NAME, PRCP_WIDTH, TMAX_NAME, PRCP_WIDTH, TMIN_NAME);
	for (int i = 0; i < STATION_NAME_WIDTH; i++) printf(TABLE_LINE_DIV);	//div of dashes
	fprintf(fp, " ");
	for (int i = 0; i < DATE_WIDTH; i++) fprintf(fp, TABLE_LINE_DIV);
	fprintf(fp, " ");
	for (int i = 0; i < PRCP_WIDTH; i++) fprintf(fp, TABLE_LINE_DIV);
	fprintf(fp, " ");
	for (int i = 0; i < PRCP_WIDTH; i++) fprintf(fp,TABLE_LINE_DIV);
	fprintf(fp, " ");
	for (int i = 0; i < PRCP_WIDTH; i++) fprintf(fp, TABLE_LINE_DIV);
	fprintf(fp, "\n");
	for (int i = 0; i < length; i++)								//print data
	{
		fprintf(fp, TABLE_LINE_DATA, STATION_NAME_WIDTH, in[i].name.c_str(), in[i].date.year, in[i].date.month, in[i].date.day, PRCP_WIDTH, in[i].prcp, PRCP_WIDTH, in[i].tempMax, PRCP_WIDTH, in[i].tempMin);
	}
	fprintf(fp, "\n");
	return;
}//exsport table

int orderStrings(string s1, string s2)								//this may be over complicating it but is is what worked and i dont wnat to try and fix it and mess it up again
{
	//RETURN VALUES
	//-1: the s1 comes before s2
	//0: the s1 is s2
	//1: the s1 comes after s2

	int s1L = s1.length();											// size of inputs and counters
	int s2L = s2.length();
	int g = 0;
																	//c strings arrays
	char c1[200];
	char c2[200];

	for (int i = 0; i < s1L; i++)									// convert the c++ string to the c string
	{
		if (s1[i] != ' ')											//removes spaces
		{
			c1[g++] = s1[i];
		}
	}
	c1[g++] = '\0';

	g = 0;															//reset indexes

	for (int i = 0; i < s2L; i++)									//convert from c++ to c
	{
		if (s2[i] != ' ')
		{
			c2[g++] = s2[i];										//remove spaces
		};
	}
	c2[g++] = '\0';

	return strcmp(c1, c2);											//return -1 if before, 0 if same, 1 if after
}//order strings



int searchByDate(dataEntry_t in[], dataEntry_t out[], date_t date1, date_t date2, int length)
{
	//RETURN VALUES
	//-1: ERROR the date was not found
	//pos: the number of found matches

	int retVal = 0;

	bool found = false;
	int startI = 0;
	int endI = 0;
	int leftBound = 0;
	int rightBound = length - 1;

	sortDate(in, length);											//first sort the array by date
	while (!found)													//binary search for a starting point
	{
		endI = leftBound + (rightBound - leftBound) / 2;
		if (dateBeforeOrAfter(date1, in[endI].date) == 0)
		{
			found = true;
		}
		else if (dateBeforeOrAfter(date1, in[endI].date) == 1)
		{
			rightBound = endI;
		}
		else if (dateBeforeOrAfter(date1, in[endI].date) == -1)
		{
			leftBound = endI;
		}
		if (leftBound + 1 == rightBound || rightBound + 1 == leftBound)				//if not date is found return -1 to note error
		{
			retVal = -1;
			found = true;
		}
	}
	startI = endI;
	if (retVal != -1)												//if a date was found continue
	{
		for (int i = endI; i < length; i++)						//from the starting point move down till the lower bound is found
		{
			if (dateBeforeOrAfter(in[i].date, date1) == -1)
			{
				endI = i - 1;
				break;
			}
		}//find endI
		for (int i = startI; i >= 0; i--)							//from the starting point move up till the upper bound is found
		{
			if (dateBeforeOrAfter(in[i].date, date2) == 1)
			{
				startI = i + 1;
				break;
			}
			else if (i == 0)
			{
				startI = 0;
			}
		}//find startI

		retVal = copyArray(in, out, startI, endI);					//out put the copy array that has only the selected entries
		sortName(out, retVal);
	}//date was found

	return retVal;													//return length of found entries or ERROR
}//searchByDate

int searchByName(dataEntry_t in[], dataEntry_t out[], string name, int length)
{
	//RETURN VALUES
	//-1: ERROR the date was not found
	//pos: the number of found matches

	int retVal = 0;

	bool found = false;
	int startI = 0;
	int endI = 0;
	int leftBound = 0;
	int rightBound = length - 1;

	sortName(in, length);										//first sort the array by date
	while (!found)													//binary search for a starting point
	{
		endI = leftBound + (rightBound - leftBound) / 2;
		if (orderStrings(name, in[endI].name) == 0)
		{
			found = true;
		}
		else if (orderStrings(name, in[endI].name) == -1)
		{
			rightBound = endI;
		}
		else if (orderStrings(name, in[endI].name) == 1)
		{
			leftBound = endI;
		}
		if (leftBound +1 == rightBound || rightBound +1 == leftBound)				//if not date is found return -1 to note error
		{
			retVal = -1;
			found = true;
		}
	}
	startI = endI;
	if (retVal != -1)												//if a date was found continue
	{
		for (int i = endI; i < length; i++)						//from the starting point move down till the lower bound is found
		{
			if (orderStrings(in[i].name, name) != 0)
			{
				endI = i - 1;
				break;
			}
		}//find endI
		for (int i = startI; i >= 0; i--)							//from the starting point move up till the upper bound is found
		{
			if (orderStrings(in[i].name, name) != 0)
			{
				startI = i + 1;
				break;
			}
			else if (i == 0)
			{
				startI = 0;
			}
		}//find startI

		retVal = copyArray(in, out, startI, endI);					//out put the copy array that has only the selected entries
		sortName(out, retVal);
	}//date was found

	return retVal;													//return length of found entries or ERROR
}//search name exact

int searchByGroup(dataEntry_t in[], dataEntry_t out[], string name, int length)
{
	int outDex = 0;													//counter for the out array
	for (int i = 0; i < length; i++)
	{
		if (in[i].name.find(name) != string::npos)					//if the search term is found in the name add that entry to the out array
		{
			out[outDex++] = in[i];
		}
	}
	return outDex;													//return the length of the out array
}//search by group



void functionA(dataEntry_t in[], int length, int b)						//FUNCTION A FIND TOTAL PRCP FOR A DATE
{
	dataEntry_t subData[3000];										//the array and its length
	int subLength = 0;

	date_t ioDate;													//io/buffer variables
	date_t ioDate2;
	char buf;
	bool gotInput = false;

	float totalPRCP = 0;											//the total PRCP
	float averagePRCP = 0;

	system("cls");
	if (b == -1)
	{
		cout << A_QUESTION << ENTER_INPUT;								//ask for date	
		cin >> ioDate.month >> ioDate.day >> ioDate.year;
		ioDate2 = ioDate;
	}
	else
	{
		cout << B_QUESTION << ENTER_INPUT;
		cin >> ioDate.month >> ioDate.day >> ioDate.year >> ioDate2.month >> ioDate2.day >> ioDate2.year; ;
	}
	
	subLength = searchByDate(in, subData, ioDate, ioDate2, length);	//call the seach function 

	for (int i = 0; i < subLength; i++)								//total up the PRCP
	{
		totalPRCP += subData[i].prcp;
	}
	averagePRCP = totalPRCP / subLength;							//find average PRCP

	while (!gotInput)												//output data and ask 
	{
		system("cls");
		if (b == -1)
		{
			cout << A_TXT_TOTAL << ioDate.month << " " << ioDate.day << " " << ioDate.year << ": " << totalPRCP << endl;
			cout << A_TXT_AVERAGE << ioDate.month << " " << ioDate.day << " " << ioDate.year << ": " << averagePRCP << endl;
		}
		else
		{
			cout << A_TXT_TOTAL << ioDate.month << " " << ioDate.day << " " << ioDate.year << "-" << ioDate2.month << " " << ioDate2.day << " " << ioDate2.year <<": " << totalPRCP << endl;
			cout << A_TXT_AVERAGE << ioDate.month << " " << ioDate.day << " " << ioDate.year << "-" << ioDate2.month << " " << ioDate2.day << " " << ioDate2.year << ": " << averagePRCP << endl;
		}
		cout << Q_SEE_TABLE << ENTER_INPUT;							//ask if they want to see the table
		cin >> buf;
		if (toupper(buf) == 'Y' || toupper(buf) == 'N') gotInput = true;
	}
	if (toupper(buf) == 'Y')
	{
		system("cls");
		printTable(subData, subLength);								//if yes print the table
		if (b == -1)
		{
			cout << A_TXT_TOTAL << ioDate.month << " " << ioDate.day << " " << ioDate.year << ": " << totalPRCP << endl;
			cout << A_TXT_AVERAGE << ioDate.month << " " << ioDate.day << " " << ioDate.year << ": " << averagePRCP << endl;
		}
		else
		{
			cout << A_TXT_TOTAL << ioDate.month << " " << ioDate.day << " " << ioDate.year << "-" << ioDate2.month << " " << ioDate2.day << " " << ioDate2.year << ": " << totalPRCP << endl;
			cout << A_TXT_AVERAGE << ioDate.month << " " << ioDate.day << " " << ioDate.year << "-" << ioDate2.month << " " << ioDate2.day << " " << ioDate2.year << ": " << averagePRCP << endl;
		}
		PAUSE;
	}

	gotInput = false;
	while (!gotInput)												//wait till valid input
	{	
		system("cls");
		cout << Q_EXPORT << ENTER_INPUT;
		cin >> buf;
		if (toupper(buf) == 'Y' || toupper(buf) == 'N') gotInput = true;
	}

	if (toupper(buf) == 'Y')
	{
		FILE *fp;
		string txtName;
		
		cout << Q_FILE_NAME << ENTER_INPUT;
		cin >> txtName;

		fopen_s(&fp, txtName.c_str(), "w");

		if (fp == NULL)
		{
			printf(ERROR_TXT_TEMP, ERROR_FILE_NOT_FOUND, ERROR_TXT_1);
		}
		if (b == -1)
		{
			fprintf(fp, A_FILE_OUT, ioDate.month, ioDate.day, ioDate.year, totalPRCP, ioDate.month, ioDate.day, ioDate.year, averagePRCP);
		}
		else
		{
			fprintf(fp, B_FILE_OUT, ioDate.month, ioDate.day, ioDate.year, ioDate2.month, ioDate2.day, ioDate2.year, totalPRCP, ioDate.month, ioDate.day, ioDate.year, ioDate2.month, ioDate2.day, ioDate2.year, averagePRCP);
		}

		exportTable(subData, subLength, fp);

		fclose(fp);
	}
	return;
}//functionA

void functionC(dataEntry_t in[], int length)
{
	dataEntry_t subData[3000];
	int subLength = 0;

	string name;
	char buf;

	float totalPRCP = 0;
	float averagePRCO = 0;

	bool gotInput = false;

	system("cls");
	cout << C_QUESTION << ENTER_INPUT;
	cin >> name;

	subLength = searchByGroup(in, subData, name, length);

	for (int i = 0; i < subLength; i++)
	{
		totalPRCP += subData[i].prcp;
	}
	averagePRCO = totalPRCP / subLength;

	while (!gotInput)												//output data and ask 
	{
		system("cls");
		cout << A_TXT_TOTAL << " group " << name << ": " << totalPRCP << endl;
		cout << A_TXT_AVERAGE << " group " << name << ": " << averagePRCO << endl;

		cout << Q_SEE_TABLE << ENTER_INPUT;							//ask if they want to see the table
		cin >> buf;
		if (toupper(buf) == 'Y' || toupper(buf) == 'N') gotInput = true;
	}

	sortName(subData, subLength);
	if (toupper(buf) == 'Y')
	{
		system("cls");
		printTable(subData, subLength);
		cout << A_TXT_TOTAL << " group " << name << ": " << totalPRCP << endl;
		cout << A_TXT_AVERAGE << " group " << name << ": " << averagePRCO << endl;
		PAUSE;
	}

	gotInput = false;
	while (!gotInput)												//wait till valid input
	{
		system("cls");
		cout << Q_EXPORT << ENTER_INPUT;
		cin >> buf;
		if (toupper(buf) == 'Y' || toupper(buf) == 'N') gotInput = true;
	}

	if (toupper(buf) == 'Y')
	{
		FILE *fp;
		string txtName;

		cout << Q_FILE_NAME << ENTER_INPUT;
		cin >> txtName;

		fopen_s(&fp, txtName.c_str(), "w");

		if (fp == NULL)
		{
			printf(ERROR_TXT_TEMP, ERROR_FILE_NOT_FOUND, ERROR_TXT_1);
		}
		
		fprintf(fp, C_FILE_OUT, name, totalPRCP, name, averagePRCO);

		exportTable(subData, subLength, fp);

		fclose(fp);
	}
	return;
}//functionB


void functionD(dataEntry_t in[], int length)
{
	dataEntry_t subData[3000];										//the array and its length
	int subLength = 0;

	string ioString;												//io/buffer variables
	char buf;
	bool gotInput = false;
	int stringCount = 0;

	float highest = 0;												//the total PRCP
	float lowest = 0;
	float averageHigh = 0;
	float averageLow = 0;

	system("cls");

	cout << D_QUESTION << ENTER_INPUT;
	cin.clear(); cin.sync();
	getline(cin, ioString);
	getline(cin, ioString);
	/*
	while ((buf = getchar()) != '\n')
	{
		ioString[stringCount++] = buf;
	}
	ioString[stringCount] = '\0';
	*/
	subLength = searchByName(in, subData, ioString, length);

	sortHighTemp(subData, subLength);
	highest = subData[0].tempMax;
	sortLowTemp(subData, subLength);
	lowest = subData[0].tempMin;

	for (int i = 0; i < subLength; i++)
	{
		averageHigh += subData[i].tempMax;
		averageLow += subData[i].tempMin;
	}
	averageHigh /= subLength;
	averageLow /= subLength;

	while (!gotInput)												//output data and ask 
	{
		system("cls");
		cout << D_EXSTREAM_HIGH << ioString << ": " << highest << " F" << endl;
		cout << D_EXTREAM_LOW << ioString << ": " << lowest << " F" << endl;
		cout << D_AVERAGE_HIGH << ioString << ": " << averageHigh << " F" << endl;
		cout << D_AVERAGE_LOW << ioString << ": " << averageLow << " F" << endl;

		cout << Q_SEE_TABLE << ENTER_INPUT;							//ask if they want to see the table
		cin >> buf;
		if (toupper(buf) == 'Y' || toupper(buf) == 'N') gotInput = true;
	}
	if (toupper(buf) == 'Y')
	{
		system("cls");
		printTable(subData, subLength);								//if yes print the table
		cout << D_EXSTREAM_HIGH << ioString << ": " << highest << " F" << endl;
		cout << D_EXTREAM_LOW << ioString << ": " << lowest << " F" << endl;
		cout << D_AVERAGE_HIGH << ioString << ": " << averageHigh << " F" << endl;
		cout << D_AVERAGE_LOW << ioString << ": " << averageLow << " F" << endl;
		PAUSE;
	}
	gotInput = false;
	while (!gotInput)												//wait till valid input
	{
		system("cls");
		cout << Q_EXPORT << ENTER_INPUT;
		cin >> buf;
		if (toupper(buf) == 'Y' || toupper(buf) == 'N') gotInput = true;
	}
	if (toupper(buf) == 'Y')
	{
		FILE *fp;
		string txtName;

		cout << Q_FILE_NAME << ENTER_INPUT;
		cin >> txtName;

		fopen_s(&fp, txtName.c_str(), "w");

		if (fp == NULL)
		{
			printf(ERROR_TXT_TEMP, ERROR_FILE_NOT_FOUND, ERROR_TXT_1);
		}
		fprintf(fp, D_EXSTREAM_HIGH_FILE, ioString, highest);
		fprintf(fp, D_EXTREAM_LOW_FILE, ioString, lowest);
		fprintf(fp, D_AVERAGE_HIGH_FILE, ioString, averageHigh);
		fprintf(fp, D_AVERAGE_LOW_FILE, ioString, averageLow);

		exportTable(subData, subLength, fp);

		fclose(fp);
	}
	return;
}//functionD

void functionE(dataEntry_t in[], int length)
{
	dataEntry_t subData[3000];										//the array and its length
	dataEntry_t subData2[1000];
	int subLength = 0;

	string ioString;												//io/buffer variables
	date_t ioDate;
	date_t ioDate2;
	char buf;
	bool gotInput = false;
	int stringCount = 0;

	float highest = 0;												//the total PRCP
	float lowest = 0;
	float averageHigh = 0;
	float averageLow = 0;

	system("cls");

	cout << D_QUESTION << ENTER_INPUT;
	cin.clear(); cin.sync();
	getline(cin, ioString);
	getline(cin, ioString);
	cout << E_QUESTION << ENTER_INPUT;
	cin >> ioDate.month >> ioDate.day >> ioDate.year >> ioDate2.month >> ioDate2.day >> ioDate2.year;
	/*
	while ((buf = getchar()) != '\n')
	{
		ioString[stringCount++] = buf;
	}
	ioString[stringCount] = '\0';
	*/
	subLength = searchByName(in, subData, ioString, length);
	subLength = searchByDate(subData, subData2, ioDate, ioDate2, subLength);

	sortHighTemp(subData2, subLength);
	highest = subData2[0].tempMax;
	sortLowTemp(subData2, subLength);
	lowest = subData2[0].tempMin;

	for (int i = 0; i < subLength; i++)
	{
		averageHigh += subData2[i].tempMax;
		averageLow += subData2[i].tempMin;
	}
	averageHigh /= subLength;
	averageLow /= subLength;

	while (!gotInput)												//output data and ask 
	{
		system("cls");
		cout << D_EXSTREAM_HIGH << ioString << " on " << ioDate.month << " " << ioDate.day << " " << ioDate.year << " - " << ioDate2.month<< " " << ioDate2.day << " " << ioDate2.year << ": " << highest << " F" << endl;
		cout << D_EXTREAM_LOW << ioString << " on " << ioDate.month << " " << ioDate.day << " " << ioDate.year << " - " << ioDate2.month << " " << ioDate2.day << " " << ioDate2.year << ": " << lowest << " F" << endl;
		cout << D_AVERAGE_HIGH << ioString << " on " << ioDate.month << " " << ioDate.day << " " << ioDate.year << " - " << ioDate2.month << " " << ioDate2.day << " " << ioDate2.year << ": " << averageHigh << " F" << endl;
		cout << D_AVERAGE_LOW << ioString << " on " << ioDate.month << " " << ioDate.day << " " << ioDate.year << " - " << ioDate2.month << " " << ioDate2.day << " " << ioDate2.year << ": " << averageLow << " F" << endl;

		cout << Q_SEE_TABLE << ENTER_INPUT;							//ask if they want to see the table
		cin >> buf;
		if (toupper(buf) == 'Y' || toupper(buf) == 'N') gotInput = true;
	}
	if (toupper(buf) == 'Y')
	{
		system("cls");
		printTable(subData2, subLength);							//if yes print the table
		cout << D_EXSTREAM_HIGH << ioString << " on " << ioDate.month << " " << ioDate.day << " " << ioDate.year << " - " << ioDate2.month << " " << ioDate2.day << " " << ioDate2.year << ": " << highest << " F" << endl;
		cout << D_EXTREAM_LOW << ioString << " on " << ioDate.month << " " << ioDate.day << " " << ioDate.year << " - " << ioDate2.month << " " << ioDate2.day << " " << ioDate2.year << ": " << lowest << " F" << endl;
		cout << D_AVERAGE_HIGH << ioString << " on " << ioDate.month << " " << ioDate.day << " " << ioDate.year << " - " << ioDate2.month << " " << ioDate2.day << " " << ioDate2.year << ": " << averageHigh << " F" << endl;
		cout << D_AVERAGE_LOW << ioString << " on " << ioDate.month << " " << ioDate.day << " " << ioDate.year << " - " << ioDate2.month << " " << ioDate2.day << " " << ioDate2.year << ": " << averageLow << " F" << endl;
		PAUSE;
	}
	gotInput = false;
	while (!gotInput)												//wait till valid input
	{
		system("cls");
		cout << Q_EXPORT << ENTER_INPUT;
		cin >> buf;
		if (toupper(buf) == 'Y' || toupper(buf) == 'N') gotInput = true;
	}
	if (toupper(buf) == 'Y')
	{
		FILE *fp;
		string txtName;

		cout << Q_FILE_NAME << ENTER_INPUT;
		cin >> txtName;

		fopen_s(&fp, txtName.c_str(), "w");

		if (fp == NULL)
		{
			printf(ERROR_TXT_TEMP, ERROR_FILE_NOT_FOUND, ERROR_TXT_1);
		}
		fprintf(fp, E_EXSTREAM_HIGH_FILE, ioString, ioDate.month, ioDate.day, ioDate.year, ioDate2.month, ioDate2.day, ioDate2.year, highest);
		fprintf(fp, E_EXTREAM_LOW_FILE, ioString, ioDate.day, ioDate.year, ioDate2.month, ioDate2.day, ioDate2.year, lowest);
		fprintf(fp, E_AVERAGE_HIGH_FILE, ioString, ioDate.day, ioDate.year, ioDate2.month, ioDate2.day, ioDate2.year, averageHigh);
		fprintf(fp, E_AVERAGE_LOW_FILE, ioString, ioDate.day, ioDate.year, ioDate2.month, ioDate2.day, ioDate2.year, averageLow);

		exportTable(subData2, subLength, fp);

		fclose(fp);
	}
	return;
}


##################################   END   ####################################
