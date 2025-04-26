/****************************************************************************************
*
*	File:		dessupercomp.c
*	Author:	Kaden Gryphon
*	Class:	CS 581-01
*	Program:	Program 5 Super Computer
*	Due:		11.30.2022
*	Source:	The class slides and textbook
*
*****************************************************************************************/


#include "dessupercomp.h"

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


//process enter system
#define DES_SC_EVENT_PES 1
#define DES_SC_EVENTTXT_PES "PES"
//Arive Processor queue
#define DES_SC_EVENT_APQ 2
#define DES_SC_EVENTTXT_APQ "APQ"
//exit processor queue
#define DES_SC_EVENT_EP 3
#define DES_SC_EVENTTXT_EP "EP"
//end stim/ stop accepting new process
#define DES_SC_EVENT_END 4
#define DES_SC_EVENTTXT_END "END"

static char DES_SC_EVENT_TXT[5][4] = {"NUL", "PES", "APQ", "EP", "END"};

//process types ids lengths and names
enum DES_PROCESS_TYPE_E {typeS, typeM, typeN, typeV, typeG, typeF};
static int DES_PROCESS_TYPE_LENGTH[3] = {4, 3, 5};
static char DES_PROCESS_TYPE_CHAR[6] = {'S', 'M', 'N', 'V', 'G', 'F'};
static char DES_PROCESS_ONE[4] = {typeS, typeN, typeM, typeF};
static char DES_PROCESS_TWO[3] = {typeG, typeV, typeM};
static char DES_PROCESS_THREE[5] = {typeS, typeG, typeV, typeN, typeF};


struct des_supercompResults_s des_supercomp1(int verbose)
{
	//print sim info
	DEBUG_LOG("\n--- START OF SUPER COMP ---\n");
	DEBUG_LOG("Execution parameters\n");
	
	//return data the calculated stats
	struct des_supercompResults_s retVal;
	retVal.success = 0;
	for(int i = 0; i < 3; i++)
	{
		retVal.submitted[i] = 0;
		retVal.completed[i] = 0;
		retVal.meanResponse[i] = 0;
	}
	for(int i = 0; i < 6; i++)
	{
		retVal.utilization[i] = 0;
		retVal.maxQueLength[i] = 0;
	}
	
	//input parms
	float endTime = 480;
	static struct desdis_empDiscreteTable_s processTypeTable = {3, {0.3775,0.7791,1,1,2,3}};
	double arivalLambda = 0.5167;
	double processorMean[6] = {14.5713,11.4586,10.6176,6.1826,14.6236,4.7322};
	double processorSD[6] = {2.7979, 3.6414, 2.0822, 2.0769, 3.7486, 1.2845};
	double forwardingDelay = 0.01;
	int processorsTypeMax[6] = {8, 6, 6, 4, 8, 2};
	
	DEBUG_LOG("\tProcess type table\n");
	DEBUG_LOG("\n");
	DEBUG_LOG("\tType\tProb\n");
	for(int i = 0; i < processTypeTable.length; i++)
	{
		DEBUG_LOG("\t%d\t%f\n", (int)processTypeTable.resultsAndProbs[i+processTypeTable.length], processTypeTable.resultsAndProbs[i]);
	}
	DEBUG_LOG("\n");
	DEBUG_LOG("\tProcess mean interarrival time\t %8.1f minutes\n", 1/arivalLambda);
	DEBUG_LOG("\n");
	for(int i = 0; i < 6; i++)
	{
		DEBUG_LOG("\tProcessor %c, Mean service time\t %8.1f minutes\n", DES_PROCESS_TYPE_CHAR[i], processorMean[i]);
		DEBUG_LOG("\tProcessor %c,   SD service time\t %8.1f minutes\n", DES_PROCESS_TYPE_CHAR[i], processorSD[i]);
		DEBUG_LOG("\n");
	}
	DEBUG_LOG("\tSimulation run length\t\t\t %8.1f\n", endTime);
	DEBUG_LOG("\n");
	DEBUG_LOG("\tProcessors available\n");
	for(int i = 0; i < 6; i++)
	{
		DEBUG_LOG("\t\tType %c\t\t %8d\n", DES_PROCESS_TYPE_CHAR[i], processorsTypeMax[i]);
	}
	
	//init varibles and generators
	DESDIS_NORMAL_INIT;
	struct DESFEL_FEL_S * fel = DESFEL_INIT();
	struct desfel_event_s bufEvent;
	
	//sim stats
	float simTime = 0;
	float lastTime = 0;
	
	int processInSystem = 0;
	
