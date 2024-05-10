#include "reg52.h"
#include "iic.h"

sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;


unsigned char SMG_DIS[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};	

unsigned char dat1 = 0,dat2 = 0,dat3 = 0;

void Display_SMG_All();

void Display_SMG_Bit(unsigned char pos,unsigned char value)
{
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0x01 << pos;
	P2 &= 0x1f;
	
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = value;
	P2 &= 0x1f;
}

void Delay_Smg(unsigned int t)
{
	while(t--);
}

void Delay(unsigned int t)
{
	while(t--)
	{
		Display_SMG_All();
	}
}

void Scan_Keys()
{
	if(S6 == 0)
	{
		Delay_Smg(100);
		if(S6 == 0)
		{
			dat1++;
			if(dat1 > 13)
			{
				dat1 = 0;
			}
//			Write_24C02(0x00,dat1);
			while(S6 == 0)
			{
				Display_SMG_All();
			}
		}
	}
	
	if(S5 == 0)
	{
		Delay_Smg(100);
		if(S5 == 0)
		{
			dat2++;
			if(dat2 > 13)
			{
				dat2 = 0;
			}
//			Write_24C02(0x01,dat2);
			while(S5 == 0)
			{
				Display_SMG_All();
			}
		}
	}
	
	if(S4 == 0)
	{
		Delay_Smg(100);
		if(S4 == 0)
		{
			dat3++;
			if(dat3 > 13)
			{
				dat3 = 0;
			}
//			Write_24C02(0x02,dat3);
			while(S4 == 0)
			{
				Display_SMG_All();
			}
		}
	}
}

void Display_SMG_All()
{
	Display_SMG_Bit(0,SMG_DIS[dat1 / 10]);
	Delay_Smg(500);
	Display_SMG_Bit(1,SMG_DIS[dat1 % 10]);
	Delay_Smg(500);
	
	Display_SMG_Bit(2,0xbf);
	Delay_Smg(500);
	
	Display_SMG_Bit(3,SMG_DIS[dat2 / 10]);
	Delay_Smg(500);
	Display_SMG_Bit(4,SMG_DIS[dat2 % 10]);
	Delay_Smg(500);
	
	Display_SMG_Bit(5,0xbf);
	Delay_Smg(500);
	
	Display_SMG_Bit(6,SMG_DIS[dat3 / 10]);
	Delay_Smg(500);
	Display_SMG_Bit(7,SMG_DIS[dat3 % 10]);
	Delay_Smg(500);
}

void Write_24C02(unsigned char addr,unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop(); 
}

unsigned char Read_24C02(unsigned char addr)
{
	unsigned char tmp;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	tmp = IIC_RecByte(); 
	IIC_SendAck(1);
	IIC_Stop();
	
	return tmp;
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
	dat1 = Read_24C02(0x01);
	dat2 = Read_24C02(0x03);
	dat3 = Read_24C02(0x05);
	
	
//	Write_24C02(0x03,11);
//	
//	dat2 = Read_24C02(0x01);
	while(1)
	{
		Scan_Keys();

		Display_SMG_All();
		
	}
}