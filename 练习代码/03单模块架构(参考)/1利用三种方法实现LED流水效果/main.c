/*****************************
方法一  利用移位（<<,>>）运算符实现（1.左流水 2.右流水 3.外到内 4.内到外）
*****************************
#include <STC15F2K60S2.h>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int
	
uchar led;
uchar i;

void Delay500ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 23;
	j = 205;
	k = 120;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void initial_all()
{
	P2=(0x1f)|0xa0;
	P0=0x00;
	P2=(0x1f);
	P2=(0x1f)|0x80;
	P0=0xff;
	P2=(0x1f);	
	P2=(0x1f)|0xc0;
	P0=0x00;
	P2=(0x1f);	
	P2=(0x1f)|0xe0;
	P0=0xff;
	P2=(0x1f);		
}
void main()
{
	initial_all();
while(1)
{
	P2=(0x1f)|0x80;
	led=0xfd;
	for(i=0;i<8;i++)
	{
		P0=led;
		led=led<<1;     //移位是整体向左移动一位，尾部补零
		
    Delay500ms();		
	}
	P0=0xff;
	Delay500ms();


//	led=0x7f;
//	for(i=0;i<8;i++)
//	{
//		P0=led;
//		led=led>>1;
//		
//    Delay500ms();		
//	}
//	P0=0xff;
//	Delay500ms();	
 }
}

*************/





/*****************************
方法二  利用本征函数_crol_() _cror_() 实现（1.左流水 2.右流水 3.外到内 4.内到外）
**********************
#include <STC15F2K60S2.h>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 

uchar led;
uchar i;

void Delay500ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 23;
	j = 205;
	k = 120;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
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
	P0=0x00;
	P2=(P2&0x1f);	
	P2=(P2&0x1f)|0xe0;
	P0=0xff;
	P2=(P2&0x1f);	
}
void main()
{
	initial_all();
	while(1)
	{
	 P2=(P2&0x1f)|0x80;
	 led=0xfe;
		for(i=0;i<8;i++)
		{
	   P0=led;
			led=_crol_(led,1);	    //移位函数是将高位拿给低位如此循环
	  Delay500ms();
			if(i==7)
			{
       P0=0xff;
       Delay500ms();
			}			
		}
	
	 led=0x7f;
		for(i=0;i<8;i++)
		{
	   P0=led;
			led=_cror_(led,1);	    
	  Delay500ms();
			if(i==7)
			{
       P0=0xff;
       Delay500ms();
			}				
		}	
	}
}

********/







/*****************************
方法三  利用数组 实现（1.左流水 2.右流水 3.外到内 4.内到外）
************************
#include <STC15F2K60S2.H>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
	
uchar code type1[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uchar code type2[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};
uchar code type3[]={0x7e,0xbd,0xdb,0xe7};
uchar code type4[]={0xe7,0xdb,0xbd,0x7e};

uchar i;

void Delay500ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 23;
	j = 205;
	k = 120;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
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
	P0=0x00;
	P2=(P2&0x1f);	
	P2=(P2&0x1f)|0xe0;
	P0=0xff;
	P2=(P2&0x1f);	
}
void main()
{
	initial_all();
	while(1)
	{
	 P2=(P2&0x1f)|0x80;
	 for(i=0;i<8;i++)
		{
     P0=type1[i];
     Delay500ms();
		}	
	 for(i=0;i<8;i++)
		{
     P0=type2[i];
     Delay500ms();
		}		
	 for(i=0;i<4;i++)
		{
     P0=type3[i];
     Delay500ms();
		}		
	 for(i=0;i<4;i++)
		{
     P0=type4[i];
     Delay500ms();
		}		

	}//while
}

******/




/*****************************
LED流水，同时继电器跟随LED流水不断动作
******************************/
#include <STC15F2K60S2.H>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
	
uchar code type1[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};


uchar i;
uchar f_2ms;
bit flag;


void Delay500ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 23;
	j = 205;
	k = 120;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
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
	P0=0x00;
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
	 if(f_2ms==1)
	 {
		 f_2ms=0;
		 
	 for(i=0;i<8;i++)
		{
		 flag=~flag;
		 P0=0xff;
		 P2=(P2&0x1f)|0x80;
     P0=type1[i];
		 P2=(P2&0x1f);
     

     if(flag==1)
		 {
			 P2=(P2&0x1f)|0xa0;
			 P0=0x10;
			 P2=(P2&0x1f);
		 }
     else
		 {
			 P2=(P2&0x1f)|0xa0;
			 P0=0x00;
			 P2=(P2&0x1f);
		 }			 
     Delay500ms();
		}	

		
		
		
	}//f_2ms;	
	}//while
}

void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;		
	f_2ms=1;
	
	
}




