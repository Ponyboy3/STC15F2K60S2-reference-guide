#include <STC15F2K60S2.H>
#include "iic.h"
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
										 	0x90,0xff,0xc1,0x8c,0xc3,0x88};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

uchar num1,num2,num3,num4,num5,num6,num7,num8;
bit f_2ms;
uchar t_2ms;
uchar GN;
sbit trig=P1^0;
sbit echo=P1^1;

uchar bdata U6=0xff;
sbit led1=U6^0;
sbit led2=U6^1;
sbit led3=U6^2;
sbit led8=U6^7;
//======================
uchar model=0;
uint volt=0, H_volt=50, L_volt=5;
uint H_V,L_V;
bit F_HL=0;
//======================
bit f_csb=0;
uchar tt_2ms;
uint csb_juli;
//======================
uchar t_led;



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
	P0=0xff;
	P2=(P2&0x1f)|0xc0;
	P0=type2[i];
	P2=(P2&0x1f);
	P0=0xff;
	switch(i)
	{
		case 0:P2=(P2&0x1f)|0xe0; P0=type1[n1]; P2=(P2&0x1f);break;
		case 1:P2=(P2&0x1f)|0xe0; P0=type1[n2]; P2=(P2&0x1f);break;		
		case 2:P2=(P2&0x1f)|0xe0; P0=type1[n3]; P2=(P2&0x1f);break;		
		case 3:P2=(P2&0x1f)|0xe0; 
		                          if(model==1)
		                             P0=type1[n4]&0x7f;
		                          else
															   P0=type1[n4];	
		                          P2=(P2&0x1f);break;		
		case 4:P2=(P2&0x1f)|0xe0; P0=type1[n5]; P2=(P2&0x1f);break;		
		case 5:P2=(P2&0x1f)|0xe0; 
                             if(model==0)
														 {
															 P0=type1[n6]&0x7f;  
														 }
		                         else
															 P0=type1[n6];
														 
															P2=(P2&0x1f); 		
		                                 break;		
		case 6:P2=(P2&0x1f)|0xe0;
		                          if(model==1)
		                             P0=type1[n7]&0x7f;
		                          else
															   P0=type1[n7];	
                              P2=(P2&0x1f);break;		
		case 7:P2=(P2&0x1f)|0xe0; P0=type1[n8]; P2=(P2&0x1f);break;		
		
	}
	if(++i==8)
		i=0;
}
//========================================按键
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
}

//=======================================超声波
uchar f_flag=10;
uchar csb_send()
{
	f_flag=10;
	trig=0;
	TR0=1;
	while(f_flag--)
	{
		while(!TF0);
		trig ^= 1;
		TH0=(65536-12)/256;
		TL0=(65536-12)%256;			
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
		return 255;
	}
	else
		return ((TH0<<8)+TL0)*0.017;

}
//====================================AD/DA
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
	D=(D*500.0)/255;
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
//===================================LED
void led_display()
{
	switch(model)
	{
		case 0:
			      P0=0xff;
		        led1=0;led2=1;led3=1;
						P2=(P2&0x1f)|0x80;
						P0=U6;
						P2=(P2&0x1f);				    
		      break;
		case 1:
			      P0=0xff;
		        led1=1;led2=1;led3=0;
						P2=(P2&0x1f)|0x80;
						P0=U6;
						P2=(P2&0x1f);				    
		      break;		
		case 2:
			      P0=0xff;
		        led1=1;led2=0;led3=1;
						P2=(P2&0x1f)|0x80;
						P0=U6;
						P2=(P2&0x1f);				    
		      break;		
	}
	
	if(f_csb==1)
	{
		if(++t_led==50)
		{
			t_led=0;
			led8=~led8;
			      P0=0xff;
						P2=(P2&0x1f)|0x80;
						P0=U6;
						P2=(P2&0x1f);				
		}				
	}
	else
	{
		led8=1;
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=U6;
		P2=(P2&0x1f);			
	}	
}
//=================================dac
uchar G;
void dac_chuli()
{
	
	if(f_csb==1)//启用
	{
		if(csb_juli<20)
		{
			xie_dac( 51 );
		}
		 		
		if(csb_juli>80)
		{
			xie_dac( 255 );
		}		 
		if(csb_juli>20 &&  csb_juli<80)
		{
			 G=(40*csb_juli)/12-18;
			 xie_dac( G );
		}		
	}
	
	else//未启用
	{
    xie_dac( 0 );	
	}
}

void time0()
{
	TMOD=(TMOD&0xf0)|0x01;
	TH0=(65536-12)/256;
	TL0=(65536-12)%256;		
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
	time1_int1();
	time0();
	
	initial_adc();
	
	
	while(1)
	{
	 if(f_2ms==1)
	 {
		 f_2ms=0;
	   key_change();
	   led_display();

		 
		if(++t_2ms==100)
		{
			t_2ms=0;
			initial_adc();
			volt=du_adc(); dac_chuli();
		}	
		if(++tt_2ms==50)
		{
			tt_2ms=0;
			csb_juli=csb_send();
		}	
		
		 if(GN==4)
		 {
			 model++;
			 if(model==3)
				 model=0;			 
			 while(key_scan());
		 }
		 
//==================================
		 switch(model)
		 {
			 case 0:
							
			        num1=11;num2=num3=num4=num5=10;
							num6=volt/100;num7=volt%100/10;num8=volt%10;
							
							F_HL=0;//默认选择上限
			   break;
			 case 1:
							if(GN==5)
							{
                F_HL=~F_HL;
								while(key_scan());
							}
							if(F_HL==0)//上限
							{
								if(GN==6)
								{
									if(H_volt==50) H_volt=5; else H_volt+=5;	while(key_scan());	   							
								}
								if(GN==7)
								{
									if(H_volt==5) H_volt=50; else H_volt-=5;	while(key_scan());   								
								}								
							}
							else//下限
							{
								if(GN==6)
								{
									if(L_volt==50) L_volt=5; else L_volt+=5;		 while(key_scan());  							
								}
								if(GN==7)
								{
									if(L_volt==5) L_volt=50; else L_volt-=5;	   while(key_scan());								
								}										
							}
								num1=12;num2=num3=10;
								num4=H_volt/10;num5=H_volt%10;
								num6=10;
								num7=L_volt/10;num8=L_volt%10;
			   break;			 
			 case 2:
							H_V=H_volt*10;
							L_V=L_volt*10;
							//---------------------------
               if(L_V< volt && volt < H_V)
								 f_csb=1;
			         else
								 f_csb=0;
			         //--------------------------
							 
							 if(f_csb==1)
							 {
								 num1=13;num2=num3=num4=num5=10;
								 if(csb_juli<10)
								 {
									 num6=num7=10;
									 num8=csb_juli;									 
								 }
								 if(9<csb_juli<100)
								 {
									 num6=10;
									 num7=csb_juli/10;num8=csb_juli%10;									 
								 }
								 if(csb_juli>99)
								 {
									 num6=csb_juli/100;num7=csb_juli%100/10;num8=csb_juli%10;									 
								 }
							 }
							 
							 if(f_csb==0)
							 {
								 num1=13;num2=num3=num4=num5=10;
								 num6=num7=num8=14;
							 }
			   break;			  
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












