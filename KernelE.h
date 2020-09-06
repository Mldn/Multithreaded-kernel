/*
 * KernelE.h
 *
 *  Created on: Sep 15, 2019
 *      Author: OS1
 */

#ifndef KERNELE_H_
#define KERNELE_H_

class Semaphore;
class PCB;
class KernelEv {
public:
	KernelEv();
	void wait();
	void signal();
	~KernelEv();
private:
	Semaphore* sem;
	PCB* boss;									//Samo running nit moze da ceka na ovom semaforu zato je potreban bos
};

#endif /* KERNELE_H_ */
