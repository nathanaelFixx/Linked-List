#ifndef WORKITEM_H
#define WORKITEM_H

#include <iostream>
#include <string>
using namespace std;

//////////////////////
//Nathanael Fixx
//CSS 342 - Project 2
//27 hours
//Cygwin
//////////////////////

//////////////////////////////////////////////////////////////////////////
//Allows for the creating of a WorkItem. Each item stores a priority level
//and a key which can be retrieved. Items can also be set equal to each
//other
//////////////////////////////////////////////////////////////////////////
class WorkItem
{
public:
	//constructors//
	WorkItem(int priority, const string & key);
	WorkItem(const WorkItem & item);
	///////////////
	
	//getters//
	int getPriority() const;
	const string &getKey() const;
	///////////
	WorkItem & operator = (const WorkItem & rhs);

	WorkItem * next;
	int priority;
	string key;
private:
	
};
#endif

