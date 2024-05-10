#ifndef _IIC_H
#define _IIC_H

#include "reg52.h"
#include "intrins.h"

sbit scl=P2^0;
sbit sda=P2^1;

void I2CStart(void);
void I2CStop(void);

void I2CSendAck(unsigned char ackbit);
unsigned char I2CWaitAck(void);

void I2CSendByte(unsigned char byt);
unsigned char I2CReceiveByte(void);

#endif