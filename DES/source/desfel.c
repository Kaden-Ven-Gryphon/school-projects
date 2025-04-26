/****************************************************************************************
*
*	File:		desfel.c
*	Author:	Kaden Gryphon
*	Class:	CS 581-01
*	Program:	Program 1 Future Event List FEL
*	Due:		10.05.2022
*	Source:	Logic for heaps pulled from geeksforgeeks.org/binary-heap
*
*****************************************************************************************/


#include "desfel.h"

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

//The function defs for heap
#ifdef DESFEL_COM_HEAP

struct desfel_heap_fel_s* desfel_heap_init()
{
	struct desfel_heap_fel_s* retVal = (struct desfel_heap_fel_s*) malloc(sizeof(struct desfel_heap_fel_s));
	if(retVal == NULL)
	{
		ERROR_LOG(stderr, "ERROR: desfel_heap_init malloc failed to allocate memory\n");
		return NULL;
	}
	retVal->length = 0;
	retVal->maxLength = DESFEL_FEL_MAX_LENGTH;
	retVal->eventsHeap = (struct desfel_event_s*) malloc(retVal->maxLength * sizeof(struct desfel_event_s));
	if(retVal->eventsHeap == NULL)
	{
		ERROR_LOG(stderr, "ERROR: desfel_heap_init malloc failed to allocate memory\n");
		return NULL;
	}
	return retVal;
}


int desfel_heap_free(struct desfel_heap_fel_s* fel)
{
	if(fel != NULL)
	{
		if(fel->eventsHeap != NULL)
		{
			free(fel->eventsHeap);
		}
		free(fel);
		return 0;
	}
	//return 1 if the point was already NULL
	return 1;
}

//take last event in the array and move it to the top of the heap
//figure out if it needs to move down a layer
//worst case it has to move to the bottom layer which is O(log n) moves
struct desfel_event_s desfel_heap_getNext(struct desfel_heap_fel_s* fel)
{
	//same warning here will segfault if fel not init
	struct desfel_event_s retVal = fel->eventsHeap[0];
	
	if(fel->length == 1)
	{
		fel->length--;
		return retVal;
	}
	else if(fel->length <= 0)
	{
		DEBUG_LOG("\nList empty, can not get next\n");
		retVal.time = -1;
		return retVal;
	}
	
	//delete [0]
	fel->length--;
	fel->eventsHeap[0] = fel->eventsHeap[fel->length];
	
	//heapify
	int i = 0;
	struct desfel_event_s buf;
	
	//while the end of filled array is not reached
	while(1)
	{
		//find the smallest child
		//assume its the right childe
		int left = 0;
		
		//if a right child does not exist then assume its the left
		if((i*2)+2 >= fel->length)
		{
			left = 1;
			
			//if the left child also does not exitst then return its fine where it is
			if((i*2)+1 >= fel->length)
			{
				return retVal;
			}
		}
		
		//at this point if left is 1 then it has to left
		//else if left is 0 it could be left or right
		
		//its the left child exists and is smaller
		if(left || fel->eventsHeap[(i*2)+1].time < fel->eventsHeap[(i*2)+2].time)
		{
			buf = fel->eventsHeap[(i*2)+1];
			
			//DEBUG_LOG("\n\n\nTEST BUF %f, %c\n\n\n", buf.time, buf.type);
			
			//if the root is larger than the child swap
			if(fel->eventsHeap[i].time > buf.time)
			{
				fel->eventsHeap[(i*2)+1] = fel->eventsHeap[i];
				fel->eventsHeap[i] = buf;
				i = (i*2)+1;
			}
			//else leave it as is
			else
			{
				break;
			}
		}
		//else its the right child
		else
		{
			buf = fel->eventsHeap[(i*2)+2];
			
			//if the root is larger than the child swap
			if(fel->eventsHeap[i].time > buf.time)
			{
				fel->eventsHeap[(i*2)+2] = fel->eventsHeap[i];
				fel->eventsHeap[i] = buf;
				i = (i*2)+2;
			}
			//else leave it as is
			else
			{
				break;
			}
		}
	}
	
