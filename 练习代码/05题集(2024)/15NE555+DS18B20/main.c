#include "reg52.h"
#include "onewire.h"

#define uchar unsigned char 
#define uint unsigned int

// 数码管显示数据， 一般前10个为0-9， 可更具需要往里面添加
uchar code type1[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf,
					  0xc6,  // C 12
					  0x89,  // H
					  0x8e,  // F
					  0x8c,  // P
					  0x86,  // E
					  0x88};
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
bit f_1s, f_10ms;  // 1s标志位

uint num_2ms = 0, num_s = 0;  // 多少个2ms, 1s

unsigned int s9_press_time = 0;
bit s9_long;
// NE555
uint num_2ms_ne555=0, freq;
uint num_2ms_mesure_ne555, num_s_mesure_ne555;


// 温度数据
uint temperature_smg;
// 湿度数据
uint humi_smg;
uint humi;
// 状态标志
uchar stat_smg = 1;  // 数码管标志
bit is_runing_ne555;  // ne555是否在测量
uchar stat_trigger = 0;  // 触发状态


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
	
//	TR0 = 0;
	
	P3 =  0xcf;P44 = 0;P42 = 0;H = (~P3) & 0x0f;
	if(H == 0) return 0;
	Delay_Keys(100);
	P3 =  0xcf;P44 = 0;P42 = 0;H = (~P3) & 0x0f;
	if(H == 0) return 0;
	P3 =  0xf0;P44 = 1;P42 = 1;L = (~P3) & 0x30;
	if(P44 == 0) L |= 0x80;
	if(P42 == 0) L |= 0x40;
	
//	TR0 = 1;
	
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

//sbit P32 = P3^2;
//sbit P33 = P3^3;
//void key_array_init() {
//	P32 = 1;
//	P33 = 1;
//	P42 = 0;
//	P44 = 0;
//}
//============================================DS18B20初始化============================================
void Init_18b20()
{
	char i;
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);  // 启动温度转换
//	Write_DS18B20(0x00);
//	Write_DS18B20(0x00);
//	Write_DS18B20(0x7f);
	
	for (i = 0; i < 8; i ++ ) {  // 等待第一次采集结束，不会显示85
		Delay_Keys(60000);
	}
}

unsigned int Read_Temperature(void)  // 当需要保留2为小数时，才需要用到unsigned int,
									 // 这里介绍3种类型，所以用unsigned int，保留整数
							         // 和一位小数可用unsigned char.
{
	unsigned char H,L;
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);
	L = Read_DS18B20();
	H = Read_DS18B20();
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	
	//1.保留整数--23
	//temp = (H<<4) + (L>>4);
	
	//2.保留1位小数--23.4
	return ((H<<8) + L)*5/8;
	
	//3.保留2位小数--23.45
