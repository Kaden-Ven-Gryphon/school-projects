/****************************************************************************************
*
*	File:		dumptruckoperation.c
*	Author:	Kaden Gryphon
*	Class:		CS 581-01
*	Program:	Program 4 dump truck sim
*	Due:		11.09.2022
*	Source:	The class slides and textbook
*
*****************************************************************************************/


#include "dumptruckoperation.h"

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

#define DES_DTO_EVENT_ALQ 1
#define DES_DTO_EVENTTXT_ALQ "ALQ"
#define DES_DTO_EVENT_EL 2
#define DES_DTO_EVENTTXT_EL "EL"
#define DES_DTO_EVENT_EW 3
#define DES_DTO_EVENTTXT_EW "EW"
#define DES_DTO_EVENT_END 4
#define DES_DTO_EVENTTXT_END "END"

static char DES_DTO_EVENT_TXT[5][4] = {"NUL", "ALQ", "EL", "EW", "END"};

struct des_dumptruckResults_s des_dumptruck1(int verbose)
{
	DEBUG_LOG("\n--- START OF DUMP TRUCK SIM ---\n");
	//the empirical tables for the discrete distributions
	static struct desdis_empDiscreteTable_s loadingTable = {3, {0.3,0.8,1,5,10,15}};
	static struct desdis_empDiscreteTable_s weighingTable = {2, {0.7,1,12,16}};
	static struct desdis_empDiscreteTable_s travelTable = {4, {0.4,0.7,0.9,1,40,60,80,100}};
	
	//return value
	struct des_dumptruckResults_s retVal = {0,0};
	
	//the futer event list
	struct DESFEL_FEL_S * fel = DESFEL_INIT();
	struct desfel_event_s bufEvent;
	
	//sim params
	static int simEndTime = 240;
	static int numberOfTrucks = 8;
	
	//sim state variables
	int simTime = 0;
	int loaders = 0;//max 2
	int loadQueue[numberOfTrucks];
	int lq = 0;
	int weigher = 0;//max 1
	int weighQueue[numberOfTrucks];
	int wq = 0;
	//initial conditions
	for(int i = 0; i < numberOfTrucks; i++)
	{
		bufEvent.time = i;
		bufEvent.type = DES_DTO_EVENT_ALQ;
		bufEvent.queue = i+1;
		DESFEL_INSERT(fel, bufEvent);
	}
	bufEvent.time = simEndTime;
	bufEvent.type = DES_DTO_EVENT_END;
	bufEvent.queue = 0;
	DESFEL_INSERT(fel, bufEvent);
	
	//print the header of the table
	if(verbose)
	{
		DEBUG_LOG("------------------------------------------------------------------------------------------------\n");
		DEBUG_LOG("%49s%43s\n", "Lists", "Cumulative");
		DEBUG_LOG("Clock\t\tSystem State\t\tLoader\tWeigh\t\tFuture event\t  statistics\n");
		DEBUG_LOG("  t\tLQ(T)\tL(t)\tWQ(t)\tW(t)\tqueue\tqueue\t\tlist\t\t  BL\tBS\n");
	}
		
