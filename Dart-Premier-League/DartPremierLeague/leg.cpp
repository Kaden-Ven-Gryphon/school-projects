#include "leg.h"

Leg::Leg()
{
    legWinner = -1;
    legWon = false;
    activePlayer = 0;
    dartsThrown = 0;
}

Leg::Leg(Player p1, Player p2, int gameID, int legNumber, int matchNumber, int startScore)
{
    players[0] = p1;
    players[1] = p2;
    legWinner = -1;
    legWon = false;
    this->gameID = gameID;
    this->legNumber = legNumber;
    this->matchNumber = matchNumber;
    playerOneScore = startScore;
    playerTwoScore = startScore;
    playerOneEffectiveScore = startScore;
    playerTwoEffectiveScore = startScore;
    activePlayer = 0;
    dartsThrown = 0;

}

//adds the three active darts to the history
//update the players scores
//updates the player stats
//resets the three darts
//switches active player
int Leg::submitTurn()
{
    int retval = 0;
    if(activePlayer == 0)
    {
        retval = playerOneScore - playerOneEffectiveScore;
        if(playerOneEffectiveScore == 0)
        {
            playerOneScore = playerOneEffectiveScore;
            legWinner = 0;
            legWon = true;
            //PLAYER WINS LEG
        }
        else if(playerOneEffectiveScore < 2)
        {
            //PLAYER OVER SHOOTS
        }
        else
        {
            playerOneScore = playerOneEffectiveScore;
        }

        playerOneEffectiveScore = playerOneScore;
        activePlayer = 1;
    }
    else
    {
        retval = playerTwoScore - playerTwoEffectiveScore;
        if(playerTwoEffectiveScore == 0)
        {
            playerTwoScore = playerTwoEffectiveScore;
            legWinner = 1;
            legWon = true;
            //PLAYER WINS LEG
        }
        else if(playerTwoEffectiveScore < 2)
        {
            //PLAYER OVER SHOOTS
        }
        else
        {
            playerTwoScore = playerTwoEffectiveScore;
        }

        playerTwoEffectiveScore = playerTwoScore;
        activePlayer = 0;
    }



    for (int i = 0; i < dartsThrown; i++)
    {
        dartHistory.push_back(activeThreeDarts[i]);
    }
    dartsThrown = 0;

    activeThreeDarts[0].clear();
    activeThreeDarts[1].clear();
    activeThreeDarts[2].clear();


    return retval;
}

