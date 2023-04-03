#include <STC15F2K60S2.H>
#include "onewire.h"
#include "ds1302.h"  
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
										 	0x90,0xff,0xbf,0xc1};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar GN;
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar f_2ms;
uchar t_2ms;
uchar t_100ms;
uint  t_5s,tt_5s;// 5s
uchar hour,minute,second;

uchar bdata U9;
sbit relay=U9^4;

uchar bdata U6=0xff;
sbit led1=U6^0;
sbit led2=U6^1;
sbit led3=U6^2;

uchar model=0;
bit k_model=0;
uint temprature;
uint canshuo=23;
bit f_time=0;//分秒
bit f_relay,f_led;
bit f_shanshuo;//led闪烁



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
		case 0:P2=(P2&0x1f)|0xe0;P0=type1[n1];P2=(P2&0x1f);break;
		case 1:P2=(P2&0x1f)|0xe0;P0=type1[n2];P2=(P2&0x1f);break;		
		case 2:P2=(P2&0x1f)|0xe0;P0=type1[n3];P2=(P2&0x1f);break;		
		case 3:P2=(P2&0x1f)|0xe0;P0=type1[n4];P2=(P2&0x1f);break;		
		case 4:P2=(P2&0x1f)|0xe0;P0=type1[n5];P2=(P2&0x1f);break;		
		case 5:P2=(P2&0x1f)|0xe0;P0=type1[n6];P2=(P2&0x1f);break;	
		case 6:P2=(P2&0x1f)|0xe0;
		                         if(model==0)
															 P0=type1[n7]&0x7f;
														 else
															 P0=type1[n7];  
		                          P2=(P2&0x1f);break;
		case 7:P2=(P2&0x1f)|0xe0;P0=type1[n8];P2=(P2&0x1f);break;		
	}
	if(++i==8)
		i=0;
}

void led_display()
{
		if(minute==0 && second==0) //L1
		{
		   f_led=1;
			
		}
		if(f_led==1)
		{	
    		EA=0;
			  led1=0;         P0=0xff;
				P2=(P2&0x1f)|0x80;
				P0=U6;
				P2=(P2&0x1f);			
			  EA=1;	
			if(++tt_5s==2500)
			{
			 	tt_5s=0;
    		EA=0;
			  led1=1;         P0=0xff;
				P2=(P2&0x1f)|0x80;
				P0=U6;
				P2=(P2&0x1f);			
			  EA=1;				
				f_led=0;
			}			
		}
	
	 
	if(k_model==0)
	{
    		EA=0;
			  led2=0;         P0=0xff;
				P2=(P2&0x1f)|0x80;
				P0=U6;
				P2=(P2&0x1f);			
			  EA=1;			
	}
	else
	{
    		EA=0;
			  led2=1;         P0=0xff;
				P2=(P2&0x1f)|0x80;
				P0=U6;
				P2=(P2&0x1f);			
			  EA=1;			
	}	
	
	if(relay==1)//L3
	{
    if(++t_100ms==50)
		{
			t_100ms=0;
			//f_shanshuo=~f_shanshuo;
			led3=~led3;;
		}
    		EA=0;           P0=0xff;
				P2=(P2&0x1f)|0x80;
				P0=U6;
				P2=(P2&0x1f);			
			  EA=1;	    
	}		
	else
	{
    		EA=0;
		    led3=1;          P0=0xff;
				P2=(P2&0x1f)|0x80;
				P0=U6;
				P2=(P2&0x1f);			
			  EA=1;			
	}
}

