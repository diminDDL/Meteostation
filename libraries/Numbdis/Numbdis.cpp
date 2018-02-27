#include "Arduino.h"
#include "Numbdis.h"
/*
Numbdis.cpp - Library for seperating numbers in arduino code.
Created by DDL, Coder_fox August 20, 2017.
Released into the public domain.
*/

Numbdis::Numbdis(unsigned long input = 0, int pos = 0)
{

	x = 0;
	i = 0;

	if (input<10) {
		i = 1;
	}
	else if (input<100) {
		i = 2;
	}
	else if (input<1000) {
		i = 3;
	}
	else if (input<10000) {
		i = 4;
	}
	else if (input<100000) {
		i = 5;
	}
	else if (input<1000000) {
		i = 6;
	}
	else if (input<10000000) {
		i = 7;
	}
	else if (input<100000000) {
		i = 8;
	}
	else if (input<1000000000) {
		i = 9;
	}
	else {
		i = 10;
	}

	x = i - y;
	for (int a = x; a > 0; a--) {
		output = input % 10;
		input /= 10;
	}
	return output;
}
