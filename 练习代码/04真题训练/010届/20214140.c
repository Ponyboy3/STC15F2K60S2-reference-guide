#include "reg52.h"
#include "iic.h"

#define uchar unsigned char

uchar code type1[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
													 0xff,0xbf,
                           0x8e,0xc1,
                           0x70,0x24,0x30,0x19,0x12,0x01,0x78,0x00,0x10};
uchar code type2[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
		
unsigned int dat_f = 0;	
uchar num1,num2,num3,num4,num5,num6,num7,num8;
unsigned int t_2ms;
uchar GN;
unsigned int VRB2,VDAC;
bit f_2ms;
bit flag;
bit f_S4;
bit f_led,f_smg,f_VRB2;

//=========数码管模块==========
void Delay_SMG(unsigned int t)
{
	while(t--);
}

void Dispaly_SMG(char n1,char n2,char n3,char n4,char n5,char n6,char n7,char n8)
{
	char i;
	P0 = 0xff;
	P2 = ((P2 & 0x1f) | 0xc0);
	P0 = type2[i];
	P2 &= 0x1f;
	switch(i)
	{
		case 0:P2 = ((P2 & 0x1f) | 0xe0);P0 = type1[n1];P2 &= 0x1f;break;
		case 1:P2 = ((P2 & 0x1f) | 0xe0);P0 = type1[n2];P2 &= 0x1f;break;
		case 2:P2 = ((P2 & 0x1f) | 0xe0);P0 = type1[n3];P2 &= 0x1f;break;
		case 3:P2 = ((P2 & 0x1f) | 0xe0);P0 = type1[n4];P2 &= 0x1f;break;
		case 4:P2 = ((P2 & 0x1f) | 0xe0);P0 = type1[n5];P2 &= 0x1f;break;
		case 5:P2 = ((P2 & 0x1f) | 0xe0);
								if(f_smg==0 && f_S4 == 0)
									P0 = type1[n6]&0x7f;
								else
									P0 = type1[n6];
								P2 &= 0x1f;break;
		
		case 6:P2 = ((P2 & 0x1f) | 0xe0);P0 = type1[n7];P2 &= 0x1f;break;
		case 7:P2 = ((P2 & 0x1f) | 0xe0);P0 = type1[n8];P2 &= 0x1f;break;
	}
	if(++i == 8)
		i = 0;
}
//=========按键模块=======
uchar Scan_Keys()
{
	uchar d;
	d = (~P3) & 0x0f;
	if(d == 0) return 0;
	Delay_SMG(100);
	d = (~P3) & 0x0f;
	if(d == 0) return 0;
	return d;
}
void Change_Keys()
{
	switch(Scan_Keys())
	{
		case 8: GN = 1;break;//S$
		case 4: GN = 2;break;//S5
		case 2: GN = 3;break;//S6
		case 1: GN = 4;break;//S7
		case 0: GN = 0;break;
	}
}
//==========RB2模块========
void Init_RB2()
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x03);
	IIC_WaitAck();
	IIC_Stop();
}

unsigned char Read_RB2()
{
	uchar d;
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	d = IIC_RecByte();
	IIC_SendAck(1); 
	IIC_Stop();
	
	return d;
}
void f_Display()
{
	if(dat_f < 10)
	{
		num8 = dat_f;
		num1=12;
		num2=num3=num4=num5=num6=num7=10;
	}
	else if((dat_f >= 10) && (dat_f < 100))
	{
		num7=dat_f/10;num8 = dat_f%10;
		num1=12;
		num2=num3=num4=num5=num6=10;
	}
	else if((dat_f >= 100) && (dat_f < 1000))
	{
		num6=dat_f/100;num7=dat_f/10%10;num8 = dat_f%10;
		num1=12;
		num2=num3=num4=num5=10;
	}
	else if((dat_f >= 1000) && (dat_f < 10000))
	{
		num5=dat_f/1000;num6=dat_f/100%10;num7=dat_f/10%10;num8 = dat_f%10;
		num1=12;
		num2=num3=num4=10;
	}
	else if(dat_f > 9999)
	{
		num4=dat_f/10000;num5=dat_f/1000%10;num6=dat_f/100%10;num7=dat_f/10%10;num8 = dat_f%10;
		num1=12;
		num2=num3=10;
	}
}
	
void Init_Timer()
{
	TMOD=(TMOD&0xf0)|0x05;//T1定时，T0计数
	TMOD=(TMOD&0x0f)|0x10;
	TL0 = 0;
	TH0 = 0;
	
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;
	
	EA = 1;
	ET1 = 1;
	TR1 = 1;
}
void Service_T1() interrupt 3
{
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;
	f_2ms = 1;
	Dispaly_SMG(num1,num2,num3,num4,num5,num6,num7,num8);
	
	
	
	if(++t_2ms == 250)
	{
		t_2ms = 0;
		flag = ~flag;
		if(flag == 0)
		{
			TL0 = 0;
			TH0 = 0;
			TR0 = 1;
		}
		else
		{
			TR0 = 0;
			dat_f = ((TH0 << 8) + TL0) * 2;
		}
	}
	
}	

