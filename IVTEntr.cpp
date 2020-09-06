/*
 * IVTEntr.cpp
 *
 *  Created on: Sep 15, 2019
 *      Author: OS1
 */

#include "IVTEntr.h"
#include "dos.h"
#include "KernelE.h"

IVTEntry* IVTEntry::ulazi[256];
IVTEntry::IVTEntry(IVTNo n,pInterrupt nov,int f) {
	starina=getvect(n);														//getect()-om se dobija rutina koja je postavljna u ulaz IVTP-a broj n
	novak=nov;
	dog=0;
	starac=f;
	setvect(n,nov);															//Setuje se nova rutina
	broj=n;
	ulazi[broj]=this;

}
void IVTEntry::postavi(KernelEv* kev){
	dog=kev;
}
void IVTEntry::signal(){
	if(dog!=0) {
        if(starac)starina();
		dog->signal();

	}
}
IVTEntry::~IVTEntry() {
	flock();
	setvect(broj,starina);
	dog=0;
	funlock();

}


