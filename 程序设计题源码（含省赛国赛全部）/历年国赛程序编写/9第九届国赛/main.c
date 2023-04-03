#include <STC15F2K60S2.H>
#include "iic.h"
#include "onewire.h"
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
										 	0x90,0xff,0x7f,0xc1,0x8e,0xc6};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

uchar num1,num2,num3,num4,num5,num6,num7,num8;
bit f_2ms;
uchar t_2ms;
uchar GN;
bit flag;
uchar t_u,t_c;

uchar bdata U6=0xff;
sbit led1=U6^0;
sbit led2=U6^1;
sbit led3=U6^2;
sbit led8=U6^7;

uchar temp_u;
uint temp_f;
uint temp_c;
//==============================
uchar set_shu,set_hui;
uchar display=0;
//==============================
bit f_save=0;
//==============================
bit jiemian=0;
bit lim_eep=0;
uchar temp_uu;
uint temp_ff;
uint temp_cc;
//==============================
char f_canshu=0;
uchar yuzhi=1;
//==============================
uint t_800ms;
bit f_800ms,x_800ms;
uchar t_time;
char a;
//==============================
uchar t_200ms;

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
//===============================key
uchar key_scan()
{
	uchar temp;
	temp=(~P3)&0x0f;
	if(temp==0) return 0;
	Delay10ms();
	temp=(~P3)&0x0f;
	if(temp==0) return 0;	
	return temp;

}
void key_change()
{
	uchar D;
	D=key_scan();
	switch(D)
	{
		case 8:GN=4;break;
		case 4:GN=5;break;
		case 2:GN=6;break;
		case 1:GN=7;break;
		case 0:GN=0;break;
	}
}
//==============================adc
void initial_adc()
{
	IIC_Start( );
	IIC_SendByte(0x90+0);
	IIC_WaitAck( );
	IIC_SendByte(0x3);
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
	D=(D*50.0)/255;
	return D;
}
//================================24c02
void xie_24c02(uchar addr,uchar dat)
{
	IIC_Start( );
	IIC_SendByte(0xa0+0);
	IIC_WaitAck( );
	IIC_SendByte(addr);
	IIC_WaitAck( );	
	IIC_SendByte(dat);
	IIC_WaitAck( );		
	IIC_Stop( ); 	
}
uchar du_24c02(uchar addr)
{
	uchar D;
	IIC_Start( );
	IIC_SendByte(0xa0+0);
	IIC_WaitAck( );
	IIC_SendByte(addr);
	IIC_WaitAck( );		
	
	IIC_Start( );
	IIC_SendByte(0xa0+1);
	IIC_WaitAck( );	
	D=IIC_RecByte( );
	IIC_SendAck(1);
	IIC_Stop( );
	return D;
	
}

//===================================频率处理
void f_chuli(uint dat)
{
	if(dat<10)
	{
		num3=num4=num5=num6=num7=10;
		num8=dat;		
	}
	if(dat>9 && dat<100)
	{
		num3=num4=num5=num6=10;
		num7=dat/10;
		num8=dat%10;		
	}	
	if(dat>99 && dat<1000)
	{
		num3=num4=num5=10;
		num6=dat/100;
		num7=dat%100/10;
		num8=dat%10;		
	}		
	if(dat>999 && dat<10000)
	{
		num3=num4=10;
		num5=dat/1000;
		num6=dat%1000/100;
		num7=dat%100/10;
		num8=dat%10;		
	}	
	if(dat>9999)
	{
		num3=10;
		num4=dat/10000;
		num5=dat%1000/1000;
		num6=dat%1000/100;
		num7=dat%100/10;
		num8=dat%10;		
	}		
}



