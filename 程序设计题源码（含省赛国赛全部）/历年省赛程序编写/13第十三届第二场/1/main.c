#include <STC15F2K60S2.H>
#include "iic.h"
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int 

uchar type1[]={ 	0xC0,
								 	0xF9,
								 	0xA4,
								 	0xB0,
								 	0x99,
								 	0x92,
								 	0x82,
								 	0xF8,
								 	0x80,
								 	0x90,0xff,0xc1,0x8c,0xc7,0x88};
uchar type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar t_2ms,tt_2ms;
bit f_100ms;
bit f_2ms;
uchar GN;

sbit trig=P1^0;
sbit echo=P1^1;

uchar bdata U6=0xff;
sbit led1=U6^0;
sbit led2=U6^1;
sbit led3=U6^2;
sbit led8=U6^7;

uchar model=0;
uint volt;
uint H_lim,L_lim,H=45,L=5;
bit f_HL;
bit start_csb=0;
uchar juli;


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
//==============================key
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
//===================================csb
uchar num;
uchar csb_send()
{
	num=10;
	trig=0;
	TR0=1;
	while(num--)
	{
		while(!TF0);
		TH0=0xff;
		TL0=0xf4;
    TF0=0;
		trig ^= 1;	
	}
	
	TR0=0;
	TH0=0;
	TL0=0;
	TR0=1;
	while(echo==1 && TF0==0);
	TR0=0;
	if(TF0)
	{
		TF0=0;
		return 255;
	}
	else
		return ((TH0<<8)+TL0)*0.017;	
}
void time0()
{
	TMOD=(TMOD&0xf0)|0x01;	
	TH0=0xff;
	TL0=0xf4;	
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

//////////////////////////////////ÏÔÊ¾
void xianshi()
{
	switch(model)
	{
		case 0:
		        num1=11;num2=num3=num4=num5=10;
		        num6=volt/100;num7=volt%100/10;num8=volt%10;
		        break;
		case 1:
			      if(f_HL==0)
						{
								if(GN==6)
								{
										H+=5;								
									while(key_scan());	
								}							
								if(GN==7)
								{
										H-=5;								
									while(key_scan());	
								}								
						}
						else
						{
								if(GN==6)
								{
										L+=5;								
									while(key_scan());	
								}							
								if(GN==7)
								{
										L-=5;								
									while(key_scan());	
								}								
						}
						if(H > 50) H=5;							
						if(H < 5)  H=50;
						if(L > 50) L=5;							
						if(L < 5)  L=50;							
		        num1=12;num2=num3=10;
		        num4=H/10;num5=H%10;
		        num6=10;
		        num7=L/10;num8=L%10;
		        break;		
      case 2:
				H_lim=H*10;
			  L_lim=L*10;
				     if(start_csb==1)
						 {
							 juli=csb_send();
							 num1=13;num2=num3=num4=num5=10;
							 if(juli<10)
							 {
								 num6=num7=10;
								 num8=juli;								 
							 }
							 if(9<juli<100)
							 {
								 num6=10;
								 num7=juli/10;num8=juli%10;								 
							 }							 
							 if(juli>99)
							 {
								 num6=juli/100;
								 num7=juli%100/10;num8=juli%10;							 
							 }							 
						 }
						 else
						 {
							num1=13;num2=num3=num4=num5=10;num6=num7=num8=14;  
						 }
			      break;	
	}			
}
//===================================led
void led_display()
{
	if(model==0)
	{
		P0=0xff;
		led1=0;led2=1;led3=1;
		P2=(P2&0x1f)|0x80;
		P0=U6;
		P2=(P2&0x1f);				
	}
	if(model==1)
	{
		P0=0xff;
		led1=1;led2=1;led3=0;
		P2=(P2&0x1f)|0x80;
		P0=U6;
		P2=(P2&0x1f);				
	}	
	if(model==2)
	{
		P0=0xff;
		led1=1;led2=0;led3=1;
		P2=(P2&0x1f)|0x80;
		P0=U6;
		P2=(P2&0x1f);				
	}	
	
	if(start_csb)
	{
    if(f_100ms)
		{
		led8 ^= 1;
	//P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=U6;
		P2=(P2&0x1f);	
    }			
	}
	else
	{
		led8=1;
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=U6;
		P2=(P2&0x1f);				
	}
}

void main()
{
	initial_all();
	time0();
	time1_int1();
	
	
	while(1)
	{
	  if(f_2ms)
		{
	    f_2ms=0;
			if(++tt_2ms>50)
			{
				tt_2ms=0;
				f_100ms=~f_100ms;
			}
			key_change();
			
			if(GN==4)
			{
				model++;
				if(model==3)
					model=0;
				while(key_scan());
			}
			if(GN==5)
			{
				f_HL=~f_HL;
			  while(key_scan());	
			}
			
			
			if(++t_2ms==50)
			{
				t_2ms=0;
				initial_adc(0x3);				
				volt=du_adc();				
			}
			if(L_lim < volt && volt < H_lim)
			{
				start_csb=1;
			}
			else
				start_csb=0;
			
			xianshi();
	    led_display();
			

			
			
			
	
	  }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;		
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	f_2ms=1;
}