	//The processors and queues
	int processorsInUse[6];
	struct des_process_s* queueHeads[6];
	struct des_process_s* queueTails[6];
	int queueLength[6];
	for(int i = 0; i < 6; i++)
	{
		processorsInUse[i] = 0;
		queueHeads[i] = NULL;
		queueTails[i] = NULL;
		queueLength[i] = 0;
	}
	
	//starting fel state
	//first arival
	bufEvent.type = DES_SC_EVENT_PES;
	bufEvent.time = 0;
	DESFEL_INSERT(fel, bufEvent);
	//end time
	bufEvent.type = DES_SC_EVENT_END;
	bufEvent.time = endTime;
	DESFEL_INSERT(fel, bufEvent);
	
	//main loop ############################################################
	int loop = 1;
	while(1)
	{
		//get next event
		if(DESFEL_LENGTH(fel) <= 0) break;
		struct desfel_event_s currentEvent = DESFEL_GETNEXT(fel);
		//print trace
		if(verbose)
		{
			
		}
		
		simTime = currentEvent.time;
		//update the util stats using simTime- lastTime
		for(int i = 0; i < 6; i++)
		{
			//the elapsed time * number of busy servers
			retVal.utilization[i] += (simTime - lastTime) * processorsInUse[i];
		}
		
		
		
		int targetQueue = 0;
		
		switch(currentEvent.type)
		{
		//Process enter system
		case DES_SC_EVENT_PES:
			
			processInSystem++;
			
			//roll process type
			int pType = (int)desdis_empDiscrete(&processTypeTable)-1;
			
			//create new process
			struct des_process_s* newProcess = (struct des_process_s*)malloc(sizeof(struct des_process_s));
			if(newProcess == NULL)
			{
				ERROR_LOG(stderr, "ERROR dessupercomp: could not malloc new Process\n");
				return retVal;
			}
			
			retVal.submitted[pType]++;
			newProcess->id = retVal.submitted[pType];
			newProcess->type = pType+1;
			newProcess->step = 0;
			newProcess->length = DES_PROCESS_TYPE_LENGTH[pType];
			newProcess->enterTime = simTime;
			newProcess->exitTime = 0;
			newProcess->next = NULL;
			
			//scedual process arriving at first queue
			bufEvent.type = DES_SC_EVENT_APQ;
			bufEvent.time = simTime+forwardingDelay;
			bufEvent.customer = newProcess;
			DESFEL_INSERT(fel, bufEvent);
			
			//schedual next process arival
			//get interarival time
			if(loop)
			{
				double interarivalTime = desdis_expo(arivalLambda);
				bufEvent.type = DES_SC_EVENT_PES;
				bufEvent.time = simTime+interarivalTime;
				bufEvent.customer = NULL;
				DESFEL_INSERT(fel, bufEvent);
			}
			
			break;
			
		//proccess ready to enter queue
		case DES_SC_EVENT_APQ: ;

			//find the processor needed for the next step of the process
			targetQueue = 0;		
			switch(((struct des_process_s*)currentEvent.customer)->type)
			{
			case 1:
				targetQueue = DES_PROCESS_ONE[((struct des_process_s*)currentEvent.customer)->step];
				break;
			case 2:
				targetQueue = DES_PROCESS_TWO[((struct des_process_s*)currentEvent.customer)->step];
				break;
			case 3:
				targetQueue = DES_PROCESS_THREE[((struct des_process_s*)currentEvent.customer)->step];
				break;
			default:
				ERROR_LOG(stderr, "ERROR dessupercomp: unknown process type\n");
				break;
			}
			
			//is a server idle
			if(processorsInUse[targetQueue] < processorsTypeMax[targetQueue])
			{
				//server is now busy
				processorsInUse[targetQueue]++;
				//schedual service time
				double serviceTime = DESDIS_NORMAL(processorMean[targetQueue], processorSD[targetQueue]);
				bufEvent.type = DES_SC_EVENT_EP;
				bufEvent.time = simTime + serviceTime;
				bufEvent.customer = currentEvent.customer;
				DESFEL_INSERT(fel, bufEvent);
			}
			//else the servers busy join the queue
			else
			{
				if(queueLength[targetQueue] == 0)
				{
					//add customer to empty queue seting the head and tail of queue
					queueLength[targetQueue]++;
					queueHeads[targetQueue] = currentEvent.customer;
					queueTails[targetQueue] = queueHeads[targetQueue];
				}
				else
				{
					queueLength[targetQueue]++;
					queueTails[targetQueue]->next = currentEvent.customer;
					queueTails[targetQueue] = queueTails[targetQueue]->next;
				}
				
				//update the max queue length
				if(retVal.maxQueLength[targetQueue] < queueLength[targetQueue])
				{
					retVal.maxQueLength[targetQueue] = queueLength[targetQueue];
				}
			}
			
			break;
		
		//process done at current step
		case DES_SC_EVENT_EP: ;
			
			//which processor just finshed?
			targetQueue = 0;		
			switch(((struct des_process_s*)currentEvent.customer)->type)
			{
			case 1:
				targetQueue = DES_PROCESS_ONE[((struct des_process_s*)currentEvent.customer)->step];
				break;
			case 2:
				targetQueue = DES_PROCESS_TWO[((struct des_process_s*)currentEvent.customer)->step];
				break;
			case 3:
				targetQueue = DES_PROCESS_THREE[((struct des_process_s*)currentEvent.customer)->step];
				break;
			default:
				ERROR_LOG(stderr, "ERROR dessupercomp: unknown process type\n");
				break;
			}
			
			//server is now idle
			processorsInUse[targetQueue]--;
			
			//up tick step counter
			((struct des_process_s*)currentEvent.customer)->step++;
			
			//is the process done with all steps?
			//no
			if(((struct des_process_s*)currentEvent.customer)->step < ((struct des_process_s*)currentEvent.customer)->length)
			{
				//send it to the next processor
				bufEvent.type = DES_SC_EVENT_APQ;
				bufEvent.time = simTime + forwardingDelay;
				bufEvent.customer = currentEvent.customer;
				DESFEL_INSERT(fel, bufEvent);
			}
			//yes
			else
			{
				//coloect stats and free mem
				int pType = ((struct des_process_s*)currentEvent.customer)->type -1;
				
				//add to the process of that type completed and add its response time to the sum
				retVal.completed[pType]++;
				retVal.meanResponse[pType]+= simTime - ((struct des_process_s*)currentEvent.customer)->enterTime;
				
				free(currentEvent.customer);
			}
			
			//now that the process that just finshed is delt with schedual the next process in the queue
			//there is no processes waiting in queue
			if(queueLength[targetQueue] <= 0)
			{
				
			}
			else
			{
				//get next process from queue
				bufEvent.customer = queueHeads[targetQueue];
				queueHeads[targetQueue] = queueHeads[targetQueue]->next;
				queueLength[targetQueue]--;
				
				//schedual service time
				double serviceTime = DESDIS_NORMAL(processorMean[targetQueue], processorSD[targetQueue]);
				bufEvent.type = DES_SC_EVENT_EP;
				bufEvent.time = simTime + serviceTime;
				DESFEL_INSERT(fel, bufEvent);
				
				//server is now busy
				processorsInUse[targetQueue]++;
			}
			
			break;
		
		case DES_SC_EVENT_END:
			loop = 0;
			break;
		default:
			ERROR_LOG(stderr, "ERROR dessupercomp: unknown event type\n");
			//SET FLAGS
			return retVal;
			break;
		}
		
		//update last time for util stat
		lastTime = simTime;
	}
	

