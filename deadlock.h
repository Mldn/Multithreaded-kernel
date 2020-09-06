/*
 * deadlock.h
 *
 *  Created on: Sep 12, 2019
 *      Author: OS1
 */

#ifndef DEADLOCK_H_
#define DEADLOCK_H_

#include "thread.h"

class Deadlock: public Thread {
public:
	int broj;
	Deadlock(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice,int br = 0);
	~Deadlock();
protected:
	virtual void run();
};


#endif /* DEADLOCK_H_ */
