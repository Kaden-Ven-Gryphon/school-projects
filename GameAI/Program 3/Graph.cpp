#include "Graph.h"



Node::Node()
{
	nodeNumber = 0;
	status = 0;
	costSoFar = 0;
	estimatedHeuristic = 0;
	estimatedTotal = 0;
	previousNodeInPath = nullptr;
	conn = nullptr;
	location.set(0, 0);
	nextNode = nullptr;
}


//Connection contructor
Connection::Connection()
{
	fromNode = nullptr;
	toNode = nullptr;
	cost = 0;
	nextConn = nullptr;
}

//graph constructor
Graph::Graph()
{
	numberOfNodes = 0;
	numberOfConnections = 0;
	nodeRoot = nullptr;
	connRoot = nullptr;
	nodes = nullptr;
	connections = nullptr;
}

//Nodes must be added in the same order they apear in the txt
void Graph::addNode(float x, float y)
{
	Node* n = new Node;
	n->location.x = x;
	n->location.z = y;
	n->nodeNumber = numberOfNodes + 1;
	if (nodeRoot != nullptr)
	{
		Node* ptrNode = nodeRoot;

		while (true)
		{
			if (ptrNode->nextNode == nullptr)
			{
				ptrNode->nextNode = n;
				numberOfNodes++;
				break;
			}
			else
			{
				ptrNode = ptrNode->nextNode;
			}
		}
	}
	else
	{
		nodeRoot = n;
		numberOfNodes++;
	}
}

//adds a connection to the conns list
//the nodes array must be initilized beofre this is called
void Graph::addConn(int fromNode, int toNode, float cost)
{
	Connection* c = new Connection;
	c->cost = cost;
	c->fromNode = nodes[fromNode - 1];
	c->toNode = nodes[toNode - 1];
	

	if (connRoot != nullptr)
	{
		Connection* ptrConn = connRoot;

		while (true)
		{
			if (ptrConn->nextConn == nullptr)
			{
				ptrConn->nextConn = c;
				numberOfConnections++;
				break;
			}
			else
			{
				ptrConn = ptrConn->nextConn;
			}
		}
	}
	else
	{
		connRoot = c;
		numberOfConnections++;
	}
}


//turns the list of nodes in to an array for easier navigation
void Graph::initNodes()
{
	nodes = new Node * [numberOfNodes];
	

	nodes[0] = nodeRoot;
	for (int i = 1; i < numberOfNodes; i++)
	{
		nodes[i] = nodes[i - 1]->nextNode;
	}

}

//turns the list of connections into an array for easier navigation
void Graph::initConns()
{
	connections = new Connection * [numberOfConnections];

	connections[0] = connRoot;
	for (int i = 1; i < numberOfConnections; i++)
	{
		connections[i] = connections[i - 1]->nextConn;
	}
}

