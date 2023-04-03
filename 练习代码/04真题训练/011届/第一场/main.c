#include "reg52.h"
#include "iic.h"

sfr P4 = 0xC0;

sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;

sbit P44 = P4^4;
sbit P42 = P4^2;

#define uchar unsigned char
#define uint unsigned int

uchar code type1[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xc1,0x8c,0x89};
uchar code type2[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

uchar num1,num2,num3,num4,num5,num6,num7,num8;
uint V_Rb2,V_AIN3,V_Set = 300;
uchar GN;
uchar model;
uchar Count;
bit f_2ms;
bit t_led;
uint t_count;
uchar k_count;
uchar P_temp;

void Display_SMG(uchar n1,uchar n2,uchar n3,uchar n4,uchar n5,uchar n6,uchar n7,uchar n8)
{
	char i;
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = type2[i];
	P2 &= 0x1f;
	switch(i)
	{
		case 0:P2=(P2&0x1f)|0xe0;P0=type1[n1];P2&=0x1f;break;
		case 1:P2=(P2&0x1f)|0xe0;P0=type1[n2];P2&=0x1f;break;
		case 2:P2=(P2&0x1f)|0xe0;P0=type1[n3];P2&=0x1f;break;
		case 3:P2=(P2&0x1f)|0xe0;P0=type1[n4];P2&=0x1f;break;
		case 4:P2=(P2&0x1f)|0xe0;P0=type1[n5];P2&=0x1f;break;
		case 5:P2=(P2&0x1f)|0xe0;
					 if(model==0||model==1)
							P0=type1[n6]&0x7f;
					 else
							P0=type1[n6];							 
					                                P2&=0x1f;break;
		case 6:P2=(P2&0x1f)|0xe0;P0=type1[n7];P2&=0x1f;break;
		case 7:P2=(P2&0x1f)|0xe0;P0=type1[n8];P2&=0x1f;break;
	}
	P0=0xff;
	if(++i==8)
		i = 0;
}
void DelaY_Keys(uint t)
{
	while(t--);
}
unsigned char Scan_Keys()
{
	uchar H,L;
	P3=0xcf;P44=0;P42=0;H=(~P3)&0x0f;//0xcf 1100 1111
	if(H==0) return 0;
	DelaY_Keys(100);
	P3=0xcf;P44=0;P42=0;H=(~P3)&0x0f;
	if(H==0) return 0;
	P3=0xf0;P44=1;P42=1;L=(~P3)&0x30;//0xcf 1111 0000
	if(P44==0) L|=0x80;
	if(P42==0) L|=0x40;
	return (H+L);
}
void Keys_Change()
{
	switch(Scan_Keys())
	{
		case 0x28:GN = 12;break;
		case 0x24:GN = 13;break;
		case 0x18:GN = 16;break;
		case 0x14:GN = 17;break;
		case 0x12:
		case 0x11:
		case 0x22:
		case 0x21:
		case 0x41:
		case 0x42:
		case 0x44:
		case 0x48:
		case 0x81:
		case 0x82:
		case 0x84:
		case 0x88:GN = 1;break;
		case 0:GN = 0;break;
	}
}
//===============RB2部分========
void Init_Rb2()
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x03);
	IIC_WaitAck();
	IIC_Stop();
}
unsigned char Read_Rb2()
{
	uchar d;
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	d = IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	return d;
}
//===============24C02部分=======
void Xie_24C02(uchar addr,uchar dat)
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
uchar Du_24C02(uchar addr)
{
	uchar d;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	d = IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	return d;
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
	Init_Rb2();
	V_Set = Du_24C02(0)*10;
	while(1)
	{
		if(f_2ms == 1)
		{
			f_2ms = 0;
			Keys_Change();
			if(Read_Rb2() > 125)
			{
				DelaY_Keys(500);
				if(Read_Rb2() < 125)
					Count++;
			}
			if(GN == 1)
			{
				k_count++;
				while(Scan_Keys());
			}
			if(GN == 12)//S12按下
			{
				k_count = 0;//关闭L3
				model++;
				if(model == 3)
					model = 0;
				while(Scan_Keys());
			}
			switch(model)
			{
				case 0://数据界面
					V_AIN3 = Read_Rb2();
					if(V_AIN3 > 250)
						V_Rb2 = 500;
					else
						V_Rb2 = V_AIN3*2;
					num6=V_Rb2/100;num7=V_Rb2/10%10;num8=V_Rb2%10;
					num1=11;num2=num3=num4=num5=10;
				break;
				
				case 1://参数界面
					if(GN == 16)
					{
						k_count = 0;//关闭L3
						if(V_Set >= 500)
							V_Set = 0;
						else
							V_Set += 50;
						Xie_24C02(0,V_Set/10);
						while(Scan_Keys());
					}
					if(GN == 17)
					{
						k_count = 0;//关闭L3
						if(V_Set <= 0)
							V_Set = 500;
						else
							V_Set -= 50;
						Xie_24C02(0,V_Set/10);
						while(Scan_Keys());
					}
					num6=V_Set/100;num7=V_Set/10%10;num8=V_Set%10;
					num1=12;num2=num3=num4=num5=10;
				break;
				
				case 2://计数界面
					if(GN == 13)
					{
						k_count = 0;//关闭L3
						Count = 0;
						while(Scan_Keys());
					}
					if(Count > 99)
					{
						num6=Count/100;num7=Count/10%10;num8=Count%10;
					}
					else if(Count >9)
					{
						num6=10;num7=Count/10;num8=Count%10;
					}
					else
					{
						num6=10;num7=10;num8=Count;
					}
					num1=13;num2=num3=num4=num5=10;
				break;
			}
			if(V_Rb2 < V_Set)
			{
				t_led = 1;
			}
			else
			{
				t_led = 0;
			}
			
//			if(Count%2 != 0)
//			{
//				P2 = (P2 & 0x1f) | 0x80;
//				P0 = 0xfd;
//				P2 &= 0x1f;
//			}
//			else
//			{
//				P2 = (P2 & 0x1f) | 0x80;
//				P0 = 0xff;
//				P2 &= 0x1f;
//			}
		}
	}
}
void Service_T1() interrupt 3
{
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;
	
	f_2ms = 1;
	Display_SMG(num1,num2,num3,num4,num5,num6,num7,num8);
	if(t_led == 1)
	{
		t_count++;
		if(t_count==2500)
		{
			t_count = 0;
			P0 = 0xff;
			P2 = (P2 & 0x1f) | 0x80;
			P0 = P0 & 0xfe;
			P_temp = P0;
			P2 &= 0x1f;
		}
		if(k_count >= 3)
		{
			P2 = (P2 & 0x1f) | 0x80;
			P0 = P_temp & 0xfb;
			P_temp = P0;
			P2 &= 0x1f;
		}
		else
		{
			P2 = (P2 & 0x1f) | 0x80;
			P0 = P_temp | 0x04;
			P_temp = P0;
			P2 &= 0x1f;
		}
		if(Count > 0 && Count%2 != 0)
		{
			P2 = (P2 & 0x1f) | 0x80;
			P0 = P_temp & 0xfd;
			P_temp = P0;
			P2 &= 0x1f;
		}
		else
		{
			P2 = (P2 & 0x1f) | 0x80;
			P0 = P_temp | 0x02;
			P_temp = P0;
			P2 &= 0x1f;
		}
	}
	else
	{
		t_count = 0;
		P0 = 0xff;
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 | 0x01;
		P_temp = P0;
		P2 &= 0x1f;
		if(k_count >= 3)
		{
			P2 = (P2 & 0x1f) | 0x80;
			P0 = P_temp & 0xfb;
			P_temp = P0;
			P2 &= 0x1f;
		}
		else
		{
			P2 = (P2 & 0x1f) | 0x80;
			P0 = P_temp | 0x04;
			P_temp = P0;
			P2 &= 0x1f;
		}
		if(Count > 0 && Count%2 != 0)
		{
			P2 = (P2 & 0x1f) | 0x80;
			P0 = P_temp & 0xfd;
			P_temp = P0;
			P2 &= 0x1f;
		}
		else
		{
			P2 = (P2 & 0x1f) | 0x80;
			P0 = P_temp | 0x02;
			P_temp = P0;
			P2 &= 0x1f;
		}
	}
}