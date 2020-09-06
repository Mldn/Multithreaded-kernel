/*
 * IVTEntr.h
 *
 *  Created on: Sep 15, 2019
 *      Author: OS1
 */

#ifndef IVTENTR_H_
#define IVTENTR_H_

#include "Event.h"
#include "Utility.h"

class KernelEv;
class IVTEntry {
public:
	static IVTEntry* ulazi[256];                             //Tabela prekida
	IVTEntry(IVTNo n,pInterrupt nov,int f);
	~IVTEntry();
	void signal();
	void postavi(KernelEv* kev);							//Postavljamo koji se dogadjaj desava za dati broj prekida
private:
	pInterrupt starina;										//Stara prekidna rutina
	pInterrupt novak;										//Nova prekidna rutina
	KernelEv* dog;											//Dogadjaj, tj broj prekida koji se zadaje
	IVTNo broj;												//Broj prekida koji se poziva
	int starac;												//Samo selektor

};

#endif /* IVTENTR_H_ */
