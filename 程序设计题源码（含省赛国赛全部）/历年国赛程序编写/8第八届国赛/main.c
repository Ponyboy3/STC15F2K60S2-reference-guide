#include <STC15F2K60S2.H>
#include "iic.h"
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned  int 
	
uchar code type1[]={ 	0xC0,
										 	0xF9,
										 	0xA4,
										 	0xB0,
										 	0x99,
										 	0x92,
										 	0x82,
										 	0xF8,
										 	0x80,
										 	0x90,0xff,0x8e};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar t_2ms;
bit f_2ms;
uchar GN;
sbit trig=P1^0;
sbit echo=P1^1;

uchar bdata U6=0xff;
sbit led1=U6^0;
sbit led7=U6^6;
sbit led8=U6^7;
//--------------------
uchar model=0;
uchar distan1,distan2;
uint num_12;
char num_d1,num_d2,num_d3;
bit f_dis=0;
uchar type3_dis[11];
//====================
bit hui=0;
char num_fan;
//====================
bit can=0;
uint mangqu=0;
//====================
bit f_led=0;
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
		case 5:P2=(P2&0x1f)|0xe0;P0=type1[n6];P2=(P2&0x1f);break;
		case 6:P2=(P2&0x1f)|0xe0;P0=type1[n7];P2=(P2&0x1f);break;
		case 7:P2=(P2&0x1f)|0xe0;P0=type1[n8];P2=(P2&0x1f);break;		
	}
	if(++i==8)
		i=0;
}
//=================================key
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
//================================超声波
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
	{
		return ((TH0<<8)+TL0)*0.017;
	}
}
//=====================================24c02
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
//=======================================8591
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
//=======================================dac输出
uint D;
void dac_shuchu()
{	
	D=(type3_dis[num_d3]-mangqu)/50.0;
	D=(D*255.0)/5;
	if(mangqu >= type3_dis[num_d3])
	{
		xie_dac(0);		
	}
	if(mangqu < type3_dis[num_d3])
	{		
		 xie_dac(D);	
	}	
}
//============================================led
uchar count_led;
void led_display()
{
	if(f_led==1)
	{
    if(++t_led==50)
		{
			t_led=0;
			led1=~led1;
			count_led++;
			P0=0xff;	
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);				
		}		
		if(count_led==20)
			{
        count_led=0;
				f_led=0;
			}		
	}
//---------------------------------
  if(model==2)
	{
			P0=0xff;
      led7=0;	led8=1;	led1=1;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);	
	}		
//--------------------------------
  if(model==1)
	{
			P0=0xff;
      led8=0;	led7=1;	led1=1;
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);	
	}	
//-------------------------------	
  if(model==0)
	{
			P0=0xff;
      led7=1;led8=1;		
			P2=(P2&0x1f)|0x80;
			P0=U6;
			P2=(P2&0x1f);	
	}	
}
//========================================保存到24c02
char S,f_save;
void save_24c02()
{	
	if(f_save==1)
	{
		for(S=0;S!=10;S++)
		{
				xie_24c02(S,type3_dis[S]);
				Delay10ms();
		}
				xie_24c02(11,mangqu);
				Delay10ms();	
    f_save=0;
	}
}
//====================================上电时读出EEPROM中的十个距离数据和盲区数据
char du_data;
void du_save()
{
	for(du_data=0;du_data!=10;du_data++)
	{
		type3_dis[du_data]=du_24c02(du_data);
		_nop_();
		_nop_();
		_nop_();
	}
	mangqu=du_24c02(11);
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
	du_save();
	while(1)
	{
	 if(f_2ms==1)
	 {
	  f_2ms=0;
		key_change(); 
		dac_shuchu(); 
		save_24c02();		 
		if(++t_2ms==100)
		{
     t_2ms=0;
		 distan2=csb_send();	
		}
//----------------------------		
		if(GN==4)
		{
      type3_dis[num_d1]=distan2;
			num_d1++;
			if(num_d1==10)
				num_d1=0;
      model=0;
      while(key_scan());
			f_led=1;
			f_save=1;
		}			
		if(GN==5)
		{
			hui=~hui;
			if(hui==1)
			 model=1;
			else
			 model=0;
			while(key_scan());
		}
		if(GN==6)
		{
      can=~can;
			if(can==1)
			  model=2;
			else
			 model=0;				
     while(key_scan());
		}			
//----------------------------
	switch(model)
	{
    case 0:
			    if(GN==7)
					{
						f_dis=~f_dis;
					 while(key_scan());	
					}
				  if(f_dis==0)//上次数据
					{
						if(num_d1==1) num_d2=9; else num_d2=num_d1-2;
						if(num_d1==0) num_d2=8; else num_d2=num_d1-2;
						if(num_d1==0) num_d3=9; else num_d3=num_d1-1;	
																			  						
            num1=0;num2=10;
						num3=type3_dis[num_d2]/100;num4=type3_dis[num_d2]%100/10;num5=type3_dis[num_d2]%10;
            num6=type3_dis[num_d3]/100;num7=type3_dis[num_d3]%100/10;num8=type3_dis[num_d3]%10;
					}						
				  if(f_dis==1)//加操作
					{
						if(num_d1==1) num_d2=9; else num_d2=num_d1-2;
						if(num_d1==0) num_d2=8; else num_d2=num_d1-2;
						if(num_d1==0) num_d3=9; else num_d3=num_d1-1;	          
            
            num_12=type3_dis[num_d2]+type3_dis[num_d3];
            num1=1;num2=10;
						num3=num_12/100;num4=num_12%100/10;num5=num_12%10;
            num6=type3_dis[num_d3]/100;num7=type3_dis[num_d3]%100/10;num8=type3_dis[num_d3]%10;         
					}					
		      break;

		case 1:
			     if(GN==7)
					 {
						 num_fan++;
						 if(num_fan==10)
							 num_fan=0; 
						 while(key_scan());
					 }
		       num1=0;num2=num_fan;
					 num3=num4=num5=10;
					 num6=type3_dis[num_fan]/100;num7=type3_dis[num_fan]%100/10;num8=type3_dis[num_fan]%10;	
		      break;

		case 2:
			     if(GN==7)
					 {
						mangqu+=10;
            if(mangqu > 90)
							mangqu=0;
						while(key_scan()); 
					 }
			     num1=11;num2=num3=num4=num5=10;
		       num6=mangqu/100;num7=mangqu%100/10;num8=mangqu%10;
		       break;

	   }	//switch
		 

	 }//f_2ms
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;		
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	f_2ms=1;
	led_display();
}




