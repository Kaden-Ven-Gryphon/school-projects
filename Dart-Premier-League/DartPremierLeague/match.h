#ifndef MATCH_H
#define MATCH_H
#include "leg.h"

class Match
{
public:
    int matchNumber;
    int numberOfLegs;
    Leg** Legs;

    int matchWinner;

    //P1 match stats
    int p1LegWins;
    //P2 match stats
    int p2LegWins;
};

#endif // MATCH_H
