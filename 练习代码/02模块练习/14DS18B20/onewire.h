#ifndef __ONEWIRE_H
#define __ONEWIRE_H

#include "reg52.h"

sbit DQ = P1^4;  

void Init_18b20();
unsigned int Read_Temperature(void);  

#endif