//takes the string name for the files for the nodes and connections and builds the graph
int Graph::loadGraphFromFile(std::string fileNameNode, std::string fileNameConn)
{

	std::ifstream fp;										//file pointer
	std::string line;										//a line of the file

	fp.open(fileNameNode);									//open the node file
	if (!fp.is_open())										//test if the file opened propurly
	{
		std::cout << "Node File failed to open.\n\n";
		return -1;
	}

	while (std::getline(fp, line))							//loop trough the file line by line
	{
		if (line[0] != '#')									//if the line starts with # ignore the line
		{
			float x = 0;
			float y = 0;
			std::string buff;
			int pos = 0;

			for (int i = 0; i < 7; i++)						//remove the fist 7 entries of the line, they are not needed
			{
				pos = line.find(",");
				line.erase(0, pos + 1);

			}

			pos = line.find(",");
			buff = line.substr(0, pos);						//get the 8th entry which is the x cord
			x = std::stof(buff);							//convert the string to float
			line.erase(0, pos + 1);

			pos = line.find(",");
			buff = line.substr(0, pos);						//get the 9th entry wichi is the y cord
			y = std::stof(buff);							//conver the string to float
	
			addNode(x, y);									//add the node to the list

		}

	}//Node File loop

	fp.close();												//close the file
	initNodes();											//convert the list of nodes into an array

	fp.open(fileNameConn);									//open the connections file
	if (!fp.is_open())										//check htat the file opened
	{
		std::cout << "Connection File failed to open.\n\n";
		return -2;
	}

	while (std::getline(fp, line))							//loop through the file line by line
	{
		if (line[0] != '#')									//ignore lines that start with #
		{
			int fromNode = 0;
			int toNode = 0;
			float cost = 0;
			std::string buff;
			int pos = 0;

			for (int i = 0; i < 2; i++)						//ignore the first two entries
			{
				pos = line.find(",");
				line.erase(0, pos + 1);

			}

			pos = line.find(",");
			buff = line.substr(0, pos);						//get the thrid entry, the fromNode
			fromNode = std::stoi(buff);						//convert string to int
			line.erase(0, pos + 1);

			pos = line.find(",");
			buff = line.substr(0, pos);						//get the 4th entry, the toNode
			toNode = std::stoi(buff);						//convert string to int
			line.erase(0, pos + 1);

			pos = line.find(",");
			buff = line.substr(0, pos);						//get the 5th entry the cost
			cost = std::stof(buff);							//convert the stirng to float
			

			addConn(fromNode, toNode, cost);				//add connection to list

		}

	}//Conn File Loop
	fp.close();												//close the file
	initConns();											//convert the connections list into an array
															//The graph should now be fully initilized
}

//gives the pointer for the first connection in the list of outgoing connecitons
Connection* Graph::getConnections(Node* fromNode)
{
	Connection* connList = nullptr;							//the list to return
	Connection* connPtr = nullptr;							//pointer of curent conection
	for (int i = 0; i < numberOfConnections; i++)			//loop though all connections
	{
		if (fromNode == connections[i]->fromNode)			//if the connection has the same from node as the serched for from node,  done by compareing the pointers adressess
		{
			if (connList != nullptr)						//if the list is not empty
			{
				connPtr->nextConn = connections[i];			//add the connection to list
				connPtr = connPtr->nextConn;				//increment the pointer
				connPtr->nextConn = nullptr;				//make sure to clear the list pointer from the init of the connections , or previus calls of getConn
			}												//doing it this way means that only one list of connections can exist at a time  every time this is called it distorys the previous list
															//This make it easy as there is only one instance of each node, and conection that are effectivly global but this could cause problems 
															//if someone wanted to have two or more lists of getConnections to compare or somthing.  I do not need that for this assigmnet so I am not going to bother fixing this now
															//Over all I think that my implementaiton of all of this is a bit poor, but I am really short on time and can not completely redesign this right now
			else
			{
				connList = connections[i];					//this is the first connection in the list
				connPtr = connList;
				connPtr->nextConn = nullptr;
			}
		}
	}
	
	return connList;
}



