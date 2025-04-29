// Project2.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Author:  Kaden Beaver
// Date:    04.08.2021

#include <iostream>
#include <fstream>>
#include <string>


using namespace std;

struct cell_t {
    int weight;
    int root;
};

string treeToString(cell_t** t, int i, int j, int spaceNeeded);

int main()
{
    ifstream fpIN;
    ofstream fpOUT;

    int count = 0;
    int* weights;
    int* keys;

    cell_t** table;
    

    int x;
    int y;
    int resetY;


    fpIN.open("input.txt");

    fpIN >> count;

    weights = new int[count];
    keys = new int[count];
    table = new cell_t * [count + 2];

    for (int i = 0; i < count; i++)
    {
        fpIN >> weights[i];
        keys[i] = 1 + i;
        
    }

    fpIN.close();

    
    for (int i = 0; i < count + 2; i++)
    {
        table[i] = new cell_t[count + 1];
    }


    for (int i = 0; i < count + 2; i++)
    {
        for (int j = 0; j < count + 1; j++)
        {
            table[i][j].weight = 999;
            table[i][j].root = 99;
        }
    }

    for (int i = 0; i < count+1; i++)
    {
        table[i + 1][i].weight = 0;
        table[i+1][i+1].weight = weights[i];
        table[i+1][i+1].root = keys[i];
    }

    x = 1;
    y = 2;
    resetY = 3;

    while (y < count + 1)
    {
        while (y<count+1)
        {
            int minWeight = 1001;
            int minRoot = 0;
            int weightSum = 0;
            for (int k = x; k < y + 1; k++)
            {
                weightSum += weights[k - 1];

                int tempSum = table[x][k - 1].weight + table[k + 1][y].weight;
                if (tempSum < minWeight)
                {
                    minWeight = tempSum;
                    minRoot = k;
                }
            }
            
            table[x][y].weight = weightSum + minWeight;
            table[x][y].root = minRoot;

            x++;
            y++;
        }
        x = 1;
        y = resetY++;
    }



    fpOUT.open("kb0125.txt");

    fpOUT << "Kaden Beaver\n\n";

    fpOUT << "\t";
    for (int i = 0; i < count + 1; i++)
    {
        fpOUT << i << "\t";
    }
    fpOUT << endl;
    for (int i = 1; i < count + 2; i++)
    {
        fpOUT << i << "\t";
        for (int j = 0; j < count + 1; j++)
        {
            if (i - 1 > j)
            {
                fpOUT << "_" << "\t";
            }
            else if (i > j)
            {
                fpOUT << "_" << ")" << table[i][j].weight << "\t";
            }
            else
            {
                fpOUT << table[i][j].root << ")" << table[i][j].weight << "\t";
            }
            
        }
        fpOUT << endl;
    }

    fpOUT << endl << endl;

    string tree = "";
    tree = treeToString(table, 1, count, 0);

    fpOUT << tree << endl;

    fpOUT.close();

}

string treeToString(cell_t** t, int i, int j, int spaceNeeded)
{

    string ret = "";
    for (int k = 0; k < spaceNeeded; k++)
    {
        ret += "\t";
    }
    ret += "_\n";
    if (i <= j)
    {
        ret = "";
        for (int k = 0; k < spaceNeeded; k++)
        {
            ret += "\t";
        }
        ret += to_string(t[i][j].root);
        ret += "\n";

        ret += treeToString(t, i, t[i][j].root - 1, spaceNeeded+1);
        ret += treeToString(t, t[i][j].root + 1, j, spaceNeeded+1);
    }
    return ret;
    
}