/****************************************
利用定时器2作为波特率发生器，与上位机通信实现数据收发工作
*********************************************/
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
										 	0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

uchar TX[]={1,2,3,4}, TXP=0;
uchar RX[]={1,2,3,4}, RXP=0;


uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar f_2ms;
uchar t_2ms;
uchar GN;

uchar vary;
bit f_1s;

uchar a,b,c;
bit k;

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


////////////////////////////key
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
	D= key_scan();
	switch(D)
	{
		case 8:GN=4;break;
		case 4:GN=5;break;
		case 2:GN=6;break;
		case 1:GN=7;break;
		case 0:GN=0;break;
	}	
}



//////////////////////////////////串口
void UartInit(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xE0;		//设定定时初值
	T2H = 0xFE;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	
	ES=1;//打开串口
//	REN=1;
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
	UartInit( );
	while(1)
	{
	 if(f_2ms==1)
	 {
		 f_2ms=0;
	   key_change();
		 
 if(GN==4)
 {
//	 SBUF=1;
 	 TI=1;
	 while(key_scan());
  
 }		 

		 num1=RX[0];num2=RX[1];num3=RX[2];num4=RX[3];
 
     num5=10;num6=10;num7=10; 
	
	
	 }
	}
}



void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;		
	
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	f_2ms=1;
	
//	if(++t_2ms==500)
//	{
//		t_2ms=0;
//		vary++;
//		f_1s=1;
//	}
}


void cl_UartInit() interrupt 4
{

	
/////////////////////////////////////对接受来的数据进行缓存
if(RI==1)
{
	RI=0;
	if(RXP!=4)
	{
		vary=SBUF;
		vary=(vary>>4)*10+(vary&0x0f);
		RX[RXP]=vary;
		RXP++;
	}
	else 
		RXP=0;
}	
	
/////////////////////////////////////向上位机发送已经缓存的数据		 
	if(TI==1)
	{
		   TI=0; 	 	   
		    if(TXP!=4)
				{
					SBUF=TX[TXP];
					TXP++;					
				}
		    else
					TXP=0;
	}	
		

	

	
}////over









