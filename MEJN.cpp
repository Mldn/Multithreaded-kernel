//pretpostavljeni memorijski model: huge

#include <iostream.h>
#include <dos.h>
#include "schedule.h"
#include "Utility.h"
#include "PCB.h"
#include "Primer.h"
#include "ListI.h"
#include "ListPCB.h"
#include "Semaphor.h"
#include "USER.H"



pInterrupt oldRoutine=getvect(0x08);



// postavlja novu prekidnu rutinu
void inic(){
	setvect(0x08,timer);
	setvect(0x60,oldRoutine);
	PCB::begin();
}

// vraca staru prekidnu rutinu
void restore(){
	setvect(0x08,oldRoutine);
	delete deadp;
	deadp=0;
	delete running;
	running=0;
}


void doSomething(){
	flock();
	Semaphore* sem=new Semaphore(0);
    Primer** primeri=new Primer*[5];
    for(int k=0;k<5;k++){
    	primeri[k]=new Primer(defaultStackSize,defaultTimeSlice,k,sem);
    	primeri[k]->start();
    }
    funlock();
    sem->wait(5);
    sem->signal(5);
    for(int j=0;j<5;j++){
    	 delete primeri[j];
    }
    delete primeri;
	for (int i = 0; i < 3; ++i) {
		flock();
		cout<<"main "<<i<<endl;
		funlock();
		for (int j = 0; j< 30000; ++j)
			for (int k = 0; k < 30000; ++k);
	}

    delete sem;
	cout<<"Happy End"<<endl;

}

/*void doSomething2(){
	List_I intl;
	for(int i=0;i<30;i++){
		intl.addL(i);
	}
	intl.Ispis();
	intl.addF(33);
	intl.Ispis();
	intl.remF();
	intl.Ispis();
	intl.sortAdd(15);
	intl.Ispis();
	Elem* x=intl.rem(16);
	cout<<x->x<<endl;
	intl.Ispis();
	List_I intl2(intl);
	intl2.Ispis();
	intl2.~List_I();
	intl2.Ispis();
	intl.Ispis();

}*/


void doSomething3(){
	ListPCB intl;
	PCB* p = new PCB();
	p -> cekanje = 3;
	PCB* d = new PCB();
	d -> cekanje = 6;
	PCB* t = new PCB();
	t -> cekanje = 7;

	intl.addV(p);
	intl.addV(d);
	intl.addV(t);
	intl.Ispis();
    intl.remV(d->id);
    intl.Ispis();
	PCB* c = new PCB();
	c -> cekanje = 6 ;

	intl.addV(c);
	intl.Ispis();

	intl.remV(t ->id);
	intl.Ispis();


}


int main(int argc,char* argv[]){
	
	cout << sch<< endl;
	inic();
  
	int userMain(int,char*[]);
	userMain(argc,argv);

	restore();
	cout << sch << endl;
    return 0;
}

