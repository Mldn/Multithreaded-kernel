// File: thread.h
#ifndef _thread_h_
#define _thread_h_


typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;							//defaultna velicina steka
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms				//Koliko se vremena daje procesima po default-u za kvant
typedef int ID;


class PCB; // Kernel's implementation of a user's thread

class Thread {
public:

	void start();
	void waitToComplete();											//Ako probamo da obrisemo nit pre nego sto se ona zavrsila blokiramo se, jer mora da se saceka da se zavrsi izvrsavanje niti
	virtual ~Thread();
	ID getId();
	static ID getRunningId();										//Trenutna nit koja se izvrsavava
	static Thread * getThreadById(ID id);							//Dobijanje niti po njenom id-u

protected:

	friend class PCB;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}

private:

	PCB* myPCB;														//Svaka nit poseduje odredjeni proces koji izvrsava

};

void dispatch ();

#endif
