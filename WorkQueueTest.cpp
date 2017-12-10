// WorkQueueTest.cpp 
//
#include <iostream>
#include <string>
#include "WorkItem.h"
#include "WorkQueue.h"

using namespace std;

////////////////////////////////////////////////////////
//  Macros and methods for magically making tests easier.
//  method must return void and have no arguments.
//  Call using macro Test.  IE
//     Test(<someMethodName>);
//
//   In your g method, use "Assert( boolean ) " for your test
//
/////////////////////////////////////////////////////////
#include <stdexcept>
#define Test( X )  testOne ( #X, &X);
// Calls testOne func passing in paramaters  magically
void testOne(string funcName, void(*fname)()) {
	try {
		fname();
	}
	catch (logic_error err) {
		cout << "ERROR in " << funcName << err.what() << endl;
		return;
	}
	cout << funcName << endl;
}
#define Assert( X )  \
if (!(X)) \
{string s = "  at line "+std::to_string(__LINE__);; \
throw std::logic_error( s ); }
///////////////////////////////////////////////////////////
//  End of testing magic.
///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
// TEst driver for WorkQueue - using TDD methodology
//////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
//  Test Work Item object.
//               REtrieve values
//               Copy Constructor
//               Operator =
/////////////////////////////////////////////////////////////////
void workItemValues()
{
	WorkItem wi(4, "sammy");
	Assert(wi.getKey().compare("sammy") == 0);
	Assert(wi.getPriority() == 4);
}
void workItemCopyCon()
{
	WorkItem *s = new WorkItem(7, "X");
	WorkItem * r = new WorkItem(*s);
	Assert(r->getKey().compare("X") == 0);
	Assert(s->getKey().compare("X") == 0);
	Assert(r->getPriority() == 7);
	Assert(s->getPriority() == 7);
}
void workItemOperatorEqual()
{
	WorkItem w1(10, "D");
	WorkItem w2(11, "E");
	w1 = w2;
	Assert(w1.getKey().compare("E") == 0);
	Assert(w1.getPriority() == 11);
	// Now change W2 and make sure w1 is disjoint
	WorkItem w3(12, "F");
	w2 = w3;
	Assert(w1.getKey().compare("E") == 0);
	Assert(w1.getPriority() == 11);
}
/////////////////////////////////////////////////////////////
//  Work Queue tests.
/////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//  Just make one and make sure nothing blows up
void simpleConstructor()
{
	WorkQueue wl;
}
///////////////////////////////////////////////////////////////
// Simply add a single item.  Make sure getNum is correct
//////////////////////////////////////////////////////////////
void addOne()
{
	WorkQueue wl;
	Assert(wl.getNumWorkItems() == 0);
	wl.addWorkItem(WorkItem(3, "A"));
	Assert(wl.getNumWorkItems() == 1);
}
///////////////////////////////////////////////////////////////
//  Add two of same prioriy - verify correct count
///////////////////////////////////////////////////////////////
void addTwoSamePri()
{
	WorkQueue wl;
	wl.addWorkItem(WorkItem(3, "A"));
	wl.addWorkItem(WorkItem(3, "B"));
//	Assert(wl.getNumWorkItems() == 2);
}
/////////////////////////////////////////////////////////////
//  Helper method to set up tests.
//  Initialize data in our list for ging.  Test data is:
//     { 3,"A"}, {3, "B"},  {4,"C"}, {6,"D"},{6,"E"},{6,"F"}
/////////////////////////////////////////////////////////////
void initData(WorkQueue & wl)
{
	wl.addWorkItem(WorkItem(3, "A"));
	wl.addWorkItem(WorkItem(4, "C"));
	wl.addWorkItem(WorkItem(6, "D"));
	wl.addWorkItem(WorkItem(6, "E"));
	wl.addWorkItem(WorkItem(3, "B"));
	wl.addWorkItem(WorkItem(6, "F"));

}
////////////////////////////////////////////////////////////
//  TEst total and priority count
//    Total count should be 6
//    priority 1 should be 0
//    priority 3 should be 2
//    priority 4 shoudl be 1
//    priority 6 should be 3
//    priority 7 should be 0
//    priority 0 should be 0
//    priority -10 shoudl be 0
////////////////////////////////////////////////////////////
void priorityCount()
{
	WorkQueue wl;
	Assert(wl.getNumWorkItems() == 0);
	initData(wl);
	Assert(wl.getNumWorkItems() == 6);
	Assert(wl.getNumWorkItems(1) == 0);
	Assert(wl.getNumWorkItems(3) == 2);
	Assert(wl.getNumWorkItems(4) == 1);
	Assert(wl.getNumWorkItems(6) == 3);
	Assert(wl.getNumWorkItems(7) == 0);
	Assert(wl.getNumWorkItems(0) == 0);
	Assert(wl.getNumWorkItems(-10) == 0);
}
/////////////////////////////////////////////////////////
//  TEst nextWorkItem with various priorities.
//  Using standard  values
//  0 == A
//  3 == A
//  4 == C
//  5 == D
//  6 == D
//  -10 = A;
//  ///////////////////////////////////////////////////////
void nextWorkItemLogic()
{
	WorkQueue wl;
	initData(wl);
	Assert(wl.nextWorkItem(0).getKey().compare("A") == 0);
	Assert(wl.nextWorkItem(3).getKey().compare("A") == 0);
	Assert(wl.nextWorkItem(4).getKey().compare("C") == 0);
	Assert(wl.nextWorkItem(5).getKey().compare("D") == 0);
	Assert(wl.nextWorkItem(6).getKey().compare("D") == 0);
	Assert(wl.nextWorkItem(-10).getKey().compare("A") == 0);
}
///////////////////////////////////////////////////////////
//  Next work item when none found  for an empty and miss list
//  This is testing for the underlying code throwing an exception
/////////////////////////////////////////////////////////////
void nextWorkItemException()
{
	WorkQueue w1;
	bool gotException = false;
	try {
		w1.nextWorkItem(0).getKey();
	}
	catch (logic_error) {
		gotException = true;
	}
	Assert(gotException);
	// Now load some up and verify same effect
	initData(w1);
	gotException = false;
	try {
		w1.nextWorkItem(7);
	}
	catch (logic_error) {
		gotException = true;
	}
	Assert(gotException);
}
//////////////////////////////////////////////////////////////
//  Test for hasWorkItem.   We first try an empty list.  Then
//  load up or standard data set and test for existance.
//  //////////////////////////////////////////////////////////
void hasWorkItem()
{
	WorkQueue w1;
	Assert(w1.hasWorkItem(3) == false);
	initData(w1);
	Assert(w1.hasWorkItem(0));
	Assert(w1.hasWorkItem(-5));
	Assert(w1.hasWorkItem(6));
	Assert(w1.hasWorkItem(7) == false);
}
////////////////////////////////////////////////////////////////
//  Test to see if the queue contains something with this key
//  ///////////////////////////////////////////////////////////
void testContainsKey()
{
	WorkQueue w1;
	Assert(w1.containsKey("A") == false);
	initData(w1);
	Assert(w1.containsKey("A") == true);
	Assert(w1.containsKey("ZZ") == false);
}
////////////////////////////////////////////////////////////////
//  Test to remove a single node.
//     { 3,"A"}, {3, "B"},  {4,"C"}, {6,"D"},{6,"E"},{6,"F"}
//////////////////////////////////////////////////////////////////
void testRemoveSingle()
{
	// First test on an empty list.  Should do nothng.
	WorkQueue w1;
	Assert(w1.getNumWorkItems() == 0);
	w1.deleteWorkItem("Fred");
	Assert(w1.getNumWorkItems() == 0);
	initData(w1);
	// Delet mod, end, head and verify count
	// Count walks thru the list so we know the chain is good
	w1.deleteWorkItem("C");
	Assert(w1.getNumWorkItems() == 5);
	w1.deleteWorkItem("F");
	Assert(w1.getNumWorkItems() == 4);
	w1.deleteWorkItem("A");
	Assert(w1.getNumWorkItems() == 3);
}
////////////////////////////////////////////////////////////////
//  Test for bumping a work item
//     { 3,"A"}, {3, "B"},  {4,"C"}, {6,"D"},{6,"E"},{6,"F"}
//
////////////////////////////////////////////////////////////////
void testBumpWorkItemHead()
{
	WorkQueue w1;
	initData(w1);
	// Bump one that is not there.  Verify no change
	w1.bumpWorkItem("G");
	Assert(w1.nextWorkItem(2).getKey().compare("A") == 0);
	Assert(w1.hasWorkItem(7) == false);
	// Bump B and verify it is at the head.
	w1.bumpWorkItem("B");
	Assert(w1.getNumWorkItems() == 6);
	Assert(w1.nextWorkItem(3).getKey().compare("B") == 0);
}
////////////////////////////////////////////////////////////////
//  Test for bumping a work item
//     { 3,"A"}, {3, "B"},  {4,"C"}, {6,"D"},{6,"E"},{6,"F"}
//   We bump the one in the middle with a single instance.
////////////////////////////////////////////////////////////////
void testBumpWorkItemSingle()
{
	WorkQueue w1;
	initData(w1);
	// Bump one that is not there.  Verify no change
	w1.bumpWorkItem("C");
	Assert(w1.nextWorkItem(4).getKey().compare("C") == 0);
}
////////////////////////////////////////////////////////////////
//  Test for bumping a work item
//     { 3,"A"}, {3, "B"},  {4,"C"}, {6,"D"},{6,"E"},{6,"F"}, {7,G}
//   We bump E, E, F, D , D and verify order
//   Then we bump G ( the last one ) to make sure no issues at the
//       end of the list
////////////////////////////////////////////////////////////////
void testBumpWorkItemMulti()
{
	WorkQueue w1;
	initData(w1);
	w1.addWorkItem(WorkItem(7, "G"));
	// Bump E
	w1.bumpWorkItem("E");
	Assert(w1.nextWorkItem(6).getKey().compare("E") == 0);
	w1.bumpWorkItem("E");
	Assert(w1.nextWorkItem(6).getKey().compare("E") == 0);
	w1.bumpWorkItem("F");
	Assert(w1.nextWorkItem(6).getKey().compare("F") == 0);
	w1.bumpWorkItem("D");
	Assert(w1.nextWorkItem(6).getKey().compare("D") == 0);
	w1.bumpWorkItem("D");
	Assert(w1.nextWorkItem(6).getKey().compare("D") == 0);
	w1.bumpWorkItem("G");
	Assert(w1.nextWorkItem(7).getKey().compare("G") == 0);
	Assert(w1.getNumWorkItems() == 7);
}
///////////////////////////////////////////////////////////
//  operator =   We will make our standard one.
//  Make and empty one.
//  do the operation.
//  Change the old one.
//  verify we have the new one with proper order.
//     { 3,"A"}, {3, "B"},  {4,"C"}, {6,"D"},{6,"E"},{6,"F"}, {7,G}
////////////////////////////////////////////////////////////
void testOperatorEqual()
{
	WorkQueue w1;
	WorkQueue w2;
	initData(w1);
	w2 = w1;
	Assert(w2.getNumWorkItems() == 6);
	Assert(w2.nextWorkItem(0).getKey().compare("A") == 0);
	// Delete the original
	w1.deleteWorkItem("A");
	Assert(w2.nextWorkItem(0).getKey().compare("A") == 0);
}
///////////////////////////////////////////////////////////
//  Test copy constructor
//  Just do the coy - delete one list.  Make sure the
//  other is copacetic
///////////////////////////////////////////////////////////
void testCopyConstructor()
{
	WorkQueue w1;
	initData(w1);
	WorkQueue w2(w1);
	w1.deleteAllWorkItems();
	Assert(w2.getNumWorkItems() == 6);

}
int main() {
	Test(workItemValues);
	Test(workItemCopyCon);
	Test(workItemOperatorEqual);
	Test(simpleConstructor);
	Test(priorityCount);
	Test(simpleConstructor);
	Test(addOne);
	Test(addTwoSamePri);
	Test(nextWorkItemLogic);
	Test(nextWorkItemException);
	Test(hasWorkItem);
	Test(testContainsKey);
	Test(testRemoveSingle);
	Test(testBumpWorkItemHead);
	Test(testBumpWorkItemSingle);
	Test(testBumpWorkItemMulti);
	Test(testOperatorEqual);
	Test(testCopyConstructor);
	cout << "testComplete" << endl;
}
