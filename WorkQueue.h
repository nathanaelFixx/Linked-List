#ifndef WORKQUEUE_H
#define WORKQUEUE_H

#include <iostream>
#include <string>
#include "WorkItem.h"

///////////////////////
//Nathanael Fixx
//CSS 342 - Project 2
//27 hours
//Cygwin
///////////////////////

///////////////////////////////////////////////////////////////////////////////
//Allows for creation of a list, use of the the "=" sign,
//copying, adding items to the list, finding the next work item of the
//same priority, checking if a work item exists, checking is a key exists, 
//moving an item to the highest position in it's priority, getting the number
//of items, getting the number of items in a given priority, deleting a
//specific item, and deleting all the work items.
///////////////////////////////////////////////////////////////////////////////

using namespace std;

class WorkQueue
{
public:
	WorkQueue();
	
	//copy constructor
	WorkQueue(const WorkQueue & lst);
	WorkQueue & operator =(const WorkQueue & lst);
	
	//adds to list
	void addWorkItem(const WorkItem & lst);
	
	//gets the next work item for a given priority
	const WorkItem & nextWorkItem(int priority) const;
	
	//checks if WorkItem is in the list
	bool hasWorkItem(int priority) const;
	
	//checks if specific key is in the list
	bool containsKey(const string & key) const;
	
	//moves item to the front of it's priority level
	void bumpWorkItem(const string & key);
	
	//getters//
	int getNumWorkItems() const;
	int getNumWorkItems(int priority) const;
	///////////
	
	void deleteWorkItem(string key);
	void deleteAllWorkItems();
	virtual ~WorkQueue();
private:
	int counter;
	WorkItem * head;
};
#endif

