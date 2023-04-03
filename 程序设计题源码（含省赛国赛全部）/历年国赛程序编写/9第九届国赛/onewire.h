#ifndef __ONEWIRE_H
#define __ONEWIRE_H


#include <STC15F2K60S2.H>

sbit DQ = P1^4;  

unsigned int rd_temperature(void);  
void initial_18b20();

#endif