Connection* Graph::pathfindAStar(Node* start, Node* end)
{
	int inter = 0;
	std::ofstream fp;

	int openNum = 0;
	Node** open = new Node*[numberOfNodes];					//create the open array
	int closedNum = 0;
	Node** closed = new Node * [numberOfNodes];				//create the closed array

	Node* current = start;

	start->conn = nullptr;									//set the data for the start node
	start->previousNodeInPath = nullptr;
	start->costSoFar = 0;
	start->estimatedTotal = distantce(start, end);
	start->estimatedHeuristic = start->estimatedTotal;

	open[openNum++] = start;								//add the start node to the open list

	while (openNum > 0)										//while there are nodes on the open list
	{
		inter++;
		//std::cout << "getting next open\n";
		current = open[0];							//the current node for the interation
		int currentIndex = 0;

		for (int i = 1; i < openNum; i++)					//find the open node with the lowest estimated cost
		{													//I know this is realy slow, I wanted to make a priority que because I have been makeing/using heaps in my other class
															//but once again I am short on time
			if (current->estimatedTotal > open[i]->estimatedTotal)
			{
				current = open[i];
				currentIndex = i;							//get the index to remove it latter to the closed
			}
		}

		if (current == end)									//if the current node is the end node terminate
		{
			break;
		}


		Connection* outgoingConn = getConnections(current);//generate the list of out going connections

		while (outgoingConn != nullptr)						//until the end of the list is reached
		{
			//std::cout << "geting toNode\n";
			Node* toNode = outgoingConn->toNode;			//the to node of the tested connection
			float toNodeNewCost = current->costSoFar + outgoingConn->cost;	//the total cost of taking this path

			
			int existIndex = 0;
			bool onOpen = false;
			bool onClosed = false;
			for (int i = 0; i < closedNum; i++)				//check if the toNode is on the closed list
			{
				if (closed[i] == toNode)					//the node wsa found
				{
					onClosed = true;
					existIndex = i;							//get the index if it needs to be removed from list
					break;
				}
			}

			if (onClosed)
			{
				if (toNode->costSoFar <= toNodeNewCost)		//if the node is on the closed list and already has a better cost
				{
					outgoingConn = outgoingConn->nextConn;
					continue;								//contine
				}
				else										//else remove it from the closed list
				{
					closed[existIndex] = closed[closedNum - 1];
					closedNum--;
				}

			}
			if (!onClosed)								//only if it was not found on the closed list
			{
				
				for (int i = 0; i < openNum; i++)				//seach the open array for toNode
				{
					if (open[i] == toNode)						//node found
					{
						existIndex = i;
						onOpen = true;
						break;
					}
				}

				if (onOpen)									//if node is on open list
				{
					if (toNode->costSoFar <= toNodeNewCost)		//if it already had a shorter path
					{
						outgoingConn = outgoingConn->nextConn;
						continue;								//contiue
					}

				}
			}

			if (!onClosed && !onOpen)									//the node is not any list and needs to update its heuristic
			{
				toNode->estimatedHeuristic = distantce(toNode, end);
			}

			toNode->costSoFar = toNodeNewCost;					//update the cost so far
			toNode->conn = outgoingConn;						//record the connection
			toNode->previousNodeInPath = current;				//get the previous node, this is probuly redunant with the the recording the connection
			toNode->estimatedTotal = toNode->costSoFar + toNode->estimatedHeuristic;	//get teh estimaded cost

			if (!onOpen)										//if it is not on the open array
			{
				open[openNum++] = toNode;						//add it to the open array
			}
			
			outgoingConn = outgoingConn->nextConn;

		}//looping through the outGoing connections

		closed[closedNum++] = open[currentIndex];				//add current to the closed array
		open[currentIndex] = open[openNum - 1];					//remove it from the open list
		openNum--;


		fp.open("CS 330, Pathfinding AB, Output.txt", std::ios_base::app);
		fp << "Iteration: " << inter << " Current Node: " << current->nodeNumber<< "\n";
		fp << "Open: ";
		for (int i = 0; i < openNum; i++)
		{
			fp << open[i]->nodeNumber << ", ";
		}
		fp << "\nClosed: ";
		for (int i = 0; i < closedNum; i++)
		{
			fp << closed[i]->nodeNumber << ", ";
		}
		fp << "\n\n";
		fp.close();





	}//Looping while there are nodes on open array


	if (current != end)											//no solution was found
	{
		return nullptr;
	}

	Connection* path = current->conn;							//this is the list that will be returned

	current->conn->nextConn = nullptr;							//set the endpoint of the list
	current = current->conn->fromNode;							//move to the previous node

	while (current != start)									//while the start has not bee reached keep adding connections
	{
		current->conn->nextConn = path;							//the next connections next conn is the the path so far
		path = current->conn;									//has the start of the path move to include the new connection
		current = current->conn->fromNode;						//move back a Node
	}

	return path;												//return the path
}


//The Heuristic for Astar gets the distaince between 2 nodes
float Graph::distantce(Node* s, Node* e)
{
	Vector2 d;
	d.set(e->location.minus(s->location));

	return d.length();;
}




