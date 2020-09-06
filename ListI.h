/*
 * ListI.h
 *
 *  Created on: Sep 13, 2019
 *      Author: OS1
 */

#ifndef LISTI_H_
#define LISTI_H_

struct element{
	int x ;
	element* sled;

	element(int b){
		x = b;
		sled = 0 ;
	}
};


class List_I {
private:
	element* head;
	element* tail;
	int empty;


public:

	void addF(int i);
	void addL(int i);
	element* rem(int i);
	void sortAdd(int i);
	element* remF();
	int isEmpty();
	element* getHead();
	element* getTail();
	void Ispis();

	List_I() ;
	List_I(const List_I& li) ;
	~List_I();


};

#endif /* LISTI_H_ */
