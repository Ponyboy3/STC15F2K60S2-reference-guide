/********************************************
(1.1)led依次向右点亮，亮度可用软件进行调节
********************************************/
/****************
#include <STC15F2K60S2.H>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
uchar t_2ms,l_2ms;
bit f_2ms;

uchar led_type[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
//uchar bdata U6=0xff;	
//sbit led1=U6^0;
//sbit led2=U6^1;
//sbit led3=U6^2;
//sbit led4=U6^3;
//sbit led5=U6^4;
//sbit led6=U6^5;
//sbit led7=U6^6;
//sbit led8=U6^7;

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
	uchar led;
	initial_all();
	time1_int1();
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;

		if(++l_2ms==10)
		{
			l_2ms=0;
			P2=(P2&0x1f)|0x80;	
			P0=0xff;	
			P2=(P2&0x1f);			
		}
		 if(l_2ms==1) 
		 {
			P2=(P2&0x1f)|0x80;	
			P0=led_type[led];
			P2=(P2&0x1f);				 
		 }   			 
	  if(++t_2ms==250)
		{
			t_2ms=0;
			led++;
			if(led==8)
				led=0;
		} 
	 }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	f_2ms=1;
}


***************/

/********************************************
(1.2)蜂鸣器与继电器每秒点亮其中一个
********************************************/
/*****************
#include <STC15F2K60S2.H>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
uint t_2ms;
bit f_2ms;
bit flag;


uchar bdata U9;
sbit relay=U9^4;
sbit beep=U9^6;


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
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;
    if(++t_2ms==500)
		{
			t_2ms=0;
			flag ^= 1;			
		}
		 if(flag)
		 {
			relay=1;beep=0; 
			P2=(P2&0x1f)|0xa0;	
			P0=U9;	
			P2=(P2&0x1f);				  
		 }
		 else
		 {
			relay=0;beep=1;
			P2=(P2&0x1f)|0xa0;	
			P0=U9;	
			P2=(P2&0x1f);				  
		 }
		 
	 }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	f_2ms=1;
}
*****************/


/********************************************
(1.3)P34输出不同占空比脉冲信号，软件调节。可测出（1，2，3，4，5）V电压
********************************************/
/********************
#include <STC15F2K60S2.H>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
uint t_2ms;
bit f_2ms;

sbit output=P3^4;

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
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;
    if(++t_2ms==10)
		{
			t_2ms=0;
			output=0;
		}
		if(t_2ms==8)
		{
      output=1;
		}					 
	 }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	f_2ms=1;
}
*******************/


/********************************************
(2.1)利用定时器1中断扫描数码管动态显示，交替显示带点数字与不带电数字
********************************************/
/**************
#include <STC15F2K60S2.H>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
uchar type1[]={0xc0,  
								0xf9,  
								0xa4,   
								0xb0,  
								0x99,  
								0x92,  
								0x82,  
								0xf8,  
								0x80,  
								0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uint t_2ms;
bit f_2ms;
bit flag;

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
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;
		if(++t_2ms==500)
		{
      t_2ms=0;
			flag ^= 1;
		}	
    if(flag)
		{
			type1[0]=0xc0;type1[1]=0xf9;type1[2]=0xa4;type1[3]=0xb0;
			type1[4]=0x99;type1[5]=0x92;type1[6]=0x82;type1[7]=0xf8;
			type1[8]=0x80;type1[9]=0x90;
		}
		else
		{
			type1[0]=0xc0&0x7f;type1[1]=0xf9&0x7f;type1[2]=0xa4&0x7f;type1[3]=0xb0&0x7f;
			type1[4]=0x99&0x7f;type1[5]=0x92&0x7f;type1[6]=0x82&0x7f;type1[7]=0xf8&0x7f;
			type1[8]=0x80&0x7f;type1[9]=0x90&0x7f;
		}	
		num1=1;num2=2;num3=3;num4=4;num5=5;num6=6;num7=7;num8=8;
	 }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	f_2ms=1;
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
}

****************/

