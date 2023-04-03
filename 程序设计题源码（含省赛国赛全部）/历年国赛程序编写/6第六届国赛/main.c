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
bit f_2ms;
uchar t_2ms;
uchar GN;
sbit trig=P1^0;
sbit echo=P1^1;
//--------------
uchar bdata U6=0xff;
sbit led1=U6^0;
sbit led2=U6^1;
sbit led3=U6^2;
sbit led4=U6^3;
uchar bdata U9;
sbit relay=U9^4;
sbit beep=U9^6;
//--------------
uchar model=0;
uchar volt;
uchar distan;
uchar t_csb;
uchar type;
//--------------
bit convey=0;
uchar con_time1;
uchar con_time2;
uint t_con;
//--------------
bit f_stop=0;
//--------------
char tiao_t1=2;
char tiao_t2=4;
uchar t_shanshuo;
bit f_ss;
uchar type_model=2;
//--------------
bit f_save=0;
//----------------
uchar t1_500ms,t4_500ms;


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
//=========================================key
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
//========================================csb	
char f_count=10;
uchar  csb_send()
{
	f_count=10;
	trig=0;
	TR0=1;
	while(f_count--)
	{
		while(!TF0);
		trig ^= 1;
		TH0=(65536-12)/256;
		TL0=(65536-12)%256;			
		TF0=0;
	}
	
	TR0=0;
	TH0=0;
	TL0=0;
	TR0=1;
	while(echo && !TF0);
	TR0=0;
	
	if(TF0==1)
	{
		TF0=0;
		return 99;		
	}
	else
		return ((TH0<<8)+TL0)*0.017;	
}
//============================================24c02
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
//============================================adc
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
	D=(D*5.0)/255;
	return D;
}
//======================================led
void led_display()
{
	if(model==0)
	{
		led1=0;led2=1;led3=1;
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=U6;
		P2=(P2&0x1f);	

			beep=0;
			P2=(P2&0x1f)|0xa0;
			P0=U9;
			P2=(P2&0x1f);			
	}
	if(model==1)
	{
		led1=1;led2=0;led3=1;
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=U6;
		P2=(P2&0x1f);	

			beep=0;
			P2=(P2&0x1f)|0xa0;
			P0=U9;
			P2=(P2&0x1f);			
	}
	if(model==2)
	{
		if(++t1_500ms==250)
		{
			t1_500ms=0;
			led3=~led3;led1=1;led2=1;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);		

			beep=1;
			P2=(P2&0x1f)|0xa0;
			P0=U9;
			P2=(P2&0x1f);				
		}	
	}
	
	if(f_stop==1)
	{
		if(++t4_500ms==250)
		{
			t1_500ms=0;
			led4=~led4;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);						
		}				
	}
	if(f_stop==0)
	{
			led4=1;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);			
	}
	
}

