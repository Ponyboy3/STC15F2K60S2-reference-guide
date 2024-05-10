#ifndef __DS1302_H
#define __DS1302_H

#include "reg52.h"
#include "intrins.h"

sbit SCK = P1^7;		
sbit SDA1 = P2^3;		
sbit RST = P1^3; 

void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302_Byte( unsigned char address );

#endif