/*
 * semaphor.h
 *
 *  Created on: Sep 15, 2019
 *      Author: OS1
 */

#ifndef _semaphor_h_
#define _semaphor_h_

typedef unsigned int Time;

class KernelSem;													//semaphor sluzi kao wrapper za klasu kernelSem


class Semaphore {
public:
	Semaphore (int init=1);
	virtual ~Semaphore ();
	virtual int wait (Time maxTimeToWait);
	virtual int signal(int n=0);
	void rem(int i);
	int val () const; // Vraca trenutnu vrednost semafora
private:

	KernelSem* myImpl;

};
#endif
