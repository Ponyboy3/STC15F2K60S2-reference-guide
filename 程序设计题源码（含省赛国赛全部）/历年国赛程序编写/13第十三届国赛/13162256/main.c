#include <STC15F2K60S2.H>
#include "iic.h"
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
	
uchar code type1[]={0xc0,  
										0xf9,  
										0xa4,   
										0xb0,  
										0x99, 
										0x92,  
										0x82,  
										0xf8,  
										0x80,  
										0x90,0xff,0x8e,0x89,0x88,0x8c };
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
bit f_2ms;
uchar t_2ms;
sbit trig=P1^0;
sbit echo=P1^1;
uchar bdata U6=0xff;
sbit led1=U6^0;
sbit led2=U6^1;
sbit led3=U6^2;
sbit led4=U6^3;
sbit led5=U6^4;
sbit led6=U6^5;

uchar bdata U9;
sbit relay=U9^4;
sbit mot=U9^5;

bit f_hz;
uint temp_f,temp_kf;
//------------------------------
bit f_lim;
bit f_danwei=0;
uchar model_ce;
uchar model_can;
//--------------
uchar t_adc;
uchar temp_shidu;
uchar distance;
bit dis_danwei;
//----------------
uchar f_can=90;
uchar c_can=40;
uchar d_can=6;

//-----------------
bit f_relay;
uint count_relay;
bit f_start;
uint t_rel;



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
	P2=(P2&0x1f)|0xc0;
	P0=type2[i];
	P2=(P2&0x1f);
	P0=0xff;
	switch(i)
	{
	  case 0:P2=(P2&0x1f)|0xe0;P0=type1[n1];P2=(P2&0x1f);break;
	  case 1:P2=(P2&0x1f)|0xe0;P0=type1[n2];P2=(P2&0x1f);break;
		case 2:P2=(P2&0x1f)|0xe0;P0=type1[n3];P2=(P2&0x1f);break;
		case 3:P2=(P2&0x1f)|0xe0;P0=type1[n4];P2=(P2&0x1f);break;
		case 4:P2=(P2&0x1f)|0xe0;P0=type1[n5];P2=(P2&0x1f);break;
		case 5:P2=(P2&0x1f)|0xe0;
		                         if(model_ce==2 && dis_danwei==1)
														 {
															 P0=type1[n6]&0x7f;
														 }		
		                         else
															 P0=type1[n6];	
														 
		                         P2=(P2&0x1f);break;
		case 6:P2=(P2&0x1f)|0xe0;
		                         if(f_danwei==1 && model_ce==0)
														 {
															 P0=type1[n7]&0x7f;
														 }														 
//		                         else
//															 P0=type1[n7];															 
														 else if(model_ce==3 && model_can==0)
														 {
                               P0=type1[n7]&0x7f;
														 }
//		                         else
//															 P0=type1[n7];
														 else if(model_ce==3 && model_can==2)
														 {
                               P0=type1[n7]&0x7f;
														 }
		                         else
															 P0=type1[n7];															 
		                         P2=(P2&0x1f);break;
		case 7:P2=(P2&0x1f)|0xe0;P0=type1[n8];P2=(P2&0x1f);break;	
	}
	if(++i==8)
		i=0;
}
//========================================key
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
//========================================超声波
uchar f_count=10;
uchar csb_send()
{
	f_count=10;
	trig=0;
	TR0=1;
	while(f_count--)
	{
		while(!TF0);
		trig ^= 1;
		TH0=(65535-12)/256;
		TL0=(65535-12)%256;			
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
		return 999;
	}
	else
		return ((TH0<<8)+TL0)*0.017;
	
}
//===================================24c02
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
//=========================================adc
void initial_adc()
{
	IIC_Start( ); 
	IIC_SendByte(0x90+0); 
	IIC_WaitAck( );  
	IIC_SendByte(0x3); 
	IIC_WaitAck( );	
	IIC_Stop( );	
}
uchar  du_adc()
{
	uchar D;
	IIC_Start( ); 
	IIC_SendByte(0x90+1); 
	IIC_WaitAck( );	
	D=IIC_RecByte( );
	IIC_SendAck(1);
	IIC_Stop( );
	D=(D*100.0)/255;
	return D;		
}
//========================================DAC
void xie_dac(uchar D)
{
	IIC_Start( ); 
	IIC_SendByte(0x90+0); 
	IIC_WaitAck( );  
	IIC_SendByte(0x40); 
	IIC_WaitAck( );		
	IIC_SendByte(D); 
	IIC_WaitAck( );		
	IIC_Stop( );	
}
//========================================数据处理
void shuju(uint D)
{
if(f_danwei==0)
{
	if(D<10)
	{
		if(f_danwei==0)
		{
		num4=num5=num6=num7=10;
		num8=D;		
		}	
	}
	if(D>10 && D<100)
	{
		if(f_danwei==0)
		{
		num4=num5=num6=10;
		num7=D/10;num8=D%10;						
		}
	}	
	if(D>99 && D<1000)
	{
		if(f_danwei==0)
		{
		num4=num5=10;
		num6=D/100;num7=D%100/10;num8=D%10;							
		}
	}
	if(D>999 && D<10000)
	{
		if(f_danwei==0)
		{
		num4=10;
		num5=D/1000;num6=D%1000/100;num7=D%100/10;num8=D%10;						
		}
	}
	if(D>9999)
	{	
   if(f_danwei==0)	
	 {
     num4=D/10000;num5=D%10000/1000;num6=D%1000/100;num7=D%100/10;num8=D%10;	
	 }	
 }

}	
		if(f_danwei==1)
		{
			if(D<100)
			{
       num4=num5=num6=10;num7=0;num8=0;
			}	
      if(D>99 && D<1000)
			{
       num4=num5=num6=10;num7=0;num8=D/100;
			}
      if(D>990 && D<10000)
			{
       num4=num5=num6=10;num7=D/1000;num8=D%1000/100;
			}
      if(D>9999)
			{
       num4=num5=10;num6=D/10000;num7=D%10000/1000;num8=D%1000/100;
			}				
		}

	}		
