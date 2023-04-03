#ifndef __IIC_H__
#define __IIC_H__

#include "reg52.h"
#include "intrins.h"

sbit sda = P2^1;
sbit scl = P2^0;

void I2CStart(void);
void I2CStop(void);
void I2CSendByte(unsigned char byt);
unsigned char I2CReceiveByte(void);
unsigned char I2CWaitAck(void);
void I2CSendAck(unsigned char ackbit);

#endif