	//I am pretty sure the function never makes it to here
	return retVal;
}

//places new event at end of array
//then checks if it needs to move up a layaer of the heap
//worst case it has to travel to the top which a binary tree has log(n) layers
//Thus O(log n)
int desfel_heap_insert(struct desfel_heap_fel_s* fel, struct desfel_event_s newEvent)
{
	//check if there is room in the heap
	if(fel->length == fel->maxLength)
	{
		DEBUG_LOG("\nHeap overflow: Could not add event\n");
		return 1;
	}
	
	//insert new key at the end
	int i = fel->length++;
	fel->eventsHeap[i] = newEvent;
	
	//heapify
	struct desfel_event_s buf;
	while( i != 0 && fel->eventsHeap[(i-1)/2].time > fel->eventsHeap[i].time )
	{
		//swap i and its parrent
		buf = fel->eventsHeap[(i-1)/2];
		fel->eventsHeap[(i-1)/2] = fel->eventsHeap[i];
		fel->eventsHeap[i] = buf;
		
		//move index to the parent index
		i = (i-1)/2;
	}
	
	return 0;
}

//scans trhough the heap array, O(n)
int desfel_heap_delete(struct desfel_heap_fel_s* fel, char type, float time)
{
	for(int i = 0; i < fel->length; i++)
	{
		if(fel->eventsHeap[i].type == type && fel->eventsHeap[i].time == time)
		{
			fel->eventsHeap[i] = fel->eventsHeap[fel->length-1];
			fel->length--;
			//heapify
			struct desfel_event_s buf;
	
			//while the end of filled array is not reached
			while(1)
			{
				//find the smallest child
				//assume its the right childe
				int left = 0;
				
				//if a right child does not exist then assume its the left
				if((i*2)+2 >= fel->length)
				{
					left = 1;
					
					//if the left child also does not exitst then return its fine where it is
					if((i*2)+1 >= fel->length)
					{
						return 0;
					}
				}
				
				//at this point if left is 1 then it has to left
				//else if left is 0 it could be left or right
				
				//its the left child exists and is smaller
				if(left || fel->eventsHeap[(i*2)+1].time < fel->eventsHeap[(i*2)+2].time)
				{
					buf = fel->eventsHeap[(i*2)+1];
					
					//DEBUG_LOG("\n\n\nTEST BUF %f, %c\n\n\n", buf.time, buf.type);
					
					//if the root is larger than the child swap
					if(fel->eventsHeap[i].time > buf.time)
					{
						fel->eventsHeap[(i*2)+1] = fel->eventsHeap[i];
						fel->eventsHeap[i] = buf;
						i = (i*2)+1;
					}
					//else leave it as is
					else
					{
						break;
					}
				}
				//else its the right child
				else
				{
					buf = fel->eventsHeap[(i*2)+2];
					
					//if the root is larger than the child swap
					if(fel->eventsHeap[i].time > buf.time)
					{
						fel->eventsHeap[(i*2)+2] = fel->eventsHeap[i];
						fel->eventsHeap[i] = buf;
						i = (i*2)+2;
					}
					//else leave it as is
					else
					{
						break;
					}
				}
			}
		}
	}
	return 1;
}


int desfel_heap_length(struct desfel_heap_fel_s* fel)
{
	return fel->length;
}


struct desfel_event_s desfel_heap_peak(struct desfel_heap_fel_s* fel)
{
	//fel must be inilized or this will segfault
	//I could check for that here, but that could slow things down
	if(fel->length >0) return fel->eventsHeap[0];
	else
	{
		struct desfel_event_s buf;
		buf.time = 0;
		return buf;
	}
}


