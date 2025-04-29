//
//Homework 4 Problems 1,2 and 3: cleaing up the data files
//


#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#define PAUSE cout <<endl; system("Pause"); cout << endl;			//system pause

#define NUM_OF_OUT_ENTRIES 25
#define WIDTH 7

using namespace std;

typedef struct {													//the data from the file broken up into indevidual variables
	string name;
	float tempMax;
	float tempMin;
}dataEntry;

void sortHighTemp(dataEntry in[], int length);
void sortLowTemp(dataEntry in[], int length);
void copyArray(dataEntry in[], dataEntry out[], int length);

int main(void)
{
	dataEntry data[3000];											//array to contain all the data of the file
	dataEntry dataLowCopy[3000];

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

	int numOfEntries = 0;											//variables that io uses



	infp.open("weather_station_five_column.txt");					//open in file
	if (!infp)														//check for error
	{
		cout << "ERROR: File failed to open\n\n";
		system("pause");
		return -1;
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
		cout << "ERROR failed to find PRCP pos.\n\n";
		system("pause");
		return -1;
	}

	if (tmaxPos == string::npos)
	{
		cout << "ERROR failed to find TMAX pos.\n\n";
		system("pause");
		return -1;
	}

	if (tminPos == string::npos)
	{
		cout << "ERROR failed to find TMIN pos.\n\n";
		system("pause");
		return -1;
	}

	if (datePos == string::npos)					
	{
		cout << "ERROR failed to find DATE pos.\n\n";
		system("pause");
		return -1;
	}

	if (stationPos == string::npos)
	{
		cout << "ERROR failed to find STATION_NAME pos.\n\n";
		system("pause");
		return -1;
	}																//END check

	getline(infp, inLine);											//clear devider line

	while (getline(infp, inLine))									//loop through the file and fill in name,tmax,and tmin into the array
	{
		data[index].name = inLine.substr(stationPos, datePos - stationPos);
		data[index].tempMax = stof(inLine.substr(tmaxPos, (tminPos - tmaxPos)));
		data[index].tempMin = stof(inLine.substr(tminPos));
		index++;													//index-1 is the last element/ index it equale to the number of elements
	}
	infp.close();													//close file

	PAUSE

	copyArray(data, dataLowCopy, index);
	sortHighTemp(data, index);
	sortLowTemp(dataLowCopy, index);

	cout << "how many entries do you want to display:> ";
	cin >> numOfEntries;
	cout << endl;

	cout << setprecision(4);
	cout << showpoint;
	
	cout << setw((datePos - stationPos)) << "Stations with the highest tempetures top " << setw(WIDTH) << numOfEntries << setw((datePos - stationPos)+1) << "Stations with the lowest tempetures bottom " << setw(WIDTH) << numOfEntries;
	cout << "\n\n";
	cout << setw((datePos - stationPos)-1) << "STATION NAMES" << setw(WIDTH+1) << "TEMP" << setw((datePos - stationPos)) << "STATION NAMES" << setw(WIDTH+1) << "TEMP" << endl;
	cout << setfill('-') << setw((datePos - stationPos) - 1) << "-" << " " << setw(WIDTH) << "-" << " " << setfill('-') << setw((datePos - stationPos) - 1) << "-" << " " << setw(WIDTH) << "-" << " " << endl;
	cout << setfill(' ');
	for (int i = 0; i < numOfEntries; i++)
	{
		cout << setw((datePos - stationPos)) << data[i].name << setw(WIDTH) << data[i].tempMax << setw((datePos - stationPos)+1) << dataLowCopy[i].name << setw(WIDTH) << dataLowCopy[i].tempMin << endl;;
	}

	PAUSE

	return 0;
}//main

//##################################################################   functions    ######################################

void sortHighTemp(dataEntry in[], int length)
{
	float highest = 0;
	int highestI = 0;
	int sortedIndex = 0;

	dataEntry buf;

	while (sortedIndex < length)
	{
		highest = 0;
		for (int i = sortedIndex; i < length; i++)
		{
			if (in[i].tempMax > highest)
			{
				highest = in[i].tempMax;
				highestI = i;
			}
		}
		buf = in[sortedIndex];
		in[sortedIndex++] = in[highestI];
		in[highestI] = buf;
	}
	return;
}//sortHighTemp

void sortLowTemp(dataEntry in[], int length)
{
	float lowest = 0;
	int lowestI = 0;
	int sortedIndex = 0;

	dataEntry buf;

	while (sortedIndex < length)
	{
		lowest = 9999;
		for (int i = sortedIndex; i < length; i++)
		{
			if (in[i].tempMin < lowest)
			{
				lowest = in[i].tempMin;
				lowestI = i;
			}
		}
		buf = in[sortedIndex];
		in[sortedIndex++] = in[lowestI];
		in[lowestI] = buf;
	}
	return;
}//sortLowTemp

void copyArray(dataEntry in[], dataEntry out[], int length)
{
	for (int i = 0; i < length; i++)
	{
		out[i] = in[i];
	}
	return;
}//copyArray
