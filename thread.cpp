/*
 * thread.cpp
 *
 *  Created on: Sep 12, 2019
 *      Author: OS1
 */
#include "thread.h"
#include "Utility.h"
#include "PCB.h"
#include "ListPCB.h"
#include "SCHEDULE.H"
#include "iostream.h"


void dispatch(){
	if(lokbr==0) switchcontext();    							//Duplirana provera za svaki slucaj(Dispat)
	else zahtevana_promena_konteksta=1;
}

Thread::Thread(StackSize stackSize, Time timeSlice){
	Thread::myPCB = new PCB(stackSize,timeSlice,this);
}

void Thread::start() {
	myPCB->start();
}

void Thread::waitToComplete() {
	myPCB ->waitToComplete();
}

ID Thread::getId() {
	return myPCB->id;
}

ID Thread::getRunningId() {
	return running->id;
}

Thread* Thread::getThreadById(ID id){
	if(lista.get(id) != 0 ){
		return lista.get(id)->boss;
	}
	return 0 ;
}


Thread::~Thread() {
	flock();

	myPCB -> boss = 0;
	delete myPCB;

	funlock();
}



