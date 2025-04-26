#ifndef PLAYERSAVEMANAGER_H
#define PLAYERSAVEMANAGER_H
#include <string>
#include "player.h"
#include "playerparser.h"

class PlayerSaveManager
{
public:
    PlayerSaveManager();

    std::list<Player> players;
    std::string dirPath;

    void loadData();
    void saveData();

    void addPlayer();
    void editPlayer();
};

#endif // PLAYERSAVEMANAGER_H
