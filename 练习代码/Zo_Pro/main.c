#include "reg52.h"
#include "onewire.h"
#include "iic.h"
#include "ds1302.h"
#include "intrins.h"

#define uchar unsigned char 
#define uint unsigned int

// 数码管显示数据， 一般前10个为0-9， 可更具需要往里面添加
unsigned char code type1[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
							  0xff,0xbf,0x8e,0xc8,0xc1};
// 数码管位
uchar code type2[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

unsigned char num1,num2,num3,num4,num5,num6,num7,num8;  // 数码管每一位（例如 num1 = 2, 第一个数码管显示 2）
unsigned char GN;  // 按键标志位
bit f_2ms, f_50ms, f_500ms_smg, f_200ms_temperature;  // 2ms, 50ms, 500ms标志位
unsigned int num_2ms_smg;  // 多少个2ms
unsigned char temp_Led_P0 = 0xff;  // 用来存上一次LED状态
unsigned int temperature;  // 温度
unsigned int num_2ms_temperature;  // 多少个2ms
unsigned char ds1302_h, ds1302_m, ds1302_s;  // 
unsigned char dat1 = 0x16,dat2 = 0x55,dat3 = 0x50;  // （16进制）ds1302写数据
unsigned char dat[8];
int set_ds1302_h = 23,set_ds1302_m = 59,set_ds1302_s = 45;  // 默认（24c02写数据）时间设置
uchar set_ds1302_key_modele;  // 时间设置的按键模式
bit f_set_time, f_key_long, is_key_set_time;
uchar t_key;  // 按键按住时间
uchar stat_led[8];

unsigned char dis_model = 0;  // 0:显示温度（DS18B20）， 1:显示时钟
// 超声波测距
uint distance;
// PCF8951
unsigned char dat_rd1, dat_rd2;  // ADC读取数据
float rd1_v;
unsigned int smg_rd1_v;
// 按键
unsigned char stat_key;  // 按键按下次数
unsigned int count_key_t;  // 10ms单位累计


sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

sbit L0 = P0^0;
sbit L1 = P0^1;
sbit L2 = P0^2;
sbit L3 = P0^3;
sbit L4 = P0^4;
sbit L5 = P0^5;
sbit L6 = P0^6;
sbit L7 = P0^7;

// =========================================== 代码开始 ======================================
bit f_1s;  // 1s标志位

unsigned int num_2ms = 0, num_s = 0, num_s_key = 0;  // 多少个2ms, 1s, 按键按下时间

bit f_time = 1;  // 定时器0秒钟是否暂停
unsigned char urdat;  // 串口接收数据

unsigned int num_On_Off;  // 开关次数

// =========================================== 代码结束 ======================================


//=========================================普通延时函数(模板函数)=================================
void Delay(uint t) {
	while( t -- );
	while( t -- );
}
//=======================================单个LED控制函数(模板函数)================================
// 根据项目需要删减

// ======================LED0====================
void LED0_On() {
	P0 = 0xff & temp_Led_P0;  // 清除其他地方对P0的值（特别是数码管一直在刷新），并且结合LED前一次的状态
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L0 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
	
	stat_led[0] = 1;
}
void LED0_Off() {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L0 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
	
	stat_led[0] = 0;
}
// ======================LED1====================
void LED1_On() {
	P0 = 0xff & temp_Led_P0;  // 清除其他地方对P0的值（特别是数码管一直在刷新），并且结合LED前一次的状态
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L1 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
	
	stat_led[1] = 1;
}
void LED1_Off() {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L1 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
	
	stat_led[1] = 1;
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
	
	stat_led[6] = 1;
}
void LED6_Off() {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L6 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
	
	stat_led[6] = 0;
}
// ======================LED7====================
void LED7_On() {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L7 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
	
	stat_led[7] = 1;
}
void LED7_Off() {
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L7 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	P2 &= 0x1f;
	
	stat_led[7] = 0;
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
									if(dis_model == 1 || dis_model == 4)  // 根据需要的条件
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
//void ScanKeys_Alone() 
//{
//	if (S7 == 0) {
//		Delay_Keys(100);
//		if (S7 == 0) {
//			// =========================================== 代码开始 ======================================
//			
//			// =========================================== 代码结束 ======================================
//			while(S7 == 0);
//			// =========================================== 代码开始 ======================================

//			// =========================================== 代码结束 ======================================
//		}
//	}
//	
//	if (S6 == 0) {
//		Delay_Keys(100);
//		if (S6 == 0) {
//			// =========================================== 代码开始 ======================================
//			
//			// =========================================== 代码结束 ======================================
//			while(S6 == 0);
//			// =========================================== 代码开始 ======================================

//			// =========================================== 代码结束 ======================================
//		}
//	}
//	
//	if (S5 == 0) {
//		Delay_Keys(100);
//		if (S5 == 0) {
//			// =========================================== 代码开始 ======================================

//			// =========================================== 代码结束 ======================================
//			while(S5 == 0);
//			// =========================================== 代码开始 ======================================

//			// =========================================== 代码结束 ======================================
//		}
//	}

//	if (S4 == 0) {
//		Delay_Keys(100);
//		if (S4 == 0) {
//			// =========================================== 代码开始 ======================================

//			// =========================================== 代码结束 ======================================
//			while(S4 == 0);
//			// =========================================== 代码开始 ======================================

//			// =========================================== 代码结束 ======================================
//		}
//	}
//}

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
//============================================24C02初始化============================================
void Write_24C02_Byte(unsigned char addr,unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop(); 
}

unsigned char Read_24C02_Byte(unsigned char addr)
{
	unsigned char tmp;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	tmp = IIC_RecByte(); 
	IIC_SendAck(1);
	IIC_Stop();
	
	return tmp;
}

void Write_24C02_Page(unsigned char addr, unsigned char *dat, unsigned char len)
{
	unsigned char i;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	
	for (i = 0; i < len; i ++ ) {
		IIC_SendByte(dat[i]);
		IIC_WaitAck();
	}
	IIC_Stop(); 
}

void Read_24C02_Page(unsigned char addr, unsigned char *dat, unsigned char len)
{
	unsigned char i;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	for (i = 0; i < len - 1; i ++ ) {
		dat[i] = IIC_RecByte();
		IIC_SendAck(0);
	}
	dat[len - 1] = IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
}
//============================================DS18B20初始化============================================
void Init_18b20()
{
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x4e);
	Write_DS18B20(0x00);
	Write_DS18B20(0x00);
	Write_DS18B20(0x7f);
}
unsigned int Read_Temperature(void)  // 当需要保留2为小数时，才需要用到unsigned int,
									 // 这里介绍3种类型，所以用unsigned int，保留整数
							         // 和一位小数可用unsigned char.
{
	unsigned char H,L;
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
	//temp = ((H<<8) + L)*5/8;
	
	//3.保留2位小数--23.45
	return ((H<<8)+L)*25/4;  // (50/8)
}
//============================================Ds1302初始化============================================
void My_Write_Ds1302()
{
	Write_Ds1302_Byte(0x8e,0x00);
	Write_Ds1302_Byte(0x84,dat1);//时
	Write_Ds1302_Byte(0x82,dat2);//分
	Write_Ds1302_Byte(0x80,dat3);//秒
}
void My_Read_Ds1302()
{
	ds1302_h = Read_Ds1302_Byte(0x85);//时
	ds1302_h = (ds1302_h >> 4) * 10 + (ds1302_h & 0x0f);
	ds1302_m = Read_Ds1302_Byte(0x83);//分
	ds1302_m = (ds1302_m >> 4) * 10 + (ds1302_m & 0x0f);
	ds1302_s = Read_Ds1302_Byte(0x81);//秒
	ds1302_s = (ds1302_s >> 4) * 10 + (ds1302_s & 0x0f);
}
//============================================PCF8951部分=======================================
// A/D读取光敏电阻(RD1)或电压采集(RB2)数值
unsigned char Read_ADC(unsigned char n)
{
	unsigned char temp;
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	if (n == 1)
	{
		IIC_SendByte(0x01);//读取光敏电阻RB1
	}
	else if (n == 2)
	{
		IIC_SendByte(0x03);//读取滑动变阻器RB2
	}
	IIC_WaitAck();
	IIC_Stop();
	
	Delay_Keys(500);//while(500--);延时等待
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	temp = IIC_RecByte();//读取
	IIC_SendAck(1); 
	IIC_Stop();
	return temp;
}

//Read_ADC(1) -> (0~255) -> *2 -> (0~500)V

// D/A写电压
void Xie_ADC(unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0x90);
    IIC_WaitAck();
    IIC_SendByte(0x40);
    IIC_WaitAck();
    IIC_SendByte(dat);
    IIC_WaitAck();
    IIC_Stop();
}
//============================================定时器T0初始化=======================================
uchar count = 0;
uchar pwm_dut = 0;//占空比
void Init_T0()
{
	TMOD = (TMOD & 0xf0) | 0x01;  // 定时
	TH0 = (65536 - 2000) / 256;
	TL0 = (65536 - 2000) % 256;
	
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}
//============================================定时器T1初始化=======================================
void Init_T1()
{
	TMOD = (TMOD & 0x0f) | 0x00;//定时
//	TH1 = (65536 - 12) / 256;
//	TL1 = (65536 - 12) % 256;
	
//	EA = 1;
//	ET1 = 1;
//	TR1 = 1;
}
//============================================超声波测距=======================================
sbit TX = P1 ^ 0;
sbit RX = P1 ^ 1;

