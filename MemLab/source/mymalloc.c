/*
*	mymalloc.c
*	Kaden Gryphon
*	
*
*	simple libray for lab 1 that implments mymalloc, and myfree for a mmap heap
*
*
*/

#include "mymalloc.h"

//#define _DEBUG_MESSAGES
#ifdef _DEBUG_MESSAGES
	#include <stdio.h>
	#define DEBUG_LOG(fmt, ...) fprintf(stdout, "DEBUG:%s:%d:%s()" fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
	#define DEBUG_LOG(...) {}
#endif
#define _ERROR_MESSAGES
#ifdef _ERROR_MESSAGES
	#include <stdio.h>
	#define ERROR_LOG(fmt, ...) fprintf(stderr, "DEBUG:%s:%d:%s()" fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
	#define ERROR_LOG(...) {}
#endif



#ifndef HEAP_SIZE_OVERRIDE
#define HEAP_SIZE 4096
#else
#define HEAP_SIZE HEAP_SIZE_OVERRIDE
#endif

typedef struct __node_t {
	int size;
	struct __node_t *next;
} node_t;


typedef struct {
	int size;
	//this var is unused but is here to fill space to garentee a header is larger than a node
	node_t n;
	//int magic;
} header_t;


int heapInit = 0;
node_t *head;


void* mymalloc(int size)
{
	DEBUG_LOG("START: mymalloc with size:%d\n", size);
	
	//use mmap to init the heap
	if (!heapInit)
	{
		DEBUG_LOG("Init heap\n");
		head = mmap(NULL, HEAP_SIZE, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
		head->size = HEAP_SIZE - sizeof(node_t);
		head->next = NULL;
		heapInit = 1;
		DEBUG_LOG("Size of header_t %ld\n", sizeof(header_t));
		DEBUG_LOG("Size of node_t %ld\n", sizeof(node_t));
		DEBUG_LOG("Size of ptrNode %ld and %ld\n", sizeof(node_t*), sizeof(struct __node_t*)); 
	}
	
	//search the list for a large enough space or nodeptr will be null
	node_t *nodeptr = head;
	node_t *nodeptrLast = NULL;
	do
	{
		if(nodeptr->size > size + sizeof(header_t))
		{
			break;
		}
		nodeptrLast = nodeptr;
		nodeptr = nodeptr->next;
	} while (nodeptr != NULL);
	
	//if an apropriate free mem was not located
	if (nodeptr == NULL)
	{
		DEBUG_LOG("Heap does not have a large enough memory chunk to alocate\n");
		return NULL;
	}
	
	//move the node struct down the heap
	//newNode is the target size + header size bytes after the current node
	node_t *newNode = (node_t*) (((char*)nodeptr) + (sizeof(header_t) + size));
	
	//copy the node to the new location
	*newNode = *nodeptr;
	//update the nodes free space
	newNode->size -= (sizeof(header_t) + size);
	//if a previous node pointed to this one update it's next pointer
	if(nodeptrLast != NULL)
	{
		nodeptrLast->next = newNode;
	}
	//else this was the head node, update the head pointer
	head = newNode;
	
	
	//the free list is now updated and nodeptr points to the allicated mem
	//write the header
	((header_t*)nodeptr)->size = size;
	//((header_t*)nodeptr)->magic = 0;
	
	//sanity check, tally up free memory and count list entries
	#ifdef _DEBUG_MESSAGES
		DEBUG_LOG("\t--Malloc Summary--\n");
		int dFreespace = 0;
		int dNumNodes = 0;
		node_t *dnodeptr = head;
		while(dnodeptr != NULL)
		{
			dNumNodes++;
			dFreespace += dnodeptr->size;
			dnodeptr = dnodeptr->next;
		}
		DEBUG_LOG("\t--Number of nodes: %d\n", dNumNodes);
		DEBUG_LOG("\t--Free space: %d\n", dFreespace);
	#endif
	
	
	
	DEBUG_LOG("END: mymalloc with size:%d\n", size);
	//return a pointer to right after the header
	return (header_t*)nodeptr+1;
}

void myfree(void *ptr)
{
	DEBUG_LOG("START: myfree\n");
	
	if (ptr == NULL)
	{
		DEBUG_LOG("ptr already NULL\n");
		return;
	}
	DEBUG_LOG("Freeing %d + %ld bytes\n", ((header_t*)ptr -1)->size, sizeof(header_t));
	//get a ptr to the node of the newly free section
	node_t *newNode = (node_t*)((header_t*)ptr - 1);
	newNode->size = ((header_t*)newNode)->size + sizeof(header_t) - sizeof(node_t);
	
	//now needs to find the node, if it exists, that comes before and after the freed mem
	node_t *prevNode = NULL;
	node_t *nextNode = NULL;
	node_t *nodeptr = head;
	
	DEBUG_LOG("Find prev node\n");
	//find the node before the freed mem
	while(nodeptr != NULL)
	{
		if (nodeptr < newNode)
		{
			prevNode = nodeptr;
		}
		nodeptr = nodeptr->next;
	}
	
	DEBUG_LOG("Find next node\n");
	//find the node after the freed mem
	nodeptr = head;
	while(nodeptr != NULL)
	{
		if (nodeptr > newNode)
		{
			nextNode = nodeptr;
			break;
		}
		nodeptr = nodeptr->next;
	}
	
	//I am sure there is a more effciant way to find these, going through the list once instead of twice, but I am just keeping it simple
	//check if the next node is next to newnode
	if(((char*)newNode) + newNode->size + sizeof(node_t) == (char*)nextNode)
	{
		DEBUG_LOG("Merging with nextNode\n");
		newNode->size += nextNode->size + sizeof(node_t);
		newNode->next = nextNode->next;
	}
	//else add nextnode to the list
	else
	{
		DEBUG_LOG("added nextNode to list\n");
		newNode->next = nextNode;
	}
	//if nextNode was the head switch head to the newNode
	if(head == nextNode)
	{
		head = newNode;
	}
	
	
	//check if the prevNode is connected to newNode
	if(prevNode != NULL)
	{
		if(((char*)prevNode) + prevNode->size + sizeof(node_t) == (char*)newNode)
		{
			DEBUG_LOG("Merging with prevNode\n");
			prevNode->size += newNode->size + sizeof(node_t);
			prevNode->next = newNode->next;
			if(head == newNode)
			{
				head = prevNode;
			}
		}
		//else link the prevNode to newNode
		else
		{
			//sanity check
			if(prevNode->next != newNode->next)
			{
				ERROR_LOG("List broken, preNode next does not equal newNode next\n");
			}
			else
			{
				DEBUG_LOG("added newNode to list\n");
				prevNode->next = newNode;
			}
		}
	}
	
	//sanity check, tally up free memory and count list entries
	#ifdef _DEBUG_MESSAGES
		DEBUG_LOG("\t--Malloc Summary--\n");
		int dFreespace = 0;
		int dNumNodes = 0;
		node_t *dnodeptr = head;
		while(dnodeptr != NULL)
		{
			dNumNodes++;
			dFreespace += dnodeptr->size;
			dnodeptr = dnodeptr->next;
		}
		DEBUG_LOG("\t--Number of nodes: %d\n", dNumNodes);
		DEBUG_LOG("\t--Free space: %d\n", dFreespace);
	#endif
	
	DEBUG_LOG("END: myfree\n");
}
