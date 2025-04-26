#ifndef DARTGAME_H
#define DARTGAME_H
#include <string>

#include "player.h"
#include "match.h"
#include "leg.h"


class DartGame
{
public:
    int id;

    Player player1;
    Player player2;

    int gameWinner;

    int p1LegWins;
    int p2LegWins;
    int p1MatchWins;
    int p2MatchWins;

    int numberOfMatches;
    int numberOfLegsPerMatch;
    int legIndex;
    int matchIndex;
    Match* matches;
    Leg* activeLeg;

    std::string location;
    unsigned int time;
    unsigned int year;
    unsigned int month;
    unsigned int day;

    //P1 game stats
    int p1num180s;
    int p1TurnAverage;
    int p1TurnCount;
    int p1LowestTurnScore;
    //P2 game stats
    int p2num180s;
    int p2TurnAverage;
    int p2TurnsCount;
    int p2LowestTurnScore;



    DartGame();
    ~DartGame();
    void initGame(int legsNum, int matchNum, int startScore);
    void submitTurn();
    void printDartHistory();

};

#endif // DARTGAME_H