	//calc stats
	//get the mean response times for each type of process
	for(int i = 0; i < 3; i++)
	{
		retVal.meanResponse[i] /= retVal.completed[i];
	}
	
	//get the util ratio
	for(int i = 0; i < 6; i++)
	{
		//the total simTime * the numbers of servers is the total posible util time
		retVal.utilization[i] /= simTime * processorsTypeMax[i];
	}
	
	DEBUG_LOG("\nSimulation end time\t\t\t\t %8.1f\n", simTime);
	
	//print output stats
	DEBUG_LOG("\n\nCalculated Statistics\n");
	DEBUG_LOG("\tJobs\n");
	for(int i = 0; i < 3; i++)
	{
		DEBUG_LOG("\t\tType %d submitted\t\t %d\n", i+1, retVal.submitted[i]);
		DEBUG_LOG("\t\t       completed\t\t %d\n", retVal.completed[i]);
		DEBUG_LOG("\t\t       mean response\t\t %0.2f\n", retVal.meanResponse[i]); 
	}
	
	DEBUG_LOG("\tProcessors\n");
	for(int i = 0; i < 6; i++)
	{
		DEBUG_LOG("\t\tType %c utilization\t\t %0.2f\n", DES_PROCESS_TYPE_CHAR[i], retVal.utilization[i]);
		DEBUG_LOG("\t\t       max queue length\t\t %d\n", retVal.maxQueLength[i]);
	}
	
	//clean up
	DESFEL_FREE(fel);
	
	DEBUG_LOG("\n--- END SUPER COMPUTER ---\n");
	retVal.success = 1;
	return retVal;
}
