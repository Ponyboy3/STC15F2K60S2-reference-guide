//#include <STC15F2K60S2.h>
#include <reg52.h>
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
uint type3[]={0xff,0xff,0xff};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar f_2ms;
uint t_2ms;
uint temp_vv,temp_v;
uchar grade;
char a,b,c;

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
///////////
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
		case 0:GN=0;break;		
	}

}
////////////////////////////////////adc
void initial_adc()
{
	IIC_Start( ); 
	IIC_SendByte(0x90+0);
	IIC_WaitAck( );
	IIC_SendByte(0x1);
	IIC_WaitAck( );	
	IIC_Stop( );
}
uint du_adc()
{
	uint D;
	IIC_Start( ); 
	IIC_SendByte(0x90+1);
	IIC_WaitAck( );	
	D=IIC_RecByte(  );
	IIC_SendAck(1);
	IIC_Stop( );

	D=D*(500.0/255);
//	D=(D*500)/255;
	return D;	
}
////////////////////////////////24c02
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
	D=IIC_RecByte(  );
	IIC_SendAck(1);
	IIC_Stop( );
	return D;	
}

void led_display()
{
	if(c==0)
	{
	if(temp_v<100 || temp_v==100)
	{
		EA=0;
    grade=5;
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0x00;
		P2=(P2&0x1f);
    EA=1;
	}

if(temp_v>100 && (temp_v<200 || temp_v==200))
{
		EA=0;
    grade=4;
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0xc0;
		P2=(P2&0x1f);
    EA=1;			
}

	if(temp_v>200 && (temp_v<300 || temp_v==300))
	{
		EA=0;
		grade=3;
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0xf0;
		P2=(P2&0x1f);	
    EA=1;		
	}		
	if(temp_v>300 && (temp_v<400 || temp_v==400))
	{
		EA=0;
		grade=2;
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0xfc;
		P2=(P2&0x1f);
    EA=1;		
	}		
	if(temp_v>400 && (temp_v<500 || temp_v==500))
	{
		EA=0;
		grade=1;
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0xfe;
		P2=(P2&0x1f);	
    EA=1;		
	}	
}

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
	
	
	initial_adc();
	while(1)
	{
	  if(f_2ms==1)
		{
	   f_2ms=0;
			key_change ();
	   led_display();
			
			
	   if(GN==1)
		 {
			 b=1;
			 while(key_scan());
		 }
		 if(GN==2)
		 {
		  c=1;
			  num1=11;num2=du_24c02(0);num3=11;
			  num4=num5=10;
       	num6=(du_24c02(1)*2)/100;num7=(du_24c02(1)*2)%100/10;num8=(du_24c02(1)*2)%10;

		EA=0;
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0xff;
		P2=(P2&0x1f);	
    EA=1;				
	 
			while(key_scan());
			 c=0;
		 }
		 
//		 if(c==1)
//		 {
//			  num1=11;num2=du_24c02(0);num3=11;
//			  num4=num5=10;
//       	num6=(du_24c02(1)*2)/100;num7=(du_24c02(1)*2)%100/10;num8=(du_24c02(1)*2)%10;				  
//		 }

		  

		 if(c==0)
		 {
			  num1=11;num2=grade;num3=11;
			  num4=num5=10;
       	num6=temp_vv/100;num7=temp_vv%100/10;num8=temp_vv%10;					 
		 }
		 
			if(b==1)
			{
				xie_24c02(0,grade);
				Delay10ms();
				xie_24c02(1,temp_vv/2);
				Delay10ms();				
				b=0;
			}
      
			
       if(++t_2ms>100)
			 {
				 t_2ms=0;
				   temp_v=du_adc(); 
				   type3[a]=temp_v;
				   if(++a==3)
					 {
						 a=0;
						 temp_vv=(type3[0]+type3[1]+type3[2])/3;
					   
					 }
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













