/****************************************************************************************
*
*	File:		desairportcheckout.c
*	Author:	Kaden Gryphon
*	Class:	CS 581-01
*	Program:	Program 2 Grocery Checkout
*	Due:		10.31.2022
*	Source:	The class slides and textbook
*
*****************************************************************************************/


#include "desairportcheckout.h"

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

struct des_airportResults_s airport1(double lambda1, double lambda2, double mu1, double sigma1, double mu2, double sigma2, float endTime, int allowTransfers, int verbose)
{
	//print sim info
	DEBUG_LOG("\n--- START OF AIRPORT CHECK IN (multi-queue, multi-server) ---\n");
	DEBUG_LOG("Execution parameters\n");
	DEBUG_LOG("\tQueue  1, Mean interarrival time\t %8.1f minutes\n", 1/lambda1);
	DEBUG_LOG("\tQueue  2, Mean interarrival time\t %8.1f minutes\n", 1/lambda2);
	DEBUG_LOG("\tServer 1, Mean service time\t\t %8.1f minutes\n", mu1);
	DEBUG_LOG("\tServer 1, Std dev service time\t\t %8.1f minutes\n", sigma1);
	DEBUG_LOG("\tServer 2, Mean service time\t\t %8.1f minutes\n", mu2);
	DEBUG_LOG("\tServer 2, Std dev service time\t\t %8.1f minutes\n", sigma2);
	if(allowTransfers) DEBUG_LOG("\tTransfers allowed\t\t\t %8s\n", "Yes");
	else DEBUG_LOG("\tTransfers allowed\t\t\t %8s\n", "No");
	DEBUG_LOG("\tSimulation run length\t\t\t %8.1f\n\n\n", endTime);
	
	//init varibles and generators
	DESDIS_NORMAL_INIT;
	struct DESFEL_FEL_S * fel = DESFEL_INIT();
	struct desfel_event_s bufEvent;
	
	//sim stats
	double serverUtilization1 = 0;
	int maximumQueueLength1 = 0;
	double meanResponseTime1 = 0;
	double arrivalTimes1[1000];
	double responseTimeProportion1 = 0;
	int numberOfArrivals1 = 0;
	int numberOfDepartures1 = 0;
	double meanInterarrivalTime1 = 0;
	double meanServiceTime1 = 0;
	int queue2CustomersServed = 0;
	
	int arrivalTimeIndex1 = 0;
	
	double serverUtilization2 = 0;
	int maximumQueueLength2 = 0;
	double meanResponseTime2 = 0;
	double arrivalTimes2[1000];
	double responseTimeProportion2 = 0;
	int numberOfArrivals2 = 0;
	int numberOfDepartures2 = 0;
	double meanInterarrivalTime2 = 0;
	double meanServiceTime2 = 0;
	
	int arrivalTimeIndex2 = 0;
	
	double meanResponseTimeTotal = 0;
	int maxFELLength = 0;
	
	//set the first event and set sim varibles
	bufEvent.type = 'a';
	bufEvent.time = 0;
	bufEvent.queue = 1;
	DESFEL_INSERT(fel, bufEvent);
	bufEvent.type = 'a';
	bufEvent.time = 0;
	bufEvent.queue = 2;
	DESFEL_INSERT(fel, bufEvent);
	bufEvent.type = 'e';
	bufEvent.time = 120;
	DESFEL_INSERT(fel, bufEvent);
	int serverBusy1 = 0;
	int queue1 = 0;
	int serverBusy2 = 0;
	int queue2 = 0;
	float simTime = 0;
	double serverLastIdle1 = 0;
	double serverLastIdle2 = 0;
	
