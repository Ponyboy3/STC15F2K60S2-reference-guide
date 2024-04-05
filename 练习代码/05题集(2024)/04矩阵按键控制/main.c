#include "reg52.h"

#define uchar unsigned char 
#define uint unsigned int

// 数码管显示数据， 一般前10个为0-9， 可更具需要往里面添加
uchar code type1[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf,0x8e,0xc8,0xc1};
// xxx
uchar code type2[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

uchar num1,num2,num3,num4,num5,num6,num7,num8;  // 数码管每一位（例如 num1 = 2, 第一个数码管显示 2）
uchar GN;  // 按键标志位
bit f_2ms;  // 2ms标志位
uchar temp_Led_P0 = 0xff;  // 用来存上一次LED状态

sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;
sbit L4 = P0^3;
sbit L5 = P0^4;
sbit L6 = P0^5;

// =========================================== 代码开始 ======================================
bit f_1s;  // 1s标志位

uint num_2ms = 0, num_s = 0;  // 多少个2ms, 1s
// =========================================== 代码结束 ======================================


//=========================================普通延时函数(模板函数)=================================
void Delay(uint t) {
	while( t -- );
	while( t -- );
}
//=======================================单个LED控制函数(模板函数)================================
// ======================LED1====================
void LED1_On() {
	P0 = 0xff & temp_Led_P0;  // 清除其他地方对P0的值（特别是数码管一直在刷新），并且结合LED前一次的状态
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L1 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
}
void LED1_Off() {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L1 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
}
// ======================LED2====================
void LED2_On() {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L2 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
}
void LED2_Off() {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L2 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
}
// ======================LED3====================
void LED3_On() {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L3 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
}
void LED3_Off() {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L3 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
}
// ======================LED4====================
void LED4_On() {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L4 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
}
void LED4_Off() {
	P0 = 0xff & temp_Led_P0 & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L4 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
}
// ======================LED5====================
void LED5_On() {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L5 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
}
void LED5_Off() {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L5 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
}
// ======================LED6====================
void LED6_On() {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L6 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
}
void LED6_Off() {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L6 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
}

void LED_On(uchar n) {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L6 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
}
//=========================================数码管显示==============================================
void Display_SMG(uchar n1,uchar n2,uchar n3,uchar n4,uchar n5,uchar n6,uchar n7,uchar n8)
{
	char i;
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xc0;//数码管位选
	P0 = type2[i];
	P2 &= 0x1f;
	switch(i)//数码管位选
	{
		case 0:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n1];P2 &= 0x1f;break;
		case 1:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n2];P2 &= 0x1f;break;
		case 2:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n3];P2 &= 0x1f;break;
		case 3:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n4];P2 &= 0x1f;break;
		case 4:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n5];P2 &= 0x1f;break;
		case 5:P2 = (P2 & 0x1f) | 0xe0;
									if(0)  // 根据需要的条件
									{
										P0 = type1[n6]&0x7f;  // 加入小数点
									}
									else
										P0 = type1[n6];
												  P2 &= 0x1f;break;
		case 6:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n7];P2 &= 0x1f;break;
		case 7:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n8];P2 &= 0x1f;break;
	}
	if(++i==8)
		i = 0;
}
//============================================独立按键============================================
/***************************************************
写法一：
有按键被按下，GN标记，后面根据需要功能进行操作
***************************************************/
void Delay_Keys(uint t)
{
	while(t--);
}
uchar Scan_Keys_Alone()
{
	char d;
	d = (~P3) & 0x0f;
	if(d == 0) return 0;
	Delay_Keys(100);
	if(d == 0) return 0;
	Delay_Keys(100);
	return d;
}
void Change_Keys_Alone()
{
	switch(Scan_Keys_Alone())
	{
		case 8:GN = 1;break;//S4
		case 4:GN = 2;break;//S5
		case 2:GN = 3;break;//S6
		case 1:GN = 4;break;//S7
		case 0:GN = 0;break;
	}
}

/***************************************************
写法二：
有按键被按下，按下是一种状态，松开是另一种状态
（如：按下时某个灯亮，松开时灯灭）
***************************************************/
void ScanKeys_Alone() 
{
	if (S7 == 0) {
		Delay_Keys(100);
		if (S7 == 0) {
			// =========================================== 代码开始 ======================================
			
			// =========================================== 代码结束 ======================================
			while(S7 == 0);
			// =========================================== 代码开始 ======================================

			// =========================================== 代码结束 ======================================
		}
	}
	
	if (S6 == 0) {
		Delay_Keys(100);
		if (S6 == 0) {
			// =========================================== 代码开始 ======================================
			
			// =========================================== 代码结束 ======================================
			while(S6 == 0);
			// =========================================== 代码开始 ======================================

			// =========================================== 代码结束 ======================================
		}
	}
	
	if (S5 == 0) {
		Delay_Keys(100);
		if (S5 == 0) {
			// =========================================== 代码开始 ======================================

			// =========================================== 代码结束 ======================================
			while(S5 == 0);
			// =========================================== 代码开始 ======================================

			// =========================================== 代码结束 ======================================
		}
	}

	if (S4 == 0) {
		Delay_Keys(100);
		if (S4 == 0) {
			// =========================================== 代码开始 ======================================

			// =========================================== 代码结束 ======================================
			while(S4 == 0);
			// =========================================== 代码开始 ======================================

			// =========================================== 代码结束 ======================================
		}
	}
}

