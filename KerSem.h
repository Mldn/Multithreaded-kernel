/*
 * KerSem.h
 *
 *  Created on: Sep 15, 2019
 *      Author: OS1
 */

#ifndef KERSEM_H_
#define KERSEM_H_

class ListPCB;
#include "semaphor.h"

class KernelSem {
public:
	KernelSem(int v);
	int wait (Time maxTimeToWait);
	int signal(int n=0);
	int val () const;
	void rem(int i);
	~KernelSem();


private:
	int value;
	ListPCB* blokirani;
};

#endif /* KERSEM_H_ */
