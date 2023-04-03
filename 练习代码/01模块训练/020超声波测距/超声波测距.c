#include "reg52.h"
#include "intrins.h"

sbit TX = P1^0;
sbit RX = P1^1;

unsigned char code SMG_NUM[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

unsigned int d = 0;

//==========数码管显示函数=======
void Delay(unsigned int t)
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
	if(d == 999)
	{
		Display_SMG_Bit(0,0x8e);
		Delay(500);
	}
	else
	{
		
		
		Display_SMG_Bit(5,SMG_NUM[d / 100]);
		Delay(500);
		Display_SMG_Bit(6,SMG_NUM[(d % 100) / 10]);
		Delay(500);
		Display_SMG_Bit(7,SMG_NUM[d % 10]);
		Delay(500);
	}
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0xff;
	P2 &= 0x1f;
	
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0xff;
	P2 &= 0x1f;
	Delay(500);
}

//===========超声波函数==========
void Delay_12us()
{
	unsigned char i;
	_nop_();
	_nop_();
	
	i = 33;
	while(--i);
}

void Send_W()
{
	unsigned char i;
	for(i = 0;i < 8;i++)
	{
		TX = 1;
		Delay_12us();
		TX = 0;
		Delay_12us();
	}
}

void Get_Distance()
{
	unsigned int t;
	TMOD &= 0x0f;	            //定时器1模式0，13位，最大8192个计数脉冲								
	TL1 = 0x00;										
	TH1 = 0x00;	
	
	Send_W();
	TR1 = 1;            //启动定时器						
	while((RX == 1) && (TF1 == 0));    //等待超声波信号返回或者等到测量超出范围
	TR1 = 0;            //停止定时器				
	
	if(TF1 == 0)	            //正常测量范围							
	{
		t = TH1;									
		t = (t << 8) | TL1;		
		d = ((t / 10) * 17) / 100 + 3;
	}
	else                        //超出测量范围			
	{
		TF1 = 0;
		d = 999;
	}
	
}

//==========系统初始化===========
void Init_Sys()
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;
	P2 &= 0x1f;
	
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x00;
	P2 &= 0x1f;
}


void Delay1(unsigned char n)        //数码管显示增强
{
	while(n--)
	{
		Display_SMG();
	}
}

void main()
{
	Init_Sys();
	while(1)
	{
		Get_Distance();
		Delay1(20);
	}
}