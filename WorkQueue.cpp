#include "WorkQueue.h"
#include "WorkItem.h"
#include <stdexcept>

//////////////////////
//Nathanael Fixx
//CSS 342 - Project 2
//27 hours
//Cygwin
//////////////////////


//default constructor 
WorkQueue::WorkQueue()
{
	head = NULL;
	counter = 0;
}

//copy constructor, calls the assignment operator
WorkQueue::WorkQueue(const WorkQueue & lst)
{
	counter = 0;
	*this = lst;

}
///////////////////////////////////////////////////////
//Overloading operator equals
//
//Checks if lft and rht are equal and if not, deletes 
//lft, makes a copy of rht and returns that copy
///////////////////////////////////////////////////////
WorkQueue & WorkQueue::operator =(const WorkQueue & lst)
{
//checking if equal
	WorkItem * lft = head;
	WorkItem * rht = lst.head;

	while (rht == lft) {
		if (rht->next == NULL || lft->next == NULL)
			break;
		rht = rht->next;
		lft = lft->next;
	}

	if (rht != NULL && lft != NULL) { //if the lists are the same, return

		if (rht->next == NULL && lft->next == NULL && counter > 1)
			return *this;
	}

//deleting the left hand side of equation
		
	if (counter > 0){
		WorkItem * cur = head;
		WorkItem * prev = head;
		while (cur != NULL) {
			prev = cur;
			cur = cur->next;
			delete prev;
			prev = NULL;
			counter--;
		}
	}

//making the copy
		if (lst.counter == 0) {
			head = NULL;
		}

		else if (lst.counter == 1) {
			head = new WorkItem(*lst.head);
			counter++;
		}

		else {
			WorkItem * oldList, *current, *prev;

			
			head = new WorkItem (*lst.head);
			prev = head;
			current = head->next;
			oldList = (lst.head->next);
			counter++;
			
			while (oldList != NULL) {
				current = new WorkItem(*oldList);
				prev->next = current;
				oldList = oldList->next;
				prev = prev->next;
				current = current->next;
				counter++;
			}
			prev->next = NULL;
		}
	return *this;
}
////////////////////////////////////////////////
//Add Work Item
//
//adds work items to list in order from highest
//priority to lowest priority. Items in the
//same priority are ordered by the time that
//they were added to the list. 
////////////////////////////////////////////////
void WorkQueue::addWorkItem(const WorkItem & lst)
{
	WorkItem * add = new WorkItem(lst);
	
	//if adding to an empty list, make the item the head
	if (counter == 0) {
		head = add;
		head->next = NULL;
		counter++;
		return;
	}
	
	WorkItem * oldList = head;

	//decides whether to make new head or add to back
	if (counter == 1) {
		if (add->priority < oldList->priority) {
			head = add;
			head->next = oldList;
			oldList->next = NULL;
			counter++;
			return;
		}
		else if (add->priority >= oldList->priority) {
			head->next = add;
			add->next = NULL;
			counter++;
			return;
		}
	}
	
	WorkItem * oldListPrev = head;
	oldList = oldList->next;

	if (add->priority == oldListPrev->priority) {
		oldListPrev->next = add;
		add->next = oldList;
		counter++;
		return;
	}

	//goes through the list until it finds a priority that is greater than itself
	while (add->priority > oldList->priority && oldList->next != NULL) {
		oldList = oldList->next;
		oldListPrev = oldListPrev->next;
	}

	if (oldList->next == NULL) {
		
			oldList->next = add;
			add->next = NULL;
			counter++;
			return;
		}
		
	else if (oldList->next->priority != oldList->priority) {	
		add->next = oldList->next;
		oldList->next = add;
		counter++;
		return;
	}

	else {
		while (oldList->next != NULL) {
			if (oldList->next->priority != oldList->priority) {
				add->next = oldList->next;
				oldList->next = add;
				counter++;
				return;
			}
			oldList = oldList->next;
		}
		oldList->next = add;
		add->next = NULL;
		counter++;
		return;
	}

}
//////////////////////////////////////////////////
//Next Work Item
//
//returns the WorkItem for that matches the 
//priority that is passed in. If a WorkItem with
//the exact priority is not found, the closest
//priority that is lower than the priority passed
//in will be returned. Ex. If priotity 7 is passed 
//in and there are no prioity 7's, but there is a 
//priority 8. The item with priority 8 will be 
//returned.
///////////////////////////////////////////////////
const WorkItem & WorkQueue::nextWorkItem(int priority) const
{
	WorkItem * current = head;
	WorkItem * secondTry = head;

//check if the list is empty
	if (counter == 0) {
		throw logic_error("Priority Not Found!");
	}

//check if first item is equal to priority
	if (current->priority == priority) {
		return *current;
	}

	current = current->next;

//check other items	
	while (current != NULL) {
		if (current->priority == priority) {
			return *current;
		}
		else {
			current = current->next;
		}
	}
	//try again, this time not looking for exact value
	
	//try if first item is > priority
	if (secondTry->priority > priority) {
		return *secondTry;
	}

	secondTry = secondTry->next;

	while (secondTry != NULL) {
		if (secondTry->priority > priority) {
			return *secondTry;
		}
		else {
			secondTry = secondTry->next;
		}
	}

	if (secondTry == NULL)
		throw logic_error("Priority Not Found!");
}
////////////////////////////////////////////////////////
//Has Work Item
//
//goes through list trying to find a priority that is
//greater or equal to the given priority. If it finds
//a priority greater, there are no priorities that are
//are equal since the list is ordered.
////////////////////////////////////////////////////////
bool WorkQueue::hasWorkItem(int priority) const
{
	WorkItem * current = head;
	while (current != NULL) {
		if (current->priority >= priority)
			return true;
		else
			current = current->next;
	}
	return false;
}
////////////////////////////////////////////////////
//Contains Key
//
//Goes through the list until it finds a matching
//key. If the key is found, returns true, otherwise
//returns false.
////////////////////////////////////////////////////
bool WorkQueue::containsKey(const string & key) const
{
	WorkItem * current = head;

	//goes through the list trying to match the key.
	while (current != NULL) {
		if (current->key == key)
			return true;
		else
			current = current->next;
	}
	return false;
}
///////////////////////////////////////////////
//Bump Work Item
//
//finds the item with the given key and moves
//it ahead of any other items that are in the
//same priority level. If the item is the only
//one in it's priority level, it will not be 
//bumped. If the item is the head of the list,
//it will not be bumped.
///////////////////////////////////////////////
void WorkQueue::bumpWorkItem(const string & key)
{
   //the key at head will never need to be bumped
   WorkItem * currentKey = head->next;
   WorkItem * currentKeyPrev = head;
   WorkItem * currentPriority = head; 
   WorkItem * checker = head;
   int priorityCount = 0;

   //check to see if the second item in the queue is what needs to be bumped
     if (currentKey->key == key && currentKeyPrev->priority == currentKey->priority) {
	currentKeyPrev->next = currentKey->next;
	currentKey->next = currentKeyPrev;
	head = currentKey;
	return;
	}
	
     //finds the key in the queue that matches the expected key
     while (currentKey != NULL) {
	if (currentKey->key == key) {
	//iterates through list counting how many items have the same priority
			
	   //checks if the item is the first of it's priority in the queue
		if (currentKeyPrev->priority != currentKey->priority) {
			return;
		}
			
		  while (checker != NULL) {
			if (checker->priority == currentKey->priority) {
				priorityCount++;
				checker = checker->next;
			}
				else {
				 checker = checker->next;
				}
			}
			
		//cannot bump if it is already the highest in it's priority
		if (priorityCount == 1) {
			return;
		}

		//bumps to highest priority
	while (currentPriority != NULL) {
	    if (currentPriority->next->priority == currentKey->priority) {
			WorkItem * second = currentPriority;
			WorkItem * secNext = second->next;
			WorkItem * curNext = currentKey->next;
			WorkItem * curPrev = currentKeyPrev;
			second->next = currentKey;
			currentKey->next = secNext;
			curPrev->next = curNext;
			return;
		}
		
			    currentPriority = currentPriority->next;
			}
		}
		currentKey = currentKey->next;
		currentKeyPrev = currentKeyPrev->next;
	}
}
/////////////////////////////////////
//Get Num Work Items
//
//counter has been keeping track of
//the addition and removal of
//elements through all the functions
/////////////////////////////////////
int WorkQueue::getNumWorkItems() const
{
	return counter;
}
///////////////////////////////////////////////////////////
//Get Num Work Items (with priority)
//
//goes through the list and increments each time it finds
//an item with the given priority.
///////////////////////////////////////////////////////////
int WorkQueue::getNumWorkItems(int priority) const
{
	WorkItem * current = head;
	int priorityCount = 0;

	while (current != NULL) {
		if (current->priority == priority) {
			priorityCount++;
			current = current->next;
		}
		else 
			current = current->next;
	}
	return priorityCount;
}