/********************************************
(3.1)显示独立按键键值。实验按下显示与松手显示
********************************************/
/*****************
#include <STC15F2K60S2.H>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
uchar type1[]={0xc0,  
								0xf9,  
								0xa4,   
								0xb0,  
								0x99,  
								0x92,  
								0x82,  
								0xf8,  
								0x80,  
								0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uint t_2ms;
bit f_2ms;
uchar GN;



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
//=============================================key
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
		//case 0:GN=0;break;		
	}
	while(key_scan());//实现松手显示
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
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;
	  key_change();
		 

		num1=10;num2=10;num3=10;num4=10;num5=10;num6=10;num7=10;num8=GN;
	 }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	f_2ms=1;
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
}
*******************/



/********************************************
(3.2)显示矩阵按键键值。（3*4   与   4*4）
********************************************/
/*****************
#include <STC15F2K60S2.H>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
uchar type1[]={0xc0,  
								0xf9,  
								0xa4,   
								0xb0,  
								0x99,  
								0x92,  
								0x82,  
								0xf8,  
								0x80,  
								0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uint t_2ms;
bit f_2ms;
uchar GN;

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
//====================================================key
uchar key_scan()
{
	uchar H,L;
//	P3=0xcf;P44=0;P42=0;H=(~P3)&0x0f;  //4*4
  	P3=0xdf;P44=0;P42=0;H=(~P3)&0x0f;  //3*4	
	if(H==0) return 0;
	Delay10ms();
//	P3=0x0f;P44=0;P42=0;H=(~P3)&0x0f; //4*4
	P3=0xdf;P44=0;P42=0;H=(~P3)&0x0f;   //3*4
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
		case 0x82:GN=6;break;
		case 0x81:GN=7;break;
		case 0x48:GN=8;break;
		case 0x44:GN=9;break;
		case 0x42:GN=10;break;
		case 0x41:GN=11;break;
		case 0x28:GN=12;break;
		case 0x24:GN=13;break;
		case 0x22:GN=14;break;
		case 0x21:GN=15;break;
		case 0x18:GN=16;break;
		case 0x14:GN=17;break;
		case 0x12:GN=18;break;
		case 0x11:GN=19;break;

    //case 0:GN=0;break;		
	}		
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
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;
	  key_change();
		 
		num1=10;num2=10;num3=10;num4=10;num5=10;num6=10;num7=GN/10;num8=GN%10;
	 }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	f_2ms=1;
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
}

*****************/
/********************************************
(3.3)独立按键，实现长按有效，实验双击有效
********************************************/
/*****************
#include <STC15F2K60S2.H>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
uchar type1[]={0xc0,  
								0xf9,  
								0xa4,   
								0xb0,  
								0x99,  
								0x92,  
								0x82,  
								0xf8,  
								0x80,  
								0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uint t_2ms;
bit f_2ms;
uchar GN;
uchar bdata U6=0xff;	
sbit led1=U6^0;
sbit led2=U6^1;
//-------------------
bit f_led1;
bit f_led2;

bit f_chang;
bit f_lim;

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
//=============================================key
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
	//while(key_scan());//实现松手显示
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
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;
	  key_change();
		if(GN==4)//短按键
		{			
			f_chang=1;
			f_lim=0;
			while(key_scan());
			t_2ms=0;
			if(f_lim==0)
			{
				f_led1 ^= 1;				
			}
		}
		//-----------------------
    if(f_led1)
		{
			led1=0;
			P0=0xff;
			P2=(P2&0x1f)|0x80;	
			P0=U6;	
			P2=(P2&0x1f);				
		}
		else
		{
			led1=1;
			P0=0xff;
			P2=(P2&0x1f)|0x80;	
			P0=U6;	
			P2=(P2&0x1f);				
		}
    //------------------------
    if(f_led2)
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

		num1=10;num2=10;num3=10;num4=10;num5=10;num6=10;num7=10;num8=GN;
	 }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	f_2ms=1;
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	
	if(f_chang==1)
	{
		if(++t_2ms==500)
		{
			t_2ms=0;
			f_lim=1;
			f_led2 ^= 1;
			f_chang=0;
		}		
	}
}
*******************/


