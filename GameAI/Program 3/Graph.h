#pragma once
#include <iostream>
#include <fstream>>
#include <string>
#include "Vector2.h"

class Connection;

class Node
{
public:
	int nodeNumber;
	int status;
	float costSoFar;
	float estimatedHeuristic;
	float estimatedTotal;
	Node* previousNodeInPath;
	Connection* conn;
	Vector2 location;

	Node* nextNode;

	Node();
};


class Connection
{
public:
	Node* fromNode;
	Node* toNode;
	float cost;

	Connection* nextConn;

	Connection();
};


class Graph
{
private:
	Node* nodeRoot;
	Connection* connRoot;
public:


	int numberOfNodes;
	int numberOfConnections;

	Node** nodes;
	
	Connection** connections;
	

	Graph();


	
	void addNode(float x, float y);
	void addConn(int fromNode, int toNode, float cost);
	void initNodes();
	void initConns();

	int loadGraphFromFile(std::string fileName, std::string fileNameConn);

	Connection* getConnections(Node* fromNode);


	Connection* pathfindAStar(Node* start, Node* end);

	float distantce(Node* s, Node* e);

};