#include <STC15F2K60S2.h>

#include <onewire.h>
#include <iic.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int 
	
uchar type1[]={ 	0xC0,
								 	0xF9,
								 	0xA4,
								 	0xB0,
								 	0x99,
								 	0x92,
								 	0x82,
								 	0xF8,
								 	0x80,
								 	0x90,0xff,0xc6,0x8c};
uchar type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar f_2ms;
uchar t_2ms;
uint tt_2ms;
bit model;
uchar t_max=30,t_min=20;
uchar temp;
uchar lmit;
bit step;
bit shanshuo;

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
		case 0:GN=0;break;		
	}
}
//////////////////////////////dac
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
void dac_out()
{
	if(temp>t_max)
	{
		xie_dac(204);
	P0=0xff;
	P2=(P2&0x1f)|0x80;
	P0=0xfe;
	P2=(P2&0x1f);			
	}
	if((temp>t_min || temp==t_min) && (temp<t_max || temp==t_max))
	{
		xie_dac(153);
	P0=0xff;
	P2=(P2&0x1f)|0x80;
	P0=0xfd;
	P2=(P2&0x1f);			
	}	
	if(temp<t_min)
	{
		xie_dac(102);
	P0=0xff;
	P2=(P2&0x1f)|0x80;
	P0=0xfb;
	P2=(P2&0x1f);			
	}	
	if(t_max<t_min)
	{
	P0=0xff;
	P2=(P2&0x1f)|0x80;
	P0=0xf7;
	P2=(P2&0x1f);			
	}
}
void main()
{
	initial_all();
	time1_int1();
	initial_18b20();
	temp=rd_temperature( );
	while(1)
	{
		if(f_2ms==1)
		{
		 f_2ms=0;
		 key_change();
		  if(++tt_2ms==500)
			{
				tt_2ms=0;
				shanshuo=~shanshuo;
			}
			dac_out();
		 if(GN==1)
		 {
			 if(lmit==0)
         model=~model;
			step=0;
      while(key_scan());
		 }			 
			
			if(model==0)//µçÑ¹ÏÔÊ¾
			{
				if(++t_2ms>100)
				{
					t_2ms=0;
					temp=rd_temperature( );
				}
				num1=11;num2=num3=num4=num5=num6=10;
				num7=temp/10;num8=temp%10;
			}
			
			if(model==1)
			{
				 if(GN==2)
				 {
					step=~step;
					while(key_scan());
				 }		
        if(t_max>t_min || t_max>t_min)
					{
						lmit=0;
					}
				else 
				{
					lmit=1;
				}
				
				
				
				if(step==0)//
				{
				 if(GN==3)
				 {					
					 if(t_min==99)
						 t_min=99;
					 else
						 t_min++;
					while(key_scan());
				 }        
				 if(GN==4)
				 {
					 if(t_min==0)
						 t_min=0;
					 else
						 t_min--;
					while(key_scan());
				 }					
					num1=12;num2=num3=10;
				  num4=t_max/10;num5=t_max%10;
				  num6=10;
				 if(shanshuo==0)
				 {
				  num7=t_min/10;num8=t_min%10;
				 }
					else 
					{
						num7=10;num8=10;	
					}
				}//
				
				if(step==1)//
				{
				 if(GN==3)
				 {					
					 if(t_max==99)
						 t_max=99;
					 else
						 t_max++;
					while(key_scan());
				 }        
				 if(GN==4)
				 {
					 if(t_max==0)
						 t_max=0;
					 else
						 t_max--;
					while(key_scan());
				 }					
					num1=12;num2=num3=10;
				  num7=t_min/10;num8=t_min%10;
				  num6=10;
				 if(shanshuo==0)
				 {
					 num4=t_max/10;num5=t_max%10;
				 }
					else 
					{
						num4=10;num5=10;
					}						
				}
	
			}/////
	
		}//f_2ms=1
	}
}
	
void cl_time1_int1() interrupt 3
{
  TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	f_2ms=1;
	
}