	//main loop
	int loop = 1;
	while(loop)
	{
		//get next event
		struct desfel_event_s currentEvent = DESFEL_GETNEXT(fel);
		if(verbose)
		{
			DEBUG_LOG("Current Event: %c, %8.2f | Que1 Length: %d | Server1 Busy: %d | Que2 Length: %d | Server2 Busy: %d | FEL Length: %d\n", currentEvent.type, currentEvent.time, queue1, serverBusy1, queue2, serverBusy2, DESFEL_LENGTH(fel));
		}
		
		simTime = currentEvent.time;
		switch(currentEvent.type)
		{
		//proccess arrival
		case 'a':
			//First class arrival
			if(currentEvent.queue == 1)
			{
				//add the arrival time to the array for the response time later
				if(arrivalTimeIndex1 < 1000) arrivalTimes1[arrivalTimeIndex1++] = simTime;
				numberOfArrivals1++;
				//is the server busy
				if(serverBusy1)
				{
					//if they are customer joins queue
					queue1++;
					
					if(queue1 > maximumQueueLength1) maximumQueueLength1 = queue1;
				}
				else
				{
					//else server starts service
					serverBusy1 = 1;
					//cumilate the servers idle time
					serverUtilization1 += simTime - serverLastIdle1;
					//secdual the next departure
					float serviceTime = -1;
					while(serviceTime < 0)
					{
						serviceTime = DESDIS_NORMAL(mu1, sigma1);
					}
					bufEvent.type = 'd';
					bufEvent.time = simTime + serviceTime;
					bufEvent.queue = 1;
					DESFEL_INSERT(fel, bufEvent);
					meanServiceTime1 += serviceTime;
				}
				
				//scedual the next arrivl
				float interTime = desdis_expo(lambda1);
				bufEvent.type = 'a';
				bufEvent.time = simTime + interTime;
				bufEvent.queue = 1;
				DESFEL_INSERT(fel, bufEvent);
				meanInterarrivalTime1 += interTime;
			}
			//eco class arrival
			else
			{
				//add the arrival time to the array for the response time later
				if(arrivalTimeIndex2 < 1000) arrivalTimes2[arrivalTimeIndex2++] = simTime;
				numberOfArrivals2++;
				//is the server busy
				if(serverBusy2)
				{
					//if they are customer joins queue
					queue2++;
					
					if(queue2 > maximumQueueLength2) maximumQueueLength2 = queue2;
				}
				else
				{
					//else server starts service
					serverBusy2 = 1;
					//cumilate the servers idle time
					serverUtilization2 += simTime - serverLastIdle2;
					//secdual the next departure
					float serviceTime = -1;
					while(serviceTime < 0)
					{
						serviceTime = DESDIS_NORMAL(mu2, sigma2);
					}
					bufEvent.type = 'd';
					bufEvent.time = simTime + serviceTime;
					bufEvent.queue = 2;
					DESFEL_INSERT(fel, bufEvent);
					meanServiceTime2 += serviceTime;
				}
				
				//scedual the next arrivl
				float interTime = desdis_expo(lambda2);
				bufEvent.type = 'a';
				bufEvent.time = simTime + interTime;
				bufEvent.queue = 2;
				DESFEL_INSERT(fel, bufEvent);
				meanInterarrivalTime2 += interTime;
			}
			//COLECT STATS
			if(maxFELLength < DESFEL_LENGTH(fel)) maxFELLength = DESFEL_LENGTH(fel);
			break;
			
		//proccess departure
		case 'd':
			//first class
			if(currentEvent.queue == 1)
			{
				//update the response time
				meanResponseTime1 += simTime - arrivalTimes1[arrivalTimeIndex1-1];
				if((simTime - arrivalTimes1[arrivalTimeIndex1-1]) >= 4) responseTimeProportion1++;
				arrivalTimeIndex1--;
				numberOfDepartures1++;
				//is there customers in the queue
				if(queue1 > 0)
				{
					queue1--;
					//secdual the next departure
					float serviceTime = -1;
					while(serviceTime < 0)
					{
						serviceTime = DESDIS_NORMAL(mu1, sigma1);
					}
					bufEvent.type = 'd';
					bufEvent.time = simTime + serviceTime;
					bufEvent.queue = 1;
					DESFEL_INSERT(fel, bufEvent);
					meanServiceTime1 += serviceTime;
				}
				//else the first class queue is empty
				else
				{
					if(allowTransfers)
					{
						//if idle and the eco class has a line
						if(queue2 > 0)
						{
							//make trasfer
							queue2CustomersServed++;
							queue2--;
							
							//move the arrival time from list 2 to list 1
							if(arrivalTimeIndex1 < 1000)
							{
								arrivalTimes1[arrivalTimeIndex1++] = arrivalTimes2[--arrivalTimeIndex2];
							}
							//secdual the next departure
							float serviceTime = -1;
							while(serviceTime < 0)
							{
								serviceTime = DESDIS_NORMAL(mu1, sigma1);
							}
							bufEvent.type = 'd';
							bufEvent.time = simTime + serviceTime;
							bufEvent.queue = 1;
							DESFEL_INSERT(fel, bufEvent);
							meanServiceTime1 += serviceTime;
						}
						else
						{
							serverBusy1 = 0;
							//keep track of when the server started to idle
							serverLastIdle1 = simTime;
						}
					}
					else
					{
						serverBusy1 = 0;
						//keep track of when the server started to idle
						serverLastIdle1 = simTime;
					}
				}
			}
			//eco
			else
			{
				//update the response time
				meanResponseTime2 += simTime - arrivalTimes2[arrivalTimeIndex2-1];
				if((simTime - arrivalTimes2[arrivalTimeIndex2-1]) >= 4) responseTimeProportion2++;
				arrivalTimeIndex2--;
				numberOfDepartures2++;
				//is there customers in the queue
				if(queue2 > 0)
				{
					queue2--;
					//secdual the next departure
					float serviceTime = -1;
					while(serviceTime < 0)
					{
						serviceTime = DESDIS_NORMAL(mu2, sigma2);
					}
					bufEvent.type = 'd';
					bufEvent.time = simTime + serviceTime;
					bufEvent.queue = 2;
					DESFEL_INSERT(fel, bufEvent);
					meanServiceTime2 += serviceTime;
				}
				else
				{
					serverBusy2 = 0;
					//keep track of when the server started to idle
					serverLastIdle2 = simTime;
				}
			}
			//clect stats
			if(maxFELLength < DESFEL_LENGTH(fel)) maxFELLength = DESFEL_LENGTH(fel);
			break;
			
		case 'e':
			loop = 0;
			break;
		default:
			ERROR_LOG(stderr, "ERROR desairport: unknown event type\n");
			struct des_airportResults_s retVal;
			retVal.meanResponseTotal = 0;
			return retVal;
			break;
		}
	}
	
