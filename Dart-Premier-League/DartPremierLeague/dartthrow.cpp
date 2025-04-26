#include "dartthrow.h"

using namespace std;

DartThrow::DartThrow()
{
    playerName = "";
    playerID = 0;
    gameID = 0;
    boardHit = "";
    points = 0;
    leg = 0;
    match = 0;
}

void DartThrow::clear()
{
    playerName = "";
    playerID = 0;
    gameID = 0;
    boardHit = "";
    points = 0;
    leg = 0;
    match = 0;
}

std::string DartThrow::printThrow()
{
    return playerName + "   ID: " + to_string(playerID) + "   Game ID: " + to_string(gameID) + "   Leg: " + to_string(leg)
            + "   Match: " + to_string(match) + "   Board Hit: " + boardHit + "\n";
}
