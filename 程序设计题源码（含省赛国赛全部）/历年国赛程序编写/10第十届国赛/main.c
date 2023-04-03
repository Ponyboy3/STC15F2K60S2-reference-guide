#include <STC15F2K60S2.H>
#include "onewire.h"
#include "iic.h"
#include <intrins.h>
#include <stdio.h>
#include <string.h>

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
										 	0x90,0xff,0xc6,0xc3,0x98,0x8c};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
bit f_2ms;
uchar t_2ms;
uchar GN;
sbit trig=P1^0;
sbit echo=P1^1;
uchar bdata U6=0xff;
sbit led1=U6^0;
sbit led2=U6^1;
sbit led3=U6^2;
//--------------
uchar model_jm;
uchar model_sj;
uchar model_cs;
//--------------
uint temprature,time_c;
uchar distance,time_l;
//--------------
uint cishu=6524;
//--------------
uchar temprature_canshuo=30;
uchar distance_canshuo=35;
//-------------------------
bit f_canshuo=0;
uint t_cs;
bit f_dac=0,output=0;
uint t_dac;
//-------------------------
uchar a,b,c;





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
//=============================================超声波
char f_count=10;
uchar csb_x;
uchar csb_send()
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
		if(TF0)
		{
			TF0=0;
			return 99;
		}
		else
		{
			csb_x=((TH0<<8)+TL0)*0.017;
			if(csb_x>99)
				csb_x=99;
			return csb_x;
		}					
	}
//=========================================键盘
uchar key_scan()
	{
		uchar H,L;
		P3=0xcf;P44=0;P42=0;H=(~P3)&0x0f;
		if(H==0) return 0;
		Delay10ms();
		P3=0x0f;P44=0;P42=0;H=(~P3)&0x0f;
		if(H==0) return 0;		
		P3=0xf0;P44=1;P42=1;L=(~P3)&0x30;
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
		case 0x28:GN=12;break;
		case 0x24:GN=13;break;
		case 0x18:GN=16;break;
		case 0x14:GN=17;break;
		case 0:GN=0;break;
  }
}	
//====================================24c02
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
//======================================串口通信
uchar busy;
uchar pass_word1[]="ST\r\n",len_word1;
uchar pass_word2[]="PARA\r\n",len_word2;
uchar idata str1[40];
uchar idata str2[40];
uint c1,c2,c3,c4,c5;
void send_byte(uchar *byte,uchar len)
{
	while(len--)
	{
		SBUF=*byte;
		byte++;
		busy=1;
		while(busy);				
	}	
}
//===========================================DAC输出
void xie_dac(uchar dat)
{
	IIC_Start( );
	IIC_SendByte(0x90+0);
	IIC_WaitAck( );
	IIC_SendByte(0x40);
	IIC_WaitAck( );		
	IIC_SendByte(dat);
	IIC_WaitAck( );		
	IIC_Stop();	
}
//=====================================显示处理
void xs_display(uint D)
{
	if(D<10)
	{
		num4=num5=num6=num7=10;
		num8=D;
	}
	if(9<D && D<100)
	{
		num4=num5=num6=10;
		num7=D/10;num8=D%10;			
	}
	if(99<D && D<1000)
	{
		num4=num5=10;
		num6=D/100;num7=D%100/10;num8=D%10;			
	}
	if(999<D && D<10000)
	{
		num4=10;
		num5=D/1000;num6=D%1000/100;num7=D%100/10;num8=D%10;			
	}	
	if(D>9999)
	{
		num4=D/10000;num5=D%10000/1000;num6=D%1000/100;num7=D%100/10;num8=D%10;					
	}	
}