	//server 1 stats
	//total idle time / total time
	serverUtilization1 /= simTime;
	
	//sum of all response times / number of response times
	meanResponseTimeTotal = meanResponseTime1;
	meanResponseTime1 /= numberOfDepartures1;
	
	//number of resposne time >= 4 / number of response times total
	responseTimeProportion1 /= numberOfDepartures1;
	meanInterarrivalTime1 /= (numberOfArrivals1-1);
	meanServiceTime1 /= numberOfDepartures1;
	
	//server 2 stats
	//total idle time / total time
	serverUtilization2 /= simTime;
	
	//sum of all response times / number of response times
	meanResponseTimeTotal += meanResponseTime2;
	meanResponseTime2 /= numberOfDepartures2;
	
	//number of resposne time >= 4 / number of response times total
	responseTimeProportion2/= numberOfDepartures2;
	meanInterarrivalTime2 /= (numberOfArrivals2-1);
	meanServiceTime2 /= numberOfDepartures2;
	
	
	meanResponseTimeTotal /= (numberOfDepartures1 + numberOfDepartures2);
	
	//print output stats
	DEBUG_LOG("\n\nCalculated Statistics\n");
	
	
	DEBUG_LOG("\tQueue  1, Maximum queue length\t\t %8d\n", maximumQueueLength1);
	DEBUG_LOG("\tQueue  1, Number of arrivals\t\t %8d\n", numberOfArrivals1);
	DEBUG_LOG("\tQueue  1, Mean interarrival time\t %8.3f minutes\n", meanInterarrivalTime1);
	DEBUG_LOG("\tQueue  1, Length at simulation end\t %8d\n", queue1);
	DEBUG_LOG("\tServer 1, Server utilization:\t\t %8.3f\n", 1-serverUtilization1);
	DEBUG_LOG("\tServer 1, Number of departures\t\t %8d\n", numberOfDepartures1);
	DEBUG_LOG("\tServer 1, Mean response time\t\t %8.3f minutes\n", meanResponseTime1);
	DEBUG_LOG("\tServer 1, Response >= 4 proportion\t %8.3f\n", responseTimeProportion1);
	DEBUG_LOG("\tServer 1, Mean service time\t\t %8.3f minutes\n", meanServiceTime1);
	if(serverBusy1) DEBUG_LOG("\tServer 1, Status at simulation end\t %8s\n", "BUSY");
	else DEBUG_LOG("\tServer 1, Status at simulation end\t %8s\n", "IDLE");
	DEBUG_LOG("\tQueue  1, Queue 2 customers served\t %8d\n", queue2CustomersServed);
	DEBUG_LOG("\n");
	DEBUG_LOG("\tQueue  2, Maximum queue length\t\t %8d\n", maximumQueueLength2);
	DEBUG_LOG("\tQueue  2, Number of arrivals\t\t %8d\n", numberOfArrivals2);
	DEBUG_LOG("\tQueue  2, Mean interarrival time\t %8.3f minutes\n", meanInterarrivalTime2);
	DEBUG_LOG("\tQueue  2, Length at simulation end\t %8d\n", queue2);
	DEBUG_LOG("\tServer 2, Server utilization:\t\t %8.3f\n", 1-serverUtilization2);
	DEBUG_LOG("\tServer 2, Number of departures\t\t %8d\n", numberOfDepartures2);
	DEBUG_LOG("\tServer 2, Mean response time\t\t %8.3f minutes\n", meanResponseTime2);
	DEBUG_LOG("\tServer 2, Response >= 4 proportion\t %8.3f\n", responseTimeProportion2);
	DEBUG_LOG("\tServer 2, Mean service time\t\t %8.3f minutes\n", meanServiceTime2);
	if(serverBusy1) DEBUG_LOG("\tServer 2, Status at simulation end\t %8s\n", "BUSY");
	else DEBUG_LOG("\tServer 2, Status at simulation end\t %8s\n", "IDLE");
	DEBUG_LOG("\n");
	DEBUG_LOG("\tBoth queues, Total arrivals\t\t %8d\n", numberOfArrivals1 + numberOfArrivals2);
	DEBUG_LOG("\tBoth servers, Total departures\t\t %8d\n", numberOfDepartures1 + numberOfDepartures2);
	int extra = 0;
	if(serverBusy1) extra++;
	if(serverBusy2) extra++;
	DEBUG_LOG("\tTotal in system at end\t\t\t %8d\n", queue1 + queue2 + extra);
	DEBUG_LOG("\tSimulation run length\t\t\t %8.3f minutes\n", simTime);
	DEBUG_LOG("\tMax FEL Length\t\t\t\t %8d\n", maxFELLength);
	DEBUG_LOG("\n");
	DEBUG_LOG("\n");
	DEBUG_LOG("\tServer 1, Mean response time\t\t %8.3f minutes\n", meanResponseTime1);
	DEBUG_LOG("\tServer 2, Mean response time\t\t %8.3f minutes\n", meanResponseTime2);
	DEBUG_LOG("\tTotal   , Mean response time\t\t %8.3f minutes\n", meanResponseTimeTotal);
	//clean up
	DESFEL_FREE(fel);
	
	DEBUG_LOG("\n--- END OF AIRPORT CHECK IN (multi-queue, multi-server) ---\n");
	struct des_airportResults_s retVal;
	retVal.meanResponse1 = meanResponseTime1;
	retVal.meanResponse2 = meanResponseTime2;
	retVal.meanResponseTotal = meanResponseTimeTotal;
	return retVal;
}
