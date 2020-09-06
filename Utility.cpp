#include "Utility.h"
#include "malloc.h"
#include <stdlib.h>
#include "dos.h"
#include "iostream.h"
#include "schedule.h"
#include "PCB.h"
#include "thread.h"
#include "ListPCB.h"
#include "KerSem.h"

ListPCB lista;
ListPCB listaCekanja;

volatile int alokacije = 0 ;                                                  	//Cisto da znamo da li je sve na kraju programa dealocirano, da ne bi doslo do memory leak-a
volatile int lokbr = 0;
volatile int sch=0;
volatile int zahtevana_promena_konteksta = 0;
volatile int brojac=20;															//Vreme dodeljeno procesu je deafalt-no jednako 20. U timer-u se zove
unsigned tbp;
unsigned tss;
unsigned tsp;
volatile PCB* running;
volatile PCB* deadp;

void* operator new (unsigned size){
	lock                                          	//Zakljucavamo prilikom ulaza u kriticnu sekciju

	void* a = malloc(size);							// Alociramo prostor size za a
	if(a == 0 && size > 0 ) exit(1);				//Ako prostor nije alociran a size je veci od nule(Sto bi znacilo da je nesto trebalo biti alocirano, doslo je do greske
	alokacije ++ ;									//Povecavamo broj alokacija da znamo da smo koliko kasnije treba da oslobodimo

	unlock											//Otkljucava proces

	return a;
}

void operator delete(void* pok){
	lock											//Zakljucava prilikom ulaza
	if(pok != 0) {									//Proverava da li uopste postoji alociran pok
		free(pok);									//Oslobadja memoriju koju je pok zauzeo
		alokacije--;								//Smanjuje broj generalnih alokacija
	}
	unlock											//Otkljucavanje
}

void interrupt odblk(){
	if(!(listaCekanja.isEmpty())){					//Da li postoje neki blokirani procesi?
		//listaCekanja.Ispis();
		Elem* p = listaCekanja.getHead();			//Hvata prvi blokirani proces
		if(p!=0){									//Proverava da li je alociran dati proces
			p -> x -> cekanje -- ;					//Dekrementira se cekanje za dati proces(Cekanje ostaih se gleda u odnosu na prvi u listi tako da dekrementiranjem jednog i ostalima se smanjuje vreme cekanje, sto je dosta efikasije nego da prolazimo kroz celu listu blokiranih i svima dekrementiramo vreme)
		}
		while(listaCekanja.getHead()!=0 && listaCekanja.getHead() -> x -> cekanje == 0){ // Provreava se da li je vreme cekanja prvog procesa u listi isteklo
			Elem* pom = listaCekanja.remF();		//Ako jeste izbacujemo prvi element iz liste cekanja
			PCB* pp = pom->x ;						//Pokupimo njegov PCB
			pp -> stance = 1;						//Postavljamo stance pcb-a u spreman
			pp -> unblockwae = 0 ;					//Govori da li je semafor ili smo se vremenski odblokiralim, posto je vremenski onda se postavlja na 0
			if(pp->sem!=0){							//Ako je proces blokiran na nekom semaforu?
				pp -> sem ->rem(pp -> id);			//Sklonimo ga iz semaforske liste(Nadjemo po id procesa iz njegovog semafora)
			}
			pp -> sem = 0 ;							//Nije blokiran ni na jednom semaforu trenutno
			delete pom;								//Dealociranje
			Scheduler::put(pp);						//Stavlja se u scheduler jer je proces sada spreman
			sch++;									//Povecava se broj procesa u scheduleru
		}
	}
}


void flock() {   //Funkcija potrebna za ugnjezdena zakljucavanja
	lock;
	lokbr--;
	unlock;
}

void funlock() { //Funkcija potrebna za ugnjezdjena otkljucavanja
	lock

	lokbr++ ;
	if(lokbr == 0 && zahtevana_promena_konteksta ==1) dispatch(); 	//Ako je doslo do zahteva za promenu konteksta u toku kriticne sekcije u kojoj je bio proces sada se poziva dispatch() koji ce da pozove promenu konteksta
	if(lokbr > 0 ) lokbr = 0 ;										//Ako slucajno predje preko nule, vraca se na nulu, da ne bi dolazilo do udnjezdjenog otkljucavanja

	unlock

}

void interrupt switchcontext(){
	asm {
		mov tsp, sp
		mov tss, ss								//Cuvaju se trenutne vrednosti sp,bp i ss
		mov tbp, bp
	}
	zahtevana_promena_konteksta=0;				//Ako je zpk bila promenjena na 1, sada se vraca na 0 jer je doslo do promene konteksta koja se zahtevala, ako se zahtevala
	running->sp = tsp;							//Trenutni proces je stao negde u izvrsavanju i mora i tu da se vrati kada se ponovo pokrene, zato se cuvaju u njega trenutne vrednosti za sp, bp i ss
	running->ss = tss;
	running->bp = tbp;
	if(running->stance==2){						//Proverava se da li je u stanju running trenutni proces
		running->stance=1;						//Postavlja se stanje trenutnog procesa u spreman
		if(running!=deadp) {					//Ako proces nije idle stalja se u scheduler(idle nije prakticno proces pa ne bi trebalo da se stavlja u scheduler)
			Scheduler::put((PCB*)running);      //Ubacuje se u scheduler trenutni proces
			sch++;								//Povecava se broj
		}
	}
	running=Scheduler::get();					//Kupi se novi proces iz scheduler-a
	sch--;										//Smanjuje se broj procesa u scheduleru
	if(running==0){								//Ako ne postoji proces koji trenutno moze da se izvrsava
		running=deadp;							//Ranuje se idle proces
		sch++;									//Vraca se broj u scheduleru na 0
	}
	running->stance=2;							//Postavlja se stanje na running
	tsp = running->sp;							// tsp, tss i tbp se azuriraju
	tss = running->ss;
	tbp = running->bp;
	brojac = running->kvant;					//Dodeljuje mu se trenutno vreme izvrsavanja koje je preostalo procesu
	asm {
		mov sp, tsp   // restore sp
		mov ss, tss
		mov bp, tbp
	}
}

void interrupt timer(...){	// prekidna rutina
	asm int 60h;								//Zove se prekin koji je na 60h lokaciji(odredjena integralna funkcija prekida)
	odblk();									//Vremenski blokirane procese odblokira
	if(brojac>0) brojac--;						//Smanji vreme trenutnom procesu
	if(brojac==0){								//Proverava je li mu isteklo vreme za izvrsavanje
		if(lokbr==0)switchcontext();			//Ako jeste onda proverava da li moze da se promeni kontekst zavisno od toga da li je trenutni proces u kritincoj sekciji
		else zahtevana_promena_konteksta=1;		//Ako jeste u kriticoj sekciji zpk se postavlja na 1 da bi se kontekst promenio nakon sto se izadje iz kritince sekcije
	}
	void tick();								//Poziva se funkcija tick()
	tick();


}




