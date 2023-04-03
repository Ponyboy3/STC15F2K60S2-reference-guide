/****************************************
利用定时器1实现对数码管动态扫描 显示
*********************************************/
#include <STC15F2K60S2.H>
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

sbit trig=P1^0;
sbit echo=P1^1;


uint temp1;
uchar temp=10;

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
////////////////////////////////超声波


uchar csb()
{
	temp=10;
	trig=0;	
	TR0=1;
	//引脚发送40Khz方波信号
	while(temp--)  //每隔12us发5个方波
	{
	  while(!TF0);
      trig ^= 1;          // if(trig==1) { Delay20us();}//可以不用
			TH0=(65536-12)/256;
			TL0=(65536-12)%256;		  
		  TF0=0;       //TF0必须要先赋值在清零
	}		
	      TR0=0;
				TL0=0;
				TH0=0;
	      TR0=1;
	    while( echo && !TF0 );
		  TR0=0;
	
	    if(TF0==1)
			{
				TF0=0;
				return 999;
			}
		  else
				return ((TH0<<8)+TL0)*0.017;
}

void main()
{
  initial_all();	
	time1_int1();
	time0();
	num1=num2=num3=num4=num5=num6=num7=num8=10;
	while(1)
	{
	 if(f_2ms==1)
	 {
		 f_2ms=0;
		 
		 
		   temp1=csb();
			 num1=temp1/100;num2=temp1%100/10;num3=temp1%10;
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



/*===========================================
//官方程序
=============================================*/

/*
#include <STC15F2K60S2.H>
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

sbit TX = P1^0; // 发射引脚
sbit RX = P1^1; // 接收引脚

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

//==============================================
void Timer0Init(void) // 12us@12.000MHz 
{ 
 TMOD=(TMOD&0xf0)|0x01;
 TL0 = 0xF4; // 设置定时初值
 TH0 = 0xFF; // 设置定时初值
// TF0 = 0; // 清除 TF0 标志
// TR0 = 0; // 定时器 0 停止
}

unsigned char ucDist, ucNum = 10;
unsigned char Wave_Recv(void) 
	{ 。
	 ucNum = 10; 
	 TX = 0; 
	 TR0 = 1; // 定时器 0 计时
	 // TX 引脚发送 40kHz 方波信号驱动超声波发送探头
	 while(ucNum--) 
	 { 
			 while(!TF0); 
			 TX ^= 1; 
			 TL0 = 0xF4; // 设置定时初值
			 TH0 = 0xFF; // 设置定时初值		 
			 TF0 = 0; 
	 } 
	 TR0 = 0; 
	 TL0 = 0; // 设置定时初值
	 TH0 = 0; // 设置定时初值
	 TR0 = 1; 
	 while(RX && !TF0); // 等待收到脉冲
	 TR0 = 0; 
	 
	 if(TF0) // 发生溢出
	 { 
		 TF0 = 0; 
		 ucDist = 900; 
	 } 
	 else // 计算距离
	    ucDist = ((TH0<<8)+TL0)*0.017; 
	 
	    return ucDist; 		 
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
	Timer0Init( );
	while(1)
	{
//	 if(f_2ms==1)
//	 {
//		 f_2ms=0;
	
	   
	
		 
		 
		 
		 num1=Wave_Recv( )/100; num2=Wave_Recv( )%100/10;num3=Wave_Recv( )%10;
		 num4=10;num5=10;num6=10;num7=10;num8=10;
	
//	 }//f_2ms
	}
}



void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;		
	
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	f_2ms=1;
}


*/
