struct desfel_event_s desfel_heap_peakith(struct desfel_heap_fel_s* fel, int i)
{
	if(i == 0) return desfel_heap_peak(fel);
	struct desfel_event_s buf;
	if(i >= fel->length)
	{
		struct desfel_event_s buf;
		buf.time = 0;
		return buf;
	}
	struct desfel_heap_fel_s* cpy = desfel_heap_init();
	for(int j = 0; j < fel->length; j++)
	{
		desfel_heap_insert(cpy, fel->eventsHeap[j]);
	}
	for(int j = 0; j <= i; j++)
	{
		buf = desfel_heap_getNext(cpy);
	}
	return buf;
}

int desfel_heap_tostring(struct desfel_heap_fel_s* fel, char* outString, int outStringMax)
{
	struct desfel_heap_fel_s * felCopy = desfel_heap_init();
	struct desfel_event_s bufEvent;
	DEBUG_LOG("\n- Print FEL -\n");
	DEBUG_LOG("Length: %d\n\n", fel->length);
	
	//make copy of fel heap
	for(int i = 0; i < fel->length; i++)
	{
		felCopy->eventsHeap[i] = fel->eventsHeap[i];
	}
	felCopy->length = fel->length;
	
	//get next on the copy till empty and print each entry
	int i = 0;
	while(felCopy->length > 0)
	{
		bufEvent = desfel_heap_getNext(felCopy);
		DEBUG_LOG("Event %4d | Type: %c, Time %4.2f\n", i, bufEvent.type, bufEvent.time);
		i++;
	}
	DEBUG_LOG("\n- End of Print FEL -\n");
	return 0;
}
//END OF HEAP
#endif

//############################## ARRAY BASED  #####################################################
//function defs for array based fel
#ifdef DESFEL_COM_ARRAY

struct desfel_array_fel_s* desfel_array_init()
{
	struct desfel_array_fel_s* retVal = (struct desfel_array_fel_s*) malloc(sizeof(struct desfel_array_fel_s));
	retVal->length = 0;
	retVal->maxLength = 1024;
	retVal->eventsArray = (struct desfel_event_s*) malloc(retVal->maxLength * sizeof(struct desfel_event_s));
	return retVal;
}


int desfel_array_free(struct desfel_array_fel_s* fel)
{
	if(fel != NULL)
	{
		if(fel->eventsArray != NULL)
		{
			free(fel->eventsArray);
		}
		free(fel);
		return 0;
	}
	//return 1 if the point was already NULL
	return 1;
}


struct desfel_event_s desfel_array_getNext(struct desfel_array_fel_s* fel)
{
	struct desfel_event_s buf;
	if(fel->length <= 0)
	{
		DEBUG_LOG("\nList empty, can not get next\n");
		buf.time = -1;
		return buf;
	}
	buf = fel->eventsArray[0];
	for(int i = 1; i < fel->length; i++)
	{
		fel->eventsArray[i-1] = fel->eventsArray[i];
	}
	fel->length--;
	return buf;
}


int desfel_array_insert(struct desfel_array_fel_s* fel, struct desfel_event_s newEvent)
{
	//check if there is room in the array
	if(fel->length == fel->maxLength)
	{
		DEBUG_LOG("\nArray overflow: Could not add event\n");
		return 1;
	}

	
	int i = 0;
	int left = 0;
	int right = fel->length-1;
	if(newEvent.time >= fel->eventsArray[right].time)
	{
		left = right;
	}
	else if (newEvent.time <= fel->eventsArray[left].time)
	{
		left = -1;
		right = left;
	}
	while(left<right && left != right-1)
	{
		i = (right - left)/2 + left;
		if(fel->eventsArray[i].time < newEvent.time)
		{
			left = i;
		}
		else
		{
			right = i;
		}
	}
	//left = right = the index before where the new event should go
	for(int i = fel->length-1; i > left; i--)
	{
		fel->eventsArray[i+1] = fel->eventsArray[i];
	}
	fel->eventsArray[left+1] = newEvent;
	fel->length++;
	return 0;
}