//==========系统初始化=========
void Init_Sys()
{
	P2 = ((P2 & 0x1f) | 0x80);
	P0 = 0xff;
	P2 &= 0x1f;
	
	P2 = ((P2 & 0x1f) | 0xa0);
	P0 = 0x00;
	P2 &= 0x1f;
	
	P2 = ((P2 & 0x1f) | 0xc0);
	P0 = 0xff;
	P2 &= 0x1f;
	
	P2 = ((P2 & 0x1f) | 0xe0);
	P0 = 0xff;
	P2 &= 0x1f;
}
void main()
{
	Init_Sys();
	Init_Timer();
	Init_RB2();
	while(1)
	{
		if(f_2ms == 1)
		{
			f_2ms = 0;
			
			Change_Keys();
			if(GN == 3)  //led显示开关
			{
				f_led = ~f_led;
				while(Scan_Keys());
			}
			if(GN == 4)  //数码管显示开关
			{
				f_smg = ~f_smg;
				while(Scan_Keys());
			}
			
			if(GN == 1) //数码管界面切换
			{
				f_S4 = ~f_S4;
				while(Scan_Keys());
			}
				if(f_S4 == 0)//电压测量界面
				{
					VRB2 = Read_RB2();
					if(f_VRB2 == 1)
					{
						if(VRB2>250)
							VRB2 = 250;
						VDAC = VRB2*2;
					}
					else
					{
						VDAC = 200;
					}
					if(f_led==1)
					{
						P0 = 0xff;
						P2 = ((P2 & 0x1f) | 0x80);
						P0 = 0xff;//全灭
						P2 &= 0x1f;
						f_Display();
					}
					else
					{
						P0 = 0xff;
						P2 = ((P2 & 0x1f) | 0x80);
						P0 = P0 & 0xfe;//Led1亮
						P2 &= 0x1f;
						if(f_VRB2 == 0)
						{
							P2 = ((P2 & 0x1f) | 0x80);
							P0 = P0 & 0xef;//Led5亮
							P2 &= 0x1f;
						}
						else
						{
							P2 = ((P2 & 0x1f) | 0x80);
							P0 = P0 | 0x10;//Led5灭,同时不影响其他
							P2 &= 0x1f;
						}
						if(VRB2<75)
						{
							P2 = ((P2 & 0x1f) | 0x80);
							P0 = P0 | 0x04;//Led3灭,同时不影响其他
							P2 &= 0x1f;
						}
						else if(VRB2>=75 && VRB2<125)
						{
							P2 = ((P2 & 0x1f) | 0x80);
							P0 = P0 & 0xfb;//Led3亮
							P2 &= 0x1f;
						}
						else if(VRB2>=125 && VRB2<175)
						{
							P2 = ((P2 & 0x1f) | 0x80);
							P0 = P0 | 0x04;//Led3灭,同时不影响其他
							P2 &= 0x1f;
						}
						else if(VRB2>=175 && VRB2<=255)
						{
							P2 = ((P2 & 0x1f) | 0x80);
							P0 = P0 & 0xfb;//Led3亮
							P2 &= 0x1f;
						}
					}
					if(f_smg==0)
					{
						num6=VDAC/100;num7=VDAC/10%10;
						num8 = VDAC%10;
						num1=13;
						num2=num3=num4=num5=10;
					}
					else
					{
						num1=num2=num3=num4=num5=num6=num7=num8=10;
					}
					
				}
				else         //频率测量界面
				{
					VRB2 = Read_RB2();
					if(f_VRB2 == 1)
					{
						VDAC = VRB2;
					}
					else
					{
						VDAC = 200;
					}
					if(f_led==1)
					{
						P0 = 0xff;
						P2 = ((P2 & 0x1f) | 0x80);
						P0 = 0xff;//全灭
						P2 &= 0x1f;
						f_Display();
					}
					else
					{
						P0 = 0xff;
						P2 = ((P2 & 0x1f) | 0x80);
						P0 = P0 & 0xfd;//Led2亮
						P2 &= 0x1f;
						if(dat_f < 1000)
						{
							P2 = ((P2 & 0x1f) | 0x80);
							P0 = P0 | 0x08;//Led4灭,同时不影响其他
							P2 &= 0x1f;
						}
						else if(dat_f < 5000 && dat_f >=1000)
						{
							P2 = ((P2 & 0x1f) | 0x80);
							P0 = P0 & 0xf7;//Led4亮
							P2 &= 0x1f;
						}
						else if(dat_f < 10000 && dat_f >=5000)
						{
							P2 = ((P2 & 0x1f) | 0x80);
							P0 = P0 | 0x08;//Led4灭
							P2 &= 0x1f;
						}
						else if(dat_f >=10000)
						{
							P2 = ((P2 & 0x1f) | 0x80);
							P0 = P0 & 0xf7;//Led4亮
							P2 &= 0x1f;
						}
						
						
					}
					if(f_smg==0)
					{
						f_Display();
					}
					else
					{
						num1=num2=num3=num4=num5=num6=num7=num8=10;
					}
				}
			
			if(GN == 2)  //RB2输出值改变
			{
				f_VRB2=~f_VRB2;
				while(Scan_Keys());
			}
		}
	}
}