void time0()
{
	TMOD=(TMOD&0xf0)|0x01;
	TH0=(65536-12)/256;
	TL0=(65536-12)%256;	
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
	tiao_t1=du_24c02(1);
	tiao_t2=du_24c02(2);
	num1=num2=num3=num4=num5=num6=num7=num8=10;
	while(1)
	{
	 if(f_2ms==1)
	 {
		 f_2ms=0;
	   key_change();
		 led_display();
//----------------------		 
     if(++t_2ms==50)
		 {
			 t_2ms=0;
			 volt=du_adc();	
       if(volt==0)
         model=0;//空载	
       if(volt>=1 && volt<4)
         model=1;//非空载	
       if(volt>4)
         model=2;//过载				 
		 }
//-------------------------------     
 if(model==1)
 {
	 if(GN==4)
	 {
		 convey=~convey;		 
		 while(key_scan()); 
	 }
	 //------------------------------------
	if(convey==0)  //判断货物类型
	{		
	 if(++t_csb==50)
	 {
		 t_csb=0;
		 distan=csb_send();
	 }
	 if(distan <= 30)
	 {
		 type=1;  con_time1=tiao_t1;
		 num1=1;num2=num3=10;
		 num4=distan/10;num5=distan%10;
		 num6=num7=10;num8=type;		 
	 }
	 if(distan > 30)
	 {
		 type=2;  con_time2=tiao_t2;
		 num1=1;num2=num3=10;
		 num4=distan/10;num5=distan%10;
		 num6=num7=10;num8=type;		 
	 }	 
 } 
	//--------------------------------
	if(convey==1)   //开始传送
	{
 //--------------------------------1类
  if(f_stop==0)
	{		
	 if(type==1)
	 {
		if(++t_con==500)
		{
			t_con=0;		
			  con_time1--;	
			if(con_time1==0)
				type=10;
		}
		relay=1;
		P2=(P2&0x1f)|0xa0;
		P0=U9;
		P2=(P2&0x1f);			
    num1=2;num2=num3=num4=num5=num6=10;
		num7=con_time1/10;num8=con_time1%10;
	}
  if(type==10)
	{
		convey=0;//返回判断类型界面
		relay=0;
		P2=(P2&0x1f)|0xa0;
		P0=U9;
		P2=(P2&0x1f);		
	}
//----------------------------------2类
	 if(type==2)
	 {
		if(++t_con==500)
		{
			t_con=0;		
			  con_time2--;	
			if(con_time2==0)
				type=11;
		}
		relay=1;
		P2=(P2&0x1f)|0xa0;
		P0=U9;
		P2=(P2&0x1f);			
    num1=2;num2=num3=num4=num5=num6=10;
		num7=con_time2/10;num8=con_time2%10;
	}
  if(type==11)
	{
		convey=0;//返回判断类型界面
		relay=0;
		P2=(P2&0x1f)|0xa0;
		P0=U9;
		P2=(P2&0x1f);		
	}	
 }	
//---------------------------------停止
	if(GN==5)
	{
		f_stop=~f_stop;
		while(key_scan());		
	}
//---------------------------------
	if(f_stop==1)
	{
		relay=0;
		P2=(P2&0x1f)|0xa0;
		P0=U9;
		P2=(P2&0x1f);   
    num1=2;num2=num3=num4=num5=num6=10;    
    if(type==1)
		{
			num7=con_time1/10;num8=con_time1%10;
		}
    if(type==2)
		{
			num7=con_time2/10;num8=con_time2%10;
		}
	 }		
	}	//开始传送	
	
 }//model==1

//==========================================
	if(model==0)  //空载的时候进行时间调节
	{
		if(GN==6)
		{
			type_model++;
			if(type_model==3)
				type_model=0;
			while(key_scan());
		}		
		if(++t_shanshuo==150)
		{
			t_shanshuo=0;
			f_ss=~f_ss;		
		}
		//----------------------------
		switch(type_model)
		{
			case 0:
				     if(GN==7)
						 {
							 tiao_t1++;
							 if(tiao_t1>10)
								 tiao_t1=1;
							 while(key_scan()); 
						 }
				     if(f_ss==1)
						  {
							 num4=tiao_t1/10;num5=tiao_t1%10;
						  }
						 else
						 {
               num4=10;num5=10;
						 }							 						 
				     num1=3;num2=num3=10;
			       num6=10;num7=tiao_t2/10;num8=tiao_t2%10;
			      break;
			case 1:
				     if(GN==7)
						 {
							 tiao_t2++;
							 if(tiao_t2>10)
								 tiao_t2=1;
							 while(key_scan()); 
						 }				
				     if(f_ss==1)
						  {
							 num7=tiao_t2/10;num8=tiao_t2%10;
						  }
						 else
						 {
               num7=10;num8=10;
						 }							 						 
				     num1=3;num2=num3=10;
			       num4=tiao_t1/10;num5=tiao_t1%10;num6=10;
						 f_save=0;
			      break;			
			case 2:
				     num1=num2=num3=num4=num5=num6=num7=num8=10;
			       if(f_save==0)
						 {
							 xie_24c02(1,tiao_t1);
							 Delay10ms();
							 xie_24c02(2,tiao_t2);
							 Delay10ms();							 						 
							f_save=1; 
						 }
			       break;	
			}	
		}//model=0
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


