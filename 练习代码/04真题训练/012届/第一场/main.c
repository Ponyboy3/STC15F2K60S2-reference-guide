#include "reg52.h"
#include "iic.h"
#include "onewire.h"

sfr P4 = 0xc0;

sbit P44 = P4^4;
sbit P42 = P4^2;

#define uchar unsigned char
#define uint unsigned int
	
uchar code type1[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xc6,0x8c,0x88};
uchar code type2[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar model;
uchar GN;
uint V_DAC;
uchar temp = 25;
uint temperature;
bit f_2ms;
bit f_S5;

//数码管部分
void Display_SMG(uchar n1,uchar n2,uchar n3,uchar n4,uchar n5,uchar n6,uchar n7,uchar n8)
{
	char i;
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xc0;P0 = type1[n1];
	P0 = type2[i];
	P2 &= 0x1f;
	switch(i)
	{
		case 0:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n1];P2 &= 0x1f;break;
		case 1:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n2];P2 &= 0x1f;break;
		case 2:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n3];P2 &= 0x1f;break;
		case 3:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n4];P2 &= 0x1f;break;
		case 4:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n5];P2 &= 0x1f;break;
		case 5:P2 = (P2 & 0x1f) | 0xe0;
								if(model == 0 || model == 2)
								{
									P0 = type1[n6] & 0x7f;
								}
								else
									P0 = type1[n6];
																	 P2 &= 0x1f;break;
		case 6:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n7];P2 &= 0x1f;break;
		case 7:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n8];P2 &= 0x1f;break;
	}
	if(++i == 8)
		i = 0;
}
//=======按键部分
void Delay_Keys(uint t)
{
	while(t--);
}
uchar Scan_Keys()
{
	char H,L;
	P3 =  0xcf;P44 = 0;P42 = 0;H = (~P3) & 0x0f;
	if(H == 0) return 0;
	Delay_Keys(100);
	P3 =  0xcf;P44 = 0;P42 = 0;H = (~P3) & 0x0f;
	if(H == 0) return 0;
	P3 =  0xf0;P44 = 1;P42 = 1;L = (~P3) & 0x30;
	if(P44 == 0) L |= 0x80;
	if(P42 == 0) L |= 0x40;
	return (H+L);
}

void Change_Keys()
{
	switch(Scan_Keys())
	{
		case 0x88:GN = 1;break;//S4
		case 0x84:GN = 2;break;
		case 0x48:GN = 3;break;
		case 0x44:GN = 4;break;
		case 0:GN = 0;break;
	}
}
//========DAC部分
void Xie_DAC(uchar dat)
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x40);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();
}

//=======定时器1
void Init_T1()
{
	TMOD = (TMOD & 0x0f) | 0x10;
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;
	
	EA = 1;
	ET1 = 1;
	TR1 = 1;
}
//=======Led控制====
void Led_Control()
{
	if(model == 0)//控制L2
	{
		P0 = 0xff;
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 & 0xfd;
		P2 &= 0x1f;
	}
	else
	{
		P0 = 0xff;
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 | 0x02;
		P2 &= 0x1f;
	}
	if(model == 1)//控制L3
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 & 0xfb;
		P2 &= 0x1f;
	}
	else
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 | 0x04;
		P2 &= 0x1f;
	}
	if(model == 2)//控制L4
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 & 0xf7;
		P2 &= 0x1f;
	}
	else
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 | 0x08;
		P2 &= 0x1f;
	}
	if(f_S5 == 0)//控制L1
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 & 0xfe;
		P2 &= 0x1f;
	}
	else
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 | 0x01;
		P2 &= 0x1f;
	}
}
//=======系统初始化
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
	Init_18b20();
	while(1)
	{
		if(f_2ms == 1)
		{
			f_2ms = 0;
			Change_Keys();
			if(GN == 1)
			{
				model++;
				if(model == 3)
					model = 0;
				while(Scan_Keys());
			}
			if(GN == 2)
			{
				f_S5 = ~f_S5;
				while(Scan_Keys());
			}
			Led_Control();
			switch(model)
			{
				case 0:
					temperature = rd_temperature();
					num1 = 11;
					num2=num3=num4=10;
					num5 = temperature / 1000;num6 = (temperature % 1000) / 100;num7 = (temperature % 100) / 10,num8 = temperature % 10;
				break;
				
				case 1:
					if(GN == 3)
					{
						if(temp < 1)
						{
							temp = 0;
						}
						else
						{
							temp--;
						}
						while(Scan_Keys());
					}
					if(GN == 4)
					{
						if(temp > 99)
						{
							temp = 100;
						}
						else
						{
							temp++;
						}
						while(Scan_Keys());
					}
					num1 = 12;
					num2=num3=num4=num5=num6=10;
					num7 = temp / 10,num8 = temp % 10;
				break;
				
				case 2:
					temperature = rd_temperature();
					if(f_S5 == 0)
					{
						if(temp > temperature/100)
						{
							Xie_DAC(0);
							V_DAC = 0;
						}
						else
						{
							Xie_DAC(255);
							V_DAC = 500;
						}
					}
					else
					{
						if((temperature/100) < 20)
						{
							Xie_DAC(51);
							V_DAC = 100;
						}
						else if((temperature/100) > 40)
						{
							Xie_DAC(204);
							V_DAC = 400;
						}
						else
						{
							V_DAC = (153*(temperature/100))/20 - 102;
							Xie_DAC(V_DAC);
							V_DAC = V_DAC*500/255;
						}
					}
					num1 = 13;
					num2=num3=num4=num5=num6=10;
					num6=V_DAC / 100;num7 = (V_DAC / 10) %10,num8 = V_DAC % 10;
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
	
	Display_SMG(num1,num2,num3,num4,num5,num6,num7,num8);
}