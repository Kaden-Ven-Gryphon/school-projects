#ifndef MAINMANAGER_H
#define MAINMANAGER_H
#include "dartgame.h"
#include "gamesavemanager.h"
#include "playersavemanager.h"

class MainManager
{
public:
    static DartGame mainGame;
    void startNewGame();
    void saveGame();
    void viewPlayerData();
    void viewGameData();
};

#endif // MAINMANAGER_H