//======发射超声波信号====
void send_Wave()
{
	unsigned char i = 8;
	// 12MHZ的12分频，间隔定时12us;
	TR1 = 0;
	TH1 = 0xff;
	TL1 = 0xf4;
	TF1 = 0;
	TR1 = 1;
	// 发射8个40khz的超声波信号
	while (i -- ) {
		TX = 0;
		while (!TF1);
		TF1 = 0;
		TX = 1;
		while (!TF1);
		TF1 = 0;
	}
}

void Get_Distance() {
	// 先发射8个40khz的超声波信号
	send_Wave();
	
	TR1 = 0;
	TL1 = 0x00;
	TH1 = 0xe0;
	TF1 = 0;
	TR1 = 1;
	while (RX && !TF1);
	TR1 = 0;
	
	if (TF1 == 1)
	{
		TF1 = 0;
		distance = 999;
	}
	// 超声波速度约344m/s
	// 距离 L = （344m/s*t）/2=172m/s*t
	// t取微秒，即：L = 0.0172cm/us*t
	else
		distance = (((TH1 & 0x1f) << 8) | TL1) * 0.0172;
}

uchar Get_Distance_2()
{
	uchar temp = 16;
	TX = 0;
	TR1 = 1;
	// 引脚发送40Khz方波信号
	while (temp--) // 每隔12us发8个方波
	{
		while (!TF1)
			;
		TX ^= 1; // if(TX==1) { Delay20us();}//可以不用
		TH1 = (65536 - 12) / 256;
		TL1 = (65536 - 12) % 256;
		TF1 = 0;
	}
	//	while(temp--)
	//	{
	//		while(!TF0);
	//		TX ^= 1;//TX = ~TX;
	//		TH0=(65536-12)/256;
	//		TL0=(65536-12)%256;
	//		TF0 = 0;//TF0必须要先赋值(益处中断，自动置1)在清零
	//	}
	TR1 = 0;
	TL1 = 0;
	TH1 = 0;
	TR1 = 1;
	while (RX && !TF1)
		;
	TR1 = 0;

	if (TF1 == 1)
	{
		TF1 = 0;
		return 999;
	}
	else
		return ((TH1 << 8) + TL1) * 0.017;
}
//============================================串口初始化============================================
#include "intrins.h"
#include "string.h"