// ================= 代码结束 =============

//============================================矩阵按键============================================
sfr P4 = 0xc0;
sbit P44 = P4^4;
sbit P42 = P4^2;

uchar Scan_Keys()
{
	char H,L;
	P3 =  0xcf;P44 = 0;P42 = 0;H = (~P3) & 0x0f;
	if(H == 0) return 0;
	Delay_Keys(100);
	P3 =  0xcf;P44 = 0;P42 = 0;H = (~P3) & 0x0f;
	if(H == 0) return 0;
	P3 =  0xf0;P44 = 1;P42 = 1;L = (~P3) & 0x30;
	if(P44 == 0) L |= 0x80;
	if(P42 == 0) L |= 0x40;
	return (H+L);
}

void Change_Keys()
{
	switch(Scan_Keys())
	{
		case 0x88:GN = 4;break;  // S4
		case 0x84:GN = 5;break;  // S5
		case 0x82:GN = 6;break;  // S6
		case 0x81:GN = 7;break;  // S7
		case 0x48:GN = 8;break;  // S8
		case 0x44:GN = 9;break;  // S9
		case 0x42:GN = 10;break;  // S10
		case 0x41:GN = 11;break;  // S11
		case 0x28:GN = 12;break;  // S12
		case 0x24:GN = 13;break;  // S13
		case 0x22:GN = 14;break;  // S14
		case 0x21:GN = 15;break;  // S15
		case 0x18:GN = 16;break;  // S16
		case 0x14:GN = 17;break;  // S17
		case 0x12:GN = 18;break;  // S18
		case 0x11:GN = 19;break;  // S19
		case 0:GN = 0;break;
	}
}
//============================================定时器T1初始化=======================================
void Init_T1()
{
	TMOD = (TMOD & 0xf0) | 0x10;//定时
	TH1 = (65536 - 2000) / 256;//2ms
	TL1 = (65536 - 2000) % 256;
	
	EA = 1;
	ET1 = 1;
	TR1 = 1;
}
//==========================================系统初始化=============================================
void Init_Sys()
{
	P2 = (P2 & 0x1f) | 0x80;  // LED
	P0 = 0xff;
	P2 &= 0x1f;
	P2 = (P2 & 0x1f) | 0xa0;  // 蜂鸣器，寄存器
	P0 = 0x00;
	P2 &= 0x1f;
	P2 = (P2 & 0x1f) | 0xc0;  // 数码管
	P0 = 0xff;
	P2 &= 0x1f;
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0xff;
	
	num1=num2=num3=num4=num5=num6=num7=num8=10;
}

void LED_control() {
	// ================= 测试开始 =============
//	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
//	P0 = 0x7f;  // 要将P0放在这两句代码之间，避免错乱
//	P2 &= 0x1f;
	// ================= 测试结束 =============
	
	// =========================================== 代码开始 ======================================
	
	// =========================================== 代码结束 ======================================
}

void main()
{
	Init_Sys();
	Init_T1();
//	Init_T0();
	
	while(1)
	{
//		LED_control();
        if(f_2ms == 1)
        {
            f_2ms = 0;			
			LED_control();  // xxx
			// =======================按键========================
//			Change_Keys_Alone();  // 1.独立按键扫描(1)
//			ScanKeys_Alone();     // 2.独立按键扫描(2)
			Change_Keys();        // 3.矩阵按键扫描
			
			// =========================================== 代码开始 ======================================
			if (GN == 4) {
				num1 = 0;
				while(Scan_Keys());
			}
			if (GN == 5) num1 = 1;
			if (GN == 6) num1 = 2;
			if (GN == 7) num1 = 3;
			if (GN == 8) num1 = 4;
			if (GN == 9) num1 = 5;
			if (GN == 10) num1 = 6;
			if (GN == 11) num1 = 7;
			if (GN == 12) num1 = 8;
			if (GN == 13) num1 = 9;
			// =========================================== 代码结束 ======================================
        }
    }
}
void Service_T1() interrupt 3
{
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;
	
	f_2ms = 1;
	
	/***************************************************
	与时间相关的变量的更新放在里面
	注意：不要把消耗过长时间的代码（如for循环等）放在这里，容易造成错乱
	***************************************************/
	// =========================================== 代码开始 ======================================

	// =========================================== 代码结束 ======================================
	
	Display_SMG(num1,num2,num3,num4,num5,num6,num7,num8);//2ms刷新数码管
}