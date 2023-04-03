#include <STC15F2K60S2.h>
#include <iic.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int 
	
uchar code type1[]={
	                   	0xC0,
										 	0xF9,
										 	0xA4,
										 	0xB0,
										 	0x99,
										  0x92,
										 	0x82,
										  0xF8,
										 	0x80,
										 	0x90,0xff,0xbf};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uint type3[]={0xff,0xff,0xff,0xff};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar f_2ms;
uint t_2ms,tt_2ms;

uint temp1,temp2,a;
bit flag;


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
		case 5:P2=(P2&0x1f)|0xe0;P0=type1[n6];P2=(P2&0x1f);break;		
		case 6:P2=(P2&0x1f)|0xe0;P0=type1[n7];P2=(P2&0x1f);break;		
		case 7:P2=(P2&0x1f)|0xe0;P0=type1[n8];P2=(P2&0x1f);break;		
	}
	if(++i==8)
		i=0;
}
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
void key_change ()
{
	uchar D;
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
/////////////////////////////////
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
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;	
}


void main()
{
	P2=(P2&0x1f)|0xa0;
	P0=0x00;
	P2=(P2&0x1f);	
	P2=(P2&0x1f)|0x80;
	P0=0xff;
	P2=(P2&0x1f);		
	time1_int1();
	while(1)
	{
	  if(f_2ms==1)
		{
	   f_2ms=0;
	   key_change ();
     if(++t_2ms==10)
		 {
			 t_2ms=0;
			 flag=~flag;
			 
			 if(flag==0)
			 {
				 initial_adc(0x1);
				 type3[a]=du_adc();
				 if(++a==3)
				 {
					 a=0;
				 temp1=(type3[0]+type3[1]+type3[2])/3;
				 //temp1=type3[1];
				 }
			 }
			 if(flag==1)
			 {
				 initial_adc(0x3);
				 temp2=du_adc();				  
			 }
			}			 
		 
		num1=temp1/100;	num2=temp1%100/10;	num3=temp1%10;	
		num4=num5=10;	
		num6=temp2/100;	num7=temp2%100/10;	num8=temp2%10;	
			

			

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