sfr AUXR = 0x8e;  // 辅助寄存器
sfr T2H=0xd6;  // 定时器2高8位
sfr T2L=0xd7;  // 定时器2低8位
#define BAUD         9600
//#define SYSTEMCLOCK  11059200L
#define SYSTEMCLOCK  12000000L
uchar command = 0x00;
void Init_Uart()
{
	// ===================T1实现===================
//	TMOD = (TMOD & 0x0f) | 0x00;
//	
//	TL1 = (65535-(SYSTEMCLOCK / 4 / BAUD));
//	TH1 = (65535-(SYSTEMCLOCK / 4 / BAUD)) >> 8l;
//	
////	ET1 = 0;		    //禁止定时器1中断
//	TR1 = 1;		    //启动定时器1
//	
//	SCON = 0x50;
//	AUXR = 0x40;
	// ===================T1实现===================
	
	// ===================T2实现===================
	SCON = 0x50;  // 8位数据,可变波特率
    AUXR |= 0x01; // 串口1选择定时器2为波特率发生器
    AUXR |= 0x04; // 定时器时钟1T模式
	
	T2L = (65535-(SYSTEMCLOCK / 4 / BAUD));
	T2H = (65535-(SYSTEMCLOCK / 4 / BAUD)) >> 8l;
	
//	ET1 = 0;		    //禁止定时器1中断
//	TR1 = 1;		    //启动定时器1
//	SCON = 0x50;
//	AUXR = 0x40;
	
	AUXR |= 0x10; // 定时器2开始计时
	// ===================T2实现===================
	
	ES = 1;
	EA = 1;
	
//	SCON = 0x50;		//8位数据,可变波特率
//	AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
//	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
//	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
//	TL1 = 0xE8;		//设定定时初值
//	TH1 = 0xFF;		//设定定时初值
//	ET1 = 0;		//禁止定时器1中断

//	ES = 1;
//	EA = 1;  
//	TR1 = 1;		//启动定时器1
}
// 发送一个字节数据
void Send_Byte(unsigned char dat)
{
	SBUF = dat;
	while(TI == 0);
	TI = 0;
}
//发送一串字符串
void Send_String(unsigned char* str)
{
  
  while(*str != '\0')
  {
    Send_Byte(*(str++));
  }
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
	
	num_On_Off = Read_24C02_Byte(0x00);
	
	Init_T1();
//	TMOD = (TMOD & 0x0f) | 0x00;//定时
	Init_T0();
	Init_18b20();
//	My_Write_Ds1302();

//	num_On_Off = Read_24C02_Byte(0x00);
//	num_On_Off ++ ;
//	Write_24C02_Byte(0x00, num_On_Off);

	Read_24C02_Page(0x01, dat, 3);
	
	
//	dat2 = (dat[2] / 10) << 4 | (dat[2] % 10);
	dat1 = dat[0] /10*16 + dat[0] % 10;
	dat2 = dat[1] /10*16 + dat[1] % 10;
	dat3 = dat[2] /10*16 + dat[2] % 10;
	
	My_Write_Ds1302();
	
	Init_Uart();
//	Send_Byte(dat[2] /10*16 + dat[2] % 10);
//	Send_Byte(0xA0);
	Send_String("I am OK\r\n");
	
	while(1)
	{
//		LED_control();
        if(f_2ms == 1)
        {
            f_2ms = 0;			
			LED_control();  // xxx
//			Write_24C02_Byte(0x02,ds1302_s);	

			distance = Get_Distance_2();  // 超声波测距		
			// =======================按键开始========================
			/*根据需要选择对应的按键类型*/
//			Change_Keys_Alone();  // 1.独立按键扫描(1)
//			ScanKeys_Alone();     // 2.独立按键扫描(2)
			Change_Keys();        // 3.矩阵按键扫描
			if (GN == 13) {  // S13
				// =========================================== 代码开始 ======================================
				LED1_On();
				// =========================================== 代码结束 ======================================
				while(Scan_Keys());
				// =========================================== 代码开始 ======================================
				LED1_Off();
				dis_model ++ ;
				if (dis_model > 5) dis_model = 0;
				// =========================================== 代码结束 ======================================
			}
			
			if (GN == 12) {  // S12 num_s 暂停
				// =========================================== 代码开始 ======================================
				LED2_On();
				// =========================================== 代码结束 ======================================
				while(Scan_Keys());
				// =========================================== 代码开始 ======================================
				LED2_Off();
				f_time = ~f_time;
				// =========================================== 代码结束 ======================================
			}
			
			if (GN == 5) {  // S12 num_s 暂停
				// =========================================== 代码开始 ======================================
				LED0_On();
				
//				count_key_t =0 ;  // 时间计数清0
				
				// =========================================== 代码结束 ======================================
				while(Scan_Keys());
				// =========================================== 代码开始 ======================================
				LED0_Off();
				stat_key ++ ;  // 按键按下次数
				// =========================================== 代码结束 ======================================
			}
			if (count_key_t > 500) {
				// 单击
				if (stat_key == 1) {
					if (stat_led[6] == 0) LED6_On();
					else LED6_Off();
				}
				// 双击
				if (stat_key == 2) {
					if (stat_led[7] == 0) LED7_On();
					else LED7_Off();
				}
				count_key_t = 0;
				stat_key = 0;
			}
			
			if (GN == 8) {  // S8
				// =========================================== 代码开始 ======================================
				LED5_On();
				// =========================================== 代码结束 ======================================
				while(Scan_Keys());
				// =========================================== 代码开始 ======================================
				LED5_Off();
				
				// 时间加
				if (set_ds1302_key_modele == 1) {  // 小时设置
					set_ds1302_h ++ ;
					if (set_ds1302_h > 23) set_ds1302_h = 0;
				}
				else if (set_ds1302_key_modele == 2) {  // 分钟设置
					set_ds1302_m ++ ;
					if (set_ds1302_m > 59) set_ds1302_m = 0;
				}
				else if (set_ds1302_key_modele == 3) {  // 秒设置
					set_ds1302_s ++ ;
					if (set_ds1302_s > 59) set_ds1302_s = 0;
				}
				
				// =========================================== 代码结束 ======================================
			}
			
			if (GN == 9) {  // S9
				// =========================================== 代码开始 ======================================
				LED5_On();
				// =========================================== 代码结束 ======================================
				while(Scan_Keys());
				// =========================================== 代码开始 ======================================
				LED5_Off();
				
				// 时间减
				if (set_ds1302_key_modele == 1) {  // 小时设置
					set_ds1302_h -- ;
					if (set_ds1302_h < 0) set_ds1302_h = 23;
				}
				else if (set_ds1302_key_modele == 2) {  // 分钟设置
					set_ds1302_m -- ;
					if (set_ds1302_m < 0) set_ds1302_m = 59;
				}
				else if (set_ds1302_key_modele == 3) {  // 秒设置
					set_ds1302_s -- ;
					if (set_ds1302_s < 0) set_ds1302_s = 59;
				}
				// =========================================== 代码结束 ======================================
			}

			if (GN == 4) {  // S4
				// =========================================== 代码开始 ======================================
				LED3_On();
				is_key_set_time = 1;  // 按键按下未松开
				
				
				// =========================================== 代码结束 ======================================
				while(Scan_Keys());
				// =========================================== 代码开始 ======================================
				LED3_Off();
				LED4_Off();
				is_key_set_time = 0;  // 按键按下松开
				num_s_key = 0;
				
				if (f_key_long) {  // 长按转换模式
					set_ds1302_key_modele ++ ;
					f_key_long = 0;
				}
				
				
//				if (set_ds1302_key_modele == 1) {  // 小时设置
//					set_ds1302_h = 23;
//				}
//				else if (set_ds1302_key_modele == 2) {  // 分钟设置
//					set_ds1302_m = 29;
//				}
//				else if (set_ds1302_key_modele == 3) {  // 秒设置
//					set_ds1302_s = 45;
//				}
				if (set_ds1302_key_modele == 4) {  // 确认
					if (dis_model == 2) {
						dat[0] = set_ds1302_h;
						dat[1] = set_ds1302_m;
						dat[2] = set_ds1302_s;
						Write_24C02_Page(0x01, dat, 3);
						
						dat1 = dat[0] /10*16 + dat[0] % 10;
						dat2 = dat[1] /10*16 + dat[1] % 10;
						dat3 = dat[2] /10*16 + dat[2] % 10;

						My_Write_Ds1302();
					}
					set_ds1302_key_modele = 0;
				}
				// =========================================== 代码结束 ======================================
			}
			/*根据需要添加对应按键*/
			// =======================按键结束========================
			
			// =======================串口调试开始====================
			if(command != 0x00)
			{
				switch(command & 0xf0)
				{
					case 0xa0:
						P0 = temp_Led_P0;
						P2 = (P2 & 0x1f) | 0x80;  // LED
						P0 = (P0 | 0x0f) & ((~command) | 0xf0);
						temp_Led_P0 = P0;
						P2 &= 0x1f;
						command = 0x00;
					break;
					case 0xb0:
						P0 = temp_Led_P0;
						P2 = (P2 & 0x1f) | 0x80;  // LED
						P0 = (P0 | 0xf0) & (((~command) << 4) | 0x0f);
						temp_Led_P0 = P0;
						P2 &= 0x1f;
						command = 0x00;
					break;
					case 0xc0:
						Send_String("system is running...!\r\n");
						command = 0x00;
					break;
				}
			}
			// =======================串口调试结束====================		
			
			// =========================================== 代码开始 ======================================			
			// =======================1.温度读取开始========================
			if (f_200ms_temperature) {
				f_200ms_temperature = 0;
				
				My_Read_Ds1302();
			}
			temperature = Read_Temperature();
//			num1=10;num2=10;num3=10;num4=10;
//			num5=temperature/1000;num6=temperature/100%10;
//			num7=temperature/10%10;num8=temperature%10;
			// =======================1.温度读取结束========================
			
			dat[0] = ds1302_h;
			dat[1] = ds1302_m;
			dat[2] = ds1302_s;
			Write_24C02_Page(0x01, dat, 3);
			
			if (dis_model == 0) {  // 时间显示
				num1=ds1302_h/10;num2=ds1302_h%10;num3=11;
				num4=ds1302_m/10;num5=ds1302_m%10;num6=11;
				num7=ds1302_s/10;num8=ds1302_s%10;
			}
			else if (dis_model == 1) {  // 温度显示
				num1=dis_model;num2=10;num3=10;num4=10;
				num5=temperature/1000;num6=temperature/100%10;
				num7=temperature/10%10;num8=temperature%10;
			}
			else if (dis_model == 2) {  // 时间设置
//				num_On_Off = Read_24C02_Byte(0x00);
//				num_On_Off ++ ;
//				Write_24C02_Byte(0x00, num_On_Off);
				f_set_time = 1;  // 时间设置标志
				
				if (set_ds1302_key_modele == 1) {
					if (f_500ms_smg) num1 = 10, num2 = 10;
					else num1 = set_ds1302_h / 10, num2 = set_ds1302_h % 10;
				}
				else num1 = set_ds1302_h / 10, num2 = set_ds1302_h % 10;
				
				if (set_ds1302_key_modele == 2) {
					if (f_500ms_smg) num4 = 10, num5 = 10;
					else num4 = set_ds1302_m / 10, num5 = set_ds1302_m % 10;
				}
				else num4 = set_ds1302_m / 10, num5 = set_ds1302_m % 10;
				
				if (set_ds1302_key_modele == 3) {
					if (f_500ms_smg) num7 = 10, num8 = 10;
					else num7 = set_ds1302_s / 10, num8 = set_ds1302_s % 10;
				}
				else num7 = set_ds1302_s / 10, num8 = set_ds1302_s % 10;
				
				num3 = 11; 
				num6 = 11;
				
			}
			else if (dis_model == 3) {
				num1 = dis_model;num2 = 10; num3 = 10; num4 = 10;
				num5= 10;
				num6=distance / 100; num7=distance % 100 / 10; num8=distance % 10;
			}
			else if (dis_model ==4) {
				dat_rd1 = Read_ADC(1);
				rd1_v = dat_rd1*5/255;
				smg_rd1_v = rd1_v * 100;
				num1=dis_model;
				num2=dat_rd1 / 100;num3=(dat_rd1 % 100) / 10;num4 = dat_rd1 % 10;
				num5=11;
				num6=smg_rd1_v / 100;num7=(smg_rd1_v % 100) / 10;num8=smg_rd1_v % 10;
			}
			else if (dis_model == 5) {
				num1 = dis_model;num2 = 10; num3 = 10; num4 = 10;
				num5= 10;
				num6=11; num7=stat_key / 10; num8=stat_key%10;
			}
			// =========================================== 代码结束 ======================================
        }
    }
}

