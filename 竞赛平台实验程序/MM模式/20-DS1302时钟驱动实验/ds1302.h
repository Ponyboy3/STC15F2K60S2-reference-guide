#ifndef __DS1302_H
#define __DS1302_H

#include "reg52.h"
#include "intrins.h"

sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;   // DS1302复位

void SetRTC(void);
unsigned char* ReadRTC(void);


#endif
