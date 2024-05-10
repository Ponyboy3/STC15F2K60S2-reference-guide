#ifndef __ONEWIRE_H
#define __ONEWIRE_H

#include "reg52.h"

sbit DQ = P1^4;  

void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20(void);
bit init_ds18b20(void);

#endif