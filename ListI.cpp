/*
 * ListI.cpp
 *
 *  Created on: Sep 13, 2019
 *      Author: OS1
 */

#include "ListI.h"
#include "Utility.h"
#include <iostream.h>

List_I::List_I() {
	head = tail = 0 ;
	empty=1;
}

void List_I::addL(int i){
	flock();
	element* e = new element(i);
	if(!head){
		head = tail =  e;
	}else {
		tail->sled = e;
		tail = e;
	}
	funlock();
}

void List_I::addF(int i) {
	flock();
	element* e = new element(i);
	if(!head){
		head = tail = e;
	}else{
		e->sled = head ;
		head = e ;
	}
	funlock();
}

element* List_I::rem(int i){
	flock();
	element* tek = head;
	element* pret = 0 ;
	while(tek){
		if(tek->x == i){
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

void List_I::sortAdd(int i){
	flock();
	int f = isEmpty();
	if(f) {
		addL(i);
		funlock();
		return;
	}
	element* e = new element(i);
	element* tek = head;
	element* pret = 0 ;
	while(i>= tek->x){
		pret = tek;
		tek = tek->sled;
	}
	if(pret) {
		e->sled = tek;
		pret->sled = e;
	}else{
		e->sled = head ->sled;
		head = e;
	}
	funlock();
}

int List_I::isEmpty(){
	flock();
	if(head) empty = 0;
	else empty = 1;
	funlock();
	return empty;
}

element* List_I::getHead(){
	return head;
}
element* List_I::getTail(){
	return tail;
}



List_I::List_I(const List_I& li){
	flock();
	head=tail=0;
	element* tek = li.head;
	empty=1;
	while(tek){
		element* e = new element(tek->x);
		if(!head) head = tail = e ;
		else {
			tail->sled = e;
			tail = e;
		}
		tek = tek->sled;
	}
	funlock();
}

void List_I::Ispis(){
	element* tek = head;
	while(tek){
		cout<< tek->x<<" ";
		tek=tek->sled;
	}
	cout<<endl;
}

element* List_I::remF(){
	element* tek = head;
	head= head->sled;
	return tek;
}


List_I::~List_I() {
	element* pom = head;
	while(pom){
		head = head->sled;
		delete pom ;
		pom = head;
	}
}


