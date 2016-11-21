//// threadtest.cc
////	Simple test case for the threads assignment.
////
////	Create two threads, and have them context switch
////	back and forth between themselves by calling Thread::Yield,
////	to illustratethe inner workings of the thread system.
////
//// Copyright (c) 1992-1993 The Regents of the University of California.
//// All rights reserved.  See copyright.h for copyright notice and limitation
//// of liability and disclaimer of warranty provisions.
//
//#include "copyright.h"
//#include "system.h"
//#include "synch.h"
//
//// testnum is set in main.cc
//int testnum = 1;
//
//int number = 0;
//Semaphore * fillCount = new Semaphore("fillCount", 0);
//Semaphore * emptyCount = new Semaphore("emptyCount", 4);
//
//
//void
//producer(int h)
//{
//    while (true) {
//    	printf("producer %d \n", number);
//	    emptyCount->P();
//	    number ++;
//	    fillCount->V();
//        //currentThread->Yield();
//	}
//}
//
//
//void
//consumer(int h)
//{
//    while (true) {
//        fillCount->P();
//        number --;
//        emptyCount->V();
//        printf("consumer %d \n", number);
//        //currentThread->Yield();
//    }
//}
//
//
//void
//os_testing(int h)
//{
//	printf("os testing: %d\n", h);
//	currentThread->Yield();
//}
//
////----------------------------------------------------------------------
//// SimpleThread
//// 	Loop 5 times, yielding the CPU to another ready thread
////	each iteration.
////
////	"which" is simply a number identifying the thread, for debugging
////	purposes.
////----------------------------------------------------------------------
//
//void
//SimpleThread(int which)
//{
//    int num;
//
//    for (num = 0; num < 5; num++) {
//	printf("*** thread %d looped %d times\n", which, num);
//        currentThread->Yield();
//    }
//}
//
//int
//fib(int n)
//{
//	if(n == 1 || n == 0)
//	{
//		return 1;
//	}
//	else
//	{
//		return fib(n-1) + fib(n-2);
//	}
//
//	//printf("testing \n");
//
//}
//
//void testing(int n)
//{
//	int a = fib(n);
//	printf("number %d \n", a);
//}
//
////----------------------------------------------------------------------
//// ThreadTest1
//// 	Set up a ping-pong between two threads, by forking a thread
////	to call SimpleThread, and then calling SimpleThread ourselves.
////----------------------------------------------------------------------
//
//void
//ThreadTest1()
//{
//    DEBUG('t', "Entering ThreadTest1");
//
//
//    Thread *t = new Thread("forked thread");
//    Thread *b = new Thread("os thread");
//
//    //t->Fork(SimpleThread, 1);
//    //b->Fork(os_testing, 1);
//    //SimpleThread(0);
//    //os_testing(3);
//    //t->Fork(producer, 0);
//    //b->Fork(consumer, 0);
//    ////consumer(0);
//    //consumer(0);
//	//
//
//	//t->Fork(testing, 10);
//	printf("salam \n");
//	//int a = fib(10);
//	//printf("fib: %d \n", a);
//
//
//}
//
//
////----------------------------------------------------------------------
//// ThreadTest
//// 	Invoke a test routine.
////----------------------------------------------------------------------
//
//void
//ThreadTest()
//{
//    switch (testnum) {
//    case 1:
//	ThreadTest1();
//	break;
//    default:
//	printf("No test specified.\n");
//	break;
//    }
//}
//


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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "copyright.h"
#include "system.h"
#include "synch.h"


#include <unistd.h>

// testnum is set in main.cc
int testnum = 1;


void os_function(int n)
{
	char * name = currentThread->getName();
	printf("thread %s, running for %d seconds\n", name ,n);
	usleep(n * 1000 * 1000);
	return ;
}


void
ThreadTest1()
{
    Thread *t1 = new Thread("1");
    Thread *t2 = new Thread("2");
    Thread *t3 = new Thread("3");
    Thread *t4 = new Thread("4");
    Thread *t5 = new Thread("5");
    Thread *t6 = new Thread("6");
    Thread *t7 = new Thread("7");
    Thread *t8 = new Thread("8");
    Thread *t9 = new Thread("9");
    Thread *t10 = new Thread("10");


    Thread *t11 = new Thread("1");
    Thread *t12 = new Thread("2");
    Thread *t13 = new Thread("3");
    Thread *t14 = new Thread("4");
    Thread *t15 = new Thread("5");
    Thread *t16 = new Thread("6");
    Thread *t17 = new Thread("7");
    Thread *t18 = new Thread("8");
    Thread *t19 = new Thread("9");
    Thread *t20 = new Thread("10");


	t1->Fork(os_function, 11);
	t2->Fork(os_function, 4);
	t3->Fork(os_function, 3);
	t4->Fork(os_function, 6);
	t5->Fork(os_function, 7);
	t6->Fork(os_function, 10);
	t7->Fork(os_function, 8);
	t8->Fork(os_function, 2);
	t9->Fork(os_function, 1);
	t10->Fork(os_function, 5);

	t11->Fork(os_function, 11);
	t12->Fork(os_function, 4);
	t13->Fork(os_function, 3);
	t14->Fork(os_function, 6);
	t15->Fork(os_function, 7);
	t16->Fork(os_function, 10);
	t17->Fork(os_function, 8);
	t18->Fork(os_function, 2);
	t19->Fork(os_function, 1);
	t20->Fork(os_function, 5);

	// second time results should be something like this if your scheduler works correctly
	// J9 J8 J3 J2 J10 J4 J5 J7 J6 J1


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
