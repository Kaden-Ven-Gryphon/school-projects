#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <time.h>
#include <random>

#define PAUSE system("pause")

#define ID_W 10
#define LAT_W 15
#define LON_W 15
#define ALT_W 15
#define MONTH_W 3
#define DAY_W 3
#define YEAR_W 5
#define H_W 3
#define M_W 3
#define S_W 3
#define TEMP_W 15
#define PRCP_W 15
#define WIND_W 15
#define PRESS_W 15

using namespace std;

typedef struct {
	float lat = 0;
	float lon = 0;
	float alt = 0;
}pos_t;

typedef struct {
	int month = 0;
	int day = 0;
	int year = 0;

	int hour = 0;
	int min = 0;
	int sec = 0;
}date_t;

typedef struct {
	string id;
	pos_t pos;
	date_t date;
	float temp = 0;
	float prcp = 0;
	float wind = 0;
	float press = 0;
}dataEntry_t;


void printData(dataEntry_t *d, int c);
void f1(dataEntry_t *d, int c);
void f2(dataEntry_t *d, int c, string id, date_t date);
void f3(dataEntry_t *d, int c, date_t);
void f4(dataEntry_t *d, int c, date_t);
void f5(dataEntry_t *d, int c, float alt);


int main(void)
{
	ifstream ifp;															//file pointer


	dataEntry_t *allData;													//to be array of data
	int count = 0;															//number of data entries

	int latP = 0;															//file pos markers
	int lonP = 0;
	int altP = 0;
	int monthP = 0;
	int dayP = 0;
	int yearP = 0;
	int hourP = 0;
	int minP = 0;
	int secP = 0;
	int tempP = 0;
	int prcpP = 0;
	int windP = 0;
	int pressP = 0;

	string line;
	date_t date;

	int userInput = 0;
	float userF = 0;
	bool loop = true;


	ifp.open("homwork_six_data.txt");										//open file
	if (!ifp)																//check file
	{
		cout << "ERROR 1: File failed to open\n";
		PAUSE;
		return -1;
	}

	getline(ifp, line);														//get the header line

	latP = line.find("LATITUDE");											//break down the header line and find the pos of each data type
	lonP = line.find("LONGITUDE");
	altP = line.find("ALTITUDE");
	monthP = line.find("MM");
	dayP = line.find("DD");
	yearP = line.find("YYYY");
	hourP = line.find("HH");
	minP = line.find("MM", monthP + 1);
	secP = line.find("SS");
	tempP = line.find("TEMP");
	prcpP = line.find("PRCP");
	windP = line.find("WIND");
	pressP = line.find("PRESS");


	while (!ifp.eof())														//find the number of data entries
	{
		getline(ifp, line);
		count++;
	}

	allData = new dataEntry_t[count];										//make an array that has the size of the number of data entries

	ifp.clear();
	ifp.seekg(0, ios::beg);													//reset the file

	count = 0;
	getline(ifp, line);
	while (!ifp.eof())														//fill the array with the file infomation
	{
		string temp;
		getline(ifp, line);
		temp = line.substr(0, latP);

		for (int i = 0; i < temp.length(); i++)
		{
			if (temp[i] != ' ')
			{
				allData[count].id += temp[i];
			}
		}
		allData[count].pos.lat = stof(line.substr(latP, lonP - latP));
		allData[count].pos.lon = stof(line.substr(lonP, altP - lonP));
		allData[count].pos.alt = stof(line.substr(altP, monthP - altP));
		allData[count].date.month = stoi(line.substr(monthP, dayP - monthP));
		allData[count].date.day = stoi(line.substr(dayP, yearP - dayP));
		allData[count].date.year = stoi(line.substr(yearP, hourP - yearP));
		allData[count].date.hour = stoi(line.substr(hourP, minP - hourP));
		allData[count].date.min = stoi(line.substr(minP, secP - minP));
		allData[count].date.sec = stoi(line.substr(secP, tempP - secP));
		allData[count].temp = stof(line.substr(tempP, prcpP - tempP));
		allData[count].prcp = stof(line.substr(prcpP, windP - prcpP));
		allData[count].wind = stof(line.substr(windP, pressP - windP));
		allData[count].press = stof(line.substr(pressP, line.length() - pressP));
		count++;
	}
	ifp.close();

	while (loop)
	{
		while (true)
		{
			system("cls");
			cout << "1. Show all stations and locations that they were placed" << endl;
			cout << "2. Search for data by station and date" << endl;
			cout << "3. Highest TEMP for given date" << endl;
			cout << "4. Total PRCP for given date" << endl;
			cout << "5. Lowest TEMP above given altitude" << endl;
			cout << "6. QUIT" << endl;
			cout << ":> ";

			cin >> userInput;
			if (userInput >= 1 && userInput <= 6) break;
		}

		switch (userInput)
		{
		case 1:
			f1(allData, count);
			PAUSE;
			break;
		case 2:
			cout << "enter an id\n:> ";
			cin >> line;
			cout << "\enter a date (MM DD YYYY)\n:> ";
			cin >> userInput;
			date.month = userInput;
			cin >> userInput;
			date.day = userInput;
			cin >> userInput;
			date.year = userInput;
			f2(allData, count, line, date);
			break;
		case 3:
			cout << "\enter a date (MM DD YYYY)\n:> ";
			cin >> userInput;
			date.month = userInput;
			cin >> userInput;
			date.day = userInput;
			cin >> userInput;
			date.year = userInput;
			f3(allData, count, date);
			break;
		case 4:
			cout << "\enter a date (MM DD YYYY)\n:> ";
			cin >> userInput;
			date.month = userInput;
			cin >> userInput;
			date.day = userInput;
			cin >> userInput;
			date.year = userInput;
			f4(allData, count, date);
			break;
		case 5:
			cout << "Enter an altitude\n:> ";
			cin >> userF;
			f5(allData, count, userF);
			break;
		case 6:
			loop = false;
			break;
		}
	}
	




	//printData(allData, count);

	PAUSE;
}