int desfel_array_delete(struct desfel_array_fel_s* fel, char type, float time)
{
	int target = -1;
	for(int i = 0; i < fel->length; i++)
	{
		if(fel->eventsArray[i].type == type && fel->eventsArray[i].time == time) target = i;
	}
	if(target == -1)
	{
		DEBUG_LOG("\nTaget for deletion not found\n");
		return -1;
	}
	for(int i = target; i < fel->length-1; i++)
	{
		fel->eventsArray[i] = fel->eventsArray[i+1];
	}
	fel->length--;
	return target;
}


int desfel_array_length(struct desfel_array_fel_s* fel)
{
	return fel->length;
}


struct desfel_event_s desfel_array_peak(struct desfel_array_fel_s* fel)
{
	if(fel->length >0) return fel->eventsArray[0];
	else
	{
		struct desfel_event_s buf;
		buf.time = -1;
		return buf;
	}
}

struct desfel_event_s desfel_array_peakith(struct desfel_array_fel_s* fel, int i)
{
	if(fel->length > i) return fel->eventsArray[i];
	else
	{
		struct desfel_event_s buf;
		buf.time = -1;
		return buf;
	}
}


int desfel_array_tostring(struct desfel_array_fel_s* fel, char* outString, int outStringMax)
{
	DEBUG_LOG("\n- Print FEL -\n");
	DEBUG_LOG("Length: %d\n\n", fel->length);
	for(int i = 0; i < fel->length; i++)
	{
		DEBUG_LOG("Event %4d | Type: %c, Time %4.2f\n", i, fel->eventsArray[i].type, fel->eventsArray[i].time);
	}
	DEBUG_LOG("\n- End of Print FEL -\n");
	return 0;
}

	
#endif








//################################ AUTO TESTS  #####################################################
//The function defs for the auto tests
#ifdef DESFEL_TEST_AND_VERIFY_FEL

//heap testing functions
#ifdef DESFEL_COM_HEAP
	
void desfel_tav_printHeap(struct desfel_heap_fel_s* fel)
{
	int j = 0;
	int k = 1;
	DEBUG_LOG("\n--- Print of Heap ---\n\n");
	for(int i = 0; i < fel->length; i++)
	{
		DEBUG_LOG("Time %2.1f Type: %c\t", fel->eventsHeap[i].time, fel->eventsHeap[i].type);
		j++;
		if(j >= k)
		{
			DEBUG_LOG("\n\n");
			j = 0;
			k = k*2;
		}
	}
	DEBUG_LOG("\n\n--- End of Heap ---\n\n");
	return;
}

#endif


int desfel_tav_all()
{
	int t1 = desfel_tav_test1();
	int t2 = desfel_tav_test2();
	int t3 = desfel_tav_test3();
	
	DEBUG_LOG("\n\n\n--- SUMMARY ---\n\n");
	if(t1) DEBUG_LOG("Test 1: OK\n");
	else DEBUG_LOG("Test 1: FAIL\n");
	if(t2) DEBUG_LOG("Test 2: OK\n");
	else DEBUG_LOG("Test 2: FAIL\n");
	if(t3) DEBUG_LOG("Test 3: OK\n");
	else DEBUG_LOG("Test 3: FAIL\n");
	
	if(t1 && t2 && t3) DEBUG_LOG("All: OK\n");
	else DEBUG_LOG("All: FAIL\n");
	
	return t1 & t2 & t3;
}




