// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"

#include "synch.h"

// testnum is set in main.cc
int testnum = 1;

Lock *lock = new Lock("pc");

int slots[10];
int slot_size = 10;
int slot_index = 0;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void Sleep()
{
	IntStatus oldLevel = interrupt->SetLevel(IntOff);
	currentThread->Sleep();
	(void) interrupt->SetLevel(oldLevel);
}

void Producer(int which)
{
	while (true)
	{
		int item = slot_index;
	
		lock->Acquire();
		if (slot_index < slot_size)
		{
			slots[slot_index++] = item;
			printf("*** producer %d slot_index %d\n", which, slot_index - 1);
			lock->Release();
		}
		else
		{
			lock->Release();
			currentThread->Yield();
		}
	}
}

void Consumer(int which)
{
	while (true)
	{
		int item;

		lock->Acquire();
		if (slot_index > 0)
		{
			item = slots[--slot_index];
			printf("*** consumer %d slot_index %d\n", which, slot_index);
			lock->Release();
		}
		else
		{
			lock->Release();
			currentThread->Yield();
		}
	}
}


//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *tP1 = new Thread("producer 1");
    Thread *tP2 = new Thread("producer 2");

    Thread *tC1 = new Thread("consumer 1");
    Thread *tC2 = new Thread("consumer 2");

    tP1->Fork(Producer, 1);
    tC1->Fork(Consumer, 1);

    tP2->Fork(Producer, 2);
    tC2->Fork(Consumer, 2);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
	ThreadTest1();
	break;
    default:
	printf("No test specified.\n");
	break;
    }
}