void printData(dataEntry_t * d, int c)											//print the data
{
	cout << setw(ID_W) << "ID" << setw(LAT_W) << "LAT" << setw(LON_W) << "LON" << setw(ALT_W) << "ALT" << setw(MONTH_W) << "MM" << setw(DAY_W) <<"DD" << setw(YEAR_W) <<"YYYY" << setw(H_W) << "HH" << setw(M_W) << "MM" << setw(S_W) << "SS" << setw(TEMP_W) << "TEEMP" << setw(PRCP_W) << "PRCP" << setw(WIND_W) <<"WIND" << setw(PRESS_W) << "PRESS" << endl;
	for (int i = 0; i < c; i++)
	{
		cout << setw(ID_W) << d[i].id << setw(LAT_W) << d[i].pos.lat << setw(LON_W) << d[i].pos.lon << setw(ALT_W) << d[i].pos.alt << setw(MONTH_W) << d[i].date.month << setw(DAY_W) << d[i].date.day << setw(YEAR_W) << d[i].date.year << setw(H_W) << d[i].date.hour << setw(M_W) << d[i].date.min << setw(S_W) << d[i].date.sec << setw(TEMP_W) << d[i].temp << setw(PRCP_W) << d[i].prcp << setw(WIND_W) << d[i].wind << setw(PRESS_W) << d[i].press << endl;
	}
}

void f1(dataEntry_t * d, int c)
{
	dataEntry_t *filterData;
	int fcount = 0;
	bool dupe = false;

	filterData = new dataEntry_t[c];

	//sort by id


	system("cls");
	cout << setw(ID_W) << "ID" << setw(LAT_W) << "LAT" << setw(LON_W) << "LON" << setw(ALT_W) << "ALT" << endl;
	for (int i = 0; i < c; i++)
	{
		dupe = false;
		for (int j = 0; j < fcount; j++)
		{
			if (d[i].id == filterData[j].id && d[i].pos.lat == filterData[j].pos.lat && d[i].pos.lon == filterData[j].pos.lon && d[i].pos.alt == filterData[j].pos.alt)
			{
				dupe = true;
			}
		}

		if (!dupe)
		{
			filterData[fcount++] = d[i];
			cout << setw(ID_W) << d[i].id << setw(LAT_W) << d[i].pos.lat << setw(LON_W) << d[i].pos.lon << setw(ALT_W) << d[i].pos.alt << endl;
		}
	}
}

