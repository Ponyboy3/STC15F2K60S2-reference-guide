/****************************
利用独立键盘实现数码管显示（S4则+1；S5则-1；S6则+2；S7则-2）
****************************
#include <STC15F2K60S2.H>
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
								 	0x90,0xff
                            };
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; 
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar num;
uchar f_2ms;
uchar t_2ms;





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
	switch(i)
	{
		case 0:P2=(P2&0x1f)|0xe0; P0=type1[n1]; P2=(P2&0x1f);break;
		case 1:P2=(P2&0x1f)|0xe0; P0=type1[n2]; P2=(P2&0x1f);break;		
		case 2:P2=(P2&0x1f)|0xe0; P0=type1[n3]; P2=(P2&0x1f);break;		
		case 3:P2=(P2&0x1f)|0xe0; P0=type1[n4]; P2=(P2&0x1f);break;		
		case 4:P2=(P2&0x1f)|0xe0; P0=type1[n5]; P2=(P2&0x1f);break;		
		case 5:P2=(P2&0x1f)|0xe0; P0=type1[n6]; P2=(P2&0x1f);break;		
		case 6:P2=(P2&0x1f)|0xe0; P0=type1[n7]; P2=(P2&0x1f);break;
		case 7:P2=(P2&0x1f)|0xe0; P0=type1[n8]; P2=(P2&0x1f);break;		
	}
	if(++i==8)
		i=0;	
}
////////////////////////独立按键
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

void time1_int1()
{
	EA=1;
	ET1=1;
	TR1=1;
	TMOD=(TMOD%0x0f)|0x10;
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
	while(1)
	{
	 if(f_2ms==1)
	 {
	  f_2ms=0;
		key_change(); 
		 
		if(GN==4) 
		{			
			if(num==100)
				num=0;
			else
				num++;		
			while(key_scan());
		}
		if(GN==5) 
		{			
			if(num==0)
				num=100;
			else
				num--;		
			while(key_scan());
		}		 
		if(GN==6) 
		{			
			if(num>98)
				num=100;
			else
				num=num+2;		
			while(key_scan());
		}		 
		if(GN==7) 
		{			
			if(num<2)
				num=0;
			else
				num=num-2;		
			while(key_scan());
		}		 
		num1=num/100;num2=num%100/10;num3=num%10;num4=10;num5=10;num6=10;num7=10;num8=10; 
		 

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

*****************************/










/**************************************
扫描矩阵按键键值利用数码管进行显示
**************************************/
#include <STC15F2K60S2.H>
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
								 	0x90,0xff
                            };
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; 
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar num;
uchar f_2ms;
uchar t_2ms;





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
	switch(i)
	{
		case 0:P2=(P2&0x1f)|0xe0; P0=type1[n1]; P2=(P2&0x1f);break;
		case 1:P2=(P2&0x1f)|0xe0; P0=type1[n2]; P2=(P2&0x1f);break;		
		case 2:P2=(P2&0x1f)|0xe0; P0=type1[n3]; P2=(P2&0x1f);break;		
		case 3:P2=(P2&0x1f)|0xe0; P0=type1[n4]; P2=(P2&0x1f);break;		
		case 4:P2=(P2&0x1f)|0xe0; P0=type1[n5]; P2=(P2&0x1f);break;		
		case 5:P2=(P2&0x1f)|0xe0; P0=type1[n6]; P2=(P2&0x1f);break;		
		case 6:P2=(P2&0x1f)|0xe0; P0=type1[n7]; P2=(P2&0x1f);break;
		case 7:P2=(P2&0x1f)|0xe0; P0=type1[n8]; P2=(P2&0x1f);break;		
	}
	if(++i==8)
		i=0;	
}
////////////////////////矩阵按键
uchar key_scan()
{
	uchar L,H;
	P3=0xcf;P44=0;P42=0;H=(~P3)&0x0f;
	if(H==0) return 0;
	Delay10ms();
	P3=0x0f;P44=0;P42=0;H=(~P3)&0x0f;
	if(H==0) return 0;
	P3=0xf0;P44=1;P42=1;L=(~P3)&0x30;	
	if(P44==0) L|=0x80;
	if(P42==0) L|=0x40;
	return (L+H);	
}
void key_change()
{
  uchar D;
	D=key_scan();
	switch(D)
	{
		case 0x88:GN=4;break;
		case 0x84:GN=5;break;
		case 0x82:GN=6;break;
		case 0x81:GN=7;break;
		case 0x48:GN=8;break;
		case 0x44:GN=9;break;
		case 0x42:GN=10;break;
		case 0x41:GN=11;break;
		case 0x28:GN=12;break;
		case 0x24:GN=13;break;
		case 0x22:GN=14;break;
		case 0x21:GN=15;break;
		case 0x18:GN=16;break;
		case 0x14:GN=17;break;
		case 0x12:GN=18;break;
		case 0x11:GN=19;break;		
//		case 0:GN=0;break;		
	}
}

void time1_int1()
{
	EA=1;
	ET1=1;
	TR1=1;
	TMOD=(TMOD%0x0f)|0x10;
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
	while(1)
	{
	 if(f_2ms==1)
	 {
	  f_2ms=0;
		key_change(); 





		 
		num1=GN/10;num2=GN%10;num3=10;num4=10;num5=10;num6=10;num7=10;num8=10; 
		 

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


