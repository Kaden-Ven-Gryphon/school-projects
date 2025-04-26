#include "dartgame.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <QFile>
#include <QTextStream>

using namespace std;

DartGame::DartGame()
{
    id = 0;
    gameWinner = -1;
    p1LegWins = 0;
    p1MatchWins = 0;
    p2LegWins = 0;
    p2MatchWins = 0;
    numberOfMatches = 0;
    numberOfLegsPerMatch = 0;
    legIndex = 0;
    matchIndex = 0;
    matches = nullptr;
    activeLeg = nullptr;
    location = "null";
    time = 0;
    year = 0;
    month = 0;

    p1num180s = 0;
    p1TurnAverage = 0;
    p1TurnCount = 0;
    p1LowestTurnScore = 9999;
    p2num180s = 0;
    p2TurnAverage = 0;
    p2TurnsCount = 0;
    p2LowestTurnScore = 9999;
}

DartGame::~DartGame()
{
    delete matches;
    delete activeLeg;
}

void DartGame::initGame(int legsNum, int matchNum, int startScore)
{
    if (legsNum == 0) legsNum = 1;
    if (matchNum == 0) matchNum =1;
    numberOfMatches = matchNum;
    numberOfLegsPerMatch = legsNum;


    matches = new Match[matchNum];
    for(int i = 0; i < matchNum; i++)
    {
        matches[i].Legs = new Leg*[legsNum];
        matches[i].matchNumber = i;
        matches[i].numberOfLegs = legsNum;

        for(int j = 0; j < legsNum; j++)
        {
            matches[i].Legs[j] = new Leg(player1, player2, id, j, i, startScore);
        }
    }
    activeLeg = matches[0].Legs[0];
}

void DartGame::submitTurn()
{
    int player = activeLeg->activePlayer;
    int score = activeLeg->submitTurn();

    if(player == 0)
    {
        int buf = p1TurnAverage * p1TurnCount;
        buf += score;
        p1TurnCount++;
        p1TurnAverage = buf / p1TurnCount;

        if(score == 180) p1num180s++;

        if(score < p1LowestTurnScore) p1LowestTurnScore = score;
    }
    else
    {
        int buf = p2TurnAverage * p2TurnsCount;
        buf += score;
        p2TurnsCount++;
        p2TurnAverage = buf / p2TurnsCount;

        if(score == 180) p2num180s++;

        if(score < p2LowestTurnScore) p2LowestTurnScore = score;
    }

    if(activeLeg->legWon)
    {
        if(activeLeg->legWinner == 0)
        {
            p1LegWins++;
            matches[matchIndex].p1LegWins = p1LegWins;
            legIndex++;
        }
        else
        {
            p2LegWins++;
            matches[matchIndex].p2LegWins = p2LegWins;
            legIndex++;
        }

        if(matches[matchIndex].p1LegWins > numberOfLegsPerMatch/2)
        {
            matches[matchIndex].matchWinner = 0;
            p1MatchWins++;
            matchIndex++;
            legIndex = 0;
            p1LegWins = 0;
            p2LegWins = 0;
        }
        else if (matches[matchIndex].p2LegWins > numberOfLegsPerMatch/2)
        {
            matches[matchIndex].matchWinner = 1;
            p2MatchWins++;
            matchIndex++;
            legIndex = 0;
            p1LegWins = 0;
            p2LegWins = 0;
        }
        else if (matches[matchIndex].p1LegWins == matches[matchIndex].p2LegWins && matches[matchIndex].p1LegWins + matches[matchIndex].p2LegWins == numberOfLegsPerMatch)
        {
            matches[matchIndex].matchWinner = 2;
            p2MatchWins++;
            matchIndex++;
            legIndex = 0;
            p2LegWins = 0;
        }

        if(p1MatchWins > numberOfMatches/2)
        {
            gameWinner = 0;
        }
        else if (p2MatchWins > numberOfMatches/2)
        {
            gameWinner = 1;
        }
        else if (p1MatchWins == p2MatchWins && p1MatchWins + p2MatchWins == numberOfMatches)
        {
            gameWinner = 2;
        }

        if(gameWinner == -1)
        {
            activeLeg = (matches[matchIndex].Legs[legIndex]);
        }
        else
        {
            //GAME WON
            activeLeg = new Leg(player1, player2, id, 0, 0, 999);
        }


    }
}

void DartGame::printDartHistory()
{
    QFile outFile("dart_history.txt");
    if(!outFile.open(QIODevice::WriteOnly))
    {
        qCritical()<<"Could not open file: ";
        qCritical() << outFile.errorString();
        return;
    }

    QTextStream stream(&outFile);

    int i;
    int j;
    int k;

    // write darthistory to the output file

    // For every match
    for (i =0; i<numberOfMatches; i++)
    {
        // For each leg in each match
        for (j = 0; j < numberOfLegsPerMatch; j++)
        {
            // For each dartThrow in the vector dartHistory
            for (k = 0; k < matches[i].Legs[j]->dartHistory.size(); k++)
            {
                stream << QString::fromStdString(matches[i].Legs[j]->dartHistory[k].printThrow());
            }
        }
    }
    outFile.close();
}
