/****************************************
利用定时器2作为波特率发生器，与上位机通信实现数据收发工作
*********************************************/
#include <STC15F2K60S2.H>
#include <intrins.h>
#include <string.h> //用来测试字符串长度
#include <stdio.h>  //使用sprintf函数


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
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar f_2ms;
uint t_2ms;
uchar GN;


bit buzy=0; //检测是否发送完成
uchar code passwd[]="LOVE YOU";//密码比对数据
uchar passwd_com=0;  //密码长度变量

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

//==============================================配置波特率要与串口波特率一致，不然会出错	
void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x8F;		//设定定时初值
	T2H = 0xFD;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	
	ES=1;
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

//========================================发送一字节数据
void send_byte(uchar *dat,uchar len)//这个指针会指向后面的数组，相当于数组变量。len是长度
{
	while(len--)
	{
		SBUF=*dat;	
		buzy=1;
		dat++;
		while(buzy);
	}	
}

void main()
{
//	uchar str[]="hello world \r\n";
	uchar str[40];   //welcome to jiangsu %f s \r\n  //定义一个存放发送字符和数据的数组尽量大一点

	int a;  //实时自加变量
	
	
  initial_all();	
	time1_int1();
	UartInit();
  num1=num2=num3=num4=num5=num6=num7=num8=10;			
	while(1)
	{
	 if(f_2ms==1)
	 {
		 f_2ms=0;
	     key_change();
		 
		 if(++t_2ms==500)
		 {
		    t_2ms=0;
			  a++; 
		 }
		 
//         if(GN==4)  //按下按键则发送数据
//		 {
//			send_byte(str,strlen(str)); 
//			while(key_scan()); 
//		 }
		 
//         if(passwd_com >= 8) //密码正确则发送数据
//		 {
//			 passwd_com=0;
//			 send_byte(str,strlen(str));  
//		 }
		 
         if(passwd_com >= 8)  //密码正确则发送字符串 和 实时变量
		 {
			 passwd_com=0;
			 
			 sprintf(str,"欢迎 to jiangsu %f s \r\n", a);	/*它把结果输出到指定的字符串中,
			                                                       "welcome to jiangsu %f s \r\n"到str数组中*/		 			 
			 send_byte(str,strlen(str));  
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


void cl_uart() interrupt 4
{
	if(RI)
	{
		RI=0;
		if(passwd[passwd_com] == SBUF)//将接收到的数据与内部密码进行一一比对
		{
		  	passwd_com++;			  //如果正确则变量加一，为接收完成在发送数据做准备
		}
		else
			passwd_com=0;
		
	}
	if(TI)
	{
		TI=0;
		buzy=0;	//发送完成
	}
	
}








