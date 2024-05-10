#include "reg52.h"
#include "iic.h"

#define uchar unsigned char 
#define uint unsigned int

uchar code type1[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf,0x8e,0xc8,0xc1};
uchar code type2[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uint count;
uchar model;
uchar K;
uchar f_S7;
bit f_2ms;
bit f;
bit f_S5;
bit f_1s;
uint V_ADC;
uint temp_f,temp_t;
uint d_f,d_v;
uint t_2ms;
uchar Led_d;

//==========数码管显示=======
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
		case 5:P2 = (P2 & 0x1f) | 0xe0;
									if(model == 2)
									{
										P0 = type1[n6]&0x7f;
									}
									else
										P0 = type1[n6];
																									P2 &= 0x1f;break;
		case 6:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n7];P2 &= 0x1f;break;
		case 7:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n8];P2 &= 0x1f;break;
	}
	if(++i==8)
		i = 0;
}
//==========独立按键=========
void Delay_Keys(uint t)
{
	while(t--);
}
//uchar Scan_Keys()
//{
//	char d;
//	d = (~P3) & 0x0f;
//	if(d == 0) return 0;
//	Delay_Keys(100);
//	if(d == 0) return 0;
//	Delay_Keys(100);
//	return d;
//}
//void Change_Keys()
//{
//	switch(Scan_Keys())
//	{
//		case 8:GN = 1;break;//S4
//		case 4:GN = 2;break;
//		case 2:GN = 3;break;
//		case 1:GN = 4;break;//S7
//		case 0:GN = 0;break;
//	}
//}
sbit R1 = P3^3;
sbit R2 = P3^2;
sbit C1 = P4^4;
sbit C2 = P4^2;

