#include "reg52.h"
#include "iic.h"

sbit S4 = P3^3;
sbit S5 = P3^2;

unsigned char code SMG_NUM[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char code SMG_NUM1[10] = {0x40,0x79,0x34,0x30,0x19,0x12,0x02,0x78,0x00,0x10};

unsigned char dat_rd1;
unsigned char bac_rd1;
unsigned char n,n1;
float read_v;
unsigned int smg_v;

//=============数码管显示函数=================
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

void Display_SMG()
{
	Display_SMG_Bit(0,0xbf);
	Delay_SMG(500);
	
	Display_SMG_Bit(1,SMG_NUM[n]);
	Delay_SMG(500);
	
	Display_SMG_Bit(2,0xbf);
	Delay_SMG(500);
	
	Display_SMG_Bit(5,SMG_NUM[dat_rd1 / 100]);
	Delay_SMG(500);
	Display_SMG_Bit(6,SMG_NUM[(dat_rd1 % 100) / 10]);
	Delay_SMG(500);
	Display_SMG_Bit(7,SMG_NUM[dat_rd1 % 10]);
	Delay_SMG(500);
	
	
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0xff;
	P2 &= 0x1f;

	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0xff;
	P2 &= 0x1f;
}

void Display_SMG_Old()
{
	Display_SMG_Bit(0,0xbf);
	Delay_SMG(500);
	
	Display_SMG_Bit(1,SMG_NUM[n1]);
	Delay_SMG(500);
	
	Display_SMG_Bit(2,0xbf);
	Delay_SMG(500);
	
	Display_SMG_Bit(5,SMG_NUM[bac_rd1 / 100]);
	Delay_SMG(500);
	Display_SMG_Bit(6,SMG_NUM[(bac_rd1 % 100) / 10]);
	Delay_SMG(500);
	Display_SMG_Bit(7,SMG_NUM[bac_rd1 % 10]);
	Delay_SMG(500);
	
	
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0xff;
	P2 &= 0x1f;

	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0xff;
	P2 &= 0x1f;
}
//==================PCF8951部分==============
void Read_Rd1()
{
	IIC_Start(); 
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x01);
	IIC_WaitAck();
	IIC_Stop();
	
	Display_SMG();
	
	IIC_Start(); 
	IIC_SendByte(0x91);
	IIC_WaitAck();
	dat_rd1 = IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	Display_SMG();
}
//===================存储器24C02===============
unsigned char Read_24C02(unsigned char addr)
{
	unsigned char tmp;
	IIC_Start(); 
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start(); 
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	tmp = IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	return tmp;
}

void write_24C02(unsigned char addr,unsigned char dat)
{
	IIC_Start(); 
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_SendAck(1);
	IIC_Stop();
}
//==============LED按键控制函数===============
void Led_Contral()
{
	read_v = dat_rd1*(5/255);
	smg_v = read_v*100;
	if( dat_rd1 > 200)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = 0xfe << 0;
		P2 &= 0x1f;
		n = 1;
	}
	else if( dat_rd1 > 150)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = 0xfe << 1;
		P2 &= 0x1f;
		n = 2;
	}
	else if(dat_rd1 > 100)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = 0xfe << 3;
		P2 &= 0x1f;
		n = 3;
	}
	else if( dat_rd1 > 50)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = 0xfe << 5;
		P2 &= 0x1f;
		n = 4;
	}
	else
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = 0xfe << 7;
		P2 &= 0x1f;
		n = 5;
	}
}

//==============按键扫描函数=================
void Scan_Keys()
{
	if(S4 == 0)
	{
		Delay_SMG(100);
		if(S4 == 0)
		{
			write_24C02(0x01,n);
			Delay_SMG(1000);
			write_24C02(0x02,dat_rd1);
			while(S4 == 0)
			{
				Display_SMG();
			}
			
		}
	}
	if(S5 == 0)
	{
		Delay_SMG(100);
		if(S5 == 0)
		{
			while(S5 == 0)
			{
				n1 = Read_24C02(0x01);
				bac_rd1 = Read_24C02(0x02);
				Display_SMG_Old();
			}
		}
	}
}

//==============系统初始化函数================
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
		Read_Rd1();
		Led_Contral();
		Scan_Keys();
	}
}