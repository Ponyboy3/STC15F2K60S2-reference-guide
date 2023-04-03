#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__

#include "reg52.h"

sbit DQ = P1^4;

void Init_18b20();
unsigned int Read_Temp();

#endif