//program 1 test 1
//add 100 events then remove all of them and check that they are in order
int desfel_tav_test1()
{
	DEBUG_LOG("\n\n--- START TEST 1 ---\n\n");
	DEBUG_LOG("Implmentation: %s\n\n",DESFEL_FEL_TYPE_STRING);
	
	struct DESFEL_FEL_S * fel = DESFEL_INIT();
	struct desfel_event_s bufEvent;
	DESDIS_NORMAL_INIT;
	
	int expLength = 100;
	int actLength = 0;
	
	//insert 100 events
	for(int i = 0; i < expLength; i++)
	{
		if(desdis_double_rand() < 0.5) bufEvent.type = 'a';
		else bufEvent.type = 'd';
		bufEvent.time = fabs(DESDIS_NORMAL(50, 30));
		DEBUG_LOG("Insert | type: %c, time: %4.2f\n", bufEvent.type, bufEvent.time);
		DESFEL_INSERT(fel, bufEvent);
	}
	DEBUG_LOG("\n");
	actLength = DESFEL_LENGTH(fel);
	
	//print the fel
	char * ph1 = NULL;
	int ph2 = 0;
	DESFEL_TOSTRING(fel, ph1, ph2);
	
	//get next until empty, count removes
	int eventsRemoved = 0;
	float lastEventTime = 0;
	int inOrder = 1;
	
	while(fel->length > 0)
	{
		bufEvent = DESFEL_GETNEXT(fel);
		if(lastEventTime <= bufEvent.time) inOrder = 1;
		else inOrder = 0;
		lastEventTime = bufEvent.time;
		eventsRemoved++;
		DEBUG_LOG("Get Next | type: %c, time: %4.2f\n", bufEvent.type, bufEvent.time);
	}
	DEBUG_LOG("\n");
	DESFEL_TOSTRING(fel, ph1, ph2);
	DEBUG_LOG("\n");
	
	int retVal = 1;
	if(expLength == actLength)
	{
		DEBUG_LOG("Expected to add %d events, added %d: OK\n", expLength, actLength);
	}
	else
	{
		DEBUG_LOG("Expected to add %d events, added %d: FAIL\n", expLength, actLength);
		retVal = 0;
	}
	
	if(expLength == eventsRemoved)
	{
		DEBUG_LOG("Expected to remove %d events, removed %d: OK\n", expLength, eventsRemoved);
	}
	else
	{
		DEBUG_LOG("Expected to remove %d events, removed %d: FAIL\n", expLength, eventsRemoved);
		retVal = 0;
	}
	
	if(inOrder) DEBUG_LOG("Removed in order: OK\n");
	else
	{
		DEBUG_LOG("Removed in order: FAIL\n");
		retVal = 0;
	}
	
	
	DEBUG_LOG("\n\n--- END TEST 1 ---\n\n");
	return retVal;
}

//program 1 test 2 run 100 add or remove events
//check that they are in order and that the number of
//added/removed matches end length
int desfel_tav_test2()
{
	DEBUG_LOG("\n\n--- START TEST 2 ---\n\n");
	DEBUG_LOG("Implmentation: %s\n\n",DESFEL_FEL_TYPE_STRING);
	
	struct DESFEL_FEL_S * fel = DESFEL_INIT();
	struct desfel_event_s bufEvent;
	DESDIS_NORMAL_INIT;
	
	float lastEventTime = 0;
	int eventsAdded = 0;
	int eventsRemoved = 0;
	int inOrder = 1;
	//run 100 steps
	for(int i = 0; i < 100; i++)
	{
		if(desdis_double_rand() < 0.6)
		{
			//add event
			if(desdis_double_rand() < 0.5) bufEvent.type = 'a';
			else bufEvent.type = 'd';
			bufEvent.time = lastEventTime + fabs(DESDIS_NORMAL(5, 3));
			DEBUG_LOG("Insert | type: %c, time: %4.2f\n", bufEvent.type, bufEvent.time);
			DESFEL_INSERT(fel, bufEvent);
			eventsAdded++;
		}
		else
		{
			//remove event
			if(DESFEL_LENGTH(fel) > 0)
			{
				bufEvent = DESFEL_GETNEXT(fel);
				if(lastEventTime <= bufEvent.time) inOrder = 1;
				else inOrder = 0;
				lastEventTime = bufEvent.time;
				eventsRemoved++;
				DEBUG_LOG("Get Next | type: %c, time: %4.2f\n", bufEvent.type, bufEvent.time);
			}
		}
	}
	
	//print the fel
	char * ph1 = NULL;
	int ph2 = 0;
	DESFEL_TOSTRING(fel, ph1, ph2);
	
	if(inOrder) DEBUG_LOG("In Order: OK\n");
	else DEBUG_LOG("In Order: FAIL\n");
	
	DEBUG_LOG("\n\nInserted: %4d, Removed: %4d, Length: %4d: ", eventsAdded, eventsRemoved, DESFEL_LENGTH(fel));
	
	if(DESFEL_LENGTH(fel) + eventsRemoved == eventsAdded)
	{
		DEBUG_LOG("OK\n");
		DEBUG_LOG("\n\n--- END TEST 2 ---\n\n");
		return 1 & inOrder;
	}
	else
	{
		DEBUG_LOG("FAIL\n");
		DEBUG_LOG("\n\n--- END TEST 2 ---\n\n");
		return 1 & inOrder;
	}
	
	return 0;
}