/////////////////////////////////////////
//Delete Work Item
//
//Searches through the list until it 
//finds a matching key. Then deletes the
//item and reconnects pointers.
/////////////////////////////////////////

void WorkQueue::deleteWorkItem(string key)
{
	WorkItem * current = head;
	WorkItem * prev = head;

	if (counter == 0) {
		return;
	}

//check if head needs to be deleted
	if (current->key == key) {
		head = current->next;
		delete current;
		counter--;
		return;
	}
	current = current->next;
//if any other item needs to be deleted
	while (current != NULL) {
		if (current->key == key) {
			prev->next = current->next;
			delete current;
			counter--;
			return;
		}
		else {
			current = current->next;
			prev = prev->next;
		}
	}
}
////////////////////////////////////////
//Delete All Work Items
//
//goes through the list item by item and
//deletes the item that it was
//previously on.
/////////////////////////////////////////
void WorkQueue::deleteAllWorkItems()
{

        WorkItem * current = head;

        while (current != NULL) {
                WorkItem * prev = current;
                current = current->next;
                delete prev;
                prev = NULL;
                counter--;
        }
}

//Destructor
WorkQueue::~WorkQueue()
{
	if (counter == 0) {
		return;
	}

	if (counter == 1) {
		delete head;
		head = NULL;
		return;
	}

	WorkItem * current = head->next;
	WorkItem * prev = head;

		while (current->next != NULL) {
			WorkItem * temp = prev;
			current = current->next;
			prev = prev->next;
			delete temp;
			counter--;
		}
		delete current;
}