void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x8F;		//设定定时初值
	T2H = 0xFD;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	EA=1;
	ES=1;
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
	initial_18b20();
	UartInit( );
	xie_24c02(0,9);
	
	
	while(1)
	{
	 if(f_2ms==1)
	 {
		 f_2ms=0;
	   key_change();
		 //--------------
	   if(++time_c==100)
		 {
			 time_c=0;
			 temprature=rd_temperature();
		 }
	   if(++time_l==100)
		 {
			 time_l=0;
			 distance=csb_send();
		 }	
		 //-----------------
		if(GN==13)
		{
			f_dac=1;
			b=0;     
     while(key_scan());
			f_dac=0; t_dac=0;
				if(b!=1)
				{
					model_jm++;
					if(model_jm==2)
						model_jm=0;
					model_sj=0;model_cs=0;						
				}
		}			
		
	
//--------------------	
if(output==0)
{
//	xie_dac(100);
	a=0;
	led1=0;led2=1;
	P2=(P2&0x1f)|0x80;
	P0=U6;
	P2=(P2&0x1f);
}
if(output==1)
{
//	xie_dac(255);
	a=0;
	led1=1;led2=0;
	P2=(P2&0x1f)|0x80;
	P0=U6;
	P2=(P2&0x1f);
}

//-----------------------		
	if(model_jm==0)  //数据显示界面	 
	{
 		 if(GN==12)
			{
			 f_canshuo=1;	
			 a=0;				
			 while(key_scan());	
			 f_canshuo=0;	
			 t_cs=0;		
       if(a!=1)
			 {
					model_sj++;
					if(model_sj==3)
						model_sj=0;	
			 }				 	     				
			}		
    switch(model_sj)
		{
      case 0:
				     num1=11;num2=num3=num4=10;
			       num5=temprature/1000;num6=temprature%1000/100;
			       num7=temprature%100/10;num8=temprature%10;
			       break;
      case 1:
				     num1=12;num2=num3=num4=num5=num6=10;
			       num7=distance/10;num8=distance%10;
			       break;
      case 2:
				     num1=13;num2=num3=10;
			       xs_display(cishu);			       
			       break;
		}
	}		//model_jm=0
//-------------------------------------
	if(model_jm==1)  //参数显示界面	 	
	{
 		if(GN==12)
			{
			 f_canshuo=1;	
			 a=0;									
			 while(key_scan());
			 f_canshuo=0;
			 t_cs=0;
				if(a!=1)
				{
					model_cs++;
					if(model_cs==2)
						model_cs=0;												
				}
			}			

		switch(model_cs)
		{
			case 0:
				     if(GN==17)
						 {
							temprature_canshuo++;
              if(temprature_canshuo==100)
                 temprature_canshuo=99;															 
							while(key_scan()); 
						 }
				     if(GN==16)
						 {							
              if(temprature_canshuo==0)
								temprature_canshuo=0;
							else
								temprature_canshuo--;														 
							while(key_scan()); 
						 }			
			       num1=14;num2=num3=10;num4=1;num5=num6=10;
			       num7=temprature_canshuo/10;num8=temprature_canshuo%10;
			       break;
			case 1:
				     if(GN==17)
						 {
							distance_canshuo+=5;
              if(distance_canshuo>99)
                 distance_canshuo=99;															 
							while(key_scan()); 
						 }
				     if(GN==16)
						 {							
              if(distance_canshuo<5)
								distance_canshuo=0;
							else
								distance_canshuo-=5;														 
							while(key_scan()); 
						 }			
			       num1=14;num2=num3=10;num4=2;num5=num6=10;
			       num7=distance_canshuo/10;num8=distance_canshuo%10;
			       break;
		}
	}//model_jm
//-------------------------------------------------------------		 
		




	
		 
//		 num1=csb_send()/10; num2=csb_send()%10;
//		 num3=rd_temperature( )/10;num4=rd_temperature( )%10;
//		 num5=du_24c02(0);num6=6;num7=7;num8=8;
//		 
//	   xie_dac(100);
//		 if(len_word1==2)
//		 {
//			 len_word1=0;
//			 sprintf  (str1,"$%d,%d.%d\r\n", c1,c2,c3);
//			 send_byte(str1,strlen(str1));
//		 }
//		 if(len_word2==4)
//		 {
//			 len_word2=0;
//			 sprintf  (str2,"$#%d,%d\r\n", c4,c5);
//			 send_byte(str2,strlen(str2));
//		 }		 
	 }//f_2ms
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;			
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	f_2ms=1;
	//------------------------
	if(f_canshuo==1)
	{
		if(++t_cs > 500)
		{
			cishu=0;
      a=1;			
		}		
	}
//--------------------------
  if(f_dac==1)
	{
		if(++t_dac>500)
		{			
			f_dac=0;
			output=~output;
			b=1;
		}
	}
}

void cl_UartInit( ) interrupt 4
{
	if(RI)
	{
		RI=0;
		if(pass_word1[len_word1] == SBUF)
		  len_word1++;
		else
			len_word1=0;
//-------------------------
		if(pass_word2[len_word2] == SBUF)
		  len_word2++;
		else
			len_word2=0;		
	}
	if(TI)
	{
		TI=0;
		busy=0;		
	}	
}