//This is called by clicking on the dartboard
//adds dart to the threedarts thrown
//updates the players effective score
void Leg::throwDart(int boardLocation)
{
    if (dartsThrown >= 3) return;               //if all three dartshave been thrown ignore it
    DartThrow dt;
    dt.playerName = players[activePlayer].name; //set the data on the dart throw
    dt.playerID = players[activePlayer].id;
    dt.gameID = gameID;

    switch (boardLocation)
    {
    case 0:
        dt.boardHit = "Foul";
        dt.points = 0;
        break;
    case 1:
        dt.boardHit = "D20";
        dt.points = 40;
        break;
    case 2:
        dt.boardHit = "D1";
        dt.points = 2;
        break;
    case 3:
        dt.boardHit = "D18";
        dt.points = 36;
        break;
    case 4:
        dt.boardHit = "D4";
        dt.points = 8;
        break;
    case 5:
        dt.boardHit = "D13";
        dt.points = 26;
        break;
    case 6:
        dt.boardHit = "D6";
        dt.points = 12;
        break;
    case 7:
        dt.boardHit = "D10";
        dt.points = 20;
        break;
    case 8:
        dt.boardHit = "D15";
        dt.points = 30;
        break;
    case 9:
        dt.boardHit = "D2";
        dt.points = 4;
        break;
    case 10:
        dt.boardHit = "D17";
        dt.points = 34;
        break;
    case 11:
        dt.boardHit = "D3";
        dt.points = 6;
        break;
    case 12:
        dt.boardHit = "D19";
        dt.points = 38;
        break;
    case 13:
        dt.boardHit = "D7";
        dt.points = 14;
        break;
    case 14:
        dt.boardHit = "D16";
        dt.points = 32;
        break;
    case 15:
        dt.boardHit = "D8";
        dt.points = 16;
        break;
    case 16:
        dt.boardHit = "D11";
        dt.points = 22;
        break;
    case 17:
        dt.boardHit = "D14";
        dt.points = 28;
        break;
    case 18:
        dt.boardHit = "D9";
        dt.points = 18;
        break;
    case 19:
        dt.boardHit = "D12";
        dt.points = 24;
        break;
    case 20:
        dt.boardHit = "D5";
        dt.points = 10;
        break;
    case 41:
        dt.boardHit = "T20";
        dt.points = 60;
        break;
    case 42:
        dt.boardHit = "T1";
        dt.points = 3;
        break;
    case 43:
        dt.boardHit = "T18";
        dt.points = 54;
        break;
    case 44:
        dt.boardHit = "T4";
        dt.points = 12;
        break;
    case 45:
        dt.boardHit = "T13";
        dt.points = 39;
        break;
    case 46:
        dt.boardHit = "T6";
        dt.points = 18;
        break;
    case 47:
        dt.boardHit = "T10";
        dt.points = 30;
        break;
    case 48:
        dt.boardHit = "T15";
        dt.points = 45;
        break;
    case 49:
        dt.boardHit = "T2";
        dt.points = 6;
        break;
    case 50:
        dt.boardHit = "T17";
        dt.points = 51;
        break;
    case 51:
        dt.boardHit = "T3";
        dt.points = 9;
        break;
    case 52:
        dt.boardHit = "T19";
        dt.points = 57;
        break;
    case 53:
        dt.boardHit = "T7";
        dt.points = 21;
        break;
    case 54:
        dt.boardHit = "T16";
        dt.points = 48;
        break;
    case 55:
        dt.boardHit = "T8";
        dt.points = 24;
        break;
    case 56:
        dt.boardHit = "T11";
        dt.points = 33;
        break;
    case 57:
        dt.boardHit = "T14";
        dt.points = 42;
        break;
    case 58:
        dt.boardHit = "T9";
        dt.points = 36;
        break;
    case 59:
        dt.boardHit = "T12";
        dt.points = 36;
        break;
    case 60:
        dt.boardHit = "T5";
        dt.points = 15;
        break;
    case 21:
    case 61:
        dt.boardHit = "20";
        dt.points = 20;
        break;
    case 22:
    case 62:
        dt.boardHit = "1";
        dt.points = 1;
        break;
    case 23:
    case 63:
        dt.boardHit = "18";
        dt.points = 18;
        break;
    case 24:
    case 64:
        dt.boardHit = "4";
        dt.points = 4;
        break;
    case 25:
    case 65:
        dt.boardHit = "13";
        dt.points = 13;
        break;
    case 26:
    case 66:
        dt.boardHit = "6";
        dt.points = 6;
        break;
    case 27:
    case 67:
        dt.boardHit = "10";
        dt.points = 10;
        break;
    case 28:
    case 68:
        dt.boardHit = "15";
        dt.points = 15;
        break;
    case 29:
    case 69:
        dt.boardHit = "2";
        dt.points = 2;
        break;
    case 30:
    case 70:
        dt.boardHit = "17";
        dt.points = 17;
        break;
    case 31:
    case 71:
        dt.boardHit = "3";
        dt.points = 3;
        break;
    case 32:
    case 72:
        dt.boardHit = "19";
        dt.points = 19;
        break;
    case 33:
    case 73:
        dt.boardHit = "7";
        dt.points = 7;
        break;
    case 34:
    case 74:
        dt.boardHit = "16";
        dt.points = 16;
        break;
    case 35:
    case 75:
        dt.boardHit = "8";
        dt.points = 8;
        break;
    case 36:
    case 76:
        dt.boardHit = "11";
        dt.points = 11;
        break;
    case 37:
    case 77:
        dt.boardHit = "14";
        dt.points = 14;
        break;
    case 38:
    case 78:
        dt.boardHit = "9";
        dt.points = 9;
        break;
    case 39:
    case 79:
        dt.boardHit = "12";
        dt.points = 12;
        break;
    case 40:
    case 80:
        dt.boardHit = "5";
        dt.points = 5;
        break;
    case 81:
        dt.boardHit = "Outer Bull";
        dt.points = 25;
        break;
    case 82:
        dt.boardHit = "Bull";
        dt.points = 50;
        break;
    }



    dt.leg = legNumber;                         //add more dart data
    dt.match = matchNumber;


    activeThreeDarts[dartsThrown++] = dt;       //increment the darts thrown

    if(activePlayer == 0)                       //calc the effective scores
    {
        playerOneEffectiveScore = calcEffectiveScore(playerOneScore);
    }
    else
    {
        playerTwoEffectiveScore = calcEffectiveScore(playerTwoScore);
    }

    return;
}

void Leg::knockOut(int i)
{
    // If(dart!= empty){
    if(activeThreeDarts[i].boardHit!=""){
        activeThreeDarts[i].boardHit = "Knock Out";
        activeThreeDarts[i].points = 0;
        if(activePlayer == 0)                       //calc the effective scores
        {
            playerOneEffectiveScore = calcEffectiveScore(playerOneScore);
        }
        else
        {
            playerTwoEffectiveScore = calcEffectiveScore(playerTwoScore);
        }
    }
    else{
     qDebug()<<" dart empty no knockout";
    }

}

void Leg::resetDart(int i)
{
    if(i >= dartsThrown || dartsThrown <= 0) return;
    qDebug() << "reset func " << i;
    dartsThrown--;
    activeThreeDarts[i].clear();
    while (i < dartsThrown)
    {
        activeThreeDarts[i] = activeThreeDarts[i+1];
        activeThreeDarts[i+1].clear();
        i++;
    }
    if(activePlayer == 0)                       //calc the effective scores
    {
        playerOneEffectiveScore = calcEffectiveScore(playerOneScore);
    }
    else
    {
        playerTwoEffectiveScore = calcEffectiveScore(playerTwoScore);
    }
}

// returns a string value for the optimal three darts to be thrown to win the leg
// index 0 of lookup table corresponds to the point value of 60
std::string Leg::calc3DartOut(int points)
{
    return lookupTable.getDartString(points, dartsThrown);
}


//subracts the current three darts points from input
int Leg::calcEffectiveScore(int score)
{
    int retScore = score;
    for(int i = 0; i < dartsThrown; i++)
    {
        retScore -= activeThreeDarts[i].points;
    }
    return retScore;
}

