/************************************
练习测量555频率的程序
************************************/
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
bit flag;
uint temp;



void smg_display(char n1,char n2,char n3,char n4,char n5,char n6,char n7,char n8)
{
	char i;
	P0=0xff;
	P2=(P2&0x1f)|0xc0;
	P0=type2[i];
	P2=(P2&0x1f);
	switch(i)
	{
		case 0:P2=(P2&0x1f)|0xe0; P0=type1[n1];P2=(P2&0x1f);break;
		case 1:P2=(P2&0x1f)|0xe0; P0=type1[n2];P2=(P2&0x1f);break;	
		case 2:P2=(P2&0x1f)|0xe0; P0=type1[n3];P2=(P2&0x1f);break;	
		case 3:P2=(P2&0x1f)|0xe0; P0=type1[n4];P2=(P2&0x1f);break;	
		case 4:P2=(P2&0x1f)|0xe0; P0=type1[n5];P2=(P2&0x1f);break;			
		case 5:P2=(P2&0x1f)|0xe0; P0=type1[n6];P2=(P2&0x1f);break;
		case 6:P2=(P2&0x1f)|0xe0; P0=type1[n7];P2=(P2&0x1f);break;
		case 7:P2=(P2&0x1f)|0xe0; P0=type1[n8];P2=(P2&0x1f);break;		
	}
	if(++i==8)
		i=0;
}


void f_display()
{
	if(temp<10)
	{
		num8=temp;
		num1=num2=num3=num4=num5=num6=num7=10;
		
	}
	else if(temp>9 && temp<100)
	{
		num7=temp/10;num8=temp%10;
		num1=num2=num3=num4=num5=num6=10;		
	}
	else if(temp>99 && temp<1000)
	{
		num6=temp/100;num7=temp%100/10;num8=temp%10;
		num1=num2=num3=num4=num5=10;		
	}	
	else if(temp>999 && temp<10000)
	{
		num5=temp/1000;num6=temp%1000/100;num7=temp%100/10;num8=temp%10;
		num1=num2=num3=num4=10;		
	}	
	else if(temp>9999)
	{
		num4=temp/10000;num5=temp%10000/1000;num6=temp%1000/100;num7=temp%100/10;num8=temp%10;
		num1=num2=num3=10;	
	}	
}





void time0()
{
	TMOD=(TMOD&0xf0)|0x05;
	TH0=TH0=0;
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
	time0();
	time1_int1();
	num1=num2=num3=num4=num5=num6=num7=num8=10;
	while(1)
	{
	 if(f_2ms==1)
	 {
	  f_2ms=0;
		 
    f_display();
		 
		
		 
		 
		 
		 
		 
		 
	
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
		if(flag==0)
		{
			TH0=TL0=0;
			TR0=1;
		}
		else
		{
			TR0=0;
			temp=((TH0<<8)+TL0)*2;

		}
	}
	
	
	
	
	
	
}//aall




