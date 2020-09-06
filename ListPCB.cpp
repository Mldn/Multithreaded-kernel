/*
 * ListPCB.cpp
 *
 *  Created on: Sep 13, 2019
 *      Author: OS1
 */
#include "ListPCB.h"
#include "Utility.h"
#include "PCB.h"
#include <iostream.h>

ListPCB::ListPCB() {
	head = tail = 0 ;
	empty=1;
}

void ListPCB::addL(PCB* i){
	flock();
	Elem* e = new Elem(i);
	if(!head){
		head = tail =  e;
	}else {
		tail->sled = e;
		tail = e;
	}
	funlock();
}

void ListPCB::addF(PCB* i) {
	flock();
	Elem* e = new Elem(i);
	if(!head){
		head = tail = e;
	}else{
		e->sled = head ;
		head = e ;
	}
	funlock();
}

Elem* ListPCB::rem(ID i){
	flock();
	Elem* tek = head;
	Elem* pret = 0 ;
	while(tek){
		if(tek->x->id == i){
			if(pret) pret->sled = tek->sled;
			if(!pret) head = head -> sled;
			tek->sled = 0 ;
			if(tail == tek) tail = pret;
			funlock();
			return tek;
		}
		pret = tek;
		tek = tek->sled;
	}
	funlock();
	return 0;
}

PCB* ListPCB::get(int i){
	flock();
	Elem* tek = head;
	while(tek){
		if(tek->x->id == i){
			funlock();
			return tek->x;
		}
		tek = tek->sled;
	}
	funlock();
	return 0;
}

void ListPCB::sortAdd(PCB* i){
	flock();
	int f = isEmpty();
	if(f) {
		addL(i);
		funlock();
		return;
	}
	Elem* e = new Elem(i);
	Elem* tek = head;
	Elem* pret = 0 ;
	while(tek!=0 && i->cekanje >= tek->x->cekanje){
		pret = tek;
		tek = tek->sled;
	}
	if(pret) {
		if(tek!=0)e->sled = tek;
		pret->sled = e;
	}else{
		e->sled = head;
		head = e;
	}
	funlock();
}

int ListPCB::isEmpty(){
	flock();
	if(head) empty = 0;
	else empty = 1;
	funlock();
	return empty;
}

Elem* ListPCB::getHead(){
	return head;
}
Elem* ListPCB::getTail(){
	return tail;
}



ListPCB::ListPCB(const ListPCB& li){
	flock();
	head=tail=0;
	Elem* tek = li.head;
	empty=1;
	while(tek){
		Elem* e = new Elem(tek->x);
		if(!head) head = tail = e ;
		else {
			tail->sled = e;
			tail = e;
		}
		tek = tek->sled;
	}
	funlock();
}

void ListPCB::Ispis(){
	flock();
	Elem* tek = head;
	while(tek){
		tek->x->Ispis();
		tek=tek->sled;
	}
	cout<<endl;
	funlock();
}

Elem* ListPCB::remF(){
	if(isEmpty()) return 0;
	flock();
	Elem* tek = head;
	head= head->sled;
	funlock();
	return tek;

}

void ListPCB::addV(PCB* i){
	flock();
	vratiV();
	sortAdd(i);
	srediV();
	funlock();
}

void ListPCB::remV(ID i){
	flock();
	vratiV();
	Elem* p = rem(i);
	if(p!=0) delete p ;
	srediV();
	funlock();
}

void ListPCB::srediV(){
	flock();
	Elem* tek = head;
	int pom = 0;
	while(tek){
		tek->x->cekanje -= pom ;
		pom += tek->x->cekanje;
		tek=tek->sled;
	}
	funlock();
}

void ListPCB::vratiV() {
	flock();
	int pom = 0 ;
	Elem* tek = head;
	while(tek){
		tek->x->cekanje += pom;
		pom += tek->x->cekanje - pom ;
		tek=tek->sled;
	}
	funlock();
}

ListPCB::~ListPCB() {
	flock();
	Elem* pom = head;
	while(head){
		pom=head;
		head = head->sled;
		delete pom;
	}
	head = tail = 0;
	funlock();
}

