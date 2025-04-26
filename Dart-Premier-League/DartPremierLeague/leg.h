#ifndef LEG_H
#define LEG_H
#include <string>
#include <list>
#include <vector>
#include <QDebug>
#include "dartthrow.h"
#include "player.h"
#include "darttable.h"

class Leg
{
public:
    Player players[2];
    int legWinner;
    bool legWon;
    int gameID;
    int legNumber;
    int matchNumber;
    int playerOneScore;
    int playerOneEffectiveScore;
    int playerTwoScore;
    int playerTwoEffectiveScore;
    int activePlayer;
    int dartsThrown;
    DartThrow activeThreeDarts[3];
    std::vector<DartThrow> dartHistory;
    DartTable lookupTable;

    //P1 leg stats

    //P2 leg stats

    Leg();
    Leg(Player p1, Player p2, int gameID, int legNumber, int matchNumber, int startScore);

    int submitTurn();
    void throwDart(int boardLocation);
    void knockOut(int i);
    void resetDart(int i);
    std::string calc3DartOut(int points);
    int calcEffectiveScore(int score);
    std::string ToString();


};

#endif // LEG_H
