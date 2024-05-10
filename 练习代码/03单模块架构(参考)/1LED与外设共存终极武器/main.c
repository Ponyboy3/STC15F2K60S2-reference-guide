/******************************
LED流水并且不影响继电器动作
*******************************/


#include <STC15F2K60S2.H>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
	
uchar code type1[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uchar f_2ms;
uint t_2ms,tt_2ms;
bit flag;


uchar bdata U6=0xff;
sbit led1=U6^0;
sbit led2=U6^1;
sbit led3=U6^2;
sbit led4=U6^3;
sbit led5=U6^4;
sbit led6=U6^5;
sbit led7=U6^6;
sbit led8=U6^7;

uchar bdata U9;
sbit beep= U9^6;
sbit relay=U9^4;




void time1_int1()
{
	EA=1;
	ET1=1;
	TR1=1;
	TMOD=(TMOD%0x0f)|0x10;
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
	
		if(++t_2ms==250)
		{
			t_2ms=0;
		 beep=~beep;
		// relay=~relay;
     P2=(P2&0x1f)|0xa0;
     P0=U9;
     P2=(P2&0x1f);		 
		}			


//    tt_2ms++;
//		if(tt_2ms==1000)
//		{
//			tt_2ms=0;	
//			led1=0;led2=1;
//		}		
//		
//		if(tt_2ms==500)
//		{
//		  led1=1;led2=0;		
//		}
//	
//     P2=(P2&0x1f)|0x80;
//     P0=U6;
//     P2=(P2&0x1f);			
		
			  led1=0;
				P2=(P2&0x1f)|0x80;
				P0=U6;
				P2=(P2&0x1f);			
		

	
	 }//f_2ms
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;	
	 
	f_2ms=1;
	

}











