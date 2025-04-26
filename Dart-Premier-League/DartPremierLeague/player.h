#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <list>

class Player
{
public:
    std::string name;
    int id;

    //Stats
    int PDCWorldRanking;
    int TelevisedTitles;
    int NonTelevisedPDCTitles;
    int TelevisedNineDartFinishes;
    std::list<int> games;

    //DOB
    unsigned int DOBYear;
    unsigned int DOBMonth;
    unsigned int DOBDay;

    //Info
    std::string WalkOnMusic;
    std::string DartsUsed;
    std::string HomeTown;



};



#endif // PLAYER_H