unsigned char key_num;
void Scan_Keys_2x2()
{
	R1 = 0;                            //扫描第一行
	R2 = 1;
//	C1 = C2 = 1;
	// S4
	if(C1 == 0)
	{
		Delay_Keys(1000);
		if (c1 == 0) {
			
			while(C1 == 0);
			
		}
	}
	// S8
	if(C2 == 0)
	{
		Delay_Keys(1000);
		if (c2 == 0) {
			
			while(C2 == 0);
			
		}
	}
	
	R1 = 1;                            //扫描第一行
	R2 = 0;
//	C1 = C2 = 1;
	// S5
	if(C1 == 0)
	{
		Delay_Keys(1000);
		if (c1 == 0) {
			
			while(C1 == 0);
			
		}
	}
	// S9
	if(C2 == 0)
	{
		Delay_Keys(1000);
		if (c2 == 0) {
			
			while(C2 == 0);
			
		}
	}
}
//==========RB1\RB3读取==========
uchar Read_Rb(uchar m)
{
	uchar d;
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	if(m == 1)
	{
		IIC_SendByte(0x01);
	}
	else if(m == 3)
	{
		IIC_SendByte(0x03);
	}
	IIC_WaitAck();
	IIC_Stop();
	
	Delay_Keys(500);
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	d = IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	return d;
}
//==========led控制==========
void Led_Control()
{
	P0 = 0xff;
	if(f_1s == 1)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = 0xff;
		P2 &= 0x1f;
	}
	else
	{
		if(model == 0)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 & 0xfb;//L3
		P2 &= 0x1f;
	}
	else
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 | 0x04;
		P2 &= 0x1f;
	}
	if(model == 1)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 & 0xf7;//L4
		P2 &= 0x1f;
	}
	else
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 | 0x08;
		P2 &= 0x1f;
	}
	
	if(model == 2)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 & 0xef;//L5
		P2 &= 0x1f;
	}
	else
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 | 0x10;
		P2 &= 0x1f;
	}
	if(temp_f > d_f)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 & 0xfd;//L2
		P2 &= 0x1f;
	}
	else
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 | 0x02;
		P2 &= 0x1f;
	}
	if(V_ADC > d_v)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 & 0xfe;//L1
		Led_d = P0;
		P2 &= 0x1f;
	}
	else
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 | 0x01;
		Led_d = P0;
		P2 &= 0x1f;
	}
	}
}
//==========定时器T1初始化===
void Init_T1()
{
	TMOD = (TMOD & 0xf0) | 0x10;
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;
	
	EA = 1;
	ET1 = 1;
	TR1 = 1;
}
//==========定时器T1初始化===
void Init_T0()
{
	TMOD = (TMOD & 0x0f) | 0x05;
	TH0 = 0;
	TL0 = 0;
}
//==========系统初始化=====
void Init_Sys()
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;
	P2 &= 0x1f;
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0xff;
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
	Init_T0();
	while(1)
	{
		if(f_2ms == 1)
		{
			f_2ms = 0;
			Change_Keys();
			if(GN == 1)
			{
				f_S5 = 0;
				model++;
				if(model == 3)
				{
					model = 0;
				}
				while(Scan_Keys());
			}
			if(GN == 3)
			{
				V_ADC = Read_Rb(3);
				if(V_ADC > 250)
				{
					V_ADC = 250;
				}
				V_ADC = 2*V_ADC;
				d_v = V_ADC;
				while(Scan_Keys());
			}
			if(GN == 4)
			{
				d_f = temp_f;
				count = 0;
				f_S7 = 1;
				while(Scan_Keys());
				f_S7 = 0;
			}
			
			if(GN == 4)
			{
				d_f = temp_f;
				count = 0;
				f_S7 = 1;
				while(Scan_Keys());
				f_S7 = 0;
			}
			
			switch(model)
			{
				case 0:
					num1 = 12;
					if(temp_f > 9999)
					{
						num4=temp_f/10000;num5=temp_f/1000%10;num6=temp_f/100%10;num7=temp_f/10%10;num8=temp_f%10;
					}
					else if(temp_f > 999)
					{
						num4=10;num5=temp_f/1000;num6=temp_f/100%10;num7=temp_f/10%10;num8=temp_f%10;
					}
					else if(temp_f > 99)
					{
						num4=10;num5=10;num6=temp_f/100;num7=temp_f/10%10;num8=temp_f%10;
					}
					else if(temp_f > 9)
					{
						num4=10;num5=10;num6=10;num7=temp_f/10;num8=temp_f%10;
					}
					else
					{
						num4=10;num5=10;num6=10;num7=10;num8=temp_f;
					}
					num2=num3=10;
				break;
				
				case 1:
					temp_t = 1000000/temp_f;
					if(temp_t > 9999)
					{
						num4=temp_t/10000;num5=temp_t/1000%10;num6=temp_t/100%10;num7=temp_t/10%10;num8=temp_t%10;
					}
					else if(temp_t > 999)
					{
						num4=10;num5=temp_t/1000;num6=temp_t/100%10;num7=temp_t/10%10;num8=temp_t%10;
					}
					else if(temp_t > 99)
					{
						num4=10;num5=10;num6=temp_t/100;num7=temp_t/10%10;num8=temp_t%10;
					}
					else if(temp_t > 9)
					{
						num4=10;num5=10;num6=10;num7=temp_t/10;num8=temp_t%10;
					}
					else
					{
						num4=10;num5=10;num6=10;num7=10;num8=temp_t;
					}
					num1 = 13;
					num2=num3=10;
				break;
				
				case 2:
					if(GN == 2)
					{
						f_S5 = ~f_S5;
						while(Scan_Keys());
					}
					if(f_S5 == 0)
					{
						V_ADC = Read_Rb(1)*2;
						if(V_ADC > 250)
						{
							V_ADC = 250;
						}
						V_ADC = 2*V_ADC;
						num3 = 1;
					}
					else
					{
						V_ADC = Read_Rb(3);
						if(V_ADC > 250)
						{
							V_ADC = 250;
						}
						V_ADC = 2*V_ADC;
						num3 = 3;
					}
					num1 = 14;
					num2 = 11;
					num6=V_ADC/100;num7=V_ADC/10%10;num8=V_ADC%10;
					num4=num5=10;
				break;
			}
//			Led_Control();
		}
	}
}
void Service_T1() interrupt 3
{
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;
	
	f_2ms = 1;
	Display_SMG(num1,num2,num3,num4,num5,num6,num7,num8);
	
	if(f_S7 == 1)
	{
		count++;
		if(count == 500)
		{
			f_1s = ~f_1s;
		}
	}
	
	if(++t_2ms == 250)
	{
		t_2ms = 0;
		f=~f;
		if(f == 0)
		{
			TH0 = 0;
			TL0 = 0;
			TR0 = 1;
		}
		else
		{
			TR0 = 0;
			temp_f=((TH0<<8)+TL0)*2;
		}
	}
}