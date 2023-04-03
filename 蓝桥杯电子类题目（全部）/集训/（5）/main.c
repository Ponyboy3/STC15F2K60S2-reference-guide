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
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar f_2ms;
uint t_2ms;

uchar temp1,temp2,temp3,temp4;




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

		if(++t_2ms==50)
		{		
     t_2ms=0;			
		
			
			
			if(temp1+1 <10)
			{
				xie_24c02(0x01,temp1+1);
				Delay10ms();				
			}
		  else
			{
				temp1=0;
				xie_24c02(0x01,temp1);
				Delay10ms();
			}				


			if(temp2+2 <20)
			{
				xie_24c02(0x03,temp2+2);
				Delay10ms();				
			}
		  else
			{
				temp2=0;
				xie_24c02(0x03,temp2);
				Delay10ms();
			}	


			if(temp3+3 <30)
			{
				xie_24c02(0x05,temp3+3);
				Delay10ms();				
			}
		  else
			{
				temp3=0;
				xie_24c02(0x05,temp3);
				Delay10ms();
			}	
			
		 temp1=du_24c02(0x01);	
		 temp2=du_24c02(0x03);			
		 temp3=du_24c02(0x05);	


			
		num1=temp1/10;num2=temp1%10;	
		num3=11;
		num4=temp2/10;num5=temp2%10;
		num6=10;
		num7=temp3/10;num8=temp3%10;	
		}	
			
			

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