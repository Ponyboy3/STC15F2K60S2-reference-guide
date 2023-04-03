//判断 00H 单元数据是否是 123，若不是则往 00H 单元写入 123，数据管显示此值。

#include <STC15F2K60S2.H>
#include "iic.h"
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
	
uchar code type1[]={ 	0xC0,
										 	0xF9,
										 	0xA4,
										 	0xB0,
										 	0x99,
										 	0x92,
										 	0x82,
										 	0xF8,
										 	0x80,
										 	0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar f_2ms;
uchar t_2ms;
uint temp;



void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}

void smg_display(char n1,char n2,char n3,char n4,char n5,char n6,char n7,char n8)
{
	char i;
	P0=0xff;
	P2=(P2&0x1f)|0xc0;
	P0=type2[i];
	P2=(P2&0x1f);
	switch(i)
	{
		case 0:P2=(P2&0x1f)|0xe0; P0=type1[n1]; P2=(P2&0x1f);break;
		case 1:P2=(P2&0x1f)|0xe0; P0=type1[n2]; P2=(P2&0x1f);break;		
		case 2:P2=(P2&0x1f)|0xe0; P0=type1[n3]; P2=(P2&0x1f);break;		
		case 3:P2=(P2&0x1f)|0xe0; P0=type1[n4]; P2=(P2&0x1f);break;		
		case 4:P2=(P2&0x1f)|0xe0; P0=type1[n5]; P2=(P2&0x1f);break;		
		case 5:P2=(P2&0x1f)|0xe0; P0=type1[n6]; P2=(P2&0x1f);break;		
		case 6:P2=(P2&0x1f)|0xe0; P0=type1[n7]; P2=(P2&0x1f);break;		
		case 7:P2=(P2&0x1f)|0xe0; P0=type1[n8]; P2=(P2&0x1f);break;		
		
	}
	if(++i==8)
		i=0;
}
//////////////////////////////////24c02
void xie_24c02(uchar addr,uchar dat)
{
	IIC_Start( );
	IIC_SendByte( 0xa0+0 );
	IIC_WaitAck( );
	IIC_SendByte( addr );
	IIC_WaitAck( );
	IIC_SendByte( dat );
	IIC_WaitAck( );	
	IIC_Stop( );
}
uchar du_24c02(uchar addr)
{
	uchar D;
	IIC_Start( );
	IIC_SendByte( 0xa0+0 );
	IIC_WaitAck( );
	IIC_SendByte( addr );
	IIC_WaitAck( );

	IIC_Start( );
	IIC_SendByte( 0xa0+1 );
	IIC_WaitAck( );
	D=IIC_RecByte( );
	IIC_SendAck(1);
	IIC_Stop( );
	
	return D;	
}


void time1_int1()
{
	EA=1;
	ET1=1;
	TR1=1;
	TMOD=(TMOD&0x0f)|0x10;
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;		
}

void initial_all()
{
	P2=(P2&0x1f)|0xa0;
	P0=0x00;
	P2=(P2&0x1f);	
	P2=(P2&0x1f)|0x80;
	P0=0xff;
	P2=(P2&0x1f);	
	P2=(P2&0x1f)|0xc0;
	P0=0xff;
	P2=(P2&0x1f);		
	P2=(P2&0x1f)|0xe0;
	P0=0xff;
	P2=(P2&0x1f);		
}

void main()
{
  initial_all();	
	time1_int1();
	while(1)
	{
	 if(f_2ms==1)
	 {
		 f_2ms=0;
	
		 temp=du_24c02(0);
		 if(temp!=226)
		 {
			 xie_24c02(0,226);
			 temp=du_24c02(0);
			 Delay10ms();
		 }
	   num1=temp/100;num2=temp%100/10;num3=temp%10;
		 num4=num5=num6=num7=num8=10;
	
	
	 }//f_2ms
	}
}



void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;		
	
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	f_2ms=1;
	
	
}



