/********************************************
(4.1)数码管显示555方波发生器频率，高位不用的应熄灭
********************************************/
/******************
#include <STC15F2K60S2.H>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
uchar type1[]={0xc0,  
								0xf9,  
								0xa4,   
								0xb0,  
								0x99,  
								0x92,  
								0x82,  
								0xf8,  
								0x80,  
								0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uint t_2ms;
bit f_2ms;
uint temp;
bit flag;


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
//===================================================数据处理
void shujv_chuli(uint D)
{
	if(D<10)
	{
		num4=num5=num6=num7=10;
		num8=D;
	}
	if(D>9 && D<100)
	{
		num4=num5=num6=10;
		num7=D/10;num8=D%10;
	}	
	if(D>99 && D<1000)
	{
		num4=num5=10;
		num6=D/100;num7=D%100/10;num8=D%10;
	}		
	if(D>999 && D<10000)
	{
		num4=10;
		num5=D/1000;num6=D%1000/100;num7=D%100/10;num8=D%10;
	}		
	if(D>9999)
	{
		num4=D/10000;num5=D%10000/1000;num6=D%1000/100;num7=D%100/10;num8=D%10;
	}
	
}

void time0()
{
	TMOD=(TMOD&0xf0)|0x05;
	TH0=0;
	TR0=0;		
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
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;
    shujv_chuli(temp);


		
	 }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	f_2ms=1;
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
  
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
			temp=((TH0<<8)+TL0)*2;
		}		
	}	
}
*************************/



/********************************************
(5.1)DS1302显示时分秒，通过软件写入1302时分秒
********************************************/
/*********************
#include <STC15F2K60S2.H>
#include "ds1302.h" 
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
								0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uint t_2ms;
bit f_2ms;
uchar hour,minute,second;



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
//===================================================1302
void xie_1302(uchar a,uchar b,uchar c)
{
	Write_Ds1302_Byte( 0x8e,0x00 );
	Write_Ds1302_Byte( 0x84,a );
	Write_Ds1302_Byte( 0x82,b );
	Write_Ds1302_Byte( 0x80,c );	
}
void du_1302()
{
	hour=Read_Ds1302_Byte( 0x85 );  hour=(hour>>4)*10+(hour&0x0f);
	minute=Read_Ds1302_Byte( 0x83 );minute=(minute>>4)*10+(minute&0x0f);
	second=Read_Ds1302_Byte( 0x81 );second=(second>>4)*10+(second&0x0f);

}
uchar d_bcd(uchar D)
{
	uchar dat1,dat2;
	dat1=D/10;
	dat2=D%10;
	dat2=dat1*16+dat2;
  return dat2;
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

	xie_1302(d_bcd(20),d_bcd(21),d_bcd(58));
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;
    du_1302();
		 
    num1=hour/10;num2=hour%10;     num3=10;
    num4=minute/10;num5=minute%10; num6=10;
		num7=second/10;num8=second%10;
	 }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	f_2ms=1;
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
  
}

************************/

/********************************************
(6.1)ds18b20显示温度数据，实验不同分辨率下的数据显示（0.5/0.25/0.125/0.0625）
********************************************/
/******************
#include <STC15F2K60S2.H>
#include "onewire.h"
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
								0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uint t_2ms;
bit f_2ms;




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
	uint temp;
	initial_all();
	time1_int1();
  initial_18b20();
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;

		 temp=rd_temperature( );
		 
		 
		 
		 num1=temp/10000; num2=temp%10000/1000;num3=temp%1000/100;num4=temp%100/10;num5=temp%10;
		 
	 }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	f_2ms=1;
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
  
}
**********/

/********************************************
(7.1)24c02存取数据，上电自动读取存入的数据。连续存入10个数据，利用数组
********************************************/
/*****************
#include <STC15F2K60S2.H>
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
								0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uint t_2ms;
bit f_2ms;




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
  xie_24c02( 1 , 1);
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;
 
    num1=du_24c02(1);num2=10;
		 
	 }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	f_2ms=1;
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
  
}
*****************/


