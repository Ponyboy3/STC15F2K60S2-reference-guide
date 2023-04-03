#include <STC15F2K60S2.h>
#include <iic.h>
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
										 	0x90,0xff,0x8e,0xc1};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar f_2ms;
uchar t_2ms;

uint temp_f,temp_u;
bit model=0;
bit dac_out=0;
bit led_tp;
bit smg_tp;
bit f;




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

void smg_display(uchar n1,uchar n2,uchar n3,uchar n4,uchar n5,uchar n6,uchar n7,uchar n8)
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
		case 5:P2=(P2&0x1f)|0xe0;
		                         if(model==0 && smg_tp==0)
		                          P0=type1[n6]&0x7f;
														 else if(model==1 || smg_tp==1)
															 P0=type1[n6];
														 
														 P2=(P2&0x1f);break;
		
		case 6:P2=(P2&0x1f)|0xe0;P0=type1[n7];P2=(P2&0x1f);break;		
		case 7:P2=(P2&0x1f)|0xe0;P0=type1[n8];P2=(P2&0x1f);break;		
	}
	if(++i==8)
		i=0;
}
////////////////////////////key
uchar key_scan()
{
	char temp;
	temp=(~P3)&0x0f;
	if(temp==0) return 0;
	Delay10ms();
	temp=(~P3)&0x0f;
	if(temp==0) return 0;	
return temp;		
}
void key_change()
{
	char D;
	D=key_scan();
	switch(D)
	{
		case 8:GN=1;break;
		case 4:GN=2;break;		
		case 2:GN=3;break;		
		case 1:GN=4;break;		
		case 0:GN=0;break;			
	}	
}
////////////////////////////iic
void initial_adc()
{
	IIC_Start( );
	IIC_SendByte(0x90+0); 
	IIC_WaitAck( );
	IIC_SendByte(0x3); 
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
  D=D*(500.0/255);	
	return D;
}
void xie_dac(uchar dat)
{
	IIC_Start( );
	IIC_SendByte(0x90+0); 
	IIC_WaitAck( );
	IIC_SendByte(0x40); 
	IIC_WaitAck( );	
	IIC_SendByte(dat); 
	IIC_WaitAck( );		
	IIC_Stop( ); 	
}


void time0()
{
	TMOD=(TMOD&0xf0)|0x05;
	TH1=0;
	TL1=0;	
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
void led_display()
{

	/////////////////////////////
if(model==0)
{	
	if(temp_u<150)
	{
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0xff&0xfe;
		P2=(P2&0x1f);				
	}
	if(temp_u>150 && temp_u<250)
	{
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0xfb&0xfe;
		P2=(P2&0x1f);				
	}	
	if(temp_u>250 && temp_u<350)
	{
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0xff&0xfe;
		P2=(P2&0x1f);				
	}		
	if(temp_u>350)
	{
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0xfb&0xfe;
		P2=(P2&0x1f);				
	}		
}	
////////////////////////////////////	
if(model==1)
{
  if(temp_f<1000)
	{
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0xff&0xfd;
		P2=(P2&0x1f);				
	}
  if(temp_f>1000 && temp_f<5000)
	{
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0xf7&0xfd;
		P2=(P2&0x1f);			
	}
  if(temp_f>5000 && temp_f<10000)
	{
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0xff&0xfd;
		P2=(P2&0x1f);			
	}
  if(temp_f>10000)
	{
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0xf7&0xfd;
		P2=(P2&0x1f);			
	}
}	
//////////////////////////
if(dac_out==0)
{
//		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P04=1;
		P2=(P2&0x1f);		
}
if(dac_out==1)
{
	  
//	  P0=0xff;
		P2=(P2&0x1f)|0x80;
		P04=0;
		P2=(P2&0x1f);		
}	
 //////////////////////
}

void main()
{
	P2=(P2&0x1f)|0xa0;
	P0=0x00;
	P2=(P2&0x1f);	
	P2=(P2&0x1f)|0x80;
	P0=0xff;
	P2=(P2&0x1f);
  time0();	
	time1_int1();
	initial_adc();
	while(1)
	{

	 if(f_2ms==1)
	 { 
	   f_2ms=0;
		 key_change();

		if(led_tp==0)
	   led_display();		 
		 
		 if(GN==1)
		 {
			 model=~model;
			 while(key_scan()); 
		 }
		 if(GN==2)
		 {
			 dac_out=~dac_out;
			 while(key_scan()); 
		 }
		 if(GN==3)
		 {
			 led_tp=~led_tp;
			 while(key_scan()); 
		 }		 
		 if(GN==4)
		 {
			 smg_tp=~smg_tp;
			 while(key_scan()); 
		 }		 
		 

		 if(led_tp==1)
		 {
			  P0=0xff;
				P2=(P2&0x1f)|0x80;
				P0=0xff;
				P2=(P2&0x1f);			 
		 }
		 
		 
		 if(dac_out==0)
		 {
			 xie_dac(100); 
		 }
		 if(dac_out==1)
		 {
			 xie_dac(temp_u/2);  
		 }		 
		 		 
		 
		 
//////////////////////////////////³ÌÐò		 

		 
		 if(model==0) 
		 {
 			  if(++t_2ms>100)
					{
						t_2ms=0;
						initial_adc();
						temp_u=du_adc();	
//						if(temp_u>250)
//							temp_u=250;
//						temp_u=temp_u*2;
					}     
			    num1=12;num2=num3=num4=num5=10;
					num6=temp_u/100;num7=temp_u%100/10;num8=temp_u%10; 
		 }//model==0
		 
		 if(model==1)
		 {

				num1=11;num2=10;
			 if(temp_u<10)
			 {
				 num3=num4=num5=num6=num7=10;
				 num8=temp_f;
			 }			 
			 if(temp_f>9 && temp_f<100)
			 {
				 num3=num4=num5=num6=10;
				 num7=temp_f/10;num8=temp_f%10;
			 }				 
			 if(temp_f>99 && temp_f<1000)
			 {
				 num3=num4=num5=10;
				 num6=temp_f/100;num7=temp_f%100/10;num8=temp_f%10;
			 }				 
			 if(temp_f>999 && temp_f<10000)
			 {
				 num3=num4=10;
				 num5=temp_f/1000;num6=temp_f%1000/100;num7=temp_f%100/10;num8=temp_f%10;
			 }				 
			 if(temp_f>9999 && temp_f<100000)
			 {
				 num3=10;
				 num4=temp_f/10000;num5=temp_f%10000/1000;num6=temp_f%1000/100;num7=temp_f%100/10;num8=temp_f%10;
			 }				 
			 if(temp_f>100000)
			 {				 
				 num3=temp_f/100000;num4=temp_f%100000/10000;num5=temp_f%10000/1000;num6=temp_f%1000/100;num7=temp_f%100/10;num8=temp_f%10;
			 }			 
		 }//model==1		 
		 

		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
	
	 }//f_2ms
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;	
	if(smg_tp==0)
	  smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	if(smg_tp==1)
		smg_display(10,10,10,10,10,10,10,10);
	
	
	f_2ms=1;
	
	if(++t_2ms==250)
	{
		t_2ms=0;
		f=~f;
		if(f==0)
		{
			TH0=TL0=0;
			TR0=1;
		}
		else
		{
			TR0=0;
			temp_f=((TH0<<8)+TL0)*2;			
		}
		
	}
	
	
	
}












