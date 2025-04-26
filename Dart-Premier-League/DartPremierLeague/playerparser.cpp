#include "playerparser.h"
using namespace std;
// ---------------------------------------------------------------------------------------------------------------
// Default constructor for PlayerParser
// param is a data file containing canned info about certain players that will be placed into a vector of records
// Local File Path: C:/Users/prest/Desktop/player_stats_csv.csv
// ---------------------------------------------------------------------------------------------------------------
PlayerParser::PlayerParser(string filePath)
{
    ifstream playerFile;
    playerFile.open(filePath);

    // make sure the file opens
    if (!playerFile)
        cout << "Unable to open file" << endl;
    // if it opens, begin reading in the data
    else
    {
        cout << "File opened" << endl;
        string lineContents;
        while(!playerFile.eof())
        {
            PlayerRecord r;

            getline(playerFile, lineContents, ',');
            r.lastName = lineContents;

            getline(playerFile, lineContents, ',');
            r.firstName = lineContents;

            getline(playerFile, lineContents, ',');
            r.nickName = lineContents;

            getline(playerFile, lineContents, ',');
            r.average = lineContents;

            getline(playerFile, lineContents, ',');
            r.nineDarters = lineContents;

            getline(playerFile, lineContents, ',');
            r.winPercent = lineContents;

            getline(playerFile, lineContents, ',');
            r.ranking = lineContents;

            getline(playerFile, lineContents, '\n');
            r.total180s = lineContents;

            players.push_back(r);
        }
    }

    // close the input file
    playerFile.close();
}


// ------------------------------------------------------
// return the string value for a player's nickname
// parameters are a players last and first name
// ------------------------------------------------------
string PlayerParser::getNickName(string playerLastName, string playerFirstName)
{
    string nickName = "";

    // iterate through the vector of records
    for (auto i : players)
    {
        // determine which record matches the first and last name entered from the parameter
        if(playerLastName.compare(i.lastName) == 0 && playerFirstName.compare(i.firstName) == 0)
        {
           nickName = i.nickName;
        }
    }

    return nickName;
}



// ----------------------------------------------------------
// return the string value for a player's average
// parameters are strings of a player's first and last name
// ----------------------------------------------------------
string PlayerParser::getAverage(string playerLastName, string playerFirstName)
{
    string average = "";

    // iterate through the vector of records
    for (auto i : players)
    {
        // determine which record matches the first and last name entered from the parameter
        if(playerLastName.compare(i.lastName) == 0 && playerFirstName.compare(i.firstName) == 0)
        {
           average = i.average;
        }
    }

    return average;
}



// -------------------------------------------------------------------------
// return the string value for the career nine darters for a player
// parameters are a string for a players first and last name
// -------------------------------------------------------------------------
string PlayerParser::getNineDarters(string playerLastName, string playerFirstName)
{
    string nineDarters = "";

    // iterate through the vector of records
    for (auto i : players)
    {
        // determine which record matches the first and last name entered from the parameter
        if(playerLastName.compare(i.lastName) == 0 && playerFirstName.compare(i.firstName) == 0)
        {
           nineDarters = i.nineDarters;
        }
    }

    return nineDarters;
}



// ----------------------------------------------------------------
// return the string value for win percentage
// paremeters are a player's last and first name
// ----------------------------------------------------------------
string PlayerParser::getWinPercent(string playerLastName, string playerFirstName)
{
    string winPerc = "";

    // iterate through the vector of records
    for (auto i : players)
    {
        // determine which record matches the first and last name entered from the parameter
        if(playerLastName.compare(i.lastName) == 0 && playerFirstName.compare(i.firstName) == 0)
        {
           winPerc = i.winPercent;
        }
    }

    return winPerc;
}



// -------------------------------------------
// return the string value of a player's rank
// params are a player's last and first name
// -------------------------------------------
string PlayerParser::getRanking(string playerLastName, string playerFirstName)
{
    string rank = "";

    // iterate through the vector of records
    for (auto i : players)
    {
        // determine which record matches the first and last name entered from the parameter
        if(playerLastName.compare(i.lastName) == 0 && playerFirstName.compare(i.firstName) == 0)
        {
           rank = i.ranking;
        }
    }

    return rank;
}



// -------------------------------------------------------
// return the string value of a player's lifetime 180s
// params are a player's last and first name
// -------------------------------------------------------
string PlayerParser::getTotal180s(string playerLastName, string playerFirstName)
{
    string total = "";

    // iterate through the vector of records
    for (auto i : players)
    {
        // determine which record matches the first and last name entered from the parameter
        if(playerLastName.compare(i.lastName) == 0 && playerFirstName.compare(i.firstName) == 0)
        {
           total = i.total180s;
        }
    }

    return total;
}

int PlayerParser::getRecordsSize()
{
    return players.size();
}



// --------------------------------------------------------------------------------
// print the vector of player records to the screen to verify they are correct
// --------------------------------------------------------------------------------
void PlayerParser::printRecs()
{
    for (auto i : players)
    {
        cout << i.lastName << ", " << i.firstName << " aka " << i.nickName
             << endl << "Average: " << i.average
             << endl << "Nine Darters: " << i.nineDarters
             << endl << "Win Percentage: " << i.winPercent << "%"
             << endl << "DDB Rank: " << i.ranking
             << endl << "Lifetime 180s: " << i.total180s
             << endl << endl;
    }
}
