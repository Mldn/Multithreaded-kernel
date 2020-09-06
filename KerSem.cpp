/*
 * KerSem.cpp
 *
 *  Created on: Sep 15, 2019
 *      Author: OS1
 */

#include "KerSem.h"
#include "ListPCB.h"
#include "PCB.h"
#include "Utility.h"
#include "Schedule.h"

KernelSem::KernelSem(int v) {
	value = v ;
	blokirani = new ListPCB();
}

KernelSem::~KernelSem() {
	flock();
	delete blokirani;
	funlock();
}

int KernelSem::val() const {
	return value;
}

int KernelSem::wait(Time maxTimeToWait){
	flock();
	value-- ;
	if(value < 0 ){
		running -> stance = 3;							//Proces se blokira ako se pozvao wait nad njim
		running -> sem = this;							//Procesu se dodeljuje semafor na kome je blokiran
		if(maxTimeToWait > 0) {							//Ako je ovo maksimalno vreme cekanja na semaforu vece od 0
			running->cekanje=maxTimeToWait;				//Vreme cekanja u running se podesi na to
			listaCekanja.addV(((PCB*) running));		//Dodaje se u globalnu listu cekanja
		}
		blokirani->addL((PCB*) running);				//Dodaje se u listu blokiranih
		dispatch();										//promena konteksta
	}
	funlock();
	return running->unblockwae;							//Nacin na koji je trenutni(novi) proces odblokiran
}

int KernelSem::signal(int n){
	flock();
	int l = 0 ;											//U slucaju da se signalizira vise od jedanput vraca se broj odblokirnih procesa
	if(n < 0) return n;									//Slucaj greske(Samo se vrati broj)
	if( n == 0 ) {
		value++ ;
		if(value <= 0 ){								//Ako je value i dalje manje ili jednako nuli to znaci da mora da odblokiramo neki proces
			Elem* p = blokirani->remF();				//Pokupimo prvi iz liste blokiranih
			PCB* pom = p ->x;
			pom -> stance = 1;							//Stavimo stanje da je spreman
			pom -> unblockwae = 1;						//Nacin odblokiranja je semaforski pa je unblockwae jednako 1
			pom->sem=0;									//Nije vise blokiran ni na jednom semaforu pa je ova vrednost sem jednaka nuli
			Scheduler::put(pom);						//Stavlja se scheduler odblokirani proces
			sch++;
			delete p;
		    listaCekanja.remV(pom -> id);				//I izbacimo ga iz liste cekanja tako da ona ostane uredjena
		}
	}else{
		for(int i = 0 ; i < n ; i ++){					//Slucaj da treba vise procesa da odblokira on odblokira i njih
			value++ ;
			if(value <= 0 ){
				Elem* p = blokirani->remF();
				PCB* pom = p ->x;
				pom -> stance = 1;
				pom -> unblockwae = 1;
				pom->sem=0;
				Scheduler::put(pom);
				sch++;
				listaCekanja.remV(pom->id);
				delete p;
				l ++ ;									//Racuna l, tj broj odblokiranih procesa na semaforu
			}
		}
	}
	funlock();
	return l;
}


void KernelSem::rem(int i){
	flock();
	Elem* p = blokirani->rem(i);
	if(p!=0){
		delete p ;
		value++;
	}
	funlock();
}

