#include "WorkItem.h"

//////////////////////
//Nathanael Fixx
//CSS 342 - Project 2
//27 hours
//Cygwin
//////////////////////

//constructor
WorkItem::WorkItem(int priority, const string & key)
{
	this->priority = priority;
	this->key = key;
	next = NULL;
}

//copy constructor
WorkItem::WorkItem(const WorkItem & item)
{
	this->priority = item.priority;
	this->key = item.key;

}

/////////////////////////////////
//Get Priority
//
//Returns the priority value of
//the item.
/////////////////////////////////
int WorkItem::getPriority() const
{
	return priority;
}
///////////////////////////////////////
//Get Key
//
//Returns the key of the item
///////////////////////////////////////
const string & WorkItem::getKey() const
{

	return key;
}
//////////////////////////////////
//Operator Equals
//
//OVerloads the equals operator
/////////////////////////////////
WorkItem & WorkItem:: operator = (const WorkItem & rhs)
{
	key = rhs.key;
	priority = rhs.priority;

	WorkItem finalItem = WorkItem(priority, key);

	return finalItem;
}
