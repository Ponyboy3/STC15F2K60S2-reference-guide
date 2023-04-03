#include "reg52.h"

sfr AUXR = 0x8e;

sbit S4 = P3^3;
sbit S5 = P3^2;

unsigned char code SMG_NUM[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};


unsigned char t_h = 0;
unsigned char t_m = 0;
unsigned char t_s = 0;
unsigned char count = 0;
unsigned char command = 0;

unsigned int stat_led = 0xff;

void Delay(unsigned int t)
{
	while(t--);
	while(t--);
}

void SelectHC573(unsigned char n,unsigned int dat)
{
	P0 = dat;
	switch(n)
	{
		case 4:
			P2 = (P2 & 0x1f) | 0x80;
		break;
		case 5:
			P2 = (P2 & 0x1f) | 0xa0;
		break;
		case 6:
			P2 = (P2 & 0x1f) | 0xc0;
		break;
		case 7:
			P2 = (P2 & 0x1f) | 0xe0;
		break;
		case 0:
			P2 = (P2 & 0x1f) | 0x00;
		break;
	}
	P2 = (P2 & 0x1f) | 0x00; //关闭译码器
}

//led检测函数
void Check_LED()
{
	unsigned char i = 0;
	for(i = 0;i < 8;i++)
	{
		stat_led = 0xfe << i;
		SelectHC573(4,stat_led);
		Delay(60000);
		Delay(60000);
	}
	for(i = 0;i < 8;i++)
	{
		stat_led = ~(0xfe << i);
		SelectHC573(4,stat_led);
		Delay(60000);
		Delay(60000);
	}
}

//数码管检测函数
void Check_SMG()
{
	unsigned char i = 0;
	for(i = 0;i < 8;i++)
	{
		SelectHC573(6,0x01 << i);
		SelectHC573(7,0x00);
		Delay(60000);
		Delay(60000);
	}
	SelectHC573(7,0xff);
}

void Init_Sys()
{
	SelectHC573(0,0x00);
	SelectHC573(5,0x00);
	SelectHC573(4,stat_led);
	SelectHC573(6,0xff);
	SelectHC573(7,0xff);
}

//定时器0初始化
void Init_Timer0()
{
	TMOD = 0x21; //T0,T1同时工作
	TH0 = (65535 - 50000) / 256;
	TL0 = (65535 - 50000) % 256;
	
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}

//串口初始化
void Init_Uart()
{
	TMOD = 0x21; //T0,T1同时工作
	TH1 = 0xfd;
	TL1 = 0xfd;
	TR1 = 1;
	
	SCON = 0x50;
	AUXR = 0x00;
	
	EA = 1;
	ES = 1;
}

void ServiceTimer0() interrupt 1
{
	TH0 = (65535 - 50000) / 256;
	TL0 = (65535 - 50000) % 256;
	
	count++;
	if(count == 20)
	{
		count = 0;
		t_s++;
	}
	if(t_s == 60)
	{
		t_s = 0;
		t_m++;
		if(t_m == 60)
		{
			t_m = 0;
			t_h++;
		}
	}
}


//串口发送函数
void SendByte(unsigned char dat)
{
	SBUF = dat;
	while(TI == 0);
	TI = 0;
}

//命令解析函数
void Executecommand()
{
	if(command != 0x00)
	{
		switch(command & 0x0f)
		{
			case 0xa0:
				stat_led = (stat_led | 0x0f) & (~command | 0xf0);
				SelectHC573(4,stat_led);
				command = 0x00;
			break;
			case 0xb0:
				SendByte((t_h / 10 << 4) | (t_h % 10));
				SendByte((t_m / 10 << 4) | (t_m % 10));
				SendByte((t_s / 10 << 4) | (t_s % 10));
				command = 0x00;
			break;
		}
	}
}

void ServiceUart() interrupt 4
{
	if(RI == 1)
	{
		command = SBUF;
		RI = 0;
	}
}

void DisplaySMG(unsigned char pos,unsigned char value)
{
	SelectHC573(6,0x01 << pos);
	SelectHC573(7,value);
}

void DelayS(unsigned int t)
{
	while(t--);
}

void DisplayTime()
{
	DisplaySMG(0,SMG_NUM[t_h / 10]);
	DelayS(500);
	DisplaySMG(1,SMG_NUM[t_h % 10]);
	DelayS(500);
	DisplaySMG(2,0xbf);
	DelayS(500);
	DisplaySMG(3,SMG_NUM[t_m / 10]);
	DelayS(500);
	DisplaySMG(4,SMG_NUM[t_m % 10]);
	DelayS(500);
	DisplaySMG(5,0xbf);
	DelayS(500);
	DisplaySMG(6,SMG_NUM[t_s / 10]);
	DelayS(500);
	DisplaySMG(7,SMG_NUM[t_s % 10]);
	DelayS(500);
}

//按键扫描
void ScanKeys()
{
	if(S4 == 0)
	{
		DisplayTime();
		if(S4 == 0)
		{
			while(S4 == 0)
			{
				DisplayTime();
			}
			stat_led = (stat_led | 0x80) & (~stat_led | 0x7f);
			SelectHC573(4,stat_led);
		}
	}
	
	if(S5 == 0)
	{
		DisplayTime();
		if(S5 == 0)
		{
			while(S5 == 0)
			{
				DisplayTime();
			}
			stat_led = (stat_led | 0x40) & (~stat_led | 0xbf);
			SelectHC573(4,stat_led);
		}
	}
}


void main()
{
	Init_Sys();
	Check_LED();
	Check_SMG();
	Init_Timer0();
	Init_Uart();
	while(1)
	{
		Executecommand();
		DisplayTime();
		ScanKeys();
	}
}