#ifndef Numbdis_h
#define Numbdis_h


#include "Arduino.h"

class Numbdis
{
public:	
	int Numbdis(unsigned long input = 0, int pos = 0);
private:
	int x;
	int i;
};


#endif