////==========================================dac湿度输出
	uchar ds;
void shidu_output()
{
	if(temp_shidu < c_can)
	{
		xie_dac(51);
	}
	if(temp_shidu >= 100)
	{
		xie_dac(255);		
	}	
	if(temp_shidu>c_can && temp_shidu<100)
	{
		ds=temp_shidu*2.55;	
	  xie_dac(ds);		
	}
}
//=============================================距离关于继电器
void relay_output()
{
	if(distance > d_can*10)
	{
		if(f_relay==1)
		{
			count_relay++;
			f_relay=0;
			xie_24c02(0,count_relay);
			Delay10ms();
		}
		relay=1;	
		P2=(P2&0x1f)|0xa0;
		P0=U9;
		P2=(P2&0x1f);			
	}
	if(distance <= d_can*10)
	{
		f_relay=1;
		relay=0;	
		P2=(P2&0x1f)|0xa0;
		P0=U9;
		P2=(P2&0x1f);			
	}	
}
//=====================================mot频率输出
uchar z_2ms;
uchar x_2ms;
void out_output()
{
	if(temp_f > f_can*100)
	{
		if(++z_2ms==10)
		{
			z_2ms=0;			
			mot=1;		
		}
		if(z_2ms==2)
		{
			mot=0;
		}
			P2=(P2&0x1f)|0xa0;
			P0=U9;
			P2=(P2&0x1f);			
	}

	if(temp_f <= f_can*100)
	{
		if(++z_2ms==10)
		{
			t_2ms=0;
			mot=1;			
		}
		if(z_2ms==8)
		{
			mot=0;
		}	
			P2=(P2&0x1f)|0xa0;
			P0=U9;
			P2=(P2&0x1f);			
	 }			
}
//============================================led
uchar t_led1;

void led_display()
{
//====================================led1	
	if(model_ce==0)
		{
			led1=0;led2=1;led3=1;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);					
		}
	if( model_can==0)
	{
		if(++t_led1==50)
		{
			t_led1=0;
			led1=~led1;			
		}
		  led2=1;led3=1;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);					
	}
//================================	led2
	if(model_ce==1)
		{
			led1=1;led2=0;led3=1;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);					
		}
	if( model_can==1)
	{
		if(++t_led1==50)
		{
			t_led1=0;
			led2=~led2;			
		}
		  led1=1;led3=1;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);					
	}	
//=======================================	led3
	if(model_ce==2)
		{
			led1=1;led2=1;led3=0;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);					
		}
	if( model_can==2)
	{
		if(++t_led1==50)
		{
			t_led1=0;
			led3=~led3;			
		}
		  led1=1;led2=1;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);					
	}		
//===============================led4	

	if(temp_f > f_can*100)
	{   
		  led4=0;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);		
	}
	if(temp_f < f_can*100)
	{
		  led4=1;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);					
	}
//===============================led5
	if(temp_shidu > c_can)
	{
		  led5=0;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);				
	}
	if(temp_shidu <= c_can)
	{
		  led5=1;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);			
	}
//====================================led6
if(distance > d_can*10)
{
		  led6=0;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);	
}
	if(distance <= d_can*10)
	{
		  led6=1;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);			
	}
}
//==========================
void time0()
{
	if(f_lim==1)
	 TMOD=(TMOD&0xf0)|0x05;
	if(f_lim==0)
	 TMOD=(TMOD&0xf0)|0x01;	
	
	TH0=(65535-12)/256;
	TL0=(65535-12)%256;		
}

