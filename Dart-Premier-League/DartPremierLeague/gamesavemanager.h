#ifndef GAMESAVEMANAGER_H
#define GAMESAVEMANAGER_H
#include <string>
#include <list>
#include "dartgame.h"

class GameSaveManager
{
    std::list<DartGame> games;
    std::string dirPath;

    void loadData();
    void saveData();
    void addGame();
};

#endif // GAMESAVEMANAGER_H
