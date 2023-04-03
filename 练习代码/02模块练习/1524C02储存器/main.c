#include "reg52.h"
#include "ds1302.h"  
#include "iic.h"

#define uchar unsigned char
#define uint unsigned int

uchar code type1[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf};
uchar code type2[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

uchar num1,num2,num3,num4,num5,num6,num7,num8;
bit f_2ms;
uchar GN;
uchar h,m,s;

void Display_Smg(uchar n1,uchar n2,uchar n3,uchar n4,uchar n5,uchar n6,uchar n7,uchar n8)
{
	uchar i;
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = type2[i];
	P2 &= 0x1f;
	switch(i)
	{
		case 0:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n1];P2 &= 0x1f;break;
		case 1:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n2];P2 &= 0x1f;break;
		case 2:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n3];P2 &= 0x1f;break;
		case 3:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n4];P2 &= 0x1f;break;
		case 4:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n5];P2 &= 0x1f;break;
		case 5:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n6];P2 &= 0x1f;break;
		case 6:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n7];P2 &= 0x1f;break;
		case 7:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n8];P2 &= 0x1f;break;
	}
	if(++i==8)
		i=0;
}
void Delay_Keys(uint t)
{
	while(t--);
}
uchar Scan_Keys()
{
	uchar d;
	d = (~P3)&0x0f;
	if(d == 0) return 0;
	Delay_Keys(100);
	if(d == 0) return 0;
	return d;
}
void Change_Keys()
{
	switch(Scan_Keys())
	{
		case 8:GN = 1;break;//S4
		case 4:GN = 2;break;
		case 2:GN = 3;break;
		case 1:GN = 4;break;//S7
		case 0:GN = 0;break;
	}
}

void Init_DS1302()
{
	Write_Ds1302_Byte(0x80,0x45);
	Write_Ds1302_Byte(0x82,0x59);
	Write_Ds1302_Byte(0x84,0x23);
}
void Du_DS1302()
{
	h = Read_Ds1302_Byte( 0x85 );
	m = Read_Ds1302_Byte( 0x83 );
	s = Read_Ds1302_Byte( 0x81 );
}

uchar Du_24c02(uchar addr)
{
	uchar d;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_Stop(); 
	
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	d = IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	return d;
}
void Xie_24c02(uchar addr,uchar dat)
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
void Init_T1()
{
	TMOD = (TMOD & 0x0f) | 0x10;
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;
	
	EA = 1;
	ET1 = 1;
	TR1 = 1;
}
void Init_Sys()
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;
	P2 &= 0x1f;
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x00;
	P2 &= 0x1f;
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0xff;
	P2 &= 0x1f;
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0xff;
	P2 &= 0x1f;
}
void main()
{
	Init_Sys();
	Init_T1();
	Init_DS1302();
	h = Du_24c02(0x01);
	m = Du_24c02(0x03);
	s = Du_24c02(0x05);
	while(1)
	{
		if(f_2ms == 1)
		{
			f_2ms = 0;
			Change_Keys();
			if(GN == 1)
			{
				Xie_24c02(0x01,h);
				Xie_24c02(0x03,m);
				Xie_24c02(0x05,s);
				while(Scan_Keys());
			}
			Du_DS1302();
			num1=h/16;num2=h%16;num3=11;num4=m/16;num5=m%16;num6=11;num7=s/16;num8=s%16;
		}
	}
}
void Service_T1() interrupt 3
{
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;
	
	f_2ms = 1;
	Display_Smg(num1,num2,num3,num4,num5,num6,num7,num8);
}