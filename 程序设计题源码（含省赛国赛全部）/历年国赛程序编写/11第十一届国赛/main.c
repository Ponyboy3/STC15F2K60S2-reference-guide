#include <STC15F2K60S2.H>
#include "onewire.h"
#include "iic.h"
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
										 	0x90,0xff,0x8e,0x89,0x92};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

uchar num1,num2,num3,num4,num5,num6,num7,num8;
bit f_2ms;
uchar t_2ms;
uchar GN;
uchar bdata U6=0xff;
sbit led1=U6^0;
sbit led2=U6^1;
sbit led3=U6^2;
sbit led4=U6^3;
sbit led5=U6^4;
sbit led6=U6^5;
sbit led7=U6^6;
sbit led8=U6^7;

//------------------
uchar hour,minute,second;
uint temprature;
//------------------
bit model_jm=0;
uchar model_sj=0;
uchar model_cs=0;
//------------------
uchar t_adc;
uint volt;
bit light;
//------------------
uchar h_can=17;
uchar c_can=25;
uchar l_can=4;
//------------------
bit flag=0;
uint t_3s_1,t_3s_2;

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
		case 0:P2=(P2&0x1f)|0xe0; P0=type1[n1]; P2=(P2&0x1f);break;
		case 1:P2=(P2&0x1f)|0xe0; P0=type1[n2]; P2=(P2&0x1f);break;		
		case 2:P2=(P2&0x1f)|0xe0; 
		                          if(model_sj==2)
																P0=type1[n3]&0x7f;
															else
																P0=type1[n3];				
		                          P2=(P2&0x1f);break;		
		case 3:P2=(P2&0x1f)|0xe0; P0=type1[n4]; P2=(P2&0x1f);break;		
		case 4:P2=(P2&0x1f)|0xe0; P0=type1[n5]; P2=(P2&0x1f);break;		
		case 5:P2=(P2&0x1f)|0xe0; P0=type1[n6]; P2=(P2&0x1f);break;		
		case 6:P2=(P2&0x1f)|0xe0; 
		                          if(model_sj==1)
																P0=type1[n7]&0x7f;
															else
																P0=type1[n7];
		                          P2=(P2&0x1f);break;		
		case 7:P2=(P2&0x1f)|0xe0; P0=type1[n8]; P2=(P2&0x1f);break;		
		
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
	uchar  D;
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
uint  du_adc()
{
	uint D;
	IIC_Start( );
	IIC_SendByte(0x90+1);
	IIC_WaitAck( );	
	D=IIC_RecByte( );
	IIC_SendAck(1);
	IIC_Stop( );
	D=(D*500.0)/255;
	return D;	
}

//======================================ds1302
void xie_1302()
{
	Write_Ds1302_Byte( 0x8e,0x00 );
	Write_Ds1302_Byte( 0x84,0x16 );
	Write_Ds1302_Byte( 0x82,0x59 );
	Write_Ds1302_Byte( 0x80,0x50 );	
}
void du_1302()
{
	hour=Read_Ds1302_Byte( 0x85 );  hour=(hour>>4)*10+(hour&0x0f);
	minute=Read_Ds1302_Byte( 0x83 );minute=(minute>>4)*10+(minute&0x0f);
	second=Read_Ds1302_Byte( 0x81 );second=(second>>4)*10+(second&0x0f);
}

//============================================led
void led_display()
{
	if(flag==0)
	{
		if(hour>=8 && hour<h_can)
		{
			led1=1;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);			
		}
		else
		{
			led1=0;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);				
		}	
	
//----------------------------
if(temprature < (c_can*10))
{
			led2=0;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);	
}	
else
{
			led2=1;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);	
}	
//------------------------------	
	if(light==1)//暗
	{
		if(++t_3s_1==1500)
		{
			t_3s_1=0;t_3s_2=0;
			led3=0;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);			
		}	
	}
	if(light==0)//亮
	{
		if(++t_3s_2==1500)
		{
			t_3s_2=0;t_3s_1=0;
			led3=1;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);			
		}	
	}	
//-------------------------------
  if(light==1)
	{
			switch(l_can)
			{
				case 4:led4=0;break;
				case 5:led5=0;break;
				case 6:led6=0;break;
				case 7:led7=0;break;
				case 8:led8=0;break;
			}
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);			
	}
	if(light==0)
	{
		  led4=1;led5=1;led6=1;led7=1;led8=1;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);		
	}
 }//退出有效	
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
	initial_18b20();
	initial_adc();
	xie_1302();
	while(1)
	{
	 if(f_2ms==1)
	 {
		 f_2ms=0;
	   key_change();
	   du_1302();
		 led_display();
//--------------------------		 
	   if(++t_2ms==100)
		 {
			 t_2ms=0;
			 temprature=rd_temperature( );
		 }
	   if(++t_adc==200)
		 {
			 t_adc=0;
			 volt=du_adc();
			 if(volt<50)
				 light=1;//暗
			 else
				 light=0;//亮
		 }		 
//----------------------------------------
		 if(GN==4)
		 {
			 model_jm=~model_jm;
			 model_sj=0;model_cs=0;
			 while(key_scan());						 
		 }
//----------------------------------------

if(model_jm==0) //数据界面
{
	flag=0;
	if(GN==5)
	{
		model_sj++;
		if(model_sj==3)
			model_sj=0;
		while(key_scan());	
	}
	switch(model_sj)
	{
	  case 0:
			     num1=hour/10;num2=hour%10;num3=0;
		       num4=minute/10;num5=minute%10;num6=0;
		       num7=second/10;num8=second%10;
		      break;
	  case 1:
			     num1=11;num2=num3=num4=num5=10;
		       num6=temprature/100;num7=rd_temperature( )%100/10;num8=rd_temperature( )%10;
		      break;	
	  case 2:
			     num1=12;num2=10;
		       num3=volt/100;num4=volt%100/10;num5=volt%10;
		       num6=num7=10;num8=light;
		      break;	
	}
}//数据界面

//------------------------------------
if(model_jm) //参数界面
{
	flag=1;
		if(GN==5)
	{
		model_cs++;
		if(model_cs==3)
			model_cs=0;
		while(key_scan());	
	}
	switch(model_cs)
	{
	  case 0:
          if(GN==8){if(h_can==23) h_can=23; else h_can++; while(key_scan());}	
										
          if(GN==9){if(h_can==0) h_can=0; else h_can--; while(key_scan());}

          num1=13;num2=4;num3=num4=num5=num6=10;
					num7=h_can/10;num8=h_can%10;	
		      break;
	  case 1:
          if(GN==8){if(c_can==99) c_can=99; else c_can++; while(key_scan());}	
										
          if(GN==9){if(c_can==0) c_can=0; else c_can--; while(key_scan());}

          num1=13;num2=5;num3=num4=num5=num6=10;
					num7=c_can/10;num8=c_can%10;
		      break;	
	  case 2:
          if(GN==8){if(l_can==8) l_can=8; else l_can++; while(key_scan());}	
										
          if(GN==9){if(l_can==4) l_can=4; else l_can--; while(key_scan());}

          num1=13;num2=6;num3=num4=num5=num6=10;
					num7=l_can/10;num8=l_can%10;
		      break;	
	}	
}//参数界面

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