/********************************************
(8.1)AD,分别读取电位器与光敏电阻的电压，实验多次读取求取平均值的方法使数据稳定
********************************************/
/******************
#include <STC15F2K60S2.H>
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
								0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uint t_2ms;
bit f_2ms;




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
//=====================================adc
void initial_adc(uchar addr)
{
	IIC_Start( );
	IIC_SendByte(0x90+0);
	IIC_WaitAck( );
	IIC_SendByte(addr);
	IIC_WaitAck( );	
  IIC_Stop( ); 		
}
uint du_adc()
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
	uint temp;
	initial_all();
	time1_int1();
  initial_adc(0x1);
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;

		if(++t_2ms==50)
		{
      t_2ms=0;
      temp=du_adc();
     // temp=(temp*3)/3;
		}			
 
		 num1=temp/100;num2=temp%100/10;num3=temp%10;num4=10;
		 
	 }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	f_2ms=1;
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
  
}
*************/

/********************************************
(8.2)DA输出，利用通道到0读取回来，进行显示
********************************************/
/*************
#include <STC15F2K60S2.H>
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
								0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uint t_2ms;
bit f_2ms;




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
void initial_adc(uchar addr)
{
	IIC_Start( );
	IIC_SendByte(0x90+0);
	IIC_WaitAck( );
	IIC_SendByte(addr);
	IIC_WaitAck( );	
  IIC_Stop( ); 		
}
uint du_adc()
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
uchar change_D(uint D)
{
	uint D1;
	D1=(D*255.0)/500;
	return D1;
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
	char a;
	uint temp;
	initial_all();
	time1_int1();
  initial_adc(0x0);
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;
    
//    if(a==0)
//		{
			xie_dac(change_D(100));
//			a=1;
//		}
		 
		 
//		if(++t_2ms==100)
//		{
//      t_2ms=0;
//			initial_adc(0x0);
      temp=du_adc();
//		}			
		num1=temp/100; num2=temp%100/10;num3=temp%10;
		 
		 
		 
	 }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	f_2ms=1;
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
  
}

*************************/


/********************************************
(9.1)超声波测距
********************************************/
/*********************
#include <STC15F2K60S2.H>
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
								0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uint t_2ms;
bit f_2ms;
sbit trig=P1^0;
sbit echo=P1^1;



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
//===========================================超声波
uchar f_count=10;
uchar csb_send()
{
	f_count=10;
	trig=0;
	TR0=1;
	while(f_count--)
	{
		while(!TF0);
		trig ^= 1;
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
	uint temp;
	initial_all();
	time1_int1();
	time0();
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;
    temp=csb_send();

		num1=temp/10; num2=temp%10;num3=10; 
	 }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	f_2ms=1;
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
  
}

************************/

/********************************************
(10.1)串口发送数据与接受数据（参考第十届的题）
********************************************/

#include <STC15F2K60S2.H>
#include <intrins.h>
#include <stdio.h>
#include <string.h>

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
								0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uint t_2ms;
bit f_2ms;




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
//===================================串口发送
uchar busy;
void send_byte(uchar *dat,uchar len)
{
	while(len--)
	{
		SBUF=*dat;
		busy=1;
		dat++;
		while(busy);	
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
	
	ES=1;
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
	uchar p1;
	uchar p2;
	uchar pass_word1[]="wy";
  uchar pass_word2[]="zcl";
	uchar type3[]="chang zhou gong xue yuan di yi shuai\r\n";
	uchar idata str1[40];
	uchar idata str2[40];
	uint a=12;
void main()
{

	initial_all();
	time1_int1();

	 UartInit( );
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;
      if(p1>=2)
			{
				p1=0;
				sprintf (str1,"nv pen you %d \r\n", a);
				send_byte(str1,strlen  (str1));
			}
      if(p2>=3)
			{
				p2=0;
				sprintf (str2,"nan pen you %d \r\n", ++a);
				send_byte(str2,strlen  (str2));
			}		 
		 
		 
		 
	 }//f_2ms;
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	f_2ms=1;
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
  
}
void cl_UartInit( )  interrupt 4
{
	if(RI)
	{
		RI=0;
		if(pass_word1[p1]==SBUF)
		  p1++;
		else
			p1=0;
//----------------------
		if(pass_word2[p2]==SBUF)
		  p2++;
		else
			p2=0;		
	}
	if(TI)
	{
		TI=0;
		busy=0;	
	}
	
	
	
	
}