void f2(dataEntry_t * d, int c, string id, date_t date)
{
	cout << setw(ID_W) << "ID" << setw(LAT_W) << "LAT" << setw(LON_W) << "LON" << setw(ALT_W) << "ALT" << setw(MONTH_W) << "MM" << setw(DAY_W) << "DD" << setw(YEAR_W) << "YYYY" << setw(H_W) << "HH" << setw(M_W) << "MM" << setw(S_W) << "SS" << setw(TEMP_W) << "TEEMP" << setw(PRCP_W) << "PRCP" << setw(WIND_W) << "WIND" << setw(PRESS_W) << "PRESS" << endl;
	for (int i = 0; i < c; i++)
	{
		if (d[i].id == id && d[i].date.month == date.month && d[i].date.day == date.day && d[i].date.year == date.year)
		{
			cout << setw(ID_W) << d[i].id << setw(LAT_W) << d[i].pos.lat << setw(LON_W) << d[i].pos.lon << setw(ALT_W) << d[i].pos.alt << setw(MONTH_W) << d[i].date.month << setw(DAY_W) << d[i].date.day << setw(YEAR_W) << d[i].date.year << setw(H_W) << d[i].date.hour << setw(M_W) << d[i].date.min << setw(S_W) << d[i].date.sec << setw(TEMP_W) << d[i].temp << setw(PRCP_W) << d[i].prcp << setw(WIND_W) << d[i].wind << setw(PRESS_W) << d[i].press << endl;
		}
	}
	PAUSE;
}

void f3(dataEntry_t * d, int c, date_t date)
{
	float high = 0;

	for (int i = 0; i < c; i++)
	{
		if (d[i].date.month == date.month && d[i].date.day == date.day && d[i].date.year == date.year)
		{
			if (d[i].temp > high) high = d[i].temp;
		}
	}
	cout << setw(ID_W) << "ID" << setw(MONTH_W) << "MM" << setw(DAY_W) << "DD" << setw(YEAR_W) << "YYYY" << setw(TEMP_W) << "TEEMP" << endl;
	for (int i = 0; i < c; i++)
	{
		if (d[i].date.month == date.month && d[i].date.day == date.day && d[i].date.year == date.year)
		{
			if (d[i].temp == high)
			{
				cout << setw(ID_W) << d[i].id << setw(MONTH_W) << d[i].date.month << setw(DAY_W) << d[i].date.day << setw(YEAR_W) << d[i].date.year << setw(TEMP_W) << d[i].temp << endl;
			}
		}
	}
	PAUSE;
}

void f4(dataEntry_t * d, int c, date_t date)
{
	float total = 0;

	for (int i = 0; i < c; i++)
	{
		if (d[i].date.month == date.month && d[i].date.day == date.day && d[i].date.year == date.year)
		{
			total += d[i].prcp;
		}
	}

	cout << "Total PRCP: " << total << endl;
	PAUSE;
}

void f5(dataEntry_t * d, int c, float alt)
{
	float min = 999;
	cout << setw(ID_W) << "ID" << setw(TEMP_W) << "TEMP" << endl;
	for (int i = 0; i < c; i++)
	{
		if (d[i].pos.alt >= alt)
		{
			if (d[i].temp < min) min = d[i].temp;
		}
	}
	for (int i = 0; i < c; i++)
	{
		if (d[i].pos.alt >= alt)
		{
			if (min == d[i].temp)
			{
				cout << setw(ID_W) << d[i].id << setw(TEMP_W) << d[i].temp << endl;
			}
		}
	}
	PAUSE;
}
