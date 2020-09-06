/*
 * Primer.h
 *
 *  Created on: Sep 12, 2019
 *      Author: OS1
 */

#ifndef PRIMER_H_
#define PRIMER_H_

#include "thread.h"
#include "semaphor.h"
class Primer : public Thread {
public:
	int broj;
	Semaphore* s;
	Primer(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice,int br = 0,Semaphore* sem=0);
	~Primer();
protected:
	virtual void run();
};

#endif /* PRIMER_H_ */
