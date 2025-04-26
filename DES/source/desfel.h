/****************************************************************************************
*
*	File:		desfel.h
*	Author:	Kaden Gryphon
*	Class:	CS 581-01
*	Program:	Program 1 Future Event List FEL
*	Due:		10.31.2022
*
*	Use:		A future event list for a des, with multiple implmentations
*****************************************************************************************/

#ifndef DESFEL_H
#define DESFEL_H

#include <stdlib.h>
#include <string.h>
#include <time.h>


#define DESFEL_FEL_MAX_LENGTH 1024

//control for added test and verify code
#define DESFEL_TEST_AND_VERIFY_FEL
#ifdef DESFEL_TEST_AND_VERIFY_FEL
	#include "desdis.h"
#endif
//control for choosing which data structure to use for the FEL
#define DESFEL_HEAP
//#define DESFEL_ARRAY
//#define DESFEL_LINKLIST

//control to chose which functions get compiled
#define DESFEL_COM_HEAP
//#define DESFEL_COM_ARRAY
//#define DESFEL_COM_LINKLIST

//the min heap data structure
#ifdef DESFEL_HEAP
	#define DESFEL_FEL_TYPE_STRING "Min Heap"
	#define DESFEL_FEL_S desfel_heap_fel_s
	#define DESFEL_INIT desfel_heap_init
	#define DESFEL_FREE desfel_heap_free
	#define DESFEL_GETNEXT desfel_heap_getNext
	#define DESFEL_INSERT desfel_heap_insert
	#define DESFEL_DELETE desfel_heap_delete
	#define DESFEL_LENGTH desfel_heap_length
	#define DESFEL_PEAK desfel_heap_peak
	#define DESFEL_PEAKITH desfel_heap_peakith
	#define DESFEL_TOSTRING desfel_heap_tostring
#endif

//array data structure
#ifdef DESFEL_ARRAY
	#define DESFEL_FEL_TYPE_STRING "Array"
	#define DESFEL_FEL_S desfel_array_fel_s
	#define DESFEL_INIT desfel_array_init
	#define DESFEL_FREE desfel_array_free
	#define DESFEL_GETNEXT desfel_array_getNext
	#define DESFEL_INSERT desfel_array_insert
	#define DESFEL_DELETE desfel_array_delete
	#define DESFEL_LENGTH desfel_array_length
	#define DESFEL_PEAK desfel_array_peak
	#define DESFEL_PEAKITH desfel_array_peakith
	#define DESFEL_TOSTRING desfel_array_tostring
#endif

//linked list data structure
#ifdef DESFEL_LINKLIST
	#define DESFEL_FEL_TYPE_STRING "Linked List"
	#define DESFEL_FEL_S desfel_ll_fel_s
	#define DESFEL_INIT desfel_ll_init
	#define DESFEL_FREE desfel_ll_free
	#define DESFEL_GETNEXT desfel_ll_getNext
	#define DESFEL_INSERT desfel_ll_insert
	#define DESFEL_DELETE desfel_ll_delete
	#define DESFEL_LENGTH desfel_ll_length
	#define DESFEL_PEAK desfel_ll_peak
	#define DESFEL_PEAKITH desfel_ll_peakith
	#define DESFEL_TOSTRING desfel_ll_tostring
#endif

struct desfel_event_s
{
	char type;
	float time;
	int queue;
	void* customer;	
};


//The function prototyes for heap
#ifdef DESFEL_COM_HEAP

	struct desfel_heap_fel_s
	{
		struct desfel_event_s* eventsHeap;
		int length;
		int maxLength;
	};
	struct desfel_heap_fel_s* desfel_heap_init();
	int desfel_heap_free(struct desfel_heap_fel_s* fel);
	struct desfel_event_s desfel_heap_getNext(struct desfel_heap_fel_s* fel);
	int desfel_heap_insert(struct desfel_heap_fel_s* fel, struct desfel_event_s newEvent);
	int desfel_heap_delete(struct desfel_heap_fel_s* fel, char type, float time);
	int desfel_heap_length(struct desfel_heap_fel_s* fel);
	struct desfel_event_s desfel_heap_peak(struct desfel_heap_fel_s* fel);
	struct desfel_event_s desfel_heap_peakith(struct desfel_heap_fel_s* fel, int i);
	int desfel_heap_tostring(struct desfel_heap_fel_s* fel, char* outString, int outStringMax);
#endif

//function protoypes for array based fel
#ifdef DESFEL_COM_ARRAY

	struct desfel_array_fel_s
	{
		struct desfel_event_s* eventsArray;
		int length;
		int maxLength;
	};
	struct desfel_array_fel_s* desfel_array_init();
	int desfel_array_free(struct desfel_array_fel_s* fel);
	struct desfel_event_s desfel_array_getNext(struct desfel_array_fel_s* fel);
	int desfel_array_insert(struct desfel_array_fel_s* fel, struct desfel_event_s newEvent);
	int desfel_array_delete(struct desfel_array_fel_s* fel, char type, float time);
	int desfel_array_length(struct desfel_array_fel_s* fel);
	struct desfel_event_s desfel_array_peak(struct desfel_array_fel_s* fel);
	struct desfel_event_s desfel_array_peakith(struct desfel_array_fel_s* fel, int i);
	int desfel_array_tostring(struct desfel_array_fel_s* fel, char* outString, int outStringMax);
#endif

#ifdef DESFEL_COM_LINKLIST

#endif





//The function prototypes for the auto tests
//This will test the primary data structure
#ifdef DESFEL_TEST_AND_VERIFY_FEL
	
	#ifdef DESFEL_COM_HEAP
		void desfel_tav_printHeap(struct desfel_heap_fel_s* fel);
	#endif
	
	int desfel_tav_all();
	int desfel_tav_test1();
	int desfel_tav_test2();
	int desfel_tav_test3();
	
#endif


#endif