//	return ((H<<8)+L)*25/4;  // (50/8)
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
//============================================定时器T0初始化=======================================
void Init_T0()
{
	TMOD = (TMOD & 0xf0) | 0x05;//定时
	TH0 = 0;
	TL0 = 0;
	
	TR0 = 1;
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

//==========================================频率转湿度=============================================
void Freq_to_Humi() {
	float humi = 0;
	if (freq >= 200 && freq <= 2000) {
		humi = 0.05 * (freq - 200);
		humi_smg = humi * 10;
		LED5_Off();
	}
	else {
		humi_smg = 99;
		LED5_On();
	}
}

//==========================================数码管显示=============================================
sbit P32 = P3^2;
sbit P33 = P3^3;
sbit P30 = P3^0;
void key_array_init() {
	P30 = 1;
	P32 = 1;
	P33 = 1;
	P42 = 0;
	P44 = 0;
}

void key_array_scan() {	
	if (P32 == 0) {
		P32 = 0;
		P42 = 1;
		P44 = 1;
		if (P42 == 0) {
			Delay_Keys(100);
			if (P42 == 0) {  // S9
				// =========================================== 代码开始 ======================================
				LED1_On();
				// =========================================== 代码结束 ======================================
				while (P42 == 0);
				// =========================================== 代码开始 ======================================
				LED1_Off();
				// =========================================== 代码结束 ======================================
			}
		}
		else if (P44 == 0) {
			Delay_Keys(100);
			if (P44 == 0) {    // S5
				// =========================================== 代码开始 ======================================
				LED1_On();
				
				if (stat_smg == 2) {  // 时间显示
					stat_smg = 3;
				}
				else if (stat_smg == 3 ) {
					stat_smg = 4;
				}
				else if (stat_smg == 4) {
					stat_smg = 2;
				}
				// =========================================== 代码结束 ======================================
				while (P44 == 0);
				// =========================================== 代码开始 ======================================
				LED1_Off();
				// =========================================== 代码结束 ======================================
			}
		}
	}
	
	if (P33 == 0) {
		P33 = 0;
		P42 = 1;
		P44 = 1;
		if (P42 == 0) {
			Delay_Keys(100);
			if (P42 == 0) {  // S8
				// =========================================== 代码开始 ======================================
				LED1_On();
				// =========================================== 代码结束 ======================================
				while (P42 == 0);
				// =========================================== 代码开始 ======================================
				LED1_Off();
				// =========================================== 代码结束 ======================================
			}
		}
		else if (P44 == 0) {
			Delay_Keys(100);
			if (P44 == 0) {  // S4
				// =========================================== 代码开始 ======================================
				LED1_On();
				
				if (stat_smg == 1) {  // 时间显示
					stat_smg = 2;
				}
				else if (stat_smg == 2 | stat_smg == 3 | stat_smg == 4) {
					stat_smg = 5;
				}
				else if (stat_smg == 5) {
					stat_smg = 1;
				}
				// =========================================== 代码结束 ======================================
				while(P44 == 0);
				// =========================================== 代码开始 ======================================
				LED1_Off();
				// =========================================== 代码结束 ======================================
			}
		}
	}
	
	if (P30 == 0) {
		P30 = 0;
		P44 = 1;
		if (P44 == 0) {
			Delay_Keys(100);
			if (P44 == 0) {  // S7
				// =========================================== 代码开始 ======================================
				LED1_On();
				is_runing_ne555 = 1;
				s9_press_time = 0;
				stat_trigger = 1;  // 触发测量
				
//				num_2ms_mesure_ne555 = 0;
//				num_s_mesure_ne555 = 0;
//				while(Scan_Keys());
				LED4_Off();
				// =========================================== 代码结束 ======================================
				while(P44 == 0);
				// =========================================== 代码开始 ======================================
				LED1_Off();
				// =========================================== 代码结束 ======================================
			}
		}
	}
	
	key_array_init();
}

void Dis_smg() {
	if (stat_smg == 1) {
		num1 = 10;
		num3 = num6 = 11;
	}
	else if (stat_smg == 2) {
		num1 = 12;
	}
	else if (stat_smg == 3) {
		num1 = 13;
	}
	else if (stat_smg == 4) {
		num1 = 14;
	}
	else if (stat_smg == 5) {
		num1 = 15;
	}
	else if (stat_smg == 6) {
		
	}
	

}

void main()
{
	Init_Sys();
	Init_T1();
	Init_T0();
	
	Init_18b20();
	
	while(1)
	{
//		if (f_10ms == 1 && is_runing_ne555 == 0) {
//			f_10ms = 0;
//			TR0 = 0;
//			Change_Keys();
//			TR0 = 1;
//		}
		
//		LED_control();
        if(f_2ms == 1)
        {
            f_2ms = 0;			
			LED_control();  // xxx	
			Dis_smg();  // 数码管显示
			// =======================按键========================
//			Change_Keys_Alone();  // 1.独立按键扫描(1)
//			ScanKeys_Alone();     // 2.独立按键扫描(2)
//			Change_Keys();        // 3.矩阵按键扫描
//			key_array_scan();     // 4.矩阵按键扫描
//			if (!is_runing_ne555) {
//				f_10ms = 0;
//				TR0 = 0;
//				Change_Keys();
//				TR0 = 1;
//			}
			key_array_scan();
			// =========================================== 代码开始 ======================================
			if (stat_trigger == 1) {
				Freq_to_Humi();
				temperature_smg = Read_Temperature();
				
				stat_smg = 6;
				
				num1 = 16;
				num2=num3 = 10;
				
				num4 = temperature_smg / 100;
				num5 = temperature_smg % 100 / 10;
				num6 = 11;
				num7 = humi_smg / 100;
				num8 = humi_smg % 100 / 10;
				
				while (stat_trigger);
			}
			// =========================================== 代码结束 ======================================
        }
    }
}
void Service_T1() interrupt 3
{
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;
	
	f_2ms = 1;
	
	if (++ num_2ms == 5) {
		num_2ms = 0;
		f_10ms = 1;
	}
	/***************************************************
	与时间相关的变量的更新放在里面
	注意：不要把消耗过长时间的代码（如for循环等）放在这里，容易造成错乱
	***************************************************/
	// =========================================== 代码开始 ======================================
//	if (GN == 4) {
//		s9_press_time ++ ;
//		if (s9_press_time >= 1000) {
//			s9_press_time = 0;
////			s9_long = 1;
//			LED4_On();
//		}
//	}
	
	if (stat_trigger) {
		if (++ num_2ms_mesure_ne555 >= 500) {
			num_2ms_mesure_ne555 = 0;
			num_s_mesure_ne555 ++ ;
		}
		if (num_s_mesure_ne555 >= 3) {
			num_s_mesure_ne555 = 0;
//			TR0 = 0;
			stat_trigger = 0;
			stat_smg = 2;  // 测量完返回界面2
		}
	}
	
	if (++ num_2ms_ne555 >= 500)
	{
		num_2ms_ne555 = 0;
		freq = (TH0 << 8) | TL0;
		
		TH0 = 0;
		TL0 = 0;
	}
	// =========================================== 代码结束 ======================================
	
	Display_SMG(num1,num2,num3,num4,num5,num6,num7,num8);//2ms刷新数码管
}