/****************************************************************************************
*
*	File:		gesgrocery.c
*	Author:	Kaden Gryphon
*	Class:	CS 581-01
*	Program:	Program 2 Grocery Checkout
*	Due:		10.17.2022
*	Source:	The class slides and textbook
*
*****************************************************************************************/


#include "desgrocery.h"

#define _DEBUG_MESSAGES
#ifdef _DEBUG_MESSAGES
	#include <stdio.h>
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...) {}
#endif
#define _ERROR_MESSAGES
#ifdef _ERROR_MESSAGES
	#include <stdio.h>
	#define ERROR_LOG fprintf
#else
	#define ERROR_LOG(...) {}
#endif

double des_grocery1(double lambda, double mu, double sigma, int numberServed, int verbose)
{
	//print sim info
	DEBUG_LOG("\n--- START OF GROCERY CHECKOUT (single-queue, single-server) ---\n");
	DEBUG_LOG("Execution parameters\n");
	DEBUG_LOG("\tMean interarrival time\t\t %8.1f minutes\n", 1/lambda);
	DEBUG_LOG("\tMean service time\t\t %8.1f minutes\n", mu);
	DEBUG_LOG("\tStd dev service time\t\t %8.1f minutes\n", sigma);
	DEBUG_LOG("\tNumber of customers served\t %8d\n\n\n", numberServed);
	
	//init varibles and generators
	DESDIS_NORMAL_INIT;
	struct DESFEL_FEL_S * fel = DESFEL_INIT();
	struct desfel_event_s bufEvent;
	
	//sim stats
	double serverUtilization = 0;
	int maximumQueueLength = 0;
	double meanResponseTime = 0;
	double arrivalTimes[numberServed];
	double responseTimes[numberServed];
	double responseTimeProportion = 0;
	int numberOfArrivals = 0;
	int numberOfDepartures = 0;
	double meanInterarrivalTime = 0;
	double meanServiceTime = 0;
	int maxFELLength = 0;
	
	//set the first event and set sim varibles
	bufEvent.type = 'a';
	bufEvent.time = 0;
	DESFEL_INSERT(fel, bufEvent);
	int serverBusy = 0;
	int queue = 0;
	float simTime = 0;
	double serverLastIdle = 0;
	
	//main loop
	while(numberOfDepartures < numberServed)
	{
		//get next event
		struct desfel_event_s currentEvent = DESFEL_GETNEXT(fel);
		if(verbose)
		{
			DEBUG_LOG("Current Event: %c, %8.2f | Que Length: %d | Server Busy: %d | FEL Length: %d\n", currentEvent.type, currentEvent.time, queue, serverBusy, DESFEL_LENGTH(fel));
		}
		
		simTime = currentEvent.time;
		switch(currentEvent.type)
		{
		//proccess arrival
		case 'a':
			//add the arrival time to the array for the response time later
			if(numberOfArrivals < numberServed) arrivalTimes[numberOfArrivals] = simTime;
			numberOfArrivals++;
			//is the server busy
			if(serverBusy)
			{
				//if they are customer joins queue
				queue++;
				
				if(queue > maximumQueueLength) maximumQueueLength = queue;
			}
			else
			{
				//else server starts service
				serverBusy = 1;
				//cumilate the servers idle time
				serverUtilization += simTime - serverLastIdle;
				//secdual the next departure
				float serviceTime = -1;
				while(serviceTime < 0)
				{
					serviceTime = DESDIS_NORMAL(mu, sigma);
				}
				bufEvent.type = 'd';
				bufEvent.time = simTime + serviceTime;
				DESFEL_INSERT(fel, bufEvent);
				meanServiceTime += serviceTime;
			}
			
			//scedual the next arrivl
			float interTime = desdis_expo(lambda);
			bufEvent.type = 'a';
			bufEvent.time = simTime + interTime;
			DESFEL_INSERT(fel, bufEvent);
			meanInterarrivalTime += interTime;
			
			//COLECT STATS
			if(maxFELLength < DESFEL_LENGTH(fel)) maxFELLength = DESFEL_LENGTH(fel);
			break;
		//proccess departure
		case 'd':
			//update the response time
			if(numberOfDepartures < numberServed)
			{
				responseTimes[numberOfDepartures] = simTime - arrivalTimes[numberOfDepartures];
				if(responseTimes[numberOfDepartures] >= 4) responseTimeProportion++;
			}
			numberOfDepartures++;
			//is there customers in the queue
			if(queue > 0)
			{
				queue--;
				//secdual the next departure
				float serviceTime = -1;
				while(serviceTime < 0)
				{
					serviceTime = DESDIS_NORMAL(mu, sigma);
				}
				bufEvent.type = 'd';
				bufEvent.time = simTime + serviceTime;
				DESFEL_INSERT(fel, bufEvent);
				meanServiceTime += serviceTime;
			}
			else
			{
				serverBusy = 0;
				//keep track of when the server started to idle
				serverLastIdle = simTime;
			}
			
			//clect stats
			if(maxFELLength < DESFEL_LENGTH(fel)) maxFELLength = DESFEL_LENGTH(fel);
			break;
		default:
			ERROR_LOG(stderr, "ERROR desgrocery1: unknown event type\n");
			return 0;
			break;
		}
	}
	
	//total idle time / total time
	serverUtilization /= simTime;
	
	//sum of all response times / number of response times
	for(int i = 0; i < numberServed; i++)
	{
		meanResponseTime += responseTimes[i];
	}
	meanResponseTime /= numberServed;
	
	//number of resposne time >= 4 / number of response times total
	responseTimeProportion /= numberServed;
	meanInterarrivalTime /= (numberOfArrivals-1);
	meanServiceTime /= numberOfDepartures;
	
	//print output stats
	DEBUG_LOG("\n\nCalculated Statistics\n");
	DEBUG_LOG("\tServer utilization:\t\t %8.3f\n", 1-serverUtilization);
	DEBUG_LOG("\tMaximum queue length\t\t %8d\n", maximumQueueLength);
	DEBUG_LOG("\tMean response time\t\t %8.3f minutes\n", meanResponseTime);
	DEBUG_LOG("\tResponse >= 4 proportion\t %8.3f\n", responseTimeProportion);
	DEBUG_LOG("\tSimulation run length\t\t %8.3f minutes\n", simTime);
	DEBUG_LOG("\tNumber of arrivals\t\t %8d\n", numberOfArrivals);
	DEBUG_LOG("\tNumber of departures\t\t %8d\n", numberOfDepartures);
	DEBUG_LOG("\tMean interarrival time\t\t %8.3f\n", meanInterarrivalTime);
	DEBUG_LOG("\tMean service time\t\t %8.3f\n", meanServiceTime);
	DEBUG_LOG("\tMax FEL Length\t\t\t %8d\n", maxFELLength);
	
	//clean up
	DESFEL_FREE(fel);
	
	DEBUG_LOG("\n--- END OF GROCERY CHECKOUT (single-queue, single-server) ---\n");
	return meanResponseTime;
}
