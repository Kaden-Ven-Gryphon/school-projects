#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#define PAUSE cout <<endl; system("Pause"); cout << endl;			//system pause

#define TEMP_SPACE "    "											//formating defines

using namespace std;

float tempCtoF(float);

int main(void)
{
	int prcpPos = 0;												//varibles for the pos of the entry types
	int tmaxPos = 0;
	int tminPos = 0;
	int stationPos = 0;
	int datePos = 0;
	int mdprPos = 0;
	int snwdPos = 0;
	int awndPos = 0;

	int count = 0;													//counters for the number of valid entries
	int totalCount = 0;
	int nonNorthCount = 0;

	string buf;														//buffer variables to prosses infupts
	string buf2;
	float temp = 0;

	string inLine;													//file managers
	ifstream infp;
	ofstream outfp;

	infp.open("AL_Weather_Station.txt");							//open in file
	if (!infp)														//check for error
	{
		cout << "ERROR: File failed to open\n\n";
		system("pause");
		return -1;
	}
	cout << "Input file opened\n";

	outfp.open("Filtered_AL_Weather_Station.txt");					//open out file
	if (!outfp)														//check for error
	{
		cout << "ERROR: File failed to open\n\n";
		system("pause");
		return -1;
	}
	cout << "Output file opened\n";

	getline(infp, inLine);											//get the first line and find the pos of the desiered values
	prcpPos = inLine.find("PRCP");
	tmaxPos = inLine.find("TMAX");
	tminPos = inLine.find("TMIN");
	datePos = inLine.find("DATE");
	stationPos = inLine.find("STATION_NAME");
	mdprPos = inLine.find("MDPR");
	snwdPos = inLine.find("SNWD");
	awndPos = inLine.find("AWND");

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

	if (datePos == string::npos)									//here down are pos's for problem 2
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
	}														

	if (mdprPos == string::npos)
	{
		cout << "ERROR failed to find MDPR pos.\n\n";
		system("pause");
		return -1;
	}															

	if (snwdPos == string::npos)
	{
		cout << "ERROR failed to find SNWD pos.\n\n";
		system("pause");
		return -1;
	}											

	if (awndPos == string::npos)
	{
		cout << "ERROR failed to find AWND pos.\n\n";
		system("pause");
		return -1;
	}																//END of check

	//#########################################################		Problem 1      ###################################

	outfp << inLine << endl;

	getline(infp, inLine);											//clear the header line
	outfp << inLine<< endl;

	while (!infp.eof())												//loop through the remaining file
	{
		bool good = true;
		getline(infp, inLine);

		buf = inLine.substr(prcpPos, 5);							//if any data is bad mark flag
		if (buf == "-9999") good = false;
		buf = inLine.substr(tmaxPos, 5);
		if (buf == "-9999") good = false;
		buf = inLine.substr(tminPos, 5);
		if (buf == "-9999") good = false;

		totalCount++;

		if (good)													//if data is good print it in the new file
		{
			outfp << inLine <<endl;
			count++;
		}
	}//while !eof
	outfp.clear();
	infp.clear();
	infp.close();													//close files
	outfp.close();

	cout << "Data filtererd\n";
	cout << count << " good data entries\n\n";
	PAUSE

	//#################################################################   Problem 2   ##################################

	infp.open("Filtered_AL_Weather_Station.txt");					//open in file
	if (!infp)														//check for error
	{
		cout << "ERROR: File failed to open\n\n";
		system("pause");
		return -1;
	}
	cout << "Input file opened\n";

	outfp.open("weather_station_five_column.txt");					//open out file
	if (!outfp)														//check for error
	{
		cout << "ERROR: File failed to open\n\n";
		system("pause");
		return -1;
	}
	cout << "Output file opened\n";

	infp.clear();													//clear eof status from problem 1
	infp.seekg(0, std::ios::beg);

	outfp << setprecision(2) << fixed;								//format floats outputs
	outfp << showpoint;
	
	while (getline(infp, inLine))									//pull out to alues that are desiered for the 5 colomes
	{
		static int lineCount = 1;										

		buf = inLine.substr(stationPos, (datePos - stationPos));
		outfp << buf;

		buf = inLine.substr(datePos, (mdprPos-datePos));
		if (lineCount < 3)											//for the first two line leave it alone
		{
			outfp << buf <<"   ";
		}
		else														//convert the date to YEAR MM DD
		{
			buf2 = buf.substr(0, 4);
			outfp << buf2 << " ";
			buf2 = buf.substr(4, 2);
			outfp << buf2 << " ";
			buf2 = buf.substr(6, 2);
			outfp << buf2 << "  ";
		}
		
		buf = inLine.substr(prcpPos, (snwdPos-prcpPos));			//pull and place percipitation
		outfp << buf;

		buf = inLine.substr(tmaxPos, (tminPos - tmaxPos));			//take as is the first 2
		if (lineCount < 3)											//convert to F for the rest
		{
			outfp << buf;
		}
		else
		{
			temp = stof(buf);
			temp = tempCtoF(temp);
			outfp << temp << TEMP_SPACE;
		}
		
		buf = inLine.substr(tminPos, (awndPos - tminPos));			//take at is the first 2
		if (lineCount < 3)											//convert to F for the rest
		{
			outfp << buf;
		}
		else
		{
			temp = stof(buf);
			temp = tempCtoF(temp);
			
			outfp << temp;
		}
		outfp << endl;												//end the line
		lineCount++;
	}//while !eof

	infp.clear();
	outfp.clear();
	infp.close();													//close files
	outfp.close();

	cout << "Five column file finshed" << endl;
	PAUSE
	
//##############################################################	Problem 4   ##################################
	
	infp.open("AL_Weather_Station.txt");							//open in file
	if (!infp)														//check for error
	{
		cout << "ERROR: File failed to open\n\n";
		system("pause");
		return -1;
	}
	cout << "Input file opened\n";

	outfp.open("North_AL_Station_Data.txt");						//open out file
	if (!outfp)														//check for error
	{
		cout << "ERROR: File failed to open\n\n";
		system("pause");
		return -1;
	}
	cout << "Output file opened\n";

	getline(infp, inLine);
	outfp << inLine << endl;
	getline(infp, inLine);
	outfp << inLine << endl;

	while (getline(infp, inLine))
	{
		if (inLine.find("ATHENS") != string::npos) outfp << inLine << endl;
		else if (inLine.find("DECATUR") != string::npos) outfp << inLine << endl;
		else if (inLine.find("CULLMAN") != string::npos) outfp << inLine << endl;
		else if (inLine.find("OWENS CROSS ROADS") != string::npos) outfp << inLine << endl;
		else if (inLine.find("MUSCLE SHOALS") != string::npos) outfp << inLine << endl;
		else if (inLine.find("SCOTTSBORO") != string::npos) outfp << inLine << endl;
		else if (inLine.find("HUNTSVILLE") != string::npos) outfp << inLine << endl;
		else nonNorthCount++;
	}

	cout << "North AL file made" << endl;
	cout << "Number of North entries: " << (totalCount - nonNorthCount) << endl;

	infp.clear();
	outfp.clear();
	infp.close();
	outfp.close();

	PAUSE

	return 0;
}//main


//##############################################################	Functions   #######################################################
float tempCtoF(float c)
{
	float f = 0;
	c /= 10;
	f = (float)(c* (9.0 / 5.0)) + 32.0;
	return f;
}//tempCtoF
