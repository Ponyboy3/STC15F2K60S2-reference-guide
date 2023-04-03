#include <STC15F2K60S2.h>
#include <iic.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int 
	
uchar code type1[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar f_2ms;
uchar t_2ms,tt_2ms;
uchar a;
uchar flsh;
uint temp_L;
uint temp_Q;



void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}

void smg_display(uchar n1,uchar n2,uchar n3,uchar n4,uchar n5,uchar n6,uchar n7,uchar n8)
{
	uchar i;
	P0=0xff;
	P2=(P2&0x1f)|0xc0;
	P0=type2[i];
	P2=(P2&0x1f);
	switch(i)
	{
		case 0:P2=(P2&0x1f)|0xe0;P0=type1[n1];P2=(P2&0x1f);break;
		case 1:P2=(P2&0x1f)|0xe0;
		                        if(GN==0)
														{
															P0=type1[n2];
														}
		                        else 
														{
															P0=type1[n2]&0x7f;
														}
		                         P2=(P2&0x1f);break;		
		case 2:P2=(P2&0x1f)|0xe0;P0=type1[n3];P2=(P2&0x1f);break;		
		case 3:P2=(P2&0x1f)|0xe0;P0=type1[n4];P2=(P2&0x1f);break;		
		case 4:P2=(P2&0x1f)|0xe0;P0=type1[n5];P2=(P2&0x1f);break;		
		case 5:P2=(P2&0x1f)|0xe0;
		                        if(GN==0)
														{
															P0=type1[n6];
														}
		                        else 
														{
															P0=type1[n6]&0x7f;
														}
		                         P2=(P2&0x1f);break;																
		case 6:P2=(P2&0x1f)|0xe0;P0=type1[n7];P2=(P2&0x1f);break;		
		case 7:P2=(P2&0x1f)|0xe0;P0=type1[n8];P2=(P2&0x1f);break;			
	}
	i++;
	if(i==8)
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
void key_change()
{
	uchar D;
	D=key_scan();
	switch(D)
	{
		case 8:GN=1;break;
		case 4:GN=2;break;		
		case 2:GN=3;break;		
		case 1:GN=4;break;		
//		case 0:GN=0;break;			
	}	
}
///////////////////////////////adc
void initial_adc()
{
	IIC_Start( );
	IIC_SendByte(0x90+0); 
	IIC_WaitAck( );
	IIC_SendByte(0x1); 
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

  num1=num2=num3=num4=num5=num6=num7=num8=10;	
	time1_int1();
	
	initial_adc();
	while(1)
	{
	  if(f_2ms==1)
		{
	    f_2ms=0;
		  key_change();
      if(GN==4)//¿ª
			{
				if(a==0)
				{
					a=1;
					temp_L=0;
				}
				EA=0;
				P2=(P2&0x1f)|0xa0;
				P0=0x10;
				P2=(P2&0x1f);					
				EA=1;
				if(++t_2ms==50)
				{
					t_2ms=0;
					temp_L++;
					if(temp_L==9999)
					{
						//temp_L=0;
						GN=3;
					}
					num5=temp_L/1000;num6=temp_L%1000/100;num7=temp_L%100/10;num8=temp_L%10;
				}
				num1=10;num2=0;num3=5;num4=0;
					
			}
		  if(GN==3)//¹Ø
			{
				a=0;
				EA=0;
				P2=(P2&0x1f)|0xa0;
				P0=0x00;
				P2=(P2&0x1f);					
				EA=1;	
        temp_Q=temp_L/2;	
        num5=temp_Q/1000;num6=temp_Q%1000/100;num7=temp_Q%100/10;num8=temp_Q%10;				
				num1=10;num2=0;num3=5;num4=0;
			}
		
		if(++tt_2ms==100)
		{
			tt_2ms=0;
			flsh=du_adc();
		}
		if(flsh<65)
		{
				EA=0;
			  P0=0xff;
				P2=(P2&0x1f)|0x80;
				P0=0xfe;
				P2=(P2&0x1f);					
				EA=1;			
		}
		else 
		{
				EA=0;
			  P0=0xff;
				P2=(P2&0x1f)|0x80;
				P0=0xff;
				P2=(P2&0x1f);					
				EA=1;			
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


