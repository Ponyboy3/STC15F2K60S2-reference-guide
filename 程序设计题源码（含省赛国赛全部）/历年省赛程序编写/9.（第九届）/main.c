#include <STC15F2K60S2.h>
#include <iic.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int 
	
uchar code type1[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar code type3[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uchar code type4[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};
uchar code type5[]={0x7e,0xbd,0xdb,0xe7};
uchar code type6[]={0xe7,0xdb,0xbd,0x7e};

	
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar f_2ms;
uint t_2ms,tt_2ms;
uchar t_10ms;

uchar model,led_mod;
char led1,led2,led3,led4;
char a,b;
uint gap=400;
bit f_led;
bit shanshuo;





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


uchar  key_scan()
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
/////////////////////////////adc
void initial_adc()
{
	IIC_Start( );
	IIC_SendByte(0x90+0);
	IIC_WaitAck( );
	IIC_SendByte(0x3);
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
char adc_change()
{
	uchar D,f;
	D=du_adc();
	if(D<64)
	{
		f=1;a=1;
	}
	if((D>64) && (D<128))
	{
		f=3;a=2;
	}	
	if((D>128) && (D<192))
	{
		f=5;a=3;
	}		
	if((D>192) && (D<256))
	{
		f=7;a=4;
	}	
	return f;
}
/////////////////////////////////24c02
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
	
	initial_adc();
//	if(gap!=400)
//	{
//		xie_24c02(0,40);
//		Delay10ms();Delay10ms();
//	}
  gap=du_24c02(0)*10;
  num1=10;num2=10;num3=10;num4=10;num5=10;num6=10;num7=10;num8=10;
	while(1)
	{
	  if(f_2ms==1)
		{
			f_2ms=0;
		  key_change();
			if(GN==4)
			{
				f_led=~f_led;
				while(key_scan());
			}
		
			
			if(f_led==0)
			{
				P0=0xff;
				P2=(P2&0x1f)|0x80;
				P0=0xff;
				P2=(P2&0x1f);
				b=adc_change();
									if(GN==1)
									{
										
										num1=10;num2=10;num3=10;num4=10;num5=10;num6=10;num7=11;num8=a;
										while(key_scan());
										num1=10;num2=10;num3=10;num4=10;num5=10;num6=10;num7=10;num8=10;
									}					
			}
			
			
			if(f_led==1)
			{
				if(GN==3)
				{
					model++;
					if(model==3)
						model=0;
					while(key_scan());
				}        
        switch(model)
				{
          case 0:
									if(GN==1)
									{
										num1=10;num2=10;num3=10;num4=10;num5=10;num6=10;num7=11;num8=a;
										while(key_scan());
									}							
									
						     num1=10;num2=10;num3=10;num4=10;num5=10;num6=10;num7=10;num8=10;
					       if(led_mod==0)
								 { 	
									 if(++t_10ms==10)
									 {
										 t_10ms=0;
												P0=0xff;
												P2=(P2&0x1f)|0x80;
												P0=type3[led1];
												P2=(P2&0x1f);										 
									 }
									 if(t_10ms==adc_change())
									 {
												P0=0xff;
												P2=(P2&0x1f)|0x80;
												P0=0xff;
												P2=(P2&0x1f);											 
									 }

										 if(++t_2ms==gap/2)
										 {
											 t_2ms=0;
											 led1++;
											 if(led1==8)
												 led1=0; 
										 }									 
								 }
					       if(led_mod==1)
								 {
									 if(++t_10ms==10)
									 {
										 t_10ms=0;
												P0=0xff;
												P2=(P2&0x1f)|0x80;
												P0=type4[led2];
												P2=(P2&0x1f);										 
									 }
									 if(t_10ms==adc_change())
									 {
												P0=0xff;
												P2=(P2&0x1f)|0x80;
												P0=0xff;
												P2=(P2&0x1f);											 
									 }

										 if(++t_2ms==gap/2)
										 {
								         t_2ms=0;
											 led2++;
											 if(led2==8)
												 led2=0; 
										 }									 
								 }
					       if(led_mod==2)
								 {
									 if(++t_10ms==10)
									 {
										 t_10ms=0;
												P0=0xff;
												P2=(P2&0x1f)|0x80;
												P0=type5[led3];
												P2=(P2&0x1f);										 
									 }
									 if(t_10ms==adc_change())
									 {
												P0=0xff;
												P2=(P2&0x1f)|0x80;
												P0=0xff;
												P2=(P2&0x1f);											 
									 }
  
										 if(++t_2ms==gap/2)
										 {
                         t_2ms=0;
											 led3++;
											 if(led3==4)
												 led3=0; 
										 }									 
								 }
					       if(led_mod==3)
								 {
									 if(++t_10ms==10)
									 {
										 t_10ms=0;
												P0=0xff;
												P2=(P2&0x1f)|0x80;
												P0=type6[led4];
												P2=(P2&0x1f);										 
									 }
									 if(t_10ms==adc_change())
									 {
												P0=0xff;
												P2=(P2&0x1f)|0x80;
												P0=0xff;
												P2=(P2&0x1f);											 
									 }
   
										 if(++t_2ms==gap/2)
										 {
                         t_2ms=0;
											 led4++;
											 if(led4==4)
												 led4=0; 
										 }									 
								 }
					       break;
					case 1:
								if(GN==2)
								{									
									if(led_mod==3)
										led_mod=3;
									else 
										led_mod++;
									while(key_scan());
								}						      
								if(GN==1)
								{
									if(led_mod==0)
										led_mod=0;
									else
										led_mod--;
									while(key_scan());
								}						
					      if(++tt_2ms==400)
								{
									tt_2ms=0;
									shanshuo=~shanshuo;										
								}
					        if(shanshuo==0)
									{
										num1=11;num2=led_mod+1;num3=11;
									}
								  else 
									{
										num1=10;num2=10;num3=10;
									}
					        num4=10;
										if(gap>999)
										{
										num5=gap/1000;num6=gap%1000/100;num7=gap%100/10;num8=gap%10;
										}
										else
										{
										num5=10;num6=gap/100;num7=gap%100/10;num8=gap%10;	
										}
									break;
					case 2:				
								if(GN==2)
								{									
									if(gap>1100)
										gap=1200;
									else 
										gap+=100;
									xie_24c02(0,gap/10);
									while(key_scan());
								}						      
								if(GN==1)
								{
									if(gap<500)
										gap=400;
									else
										gap-=100;
									xie_24c02(0,gap/10);
									while(key_scan());
								}						
					      if(++tt_2ms==400)
								{
									tt_2ms=0;
									shanshuo=~shanshuo;										
								}									
					        if(shanshuo==0)
									{
										if(gap>999)
										{
										num5=gap/1000;num6=gap%1000/100;num7=gap%100/10;num8=gap%10;
										}
										else
										{
										num5=10;num6=gap/100;num7=gap%100/10;num8=gap%10;	
										}
									}
								  else 
									{
										num5=10;num6=10;num7=10;num8=10;
									}
									num1=11;num2=led_mod+1;num3=11;
					        num4=10;
									break;														
        }

			}	//¿ªÊ¼ led		
			
			
	
		
		
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














