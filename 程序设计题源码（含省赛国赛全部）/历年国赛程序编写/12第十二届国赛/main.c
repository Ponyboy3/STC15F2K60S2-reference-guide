#include <STC15F2K60S2.H>
#include "ds1302.h"  
#include "iic.h"
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
	
uchar code type1[]={0xc0,
										0xf9,
										0xa4,
										0xb0, 
										0x99, 
										0x92, 
										0x82, 
										0xf8, 
										0x80, 
										0x90,0xff };
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};	
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar t_2ms;
bit f_2ms;
sbit trig=P1^0;
sbit echo=P1^1;
uchar hour,second,minute;
uchar bdata U6=0xff;
sbit led1=U6^0;
sbit led2=U6^1;
sbit led3=U6^2;
sbit led4=U6^3;
sbit led5=U6^4;
sbit led6=U6^5;
//---------------------------







void Delay10ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 117;
	j = 184;
	do
	{
		while (--j);
	} while (--i);
}

void smg_display(char n1,char n2,char n3,char n4,char n5,char n6,char n7,char n8)
{
	char i;
	P2=(P2&0x1f)|0xc0;
	P0=type2[i];
	P2=(P2&0x1f);
	P0=0xff;
	switch(i)
	{
	  case 0:P2=(P2&0x1f)|0xe0;P0=type1[n1];P2=(P2&0x1f);break;
	  case 1:P2=(P2&0x1f)|0xe0;P0=type1[n2];P2=(P2&0x1f);break;
		case 2:P2=(P2&0x1f)|0xe0;P0=type1[n3];P2=(P2&0x1f);break;
		case 3:P2=(P2&0x1f)|0xe0;P0=type1[n4];P2=(P2&0x1f);break;
		case 4:P2=(P2&0x1f)|0xe0;P0=type1[n5];P2=(P2&0x1f);break;
		case 5:P2=(P2&0x1f)|0xe0;P0=type1[n6];P2=(P2&0x1f);break;
		case 6:P2=(P2&0x1f)|0xe0;P0=type1[n7];P2=(P2&0x1f);break;
		case 7:P2=(P2&0x1f)|0xe0;P0=type1[n8];P2=(P2&0x1f);break;
	}
	if(++i==8)
		i=0;
}
//====================================key
uchar key_scan()
{
	uchar H,L;
	P3=0xdf;P44=0;P42=0;H=(~P3)&0x0f;
	if(H==0) return 0;
	Delay10ms();
	P3=0x0f;P44=0;P42=0;H=(~P3)&0x0f;
	if(H==0) return 0;	
	P3=0xf0;P44=1;P42=1;L=(~P3)&0x20;
	if(P44==0) L|=0x80;
	if(P42==0) L|=0x40;
	return H+L;
}
void key_change()
{
	uchar D;
	D=key_scan();
	switch(D)
	{
	  case 0x88:GN=4;break;
		case 0x84:GN=5;break;
		case 0x48:GN=8;break;
		case 0x44:GN=9;break;
		case 0:GN=0;break;	
	}
}
//======================================³¬Éù²¨
uchar f_count=10;
uchar csb_send()
{
	f_count=10;
	trig=0;
	TR0=1;
	while(f_count--)
	{
		while(!TF0);
		trig^=1;
		TH0=(65535-12)/256;
		TL0=(65535-12)%256;	
		TF0=0;		
	}
	TR0=0;
	TH0=0;
	TL0=0;
	TR0=1;
	while(echo && !TF0);
	TR0=0;
	
	if(TF0)
	{
		TF0=0;
		return 99;		
	}
	else
		return ((TH0<<8)+TL0)*0.017;
}
//==================================1302
void xie_1302()
{
	Write_Ds1302_Byte( 0x8e,0x00 );
	Write_Ds1302_Byte( 0x84,0x23 );
	Write_Ds1302_Byte( 0x82,0x59 );
	Write_Ds1302_Byte( 0x80,0x50 );	
}
void du_1302()
{
	hour=Read_Ds1302_Byte( 0x85 );   hour=(hour>>4)*10+(hour&0x0f);
	minute=Read_Ds1302_Byte( 0x83 ); minute=(minute>>4)*10+(minute&0x0f);
	second=Read_Ds1302_Byte( 0x81 ); second=(second>>4)*10+(second&0x0f);
}
//====================================adc
void initial_adc()
{
	IIC_Start( );
	IIC_SendByte(0x90+0);
	IIC_WaitAck( );
	IIC_SendByte(0x1);
	IIC_WaitAck( );	
	IIC_Stop( ); 	
}
uchar du_adc()
{
	uchar D;
	IIC_Start( );
	IIC_SendByte(0x90+1);
	IIC_WaitAck( );	
	D=IIC_RecByte( ); 
	IIC_SendAck(1);
	IIC_Stop( ); 
	
	return D;	
}
//==================================dac
void xie_dac(uchar D)
{
	IIC_Start( );
	IIC_SendByte(0x90+0);
	IIC_WaitAck( );
	IIC_SendByte(0x40);
	IIC_WaitAck( );	
	IIC_SendByte(D);
	IIC_WaitAck( );		
	IIC_Stop( ); 		
}

void time0()
{
	TMOD=(TMOD&0xf0)|0x01;
	TH0=(65535-12)/256;
	TL0=(65535-12)%256;			
}

void time1_int1()
{
	EA=1;
	ET1=1;
	TR1=1;
	TMOD=(TMOD&0x0f)|0x10;
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
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
	time0();
	xie_1302();
	initial_adc();
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;
	  key_change();
	  du_1302();
	
	  num1=hour/10; num2=hour%10;
		 num3=minute/10; num4=minute%10;
		 num5=second/10; num6=second%10;
		 num7=du_adc()/10; num8=du_adc()%10;
	
	 }//f_2ms
	}
}
void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;		
	smg_display( num1,num2,num3,num4,num5,num6,num7,num8);
	f_2ms=1;
}







