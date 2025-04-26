#ifndef PLAYERPARSER_H
#define PLAYERPARSER_H
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

// ------------------------------------------------------------------------------------
// struct: PlayerRecord
// Purpose: allows values to be read in from the csv file and placed into records
// ------------------------------------------------------------------------------------

struct PlayerRecord
{
    std::string lastName;
    std::string firstName;
    std::string nickName;
    std::string average;
    std::string nineDarters;
    std::string winPercent;
    std::string ranking;
    std::string total180s;

};



// ------------------------------------------------------------------------------------
// Class: PlayerParser
// Purpose: takes a csv file input, stores the contents into a vector of player records
// ------------------------------------------------------------------------------------
class PlayerParser
{

public:
    PlayerParser(std::string filePath);
    std::string getNickName(std::string playerLastName, std::string playerFirstName);
    std::string getAverage(std::string playerLastName, std::string playerFirstName);
    std::string getNineDarters(std::string playerLastName, std::string playerFirstName);
    std::string getWinPercent(std::string playerLastName, std::string playerFirstName);
    std::string getRanking(std::string playerLastName, std::string playerFirstName);
    std::string getTotal180s(std::string playerLastName, std::string playerFirstName);
    int getRecordsSize();
    void printRecs();
    std::vector<PlayerRecord> players;
};

#endif // PLAYERPARSER_H
