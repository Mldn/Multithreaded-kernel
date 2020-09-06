/*
 * PCB.h
 *
 *  Created on: Sep 12, 2019
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "ListPCB.h"

class KernelSem;


class PCB{
public:
	ListPCB* hold;					//Niti koje pokusavaju da obrisu dati proces, tj cekaju da on zavrsi svoj posao da bi one dobile procesor
	static int ID ;					//Staticki ID koji sluzi za dodelu id-jeva ostalim procesima
	int cekanje;					//Kvant cekanja koliko proces ceka kada se blokira
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned stance; // 0 - NEPOKRENUT ; 1 - READY ; 2 - RUNNING ; 3 - BLOCKED ; 4 - ZAVRSIO
	int kvant;						//Vreme dodeljeno za izvrsavanje procesa, resetuje se kada istekne
	int id ;
	int unblockwae;					//Nacin kako je odblokiran proces(0 za vremenski odblokiran, a  1 za semaforski
    unsigned* st;					//Pokaziac na vrh steka datog procesa
    unsigned vel;					//Velicina steka, predefinisano
    Thread* boss;					//Pokazivac na thread-wrapper oko PCB-a
    KernelSem* sem;					//Pokazivac na semafor na kome je blokiran proces ako je blokiran
	PCB(StackSize stackSize=defaultStackSize, Time timeSlice=defaultTimeSlice, Thread* gazda=0 ) ;
    static void wrapper();			//Ova funkcija se zove jer nije moguce pozvati run iz nestaticke funkcije
	void start() ;					//Start pokrece nit
	void Ispis();					//Ispisuje podatke iz niti
	void exitthread();				//Zavrsava izvrsavanje niti
	void waitToComplete();			//Ceka na zavrsetak niti
    static void begin();			//Inicijalizuje se main nit i deadlock(idle) nit
	~PCB();




};


#endif /* PCB_H_ */
