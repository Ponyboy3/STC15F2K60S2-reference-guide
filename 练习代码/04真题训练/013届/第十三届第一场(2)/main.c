#include "reg52.h"
#include "ds1302.h"
#include "onewire.h"

sfr P4 = 0xc0;
sbit P44 = P4^4;
sbit P42 = P4^2;

#define uchar unsigned char
#define uint unsigned int
	
uchar code type1[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xc1,0xbf};
uchar code type2[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

uchar num1,num2,num3,num4,num5,num6,num7,num8;
bit f_2ms;//2ms标志
uchar GN;//按键标志
uchar model;//按键模式标志
uchar h,m,s;
uint temperature;
uchar stat_temp = 23;
uint count;
bit f_S13,f_S17;
uint t_2ms,t_5s,tt_5s;
uint t_100ms;

uchar bdata U9;
sbit relay=U9^4;

uchar bdata U6=0xff;
sbit led1=U6^0;
sbit led2=U6^1;
sbit led3=U6^2;

bit f_relay,f_led;

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
		case 6:P2 = (P2 & 0x1f) | 0xe0;
														if(model == 0)
														{
															P0 = type1[n7]&0x7f;
														}
														else
															P0 = type1[n7];
																				          P2 &= 0x1f;break;
		case 7:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n8];P2 &= 0x1f;break;
	}
	if(++i == 8)
		i = 0;
}

void Delay_Keys(uint t)
{
	while(t--);
}
uchar Scan_Keys()
{
	uchar H,L;
	P3 = 0xcf;P44 = 0;P42 = 0;H = (~P3) & 0x0f;// 1 1 0 0 1 1 1 1
	if(H == 0) return 0;
	Delay_Keys(100);
	P3 = 0xcf;P44 = 0;P42 = 0;H = (~P3) & 0x0f;// 1 1 0 0 1 1 1 1
	if(H == 0) return 0;
	P3 = 0xf0;P44 = 1;P42 = 1;L = (~P3) & 0x30;// 1 1 0 0 1 1 1 1
	if(P44 == 0) L |= 0x80;
	if(P42 == 0) L |= 0x40;
	return (H+L);
}
void Change_Keys()
{
	switch(Scan_Keys())
	{
		case 0x28:GN = 12;break;
		case 0x24:GN = 13;break;
		case 0x18:GN = 16;break;
		case 0x14:GN = 17;break;
		case 0:GN = 0;break;
	}
}

void Xie_1302()
{
	Write_Ds1302_Byte(0x8e,0x00);
	Write_Ds1302_Byte(0x80,0x45);//秒
	Write_Ds1302_Byte(0x82,0x58);//分
	Write_Ds1302_Byte(0x84,0x23);//时
}

void Du_1302()
{
	s = Read_Ds1302_Byte (0x81);
	m = Read_Ds1302_Byte (0x83);
	h = Read_Ds1302_Byte (0x85);
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
	
	U6 = 0xff;
	P2 = (P2 & 0x1f) | 0x80;
	P0 = U6;
	P2 &= 0x1f;
}
void Led_Control()
{
	if(m == 0 && s == 0)
	{
		f_led = 1;
	}
	if(f_led == 1)
	{
		EA = 0;
		led1 = 0; P0 = 0xff;
		P2 = (P2 & 0x1f) | 0x80;
		P0 = U6;
		P2 &= 0x1f;
		EA = 1;
		if(++tt_5s == 2500)
		{
			tt_5s = 0;
			EA = 0;
			led1 = 1; P0 = 0xff;
			P2 = (P2 & 0x1f) | 0x80;
			P0 = U6;
			P2 &= 0x1f;
			EA = 1;
			f_led = 0;
		}
	}
	
	if(f_S13 == 0)
	{
		EA = 0;
		led2 = 0; P0 = 0xff;
		P2 = (P2 & 0x1f) | 0x80;
		P0 = U6;
		P2 &= 0x1f;
		EA = 1;
	}
	else
	{
		EA = 0;
		led2 = 1; P0 = 0xff;
		P2 = (P2 & 0x1f) | 0x80;
		P0 = U6;
		P2 &= 0x1f;
		EA = 1;
	}
	if(relay == 1)
	{
		if(++t_100ms==50)
		{
			t_100ms = 0;
			led3=~led3;
		}
		EA = 0;
		         P0 = 0xff;
		P2 = (P2 & 0x1f) | 0x80;
		P0 = U6;
		P2 &= 0x1f;
		EA = 1;
	}
	else
	{
		EA = 0;
		led3 = 1; P0 = 0xff;
		P2 = (P2 & 0x1f) | 0x80;
		P0 = U6;
		P2 &= 0x1f;
		EA = 1;
	}
}

