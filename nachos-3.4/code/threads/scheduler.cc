// scheduler.cc 
//	Routines to choose the next thread to run, and to dispatch to
//	that thread.
//
// 	These routines assume that interrupts are already disabled.
//	If interrupts are disabled, we can assume mutual exclusion
//	(since we are on a uniprocessor).
//
// 	NOTE: We can't use Locks to provide mutual exclusion here, since
// 	if we needed to wait for a lock, and the lock was busy, we would 
//	end up calling FindNextToRun(), and that would put us in an 
//	infinite loop.
//
// 	Very simple implementation -- no priorities, straight FIFO.
//	Might need to be improved in later assignments.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "time.h"

#include "copyright.h"
#include "scheduler.h"
#include "system.h"

//----------------------------------------------------------------------
// Scheduler::Scheduler
// 	Initialize the list of ready but not running threads to empty.
//----------------------------------------------------------------------

Scheduler::Scheduler()
{ 
    readyList = new List; 
    lastTime = time(0);
} 

//----------------------------------------------------------------------
// Scheduler::~Scheduler
// 	De-allocate the list of ready threads.
//----------------------------------------------------------------------

Scheduler::~Scheduler()
{ 
    delete readyList; 
} 

//----------------------------------------------------------------------
// Scheduler::ReadyToRun
// 	Mark a thread as ready, but not running.
//	Put it on the ready list, for later scheduling onto the CPU.
//
//	"thread" is the thread to be put on the ready list.
//----------------------------------------------------------------------

void
Scheduler::ReadyToRun (Thread *thread)
{
    DEBUG('t', "Putting thread %s on ready list.\n", thread->getName());
    printf("Putting thread %s on ready list.\n", thread->getName());

    thread->setStatus(READY);
    readyList->Append((void *)thread);
}

//----------------------------------------------------------------------
// Scheduler::FindNextToRun
// 	Return the next thread to be scheduled onto the CPU.
//	If there are no ready threads, return NULL.
// Side effect:
//	Thread is removed from the ready list.
//----------------------------------------------------------------------

Thread *
Scheduler::FindNextToRun ()
{	
    UpdateJobTime();
    //for (int i = 0; i < 11; ++ i)
	//printf("job %d is %d, ",i, jobTimes[i] );
	//Print();
    if (readyList->IsEmpty()) return NULL;
	List* temp= new List;
	int minTime = 9999999 ;
	while(!readyList->IsEmpty()){
		Thread * t = (Thread* ) readyList->Remove();
		if(jobTimes[atoi(t->getName())] < minTime)	minTime = jobTimes[atoi(t->getName())];
		temp->Append(t);
	}
	readyList = temp;
	   Thread *a =  (Thread *) readyList->Remove();
	 while (jobTimes[atoi(a->getName())] > minTime){
	   readyList->Append((void *)a);
	   a = (Thread *) readyList->Remove();
	}
	return a;
   /*int shortestJobName = FindShortestJob(); // shortest job name
    printf("shortest job is %d\n", shortestJobName);
 //   for (int i = 0; i < shortestJobIndex - 1; ++ i)
	//readyList->Append(readyList->Remove());
    GetShortestJob();
   /* Thread *a =  (Thread *) readyList->Remove();
    while (atoi(a->getName()) != shortestJobName){
	   readyList->Append(a);
	   a = (Thread *) readyList->Remove();
    }
*/
}

void
Scheduler::UpdateJobTime()
{
    char *name = currentThread->getName();
    if (name == "main")
    {
        lastTime = time(0);
        return ;
    }

    int currentTime = time(0);
    jobTimes[atoi(name)] = currentTime - lastTime;
    lastTime = time(0);
}

int Scheduler::FindShortestJob()
{
    int index = 1;
    int min = jobTimes[1];
    for (int i = 2; i < 11; i++)
        if (jobTimes[i] < min)
        {
            min = jobTimes[i];
            index = i;
        }
    return index;
}

Thread* Scheduler::GetShortestJob()
{
	printf("hereeeee");
	List* temp;
	int minTime = 9999999 ;
	while(!readyList->IsEmpty()){
		Thread * t = (Thread* ) readyList->Remove();
		if(jobTimes[atoi(t->getName())] < minTime)	minTime = jobTimes[atoi(t->getName())];
		printf("in while \n");
		temp->Append( t);
		printf("end of while \n");
	}
	printf("in while 2\n");
	readyList = temp;
	printf("in while 3\n");
	   Thread *a =  (Thread *) readyList->Remove();
	 printf("in while 4\n");
	 while (jobTimes[atoi(a->getName())] > minTime){
	   readyList->Append(a);
	   a = (Thread *) readyList->Remove();
	}
	return a;
	
	
}

//----------------------------------------------------------------------
// Scheduler::Run
// 	Dispatch the CPU to nextThread.  Save the state of the old thread,
//	and load the state of the new thread, by calling the machine
//	dependent context switch routine, SWITCH.
//
//      Note: we assume the state of the previously running thread has
//	already been changed from running to blocked or ready (depending).
// Side effect:
//	The global variable currentThread becomes nextThread.
//
//	"nextThread" is the thread to be put into the CPU.
//----------------------------------------------------------------------

void
Scheduler::Run (Thread *nextThread)
{
    Thread *oldThread = currentThread;
    
#ifdef USER_PROGRAM			// ignore until running user programs 
    if (currentThread->space != NULL) {	// if this thread is a user program,
        currentThread->SaveUserState(); // save the user's CPU registers
	currentThread->space->SaveState();
    }
#endif
    
    oldThread->CheckOverflow();		    // check if the old thread
					    // had an undetected stack overflow

    currentThread = nextThread;		    // switch to the next thread
    currentThread->setStatus(RUNNING);      // nextThread is now running
    
    DEBUG('t', "Switching from thread \"%s\" to thread \"%s\"\n",
	  oldThread->getName(), nextThread->getName());
    printf("Switching from thread \"%s\" to thread \"%s\"\n",
	  oldThread->getName(), nextThread->getName());
    
    // This is a machine-dependent assembly language routine defi readyList->SortedRemove()ned 
    // in switch.s.  You may have to think
    // a bit to figure out what happens after this, both from the point
    // of view of the thread and from the perspective of the "outside world".

    SWITCH(oldThread, nextThread);
    
    DEBUG('t', "Now in thread \"%s\"\n", currentThread->getName());
    printf("Now in thread \"%s\"\n", currentThread->getName());

    // If the old thread gave up the processor because it was finishing,
    // we need to delete its carcass.  Note we cannot delete the thread
    // before now (for example, in Thread::Finish()), because upreadyList->Append( to this
    // point, we were still running on the old thread's stack!
    if (threadToBeDestroyed != NULL) {
        delete threadToBeDestroyed;
	threadToBeDestroyed = NULL;
    }
    
#ifdef USER_PROGRAM
    if (currentThread->space != NULL) {		// if there is an address space
        currentThread->RestoreUserState();     // to restore, do it.
	currentThread->space->RestoreState();
    }
#endif
}

//----------------------------------------------------------------------
// Scheduler::Print
// 	Print the scheduler state -- in other words, the contents of
//	the ready list.  For debugging.
//----------------------------------------------------------------------
void
Scheduler::Print()
{
    printf("Ready list contents:\n");
    readyList->Mapcar((VoidFunctionPtr) ThreadPrint);
}
