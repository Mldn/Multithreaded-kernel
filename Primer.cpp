/*
 * Primer.cpp
 *
 *  Created on: Sep 12, 2019
 *      Author: OS1
 */

#include "Primer.h"
#include "Utility.h"
#include "iostream.h"
Primer::Primer(StackSize stackSize, Time timeSlice,int br,Semaphore* sem):Thread(stackSize,timeSlice) {
	broj = br;
	s=sem;
}

Primer::~Primer() {

	waitToComplete();
}


void Primer::run(){
	s->wait(0);
	for (int i =0; i < 3; ++i) {
		flock();
		cout<<"u "<<broj<<" i = "<<i<<endl;
		funlock();
		for (int j = 0; j< 1000; ++j)
				for (int k = 0; k < 30000; ++k);

	}
}
