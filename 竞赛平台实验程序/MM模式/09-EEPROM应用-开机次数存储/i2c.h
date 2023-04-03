/** 
* @file         i2c.h 
* @brief        51单片机通过I0模拟I2C总线
* @author       GhpZhu 
* @date     2012/8/21 
* @version  A001 
* @par Copyright (c):  
*       All User 
* @par History:          
*   version: author, date, desc\n 
*/

#ifndef __I2C_H
#define __I2C_H

void i2c_delay(unsigned char i);
void i2c_start(void);
void i2c_stop(void);

void i2c_sendbyte(unsigned char byt);
unsigned char i2c_waitack(void);
unsigned char i2c_receivebyte(void);
void i2c_sendack(unsigned char ackbit);

void write_eeprom(unsigned char add,unsigned char val);
unsigned char read_eeprom(unsigned char add);

void init_pcf8591(void);
unsigned char adc_pcf8591(void);

#endif 