void control_x()
{
	if(k_model==0)//wendu
	{
		if(temprature > canshuo*10)
		{
			  EA=0;
			  relay=1;
				P2=(P2&0x1f)|0xa0;
				P0=U9;
				P2=(P2&0x1f);			
			  EA=1;
		}
		else
		{
			  EA=0;
			  relay=0;
				P2=(P2&0x1f)|0xa0;
				P0=U9;
				P2=(P2&0x1f);			
			  EA=1;			
		}	
	}//
	if(k_model==1)//shijian
	{
		if(minute==0 && second==0)
		{
		   f_relay=1;
			
		}
		if(f_relay==1)
		{	
    		EA=0;
			  relay=1;
				P2=(P2&0x1f)|0xa0;
				P0=U9;
				P2=(P2&0x1f);			
			  EA=1;	
			if(++t_5s==2500)
			{
			 	t_5s=0;
    		EA=0;
			  relay=0;
				P2=(P2&0x1f)|0xa0;
				P0=U9;
				P2=(P2&0x1f);			
			  EA=1;				
				f_relay=0;
			}			
		}
	}	//
	
}

////////////////////////////////1302
void xie_1302()
{
  Write_Ds1302_Byte( 0x8e,0x00 );	
	Write_Ds1302_Byte( 0x84,0x23 );
	Write_Ds1302_Byte( 0x82,0x59 );
	Write_Ds1302_Byte( 0x80,0x55 );
}
void du_1302()
{
	hour=Read_Ds1302_Byte( 0x85 );   hour=(hour>>4)*10+(hour&0x0f);
	minute=Read_Ds1302_Byte( 0x83 ); minute=(minute>>4)*10+(minute&0x0f);
	second=Read_Ds1302_Byte( 0x81 ); second=(second>>4)*10+(second&0x0f);
}
//////////////////////////////key
uchar key_scan()
{
	uchar H,L;
	P3=0xcf;P44=0;P42=0;H=(~P3)&0x0f;
	if(H==0) return 0;
	Delay10ms();
	P3=0xcf;P44=0;P42=0;H=(~P3)&0x0f;
	if(H==0) return 0;	
	P3=0xf0;P44=1;P42=1;L=(~P3)&0x30;
	if(P44==0) L|=0x80;
	if(P42==0) L|=0x40;
	return (H+L);
}
void key_change()
{
	uchar D;
	D=key_scan();
	switch(D)
	{
		case 0x28:GN=12;break;
		case 0x24:GN=13;break;
		case 0x18:GN=16;break;
		case 0x14:GN=17;break;
		case 0:  GN=0;break;
	}
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

	      U6=0xff;
				P2=(P2&0x1f)|0x80;
				P0=U6;
				P2=(P2&0x1f);		
}

void main()
{
  initial_all();	
	time1_int1();
	
	initial_18b20();
	xie_1302();
	while(1)
	{
	  if(f_2ms==1)
		{
	   f_2ms=0;
			
	    key_change();
			du_1302();
			control_x();
			led_display();

		 if(++t_2ms==100)
		 {
			 t_2ms=0;
			 temprature=rd_temperature( );
		 }	
						 
		if(GN==12)//模式
		{
     model++;
			if(model==3)
				model=0;
     while(key_scan());
		}			
		if(GN==13)//温度、时间控制
		{
     k_model=~k_model;
     while(key_scan());
		}				
	

		switch(model)
		{
			case 0:

             num1=12;num2=1;num3=num4=num5=10;
						 num6=temprature/100;
             num7=temprature%100/10;
             num8=temprature%10;
             break;
      case 1:
				   if(GN==17)
						{						 
             f_time=1;
						 while(key_scan());
						 f_time=0;	
						}
												
				     num1=12;num2=2;num3=10;
			       num4=hour/10;num5=hour%10;
			       num6=11;
			       num7=minute/10;num8=minute%10;
			       break;
      case 2:
						if(GN==16)
						{						 
							if(canshuo==99)
								canshuo=99;
							else 
								canshuo++;
						 while(key_scan());
						}					     
						if(GN==17)
						{						 
							if(canshuo==10)
								canshuo=10;
							else 
								canshuo--;
						 while(key_scan());
						}				
				     num1=12;num2=3;num3=num4=num5=num6=10;
			       num7=canshuo/10;num8=canshuo%10;
			       break;			
			
		 }//switch			
			
		}//f_2ms
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;		
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	
	f_2ms=1;
	
	if(f_time==1)
	{
		du_1302();
			 num4=minute/10;num5=minute%10;
			 num6=11;
			 num7=second/10;num8=second%10;		
	}	
}

