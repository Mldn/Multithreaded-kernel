/*
 * deadlock.cpp
 *
 *  Created on: Sep 12, 2019
 *      Author: OS1
 */

#include "Deadlock.h"
#include "Utility.h"
#include "iostream.h"
Deadlock::Deadlock(StackSize stackSize, Time timeSlice,int br):Thread(stackSize,timeSlice) {
	broj = br;
}

Deadlock::~Deadlock() {

	waitToComplete();
}


void Deadlock::run(){
	while(1);
}