void time0()
{
	TMOD=(TMOD&0xf0)|0x05;
	TH0=TL0=0;
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
	time0();
	initial_adc();
	
	initial_18b20();
//--------------------
	 temp_uu=du_24c02(0);
	 temp_ff=du_24c02(1)*256 + du_24c02(2);
	 temp_cc=du_24c02(5)*256 + du_24c02(6);
	
	while(1)
	{
	 if(f_2ms==1)
	 {
		 f_2ms=0;
	   key_change();
		 if(++t_u==50)
		 {
			 t_u=0;
			 temp_u=du_adc();
		 }
		 if(++t_c==100)
		 {
			 t_c=0;
			 temp_c=rd_temperature( );
		 }		 
//===============================		 
	   if(display==0)
		 {
			 if(GN==4)
			 {
				 set_shu++;
				 if(set_shu==3)
					 set_shu=0;				 
				 while(key_scan());
			 }
		  switch(set_shu)
			{
			  case 0:
					   num1=12;
				     num2=num3=num4=num5=num6=10;
				     num7=temp_u/10;
				     num8=temp_u%10;
				      P0=0xff;
				      led3=0;led1=1;led2=1;
							P2=(P2&0x1f)|0x80;
							P0=U6;
							P2=(P2&0x1f);	
				  break;
			  case 1:
					   num1=13;
				     num2=10;
             f_chuli(temp_f);	
				      P0=0xff;
				      led2=0;led1=1;led3=1;
							P2=(P2&0x1f)|0x80;
							P0=U6;
							P2=(P2&0x1f);				
				  break;			 
			  case 2:
					   num1=14;
				     num2=num3=num4=10;
				     num5=temp_c/1000;
				     num6=temp_c%1000/100;				
				     num7=temp_c%100/10;
				     num8=temp_c%10;
				      P0=0xff;
				      led1=0;led3=1;led2=1;
							P2=(P2&0x1f)|0x80;
							P0=U6;
							P2=(P2&0x1f);					
				  break;			 
			}
		 }
//=======================================	
   if(GN==5 && display==0)
	 {
     f_save=1; 
     while(key_scan());
	 }		 
	 if(f_save==1)
	 {
     xie_24c02(0,temp_u);
     Delay10ms();
     //==
		 xie_24c02(1,temp_f/256);
     Delay10ms();
		 xie_24c02(2,temp_f%256);
     Delay10ms();
     //==
		 xie_24c02(5,temp_c/256);
     Delay10ms();
		 xie_24c02(6,temp_c%256);
     Delay10ms();		 
		 
		 f_save=0;
	 }		 
//=========================================			 
		 if(GN==6 && display!=3) 
		 { 			 
			 jiemian=~jiemian; 
			 while(key_scan());
		 //----------------	数据处理
       if(jiemian==0)
			 {
				 display=0;
				 lim_eep=0;        //为下次读取24c02做准备
			 }
			 if(jiemian==1)
				 display=1;
     //----------------				 
		 }
			 
		 if(display==1)
		 {
			 if(lim_eep==0)
			 {
         temp_uu=du_24c02(0);
         temp_ff=du_24c02(1)*256 + du_24c02(2);
         temp_cc=du_24c02(5)*256 + du_24c02(6);
				 
				 lim_eep=1;
			 }			 
			 if(GN==4)
			 {
				 set_hui++;
				 if(set_hui==3)
					 set_hui=0;				 
				 while(key_scan());
			 }
			 
		  switch(set_hui)
			{
			  case 0:
					   num1=12;
				     num2=num3=num4=num5=num6=10;
				     num7=temp_uu/10;
				     num8=temp_uu%10;
				  break;
			  case 1:
					   num1=13;
				     num2=10;
             f_chuli(temp_ff);				
				  break;			 
			  case 2:
					   num1=14;
				     num2=num3=num4=10;
				     num5=temp_cc/1000;
				     num6=temp_cc%1000/100;				
				     num7=temp_cc%100/10;
				     num8=temp_cc%10;
				  break;			 
			}			  
		 }
//===================================		 
		 if(GN==7)
		 {
			 f_canshu++;
			 if(f_canshu==3)
				 f_canshu=1;
			 while(key_scan()); 
		 }
		 //---------------
     if(f_canshu==1)
		 {
			 display=3;		 
		 }
     if(f_canshu==2)
		 {
			 display=0;	
		 }		 
		 //---------------
		 if(display==3)
		 {			 
			 if(GN==6)
			 {				 
				 if(yuzhi==50)
					 yuzhi=1;
				 else
					  yuzhi++;	
				 
         f_800ms=1;
				while(key_scan());
				 f_800ms=0;
				 t_800ms=0;
				 t_time=0;
				 x_800ms=0;
			 }
			 num1=12;
			 num2=num3=num4=num5=num6=10;
			 num7=yuzhi/10;
			 num8=yuzhi%10; 
		 }
		 
		 
		 if(temp_u > yuzhi)
		 {
			 if(++t_200ms==100)
			 {
				 t_200ms=0;
				 led8=~led8;
			 }
        P0=0xff;
				P2=(P2&0x1f)|0x80;
				P0=U6;
				P2=(P2&0x1f);				  
		 }
     else
		 {
        P0=0xff;
			  led8=1;
				P2=(P2&0x1f)|0x80;
				P0=U6;
				P2=(P2&0x1f);					 
		 }
	 }//f_2ms
	}
}



void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;		
	
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	f_2ms=1;
	
	if(++t_2ms==250)
	{
		t_2ms=0;
		flag=~flag;
		if(flag)
		{
			TH0=0;
			TL0=0;			
			TR0=1;
		}
		else
		{
		 TR0=0;
		 temp_f=((TH0<<8)+TL0)*2;
		}
	}
	
	if(f_800ms==1)
	{
		if(++t_800ms==400)
		{
      //t_800ms=0;
			x_800ms	= 1;	
		}	
		if(x_800ms==1)
		{
			if(++t_time==100)
			{
				t_time=0;
				yuzhi++;
				if(yuzhi==50)
					yuzhi=1;
			}
			 num1=12;
			 num2=num3=num4=num5=num6=10;
			 num7=yuzhi/10;
			 num8=yuzhi%10;			
		}
	}
	
	
}












