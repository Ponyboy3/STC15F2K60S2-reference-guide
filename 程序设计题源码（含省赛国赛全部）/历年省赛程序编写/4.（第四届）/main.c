#include <STC15F2K60S2.h>
#include <iic.h>
#include <ds1302.h>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
	
uchar code type1[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar HZ;
uchar hour,minute,second;
uchar f_2ms;
uchar t_2ms;
uchar shidu=50;
uchar temp,temp1;
bit model;
bit f_auto;
bit beep;

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
	i++;
	if(i==8)
		i=0;
}
///////////////////////////////////anjian
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
//////////////////////////////////24c02
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
///////////////////////////////////////ad
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

//////////////////////////////////1302
void xie_ds1302()
{
	Write_Ds1302_Byte( 0x8e,0x00 );
	Write_Ds1302_Byte( 0x84,0x08 );
	Write_Ds1302_Byte( 0x82,0x30 );
	Write_Ds1302_Byte( 0x80,0x50 );
}
void du_1302()
{
	hour=Read_Ds1302_Byte( 0x85 );   hour=(hour>>4)*10+(hour&0x0f);
	minute=Read_Ds1302_Byte( 0x83 ); minute=(minute>>4)*10+(minute&0x0f);
	second=Read_Ds1302_Byte( 0x81 ); second=(second>>4)*10+(second&0x0f);
}



void time1int1()
{
	EA=1;
	ET1=1;
	TR1=1;
	TMOD=(TMOD&0x0f)|0x10;
	TH1=(65536-2000/(120000/110592))/256;
	TL1=(65536-2000/(120000/110592))%256;
}

void main()
{
	P2=(P2&0x1f)|0xa0;
	P0=0x0;
	P2=(P2&0x1f);	
	time1int1();
  xie_ds1302();
	initial_adc();
	while(1)
	{
    if(f_2ms==1)
		{
			f_2ms=0;
		  key_change();
			
			
		  if(GN==4)
			{				
				model=~model;
				while(key_scan());
			}
		
		  if(model==0) //自动状态
			{
				if(GN==3)
				{
					f_auto=~f_auto;	
					while(key_scan());
				}				
				if(f_auto==0)
				{
					du_1302();
					shidu=du_24c02(0);
					num1=hour/10;num2=hour%10;
					num3=11;
					num4=second/10;num5=second%10;
					num6=10;
					num7=(temp1*100/255)/10;num8=(temp1*100/255)%10;
					if(++t_2ms==100)
					{
						t_2ms=0;
						temp1=du_adc();
					}
					if( (temp1*100/255) < shidu )
					{
						EA=0;
						P2=(P2&0x1f)|0xa0;
						P0=0x10;
						P2=(P2&0x1f);	
            EA=1;						
					}
					else if( (temp1*100/255) > shidu )
					{
						EA=0;
						P2=(P2&0x1f)|0xa0;
						P0=0x00;
						P2=(P2&0x1f);	
            EA=1;						
					}	
				}
				if(f_auto==1)
				{
					num1=num2=11;
					num3=num4=num5=num6=10;
					num7=shidu/10;num8=shidu%10;
					if(GN==2) //阈值+
					{
						shidu=shidu+1;
						if(shidu > 99)
							shidu=99;
					 xie_24c02(0,shidu);
					 while(key_scan());	
					}
					if(GN==1)  //阈值-
					{
						shidu=shidu-1;
						if(shidu < 1)
							shidu=1;
					 xie_24c02(0,shidu);
					 while(key_scan());	
					}					
				}
						EA=0;
				    P0=0xff;
						P2=(P2&0x1f)|0x80;
						P0=0xfe;
						P2=(P2&0x1f);	
            EA=1;						
			}//自动状态
		
		
		if(model==1) //手动状态  //蜂鸣器与继电器处理方法，将继电器嵌入到蜂鸣器中，以及湿度采集中
		{
			 if(GN==3)
			 {
				 beep=~beep;	
				 while(key_scan());
			 }
			 if(GN==2)
			 {
				 HZ=1;	
				 while(key_scan());
			 }
			 if(GN==1)
			 {
				 HZ=2;	
				 while(key_scan());
			 }			 
			 if(++t_2ms==100)
				{
						t_2ms=0;
						temp=du_adc()*100/256;
				}
				
			 if(beep==0)    //开蜂鸣器
			 {
					if( temp < shidu )
					{
						EA=0;
						P0=0x00;
						P2=(P2&0x1f)|0xa0;
						P0=0x40;
						P2=(P2&0x1f);	
            EA=1;	
					if(HZ==1)
					{   
								EA=0;
								P0=0x00;
								P2=(P2&0x1f)|0xa0;
								P0=0x10 | 0x40;
								P2=(P2&0x1f);	
								EA=1;	
					}				
					if(HZ==2)
					{
								EA=0;
								P0=0x00;
								P2=(P2&0x1f)|0xa0;
								P0=0x00 | 0x40;
								P2=(P2&0x1f);	
								EA=1;	
					 }						
				}
					if( temp > shidu )
					{
						EA=0;
						P0=0x00;
						P2=(P2&0x1f)|0xa0;
            P0=0x00;
						P2=(P2&0x1f);	
            EA=1;	
					if(HZ==1)
					{   
								EA=0;
								P0=0x00;
								P2=(P2&0x1f)|0xa0;
								P0=0x10;
								P2=(P2&0x1f);	
								EA=1;	
					}				
					if(HZ==2)
					{
								EA=0;
								P0=0x00;
								P2=(P2&0x1f)|0xa0;
								P0=0x00;
								P2=(P2&0x1f);	
								EA=1;	
					 }	
					}			
				}	

       if(beep==1)  //关蜂鸣器
			 {
					EA=0;
					P0=0x00;
					P2=(P2&0x1f)|0xa0;
					P0=0x00;
					P2=(P2&0x1f);	
					EA=1;

			if(HZ==1)
			{   
						EA=0;
				    P0=0x00;
						P2=(P2&0x1f)|0xa0;
						P0=0x10;
						P2=(P2&0x1f);	
            EA=1;	
			}				
			if(HZ==2)
			{
						EA=0;
				    P0=0x00;
						P2=(P2&0x1f)|0xa0;
						P0=0x00;
						P2=(P2&0x1f);	
            EA=1;	
			}					 
			 }		 	 
						EA=0;
				    P0=0xff;
						P2=(P2&0x1f)|0x80;
						P0=0xfd;
						P2=(P2&0x1f);	
            EA=1;				
		}//手动
		
	
	 }//f_2ms
	}//while
}


void cl_time1int1() interrupt 3
{
	TH1=(65536-2000/(120000/110592))/256;
	TL1=(65536-2000/(120000/110592))%256;	
	
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	
	f_2ms=1;
}