void time1_int1()
{
	EA=1;
	ET1=1;
	TR1=1;
	TMOD=(TMOD&0x1f)|0x10;
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;		
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
	uchar a;
	initial_all();
	time1_int1();
	time0();	
	initial_adc();
	count_relay=du_24c02(0);
	while(1)
	{
	  if(f_2ms)
		{
	   f_2ms=0;
		 key_change();	
		 relay_output();
     led_display();	
     out_output();	
     shidu_output();			
//----------------------------
    if(GN==4)
		{
			model_can=0;a=0;
			model_ce++;
			if(model_ce==4)
				model_ce=0;
			while(key_scan());			
		}
//----------------------------
		switch(model_ce)
		{
			case 0:
				     if(GN==7)
						 {
							 f_danwei=~f_danwei;
							 while(key_scan());
						 }
				     f_lim=1;///////////////
						 if(a==0)
						 {
							  time0();
							  a=1;
						 }						
			       if(f_danwei==0)
						 {
							 num1=11;num2=num3=10;
//							 num4=temp_f/10000;num5=temp_f%10000/1000;num6=temp_f%1000/100;
//							 num7=temp_f%100/10;num8=temp_f%10;
							 shuju(temp_f);							 
						 }
						 if(f_danwei==1)
						 {
							 num1=11;num2=num3=10;
							 
							 shuju(temp_f);	
						 }
			       break;
			case 1:
				     if(GN==7)
						 {
							f_start=1; 
							while(key_scan());
              f_start=0;t_rel=0;							 
						 }
				     if(++t_adc==100)
						 {
							 t_adc=0;
							 initial_adc();
							 temp_shidu=du_adc();
						 }
				     num1=12;num2=num3=num4=num5=num6=10;
			       num7=temp_shidu/10;num8=temp_shidu%10;			
			       break;
			case 2:
				     if(GN==6)
						 {
							 dis_danwei=~dis_danwei;							 
							while(key_scan()); 
						 }
				     f_lim=0;///////////
						 if(a==0)
						 {
							  time0();
							  a=1;
						 }						 
			       distance=csb_send();
			       if(dis_danwei==0)
						 {
							 num1=13;num2=num3=num4=num5=10;
							 if(distance<10)
							 {
								 num6=num7=10;num8=distance;
							 }
							 if(distance>10 && distance<100)
							 {
								 num6=10;num7=distance/10;num8=distance%10;
							 }
							 if(distance>100)
							 {
								 num6=distance/100;num7=distance%100/10;num8=distance%10;
							 }							 
						 }
						 if(dis_danwei==1)
						 {
							 num1=13;num2=num3=num4=num5=10;
							 num6=distance/100;num7=distance%100/10;num8=distance%10;						 
						  }							  	
			       break;
			case 3://参数界面
				     if(GN==5)
						 {
							 model_can++;
							 if(model_can==3)
								 model_can=0;							 
							 while(key_scan()); 
						 }
			       switch(model_can)
						 {
							 case 0://频率
								      if(GN==6)
											{
												 if(f_can==120) f_can=10; else f_can+=5; while(key_scan()); 	 
											}
								      if(GN==7)
											{
												 if(f_can==10) f_can=120; else f_can-=5; while(key_scan()); 	 
											}											
							        num1=14;num2=1;num3=num4=num5=10;
                      num6=f_can/100;num7=f_can%100/10;num8=f_can%10;								 
							        break;
					
							 case 1://湿度
								      if(GN==6)
											{
												 if(c_can==60) c_can=10; else c_can+=10; while(key_scan()); 	 
											}
								      if(GN==7)
											{
												 if(c_can==10) c_can=60; else c_can-=10; while(key_scan()); 	 
											}											
							        num1=14;num2=2;num3=num4=num5=num6=10;
                      num7=c_can/10;num8=c_can%10;								 
							        break;
							 case 2://距离
								      if(GN==6)
											{
												 if(d_can==12) d_can=1; else d_can+=1; while(key_scan()); 	 
											}
								      if(GN==7)
											{
												 if(d_can==1) d_can=12; else d_can-=1; while(key_scan()); 	 
											}											
							        num1=14;num2=3;num3=num4=num5=num6=10;
                      num7=d_can/10;num8=d_can%10;								 
							        break;																					
						  }	//内					
			      break;
		}//model_ce
		




			
//			num1=csb_send()/10;num2=csb_send()%10;
//			num3=du_24c02(0);
//			num4=du_adc()/10;num5=du_adc()%10;
//			num6=6;num7=7;num8=8;
			
		}//f_2ms
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65535-2000)/256;
	TL1=(65535-2000)%256;	
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	f_2ms=1;
	
if(f_lim==1)
{
	if(++t_2ms==250)
	{
		t_2ms=0;
		f_hz=~f_hz;
		
		if(f_hz)
		{
			TH0=0;
			TL0=0;
			TR0=1;	
		}
		else
		{
			TR0=0;
			temp_f=((TH0<<8)+TL0)*2;
		}	
	}
}	
//======================
if(f_start==1)
{
	if(++t_rel==500)
	{
		t_rel=0;
		count_relay=0;
	}	
}

}