//add 5 events
//print fel
//remove 3 events
//print fel
//confirm the the right evetns where removed
int desfel_tav_test3()
{	
	DEBUG_LOG("\n\n--- START TEST 3 ---\n\n");
	DEBUG_LOG("Implmentation: %s\n\n",DESFEL_FEL_TYPE_STRING);
	
	struct DESFEL_FEL_S * fel = DESFEL_INIT();
	struct desfel_event_s bufEvent;
	
	bufEvent.type = 'a';
	bufEvent.time = 10;
	DEBUG_LOG("Insert | type: %c, time: %4.2f\n", bufEvent.type, bufEvent.time);
	DESFEL_INSERT(fel, bufEvent);
	bufEvent.type = 'a';
	bufEvent.time = 7;
	DEBUG_LOG("Insert | type: %c, time: %4.2f\n", bufEvent.type, bufEvent.time);
	DESFEL_INSERT(fel, bufEvent);
	bufEvent.type = 'd';
	bufEvent.time = 11;
	DEBUG_LOG("Insert | type: %c, time: %4.2f\n", bufEvent.type, bufEvent.time);
	DESFEL_INSERT(fel, bufEvent);
	bufEvent.type = 'a';
	bufEvent.time = 12;
	DEBUG_LOG("Insert | type: %c, time: %4.2f\n", bufEvent.type, bufEvent.time);
	DESFEL_INSERT(fel, bufEvent);
	bufEvent.type = 'd';
	bufEvent.time = 9;
	DEBUG_LOG("Insert | type: %c, time: %4.2f\n", bufEvent.type, bufEvent.time);
	DESFEL_INSERT(fel, bufEvent);
	
	//print the fel
	char * ph1 = NULL;
	int ph2 = 0;
	DESFEL_TOSTRING(fel, ph1, ph2);
	
	DESFEL_DELETE(fel, 'a', 7);
	DEBUG_LOG("Delete | type: %c, time: %4.2f\n", 'a', 7.0);
	DESFEL_DELETE(fel, 'a', 12);
	DEBUG_LOG("Delete | type: %c, time: %4.2f\n", 'a', 12.0);
	DESFEL_DELETE(fel, 'd', 9);
	DEBUG_LOG("Delete | type: %c, time: %4.2f\n", 'd', 9.0);
	
	DESFEL_TOSTRING(fel, ph1, ph2);
	
	//check if the two remaining events are the right ones
	int isGood = 1;
	bufEvent = DESFEL_GETNEXT(fel);
	
	if(bufEvent.type == 'a' && bufEvent.time == 10);
	else isGood = 0;
	
	bufEvent = DESFEL_GETNEXT(fel);
	
	if(bufEvent.type == 'd' && bufEvent.time == 11);
	else isGood = 0;
	
	//check that there was only 2 remaining
	if(DESFEL_LENGTH(fel) == 0);
	else isGood = 0;
	
	if(isGood)
	{
		DEBUG_LOG("Checked: OK\n");
		DEBUG_LOG("\n\n--- END TEST 3 ---\n\n");
		return 1;
	}
	else
	{
		DEBUG_LOG("Checked: FAIL\n");
		DEBUG_LOG("\n\n--- END TEST 3 ---\n\n");
		return 0;
	}
	
	return 0;
}


//END OF TEST AND VERIFY
#endif
