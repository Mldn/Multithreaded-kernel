/*
 * Utility.h
 *
 *  Created on: Sep 12, 2019
 *      Author: OS1
 */

#ifndef UTILITY_H_
#define UTILITY_H_


#define lock asm cli ;														//Zabranjivanje prekida(cli je asemblerska komanda za zabranjivanje prekida), da bi zakljucali funkcije od promene konteksta
#define unlock asm sti;														//Otkljucavanje prekida

typedef void interrupt (*pInterrupt)(...);

extern volatile int alokacije;

class PCB;
class ListPCB;

extern ListPCB lista;                                                      	//Uvoz PCB liste, jer su potrebne da budu ovde gde su semafori
extern ListPCB listaCekanja;
void* operator new (unsigned size);                                        	//Mora biti definisan new da bismo mogli dinamicki da kreiramo promenljive
void operator delete(void*);											   	//Mora biti definisan delete da bismo mogli da alociramo i brisemo dinamicke promenljive
volatile extern int lokbr;                                                 	//Broji koliko je ugnjezden lock
volatile extern int zahtevana_promena_konteksta;							//Odlozeni dispatch
unsigned extern tsp;
unsigned extern tss;														//Posrednici za ss,bp,sp
unsigned extern tbp;
volatile extern int brojac;													//Dodeljuje mu se kvant trenutnog procesa
volatile extern int sch;													//Trenuran broj procesa u skedzuleru
volatile extern PCB* running;												//Trenutni proces
volatile extern PCB* deadp;													//Idle proces koga pokrecemo kada nema ko da preuzme procesor, inicijalizuje se u statickom begin()-u koji takodje inicijalizuje main nit
void flock() ;																//Zakljucavanje, proces ulazi u kriticnu sekciju, onemogucava promenu konteksta
void funlock() ;															//Otkljucavanje, proces izlazi iz kriticne sekcije
void interrupt timer(...);													//Vremenske blokade procesa, jer procesi budu blokirani na odredjeno vreme, a ova funkcija proverava da li im je to vreme isteklo i onda ih odblokira i takodje gleda da li je trenutnom procesu isteklo vreme ako jeste onda menja kontekst ako nije samo mu dekrementira brojac PCB-a
void interrupt switchcontext();												//Funkcija za promenu konteksta
void dispatch();															//Sluzi kao wrapper oko switchcontext-a, ako je otkljucan sistem zove switchcontext funkciju ako nije postavlja zahtevana_promena_konteksta na 1 da bi se kasnije nakon sto se sistem otkljuca promenio kontekst
void interrupt odblk();														//Timer zove odblk i on sluzi za odblokiranje vremenski blokiranih procesa, i ubacuje ih u listu spremnih procesa


#endif /* UTILITY_H_ */
