#include "darttable.h"

// this object stores a vector of strings that correspond to the optimal 3 darts
// a player can throw in a darts match. index 0 corresponds to 60 points, 1 corresponds to 61, etc...
DartTable::DartTable() : table {
                             "D1",
                             "1 D1",
                             "D2",
                             "1 D2",
                             "D3",
                             "3 D2",
                             "D4",
                             "1 D4",
                             "D5",
                             "3 D4",
                             "D6",
                             "5 D4",
                             "D7",
                             "7 D4",
                             "D8",
                             "1 D8",
                             "D9",
                             "3 D8",
                             "D10",
                             "5 D8",
                             "D11",
                             "7 D8",
                             "D12",
                             "9 D8",
                             "D13",
                             "11 D8",
                             "D14",
                             "13 D8",
                             "D15",
                             "15 D8",
                             "D16",
                             "17 D8",
                             "D17",
                             "3 D16",
                             "D18",
                             "5 D16",
                             "D19",
                             "7 D16",
                             "D20",
                             "9 D16",
                             "10 D16",
                             "11 D16",
                             "12 D16",
                             "13 D16",
                             "6 D20",
                             "15 D16",
                             "16 D16",
                             "17 D16",
                             "18 D16",
                             "19 D16",
                             "20 D16",
                             "13 D20",
                             "14 D20",
                             "15 D20",
                             "16 D20",
                             "17 D20",
                             "18 D20",
                             "19 D20",
                             "20 D20",
                             "T15 D8",
                             "T10 D16",
                             "T13 D12",
                             "T16 D8",
                             "T19 D4",
                             "T10 D18",
                             "T17 D8",
                             "T20 D4",
                             "T15 D12",
                             "T10 D20",
                             "T13 D16",
                             "T16 D12",
                             "T19 D8",
                             "T14 D16",
                             "T17 D12",
                             "T20 D8",
                             "T19 D10",
                             "T18 D12",
                             "T19 D11",
                             "T20 D10",
                             "T19 D12",
                             "T14 D20",
                             "T17 D16",
                             "T20 D12",
                             "T15 D20",
                             "T18 D16",
                             "T17 D18",
                             "T16 D20",
                             "T19 D16",
                             "T20 D15",
                             "T17 D20",
                             "T20 D16",
                             "T19 D18",
                             "T18 D20",
                             "T19 D19",
                             "T20 T18",
                             "T19 D20",
                             "T20 D19",
                             "T19 10 D16",
                             "T20 D20",         // 100
                             "T20 1 D20",
                             "T20 10 D16",
                             "T20 3 D20",
                             "T18 18 D16",
                             "T19 16 D16",
                             "T20 14 D16",
                             "T19 18 D16",
                             "T20 16 D16",
                             "T19 20 D16",
                             "T20 18 D16",
                             "T20 19 D16",
                             "T20 12 D20",
                             "T20 13 D20",
                             "T20 14 D20",
                             "T20 15 D20",
                             "T20 16 D20",
                             "T20 17 D20",
                             "T20 18 D20",
                             "T19 T10 D16",
                             "T20 20 D20",
                             "T17 T10 D20",
                             "T18 T20 D4",
                             "T19 T16 D9",
                             "T20 T16 D8",
                             "25 T20 D20",
                             "T19 T19 D6",
                             "T20 T17 D8",
                             "T18 T14 D16",
                             "T19 T16 D12",
                             "T20 T20 D5",
                             "T20 T13 D16",
                             "T20 T16 D12",
                             "T20 T19 D8",
                             "T20 T14 D16",
                             "T20 T17 D12",
                             "T20 T20 D8",
                             "T19 T16 D16",
                             "T20 T18 D12",
                             "T19 T14 D20",
                             "T20 T16 D16",
                             "T20 T19 D12",
                             "T20 T14 D20",
                             "T20 T17 D16",
                             "T20 T20 D12",
                             "T20 T15 D20",
                             "T20 T18 D16",
                             "T20 T17 D18",
                             "T20 T16 D20",
                             "T20 T19 D16",
                             "T20 T18 D18",
                             "T20 T17 D20",
                             "T20 T20 D16",
                             "T20 T19 D18",
                             "T20 T18 D20",
                             "T20 T19 D19",
                             "T20 T20 D18",
                             "T20 T19 D20",
                             "T20 T20 D19",
                             "No out shot",
                             "T20 T20 D20",
                             "T20 T17 Bull",
                             "No out shot",
                             "No out shot",
                             "T20 T18 Bull",
                             "No out shot",
                             "No out shot",
                             "T20 T19 Bull",
                             "No out shot",
                             "No out shot",
                             "T20 T20 BUll"
                             }
{
}

// this function takes an integer value
// If the value is between 60 and 170, it will return a string for the optimal 3 dart out
// with a double value finish
// if the value passed isn't in range, it will return "No out shot"
string DartTable::getDartString(int val, int dartsThrown)
{
    if(val <= 1)
        return "";

    if(dartsThrown == 0)    //three darts remaining
    {
        if (val >= 2 && val <= 170)
        {
            return table[val-2];
        }
        else
        {
            return "No out shot";
        }
    }
    else if(dartsThrown == 1)   //two darts remaining
    {
        if(val >= 2 && val <= 98)
        {
            return table[val-2];
        }
        else if(val == 100)
        {
            return "T20 D20";
        }
        else
        {
            return "No out shot";
        }
    }
    else
    {
        return "";
    }
}
