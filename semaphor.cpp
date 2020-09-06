/*
 * semaphor.cpp
 *
 *  Created on: Sep 15, 2019
 *      Author: OS1
 */

#include "semaphor.h"
#include "KerSem.h"
#include "Utility.h"
Semaphore::Semaphore(int init) {
	myImpl = new KernelSem(init);

}

Semaphore::~Semaphore() {
	flock();
	delete myImpl;
	funlock();
}

void Semaphore::rem(int i){
	myImpl->rem(i);
}

int Semaphore:: val() const{
	return myImpl->val() ;
}

int Semaphore::wait(Time maxTimeToWait) {
	return myImpl->wait(maxTimeToWait);									//mttw predstavlja maksimalan rok koji mora da ceka proces koji je pozvao da bi bio odblokiran
}

int Semaphore::signal(int n){											//Broj n predstavlja broj signala u jednom pozivu, ako je po deafalt-u 0, signalizira se samo jednom, a za vise od 1, n puta
	return myImpl->signal(n);
}




