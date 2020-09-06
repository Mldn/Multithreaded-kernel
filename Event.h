/*
 * Event.h
 *
 *  Created on: Sep 15, 2019
 *      Author: OS1
 */

#ifndef _event_h_
#define _event_h_


typedef unsigned char IVTNo;

#include "IVTEntr.h"
class KernelEv;
class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();
protected:
	friend class KernelEv;
	void signal(); // can call KernelEv
private:
	KernelEv* myImpl;
	IVTNo no;
};

//Makro definise funkciju koja ce se pozivati kada se desi prekid za event, zove signal eventa u prekidu

#define PREPAREENTRY(i,f)\
	void interrupt inter##i(...){\
	    IVTEntry::ulazi[i]->signal();\
    }\
    IVTEntry* IVTEntry##i=new IVTEntry(i,&inter##i,f);
#endif