void Service_T0() interrupt 1
{
	TH0 = (65536 - 2000) / 256;
	TL0 = (65536 - 2000) % 256;
	
	f_2ms = 1;
	
	/***************************************************
	与时间相关的变量的更新放在里面
	注意：不要把消耗过长时间的代码（如for循环等）放在这里，容易造成错乱
	***************************************************/
	// =========================================== 代码开始 ======================================
	num_2ms ++ ;
	num_2ms_temperature ++ ;
	if (num_2ms_temperature == 100) {
		f_200ms_temperature = 1;
		num_2ms_temperature = 0;
	}
	
	if (num_2ms == 500) {
		f_1s = 1;
		num_2ms = 0;
	}
	
	if (f_1s == 1) {
		f_1s = 0;
		
		if (f_time) num_s ++ ;
		
		// 按键长按
		if (is_key_set_time && dis_model == 2) {
			num_s_key ++ ;
//			t_key = 0;
			if (num_s_key == 2) {
				LED4_On();
				f_key_long = 1;
			}
		}			
			
		
		if (num_s == 9) 
			num_s = 0;
		
	}
	
	// 数码管闪烁
	num_2ms_smg ++ ;
	if (num_2ms_smg == 250) {
		num_2ms_smg = 0;
		f_500ms_smg = ~f_500ms_smg;
	}
	
	// 按键双击
	if (stat_key != 0) {
		count_key_t ++ ;
	}
	// =========================================== 代码结束 ======================================
	
	Display_SMG(num1,num2,num3,num4,num5,num6,num7,num8);//2ms刷新数码管
}

void ServiceUart() interrupt 4
{
	if(RI == 1)
	{
		RI = 0;//人工清零
		urdat = SBUF;
		command = SBUF;
//		Send_Byte(urdat + 1);
//		send_byte(0x5a);
//		send_byte(0xa5);
	}
}