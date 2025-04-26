#ifndef DARTTHROW_H
#define DARTTHROW_H
#include <string>
class DartThrow
{
public:
    std::string playerName;
    unsigned int playerID;
    unsigned int gameID;
    std::string boardHit;
    int points;
    int leg;
    int match;

    DartThrow();
    void clear();
    std::string printThrow();
};

#endif // DARTTHROW_H
