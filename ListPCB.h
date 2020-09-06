/*
 * ListPCB.h
 *
 *  Created on: Sep 13, 2019
 *      Author: OS1
 */

#ifndef LISTPCB_H_
#define LISTPCB_H_
#include "Utility.h"
#include "PCB.h"


struct Elem{
	PCB* x ;
	Elem* sled;

	Elem(PCB* b){
		x = b;
		sled = 0 ;
	}
	~Elem() {
		x = 0 ;
		sled = 0 ;
	}
};

class ListPCB {
private:
	Elem* head;
	Elem* tail;
	int empty;


public:

	void addF(PCB* i);
	void addL(PCB* i);
	Elem* rem(ID i);
	void sortAdd(PCB* i);
	Elem* remF();
	int isEmpty();
	Elem* getHead();
	Elem* getTail();
	void Ispis();
	void remV(ID i);
	void addV(PCB* i);
	void srediV();										//Sluzi za sredjivanje liste po cekanju
	void vratiV();										//Vraca listu u stanje kakvo je bilo pre sredjivanja
	PCB* get(int i);


	ListPCB() ;
	ListPCB(const ListPCB& li) ;

	~ListPCB();


};

#endif /* LISTPCB_H_ */
