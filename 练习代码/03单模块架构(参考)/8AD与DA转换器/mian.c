/****************************************
读电位器的电压值用数码管显示
*********************************************/
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
uint t_2ms,tt_2ms;

uint temp;
uchar vary=255;

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
///////////////////////////pcf8591
void initial_adc(uchar addr)
{
  IIC_Start( ); 	
	IIC_SendByte( 0x90+0 );
	IIC_WaitAck( );
	IIC_SendByte( addr );
	IIC_WaitAck( );
	IIC_Stop( );
}


uint du_adc()
{
	uint D;
	IIC_Start( ); 	
	IIC_SendByte( 0x90+1 );
	IIC_WaitAck( );
	D=IIC_RecByte( );
	IIC_SendAck(1);
	IIC_Stop( );
	D=(D*500.0)/255;
	return D;
}

void xie_dac(uchar dat)
{
	IIC_Start( ); 
	IIC_SendByte( 0x90+0 );
	IIC_WaitAck( );
	IIC_SendByte( 0x40 );
	IIC_WaitAck( );	
	IIC_SendByte( dat );
	IIC_WaitAck( );	
	IIC_Stop( );
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
	initial_adc(0x0);
	while(1)
	{
	 if(f_2ms==1)
	 {
		 f_2ms=0;
		 
		 
	if(++tt_2ms==500)
	{
		tt_2ms=0;

		xie_dac(vary--);
	}		 
	
//	   if(++t_2ms==100)
//		 {
//			 t_2ms=0;
//			 initial_adc(0x0);
			 temp=du_adc();
//			 
//		 }
		 
	   num1=temp/100;num2=temp%100/10;num3=temp%10;
		 num4=num5=num6=num7=num8=10;
	
	
	 }
	}
}



void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;		
	
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	f_2ms=1;
	
	

	
	
}