void Relay_Control()
{
	if(f_S13 == 0)
	{
		if(temperature > stat_temp*10)
		{
			EA = 0;
			relay = 1;
			P2 = (P2 & 0x1f) | 0xa0;
			P0 = U9;
			P2 &= 0x1f;
			EA = 1;
		}
		else
		{
			EA = 0;
			relay = 0;
			P2 = (P2 & 0x1f) | 0xa0;
			P0 = U9;
			P2 &= 0x1f;
			EA = 1;
		}
	}
	if(f_S13 == 1)
	{
		if(m==0 && s==0)
		{
			f_relay = 1;
		}
		if(f_relay == 1)
		{
			EA = 0;
			relay = 1;
			P2 = (P2 & 0x1f) | 0xa0;
			P0 = U9;
			P2 &= 0x1f;
			EA = 1;
			if(++t_5s == 2500)
			{
				t_5s = 0;
				EA = 0;
				relay = 0;
				P2 = (P2 & 0x1f) | 0xa0;
				P0 = U9;
				P2 &= 0x1f;
				EA = 1;
				f_relay = 0;
			}
		}
	}
}
void main()
{
	Init_Sys();
	Init_T1();
	Init_18b20();
	Xie_1302();
	while(1)
	{
		if(f_2ms == 1)
		{
			f_2ms = 0;
			Du_1302();
			Change_Keys();
			Led_Control();
			Relay_Control();
			if(++t_2ms == 100)
			{
				t_2ms = 0;
				temperature = Read_Temp();
			}
			if(GN == 12)
			{
				model++;
				if(model > 2)
				{
					model = 0;
				}
				while(Scan_Keys());
			}
			if(GN == 13)
			{
				f_S13 = ~f_S13;
				while(Scan_Keys());
			}
			switch(model)
			{
				case 0:
					num2 = 1;
					num1= 11;
					num3=num4=num5=10;
					num6=temperature/100;num7=temperature/10%10;num8=temperature%10;
				break;
				
				case 1:
					if(GN == 17)
					{
						f_S17 = 1;
						while(Scan_Keys());
						f_S17 = 0;
					}
					Du_1302();
					num2 = 2;
					num1= 11;
					num3=10;
					num6=12;
					num4=h/16;num5=h%16;
					num7=m/16;num8=m%16;
				break;
				
				case 2:
						if(GN == 16)
						{
							stat_temp++;
							if(stat_temp > 99)
							{
								stat_temp = 99;
							}
							while(Scan_Keys());
						}
						if(GN == 17)
						{
							stat_temp--;
							if(stat_temp < 10)
							{
								stat_temp = 10;
							}
							while(Scan_Keys());
						}
					num3=num4=num5=num6=10;
					num7=stat_temp/10;num8=stat_temp%10;
					num2 = 3;
					num1= 11;
				break;
			}
		}
	}
}
void Service_T1() interrupt 3
{
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;
	
	f_2ms = 1;
	Display_Smg(num1,num2,num3,num4,num5,num6,num7,num8);
	if(f_S17 == 1)
	{
		Du_1302();
		num2 = 2;
		num1= 11;
		num3=10;
		num6=12;
		num4=m/16;num5=m%16;
	  num7=s/16;num8=s%16;
	}				
}