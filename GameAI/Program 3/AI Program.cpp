
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include "Vector2.h"
#include "Graph.h"
//#include "SteeringBehaviors.h"
//#include "Path.h"
//#include "FollowPath.h"
//#include "LookWhereYouAreGoing.h"




using namespace std;

int main()
{
    Graph* graph = new Graph;
    Connection* path = nullptr;
    Connection* pathNodes = nullptr;
    float cost = 0;
    ofstream fp;

    fp.open("CS 330, Pathfinding AB, Output.txt");
    fp << "# Kaden Beaver\n# Program 3 A * output\n#\n#\n";

    graph->loadGraphFromFile("CS 330, Pathfinding AB, Nodes v3.txt", "CS 330, Pathfinding AB, Connections v3.txt");


    fp << "Nodes: \n";
    fp << "\tNumber\tX-cord\tY-cord\n";
    for (int i = 0; i < graph->numberOfNodes; i++)
    {
        fp << "\t" << graph->nodes[i]->nodeNumber << "\t" << graph->nodes[i]->location.x << "\t" << graph->nodes[i]->location.z << endl;
    }

    fp << "\n\nConnections:\n";
    fp << "\tNumber\tFrom\tTo\tCost\n";
    for (int i = 0; i < graph->numberOfConnections; i++)
    {
        fp << "\t" << i + 1 << "\t" << graph->connections[i]->fromNode->nodeNumber << "\t" << graph->connections[i]->toNode->nodeNumber << "\t" << graph->connections[i]->cost << endl;
    }
    fp << "\n\n";

    fp << "Path 1: From 1 To 23\n";
    fp.close();


    path = graph->pathfindAStar(graph->nodes[0], graph->nodes[22]);
    pathNodes = path;
    cost = 0;

    fp.open("CS 330, Pathfinding AB, Output.txt", std::ios_base::app);

    

    while (path != nullptr)
    {
       // fp << "from:  " << path->fromNode->nodeNumber << "\tTo: " << path->toNode->nodeNumber << "\tCost: " << path->cost << endl;
        cost += path->cost;
        path = path->nextConn;
    }

    fp << "\n\tPath: " << pathNodes->fromNode->nodeNumber << " ";
    while (pathNodes != nullptr)
    {
        fp << pathNodes->toNode->nodeNumber << " ";
        pathNodes = pathNodes->nextConn;
    }


    fp << "\nTotal Cost: " << cost << "\n\n";
    fp << "Path 2: From 1 To 59\n";
    fp.close();

    
    path = graph->pathfindAStar(graph->nodes[0], graph->nodes[58]);
    pathNodes = path;
    cost = 0;

    fp.open("CS 330, Pathfinding AB, Output.txt", std::ios_base::app);

    

    while (path != nullptr)
    {
        //fp << "from:  " << path->fromNode->nodeNumber << "\tTo: " << path->toNode->nodeNumber << "\tCost: " << path->cost << endl;
        cost += path->cost;
        path = path->nextConn;
    }

    fp << "\n\tPath: " << pathNodes->fromNode->nodeNumber << " ";
    while (pathNodes != nullptr)
    {
        fp << pathNodes->toNode->nodeNumber << " ";
        pathNodes = pathNodes->nextConn;
    }


    fp << "\nTotal Cost: " << cost << "\n\n";



    fp << "Path 3: From 8 To 58\n";

    fp.close();
    path = graph->pathfindAStar(graph->nodes[7], graph->nodes[57]);
    pathNodes = path;
    cost = 0;

    fp.open("CS 330, Pathfinding AB, Output.txt", std::ios_base::app);

    while (path != nullptr)
    {
        //fp << "from:  " << path->fromNode->nodeNumber << "\tTo: " << path->toNode->nodeNumber << "\tCost: " << path->cost << endl;
        cost += path->cost;
        path = path->nextConn;
    }

    fp << "\n\tPath: " << pathNodes->fromNode->nodeNumber << " ";
    while (pathNodes != nullptr)
    {
        fp << pathNodes->toNode->nodeNumber << " ";
        pathNodes = pathNodes->nextConn;
    }


    fp << "\nTotal Cost: " << cost << "\n\n";

    fp << "Path 4: From 9 To 65\n";

    fp.close();
    path = graph->pathfindAStar(graph->nodes[8], graph->nodes[64]);
    pathNodes = path;
    cost = 0;
    fp.open("CS 330, Pathfinding AB, Output.txt", std::ios_base::app);
    while (path != nullptr)
    {
        //fp << "from:  " << path->fromNode->nodeNumber << "\tTo: " << path->toNode->nodeNumber << "\tCost: " << path->cost << endl;
        cost += path->cost;
        path = path->nextConn;
    }

    fp << "\n\tPath: " << pathNodes->fromNode->nodeNumber << " ";
    while (pathNodes != nullptr)
    {
        fp << pathNodes->toNode->nodeNumber << " ";
        pathNodes = pathNodes->nextConn;
    }


    fp << "\nTotal Cost: " << cost << "\n\n";

    fp << "Path 5: From 27 To 33\n";
    fp.close();
    path = graph->pathfindAStar(graph->nodes[26], graph->nodes[32]);
    pathNodes = path;
    cost = 0;
    fp.open("CS 330, Pathfinding AB, Output.txt", std::ios_base::app);
    while (path != nullptr)
    {
        //fp << "from:  " << path->fromNode->nodeNumber << "\tTo: " << path->toNode->nodeNumber << "\tCost: " << path->cost << endl;
        cost += path->cost;
        path = path->nextConn;
    }

    fp << "\n\tPath: " << pathNodes->fromNode->nodeNumber << " ";
    while (pathNodes != nullptr)
    {
        fp << pathNodes->toNode->nodeNumber << " ";
        pathNodes = pathNodes->nextConn;
    }


    fp << "\nTotal Cost: " << cost << "\n\n";




    return 0;
}