	//event handeling
	int loop = 1;
	while(loop)
	{
		//prints a trace of the sim, showing the stats at each step
		if(verbose)
		{
			DEBUG_LOG("------------------------------------------------------------------------------------------------\n");
			DEBUG_LOG("%4d%7d%7d%9d%7d", simTime, lq, loaders, wq, weigher);
			if(lq) DEBUG_LOG("%8s%02d", "DT", loadQueue[0]);
			else DEBUG_LOG("          ");
			if(wq) DEBUG_LOG("%6s%02d", "DT", weighQueue[0]);
			else DEBUG_LOG("        ");
			bufEvent = DESFEL_PEAK(fel);
			if(DESFEL_LENGTH(fel)) DEBUG_LOG("%11d%s%3s, %6.2f, %2d)", 0, "(", DES_DTO_EVENT_TXT[(int)bufEvent.type], bufEvent.time, bufEvent.queue);
			else DEBUG_LOG("                            ");
			DEBUG_LOG("%4.0f%5.0f", retVal.loaderUtilization, retVal.scaleUtilization);
			//DEBUG_LOG("%5d", DESFEL_LENGTH(fel));
			DEBUG_LOG("\n");
			int printLoop = 1;
			int printIndex = 1;
			while(printLoop)
			{
				printLoop = 0;
				if(printIndex < lq)
				{
					printLoop = 1;
					DEBUG_LOG("%42s%02d", "DT", loadQueue[printIndex]);
				}
				else DEBUG_LOG("%44s", " ");
				if(printIndex < wq)
				{
					printLoop = 1;
					DEBUG_LOG("%6s%02d", "DT", weighQueue[printIndex]);
				}
				else DEBUG_LOG("        ");
				if(printIndex < DESFEL_LENGTH(fel))
				{
					printLoop = 1;
					bufEvent = DESFEL_PEAKITH(fel, printIndex);
					DEBUG_LOG("%11d%s%3s, %6.2f, %2d)", printIndex, "(", DES_DTO_EVENT_TXT[(int)bufEvent.type], bufEvent.time, bufEvent.queue);
				}
				printIndex++;
				DEBUG_LOG("\n");
			}
			
		}
		
		//get next event
		struct desfel_event_s currentEvent = DESFEL_GETNEXT(fel);
		simTime = currentEvent.time;
		
		//handle event
		switch(currentEvent.type)
		{
		case DES_DTO_EVENT_ALQ:
			//loader available
			if(loaders < 2)
			{
				//a loader is now busy
				loaders++;
				//scedual end loading event
				double serviceTime = desdis_empDiscrete(&loadingTable);
				bufEvent.time = simTime + serviceTime;
				bufEvent.type = DES_DTO_EVENT_EL;
				bufEvent.queue = currentEvent.queue;
				DESFEL_INSERT(fel, bufEvent);
				//tally up the utilization time
				if(bufEvent.time < simEndTime) retVal.loaderUtilization += serviceTime;
				else retVal.loaderUtilization += serviceTime - (bufEvent.time - simEndTime);
			}
			//servers busy join queue
			else
			{
				loadQueue[lq++] = currentEvent.queue;
				
			}
			break;
		case DES_DTO_EVENT_EL:
			//loader free
			loaders--;
			//scale free
			if(weigher == 0)
			{
				//scale now busy
				weigher = 1;
				//scedual the end weighing event
				double serviceTime = desdis_empDiscrete(&weighingTable);
				bufEvent.time = simTime + serviceTime;
				bufEvent.type = DES_DTO_EVENT_EW;
				bufEvent.queue = currentEvent.queue;
				DESFEL_INSERT(fel, bufEvent);
				//tally up the utilization time
				if(bufEvent.time < simEndTime) retVal.scaleUtilization += serviceTime;
				else retVal.scaleUtilization += serviceTime - (bufEvent.time - simEndTime);
			}
			//scale busy join queue
			else
			{
				weighQueue[wq++] = currentEvent.queue;
			}
			
			//if there are trucks waiting in the loading queue get next customere
			if(lq>0)
			{
				//loader is now busy
				loaders++;
				//remove next truck from queue
				lq--;
				//secdual end event
				double serviceTime = desdis_empDiscrete(&loadingTable);
				bufEvent.time = simTime + serviceTime;
				bufEvent.type = DES_DTO_EVENT_EL;
				bufEvent.queue = loadQueue[0];
				DESFEL_INSERT(fel, bufEvent);
				//remove the truck from the queue
				for(int i = 0; i < lq; i++)
				{
					loadQueue[i] = loadQueue[i+1];
				}
				if(bufEvent.time < simEndTime) retVal.loaderUtilization += serviceTime;
				else retVal.loaderUtilization += serviceTime - (bufEvent.time - simEndTime);
			}
			break;
		case DES_DTO_EVENT_EW:
			//scale free
			weigher = 0;
			//secdual travel time
			double serviceTime = desdis_empDiscrete(&travelTable);
			bufEvent.time = simTime + serviceTime;
			bufEvent.type = DES_DTO_EVENT_ALQ;
			bufEvent.queue = currentEvent.queue;
			DESFEL_INSERT(fel, bufEvent);
			//if trucks in weighing queue get next truck
			if(wq>0)
			{
				//scale busy now
				weigher = 1;
				//remove from queue
				wq--;
				//scedual next end event
				double serviceTime = desdis_empDiscrete(&weighingTable);
				bufEvent.time = simTime + serviceTime;
				bufEvent.type = DES_DTO_EVENT_EW;
				bufEvent.queue = weighQueue[0];
				DESFEL_INSERT(fel, bufEvent);
				//remove the truck from the queue
				for(int i = 0; i < wq; i++)
				{
					weighQueue[i] = weighQueue[i+1];
				}
				//tally up the utilization time
				if(bufEvent.time < simEndTime) retVal.scaleUtilization += serviceTime;
				else retVal.scaleUtilization += serviceTime - (bufEvent.time - simEndTime);
			}
			break;
		case DES_DTO_EVENT_END:
			loop = 0;
			break;
		default:
			ERROR_LOG(stderr, "ERROR: dumptruck1 unknown event\n");
			retVal.loaderUtilization = -1;
			retVal.scaleUtilization = -1;
			break;
		}
	}
	
	//calc the util time
	retVal.loaderUtilization /= (simEndTime*2);
	retVal.scaleUtilization /= simEndTime;
	
	//print the output
	DEBUG_LOG("Execution parameters\n");
	DEBUG_LOG("\tSimeulation run length\t %8d minutes\n", simEndTime);
	DEBUG_LOG("\tNumber of Trucks\t %8d trucks\n", numberOfTrucks);
	DEBUG_LOG("Calcullated statistics\n");
	DEBUG_LOG("\tLoader utilization\t %8.2f\n", retVal.loaderUtilization);
	DEBUG_LOG("\tScale utilization\t %8.2f\n", retVal.scaleUtilization);
	
	//clean up
	DESFEL_FREE(fel);
	DEBUG_LOG("\n--- END OF DUMP TRUCK SIM ---\n");
	return retVal;
}
