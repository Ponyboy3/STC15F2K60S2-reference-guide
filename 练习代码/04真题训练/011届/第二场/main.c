#include "reg52.h"
#include "onewire.h"
#include "iic.h"


#define uchar unsigned char
#define uint unsigned int

uchar code type1[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xc6,0x8c};
uchar code type2[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar t_max = 30,t_min = 20;
uint tt_2ms;
uchar t_led;
bit f_2ms;
bit model,f_S5;
bit shanshuo;
bit t_hege;

//===============数码管部分
void Display_SMG(uchar n1,uchar n2,uchar n3,uchar n4,uchar n5,uchar n6,uchar n7,uchar n8)
{
	char i;
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
	{
		i = 0;
	}
}

//===============按键部分
void Delay_Keys(uint t)
{
	while(t--);
}
unsigned char Scan_Keys()
{
	char d;
	d = (~P3) & 0x0f;
	if(d == 0) return 0;
	Delay_Keys(100);
	d = (~P3) & 0x0f;
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
		case 1:GN = 4;break;
		case 0:GN = 0;break;//S7
	}
}
//===============ADC部分
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

//============定时器1
void Init_T1()
{
	TMOD = (TMOD & 0x0f) | 0x10;
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;
	
	EA = 1;
	ET1 = 1;
	TR1 = 1;
}
//===============系统初始化
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
//============主函数
void main()
{
	uint temperature;
	Init_Sys();
	Init_T1();
	Init_18b20();
	while(1)
	{
		if(f_2ms == 1)
		{
			f_2ms = 0;
			Change_Keys();
			if(GN == 1)//S4按下
			{
				f_S5 = 0;
				model = ~model;
				while(Scan_Keys());
			}
			if(GN == 2)//S5按下
			{
				f_S5 = ~f_S5;
				while(Scan_Keys());
			}
			if(model == 0 && t_hege == 1)
			{
				temperature = Read_Temp();
				if(temperature > t_max)
				{
					Xie_DAC(204);
					P0 = 0xff;
					P2 = (P2 & 0x1f) | 0x80;
					P0 = 0xfe;
					t_led = P0;
					P2 &= 0x1f;
				}
				else if(temperature <= t_max && temperature > t_min)
				{
					Xie_DAC(153);
					P0 = 0xff;
					P2 = (P2 & 0x1f) | 0x80;
					P0 = 0xfd;
					t_led = P0;
					P2 &= 0x1f;
				}
				else if(temperature < t_min)
				{
					Xie_DAC(102);
					P0 = 0xff;
					P2 = (P2 & 0x1f) | 0x80;
					P0 = 0xfb;
					t_led = P0;
					P2 &= 0x1f;
				}
				num1 = 11;
				num2=num3=num4=10;
				num5=temperature/1000;
				num6=temperature/100%10;
				num7=temperature/10%10;
				num8 = temperature%10;
			}
			else if(model == 1 || t_hege == 0)
			{
				if(f_S5 == 0)//控制Tmin
				{
					if(GN == 3)//S6按下
					{
						if(t_min > 99)
							t_min = 100;
						else
							t_min++;
						while(Scan_Keys());
					}
					if(GN == 4)//S7按下
					{
						if(t_min < 1)
							t_min = 0;
						else
							t_min--;
						while(Scan_Keys());
					}
					if(++tt_2ms == 200)
					{
						tt_2ms = 0;
						shanshuo = ~shanshuo;
					}
					if(shanshuo == 0)
					{
						num7=10;num8=10;
					}
					else
					{
						num7=t_min/10;num8=t_min%10;
					}
					num1 = 12;
					num2=num3=num6=10;
					num4=t_max/10;num5=t_max%10;
				}
				else         //控制Tmax
				{
					if(GN == 3)//S6按下
					{
						if(t_max > 99)
							t_max = 100;
						else
							t_max++;
						while(Scan_Keys());
					}
					if(GN == 4)//S7按下
					{
						if(t_max < 1)
							t_max = 0;
						else
							t_max--;
						while(Scan_Keys());
					}
					if(++tt_2ms == 200)
					{
						tt_2ms = 0;
						shanshuo = ~shanshuo;
					}
					if(shanshuo == 0)
					{
						num4=10;num5=10;
					}
					else
					{
						num4=t_max/10;num5=t_max%10;
					}
					num1 = 12;
					num2=num3=num6=10;
					num7=t_min/10;num8=t_min%10;
				}
				if(t_min > t_max)
				{
					t_hege = 0;model = 1;
					P2 = (P2 & 0x1f) | 0x80;
					P0 = t_led & 0xf7;
					P2 &= 0x1f;
				}
				else
				{
					t_hege = 1;
					P2 = (P2 & 0x1f) | 0x80;
					P0 = t_led | 0x08;
					P2 &= 0x1f;
				}
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