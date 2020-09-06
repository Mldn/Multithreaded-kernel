/*
 * PCB.cpp
 *
 *  Created on: Sep 12, 2019
 *      Author: OS1
 */
#include "PCB.h"
#include "Utility.h"
#include "schedule.h"
#include "dos.h"
#include "deadlock.h"
#include "IOSTREAM.H"
#include "ListPCB.h"


int PCB::ID = 0 ;


void PCB::start() {
	flock();
	st=new unsigned[vel/sizeof(unsigned)];									//Inicijalizacija steka procesa
	int vrh=vel/sizeof(unsigned)-1;											//Inicijaliacija rha steka
	st[vrh] =0x200;//setovan I fleg u
	                      // pocetnom PSW-u za nit
	st[vrh - 1] = FP_SEG(wrapper);											//Wrapper je staticka funkcija te se moze staviti na stek, stavlja se na stek ovde, mora da se ima njegova adresa u svakom trenutnku
	st[vrh - 2] = FP_OFF(wrapper);											//Offset u datom segmentu

	sp = FP_OFF(st + vrh - 11); //svi sacuvani registri
	 //pri ulasku u interrupt
	 //rutinu
	ss = FP_SEG(st + vrh - 11);
	bp = FP_OFF(st + vrh - 11);
	stance=1;
	if(this!=deadp){//AKo proces nije idle onda se stavlja u scheduler
		Scheduler::put(this);
		sch++;
	}
	funlock();

}


void PCB::exitthread() {
	flock();
	stance=4;																//Menja stanje procesa u zavrsen
	while(!(hold ->isEmpty() ) ) {											//Niti koje drzimo blokiranim sa waitToComplete() moramo da odblokiramo
		Elem* p = hold ->getHead();
		p -> x ->stance = 1;
		Scheduler::put(p->x);
		sch++;
		p = hold -> remF();
		delete p;
	}

	dispatch();																//Poziva se promena konteksta
	funlock();
}

void PCB:: wrapper(){														//Pozove run od niti koju poseduje proces(wrepuje ga)
	running -> boss -> run();
	((PCB*)running )-> exitthread();										//Kada se zavrsi run onda se izlazi iz niti
}
void PCB::waitToComplete() {				//Provera da li mora da se blokira nit koja pokusava da unisti dati proces, npr nit trenutni running pokusa da unisti nit A, a ona i dalje nije zavrsila svoje izvrsavanje, i tako blokira running
	flock();
	if(stance > 0 && stance <4 && running->id != id){						//Ako je stanje izmedju 0 i 4 to znaci da proces nije zavrsio svoj posao, a takodje proces ne moze sam sebe da blokira jer to dovori do beskonacne petlje
		running -> stance = 3 ;												//Trenutno stanje procesa se postavlja na blokirano, jer se running
		hold->addL((PCB*) running);											//U hold listu se procesa se dodaje running koji je pokusao da ga unisti
		dispatch();															//Promena konteksta
	}
	funlock();
}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* gazda) {
	vel=stackSize;
	kvant=timeSlice;
	boss=gazda;
	st=0;
	stance=0;
	id = ID++;
	sp=0;
	ss=0;
	bp=0;
	cekanje = 0 ;
	hold = new ListPCB();
	lista.addL(this);
	sem = 0;
	unblockwae = 0;
}

void PCB::begin() {
	running = new PCB();
	running ->stance = 2;
	Deadlock* dead=new Deadlock();
	deadp=dead->myPCB;
	dead->start();

}
void PCB::Ispis(){
	flock();
	cout << id <<" : " << cekanje << "\n";                //Ispisuje ID i vreme cekanja samo
	funlock();
}

PCB::~PCB(){

	flock();
	Elem* p = lista.rem(this->id);						//Brisinje iz liste svih PCB-ova
	delete p ;
	if(stance != 0 ) delete st;							//Brisanje steka
	delete hold;										//Brisanje liste hold
	funlock();

}




