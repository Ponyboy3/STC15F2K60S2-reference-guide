#include "reg52.h"
#include "iic.h"

sbit S4 = P3^3;

unsigned char code SMG_NUM[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

unsigned char stat = 0 ;
unsigned char dat_rd1;
unsigned char dat_rb2;

void Delay_SMG(unsigned int t)
{
	while(t--);
}
void Display_SMG_Bit(unsigned char pos,unsigned char value)
{
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0x01 << pos;
	P2 &= 0x1f;
	
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = value;
	P2 &= 0x1f;
}

void Display_SMG_Rd(unsigned char channel,unsigned char dat)
{ 
	Display_SMG_Bit(0,0xbf);
	Delay_SMG(500);
	Display_SMG_Bit(1,SMG_NUM[channel]);
	Delay_SMG(500);
	Display_SMG_Bit(2,0xbf);
	Delay_SMG(500);
	
	Display_SMG_Bit(5,SMG_NUM[dat / 100]);
	Delay_SMG(500);
	Display_SMG_Bit(6,SMG_NUM[(dat % 100) / 10]);
	Delay_SMG(500);
	Display_SMG_Bit(7,SMG_NUM[dat % 10]);
	Delay_SMG(500);
	
}

void Read_Rd1()
{
	IIC_Start(); 
	IIC_SendByte(0x90);
	IIC_WaitAck();	
	IIC_SendByte(0x01);
	IIC_WaitAck();
	IIC_Stop();
	
	Display_SMG_Rd(1,dat_rd1);
	
	IIC_Start(); 
	IIC_SendByte(0x91);
	IIC_WaitAck();	
	dat_rd1 = IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	Display_SMG_Rd(1,dat_rd1);
}

void Read_Rb2()
{
	IIC_Start(); 
	IIC_SendByte(0x90);
	IIC_WaitAck();	
	IIC_SendByte(0x03);
	IIC_WaitAck();
	IIC_Stop();
	
	Display_SMG_Rd(3,dat_rb2);
	
	IIC_Start(); 
	IIC_SendByte(0x91);
	IIC_WaitAck();	
	dat_rb2 = IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	Display_SMG_Rd(3,dat_rb2);
}

void Scan_Keys()
{
	if(S4 == 0)
	{
		Delay_SMG(100);
		if(S4 == 0)
		{
			if(stat == 0)
			{
				stat = 1;
				Display_SMG_Rd(1,dat_rd1);
			}
			else if(stat == 1)
			{
				stat = 0;
				Display_SMG_Rd(3,dat_rb2);
			}
		}
	}
}

void Init_Sys()
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;
	P2 &= 0x1f;
	
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x00;
	P2 &= 0x1f;
}
void main()
{
	Init_Sys();
	while(1)
	{
		Scan_Keys();
		
		if(stat == 0)
		{
			Read_Rd1();
		}
		else if(stat == 1)
		{
			Read_Rb2();
		}
	}
}