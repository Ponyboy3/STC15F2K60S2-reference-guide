#include "reg52.h"
#include "ds1302.h"
#define uchar unsigned char
#define uint unsigned int
	
uchar code type1[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0x88,0xc6,0x86};
uchar code type2[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar h,m,s;
uint t_2ms;
bit f_2ms;

void Display_SMG(uchar n1,uchar n2,uchar n3,uchar n4,uchar n5,uchar n6,uchar n7,uchar n8)
{
	char i;
	P0 = 0xff;//消影
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

void Init_T1()
{
	TMOD = (TMOD & 0x0f) | 0x10;
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;
	
	EA = 1;
	ET1 = 1;
	TR1 = 1;
}
void Xie_DS1302()
{
	Write_Ds1302_Byte(0x8e,0x00);
	Write_Ds1302_Byte(0x84,0x23);//时
	Write_Ds1302_Byte(0x82,0x59);//分
	Write_Ds1302_Byte(0x80,0x45);//秒
}
void Du_DS1302()
{
	h = Read_Ds1302_Byte(0x85);//时
	h = (h >> 4) * 10 + (h & 0x0f);
	m = Read_Ds1302_Byte(0x83);//分
	m = (m >> 4) * 10 + (m & 0x0f);
	s = Read_Ds1302_Byte(0x81);//秒
	s = (s >> 4) * 10 + (s & 0x0f);
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
	Xie_DS1302();
	while(1)
	{
		if(f_2ms == 1)
		{
			f_2ms = 0;
			if(++t_2ms == 250)
			{
				t_2ms = 0;
				Du_DS1302();
			}
			num1=h/10;num2=h%10;num3=10;num4=m/10;num5=m%10;num6=10;num7=s/10;num8=s%10;
		}
	}
}
void Service_T1() interrupt 3
{
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;
	
	f_2ms = 1;
	Display_SMG(num1,num2,num3,num4,num5,num6,num7,num8);
}