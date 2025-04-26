#ifndef DARTTABLE_H
#define DARTTABLE_H
#include <string>
#include <vector>

using namespace std;

class DartTable
{
public:
    DartTable();
    std::vector<std::string> table;
    string getDartString(int val, int dartsThrown);
};

#endif // DARTTABLE_H
