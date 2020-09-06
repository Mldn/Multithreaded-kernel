/*
 * Event.cpp
 *
 *  Created on: Sep 15, 2019
 *      Author: OS1
 */

#include "Event.h"
#include "Utility.h"
#include "IVTEntr.h"
#include "KernelE.h"
Event::Event(IVTNo ivtNo) {
	myImpl=new KernelEv();
	IVTEntry::ulazi[ivtNo]->postavi(myImpl);
	no=ivtNo;

}

void Event:: wait(){
	myImpl -> wait();
}

void Event:: signal(){
	myImpl -> signal();
}

Event::~Event() {
	flock();
	delete myImpl;
	delete IVTEntry::ulazi[no];
	funlock();

}

