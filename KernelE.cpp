/*
 * KernelE.cpp
 *
 *  Created on: Sep 15, 2019
 *      Author: OS1
 */

#include "KernelE.h"
#include "PCB.h"
#include "Semaphor.h"

KernelEv::KernelEv() {
	sem = new Semaphore(0);
	boss = (PCB*)running;								//Samo nit koja je konstruisala event moze da ceka na njegovom semaforu
}

void KernelEv::wait(){
	if(running -> id == boss -> id){
		sem -> wait(0);
	}
}

void KernelEv::signal(){
	sem -> signal();
}

KernelEv::~KernelEv() {
	flock();
	delete sem;
	funlock();
}

