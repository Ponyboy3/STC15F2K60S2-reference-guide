#include "reg52.h"
#include "onewire.h"
#include "iic.h"
#include "ds1302.h"
#include "intrins.h"

#define uchar unsigned char 
#define uint unsigned int

// 数码管显示数据， 一般前10个为0-9， 可更具需要往里面添加
unsigned char code type1[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf,
							  0xc6,0x89,0x8e,0x8c,0x86, 0x88};
// 数码管位
uchar code type2[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

unsigned char num1,num2,num3,num4,num5,num6,num7,num8;  // 数码管每一位（例如 num1 = 2, 第一个数码管显示 2）
unsigned char GN;  // 按键标志位
bit f_2ms, f_50ms, f_500ms_smg;  // 2ms, 50ms, 500ms标志位
unsigned int num_2ms_smg;  // 多少个2ms
unsigned char temp_Led_P0 = 0xff;  // 用来存上一次LED状态
unsigned char ds1302_h, ds1302_m, ds1302_s;  // ds1302数据
bit f_200ms_ds1302;
unsigned int num_2ms_ds1302;  // // 多少个2ms
bit f_key_long, is_key_set_time;
unsigned char t_key;  // 按键按住时间
// 超声波测距
unsigned int distance;
// PCF8951
unsigned char dat_rd1, dat_rd2;  // ADC读取数据
float rd1_v;
unsigned int smg_rd1_v;
// 按键
unsigned char stat_key;  // 按键按下次数
unsigned int count_key_t;  // 10ms单位累计

bit f_1s;  // 1s标志位

unsigned int num_2ms = 0, num_s = 0, num_s_key = 0;  // 多少个2ms, 1s, 按键按下时间

bit f_time = 1;  // 定时器0秒钟是否暂停

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

unsigned char stat_led[8];  // LED状态

// =========================================== 代码开始 ======================================
unsigned char dis_model_s4 = 0;  // 0:显示温度（DS18B20）， 1:显示时钟 2:回显， 3：参数
unsigned char dis_model_s5 = 0;  // 0：温度， 1：湿度，2：时间
unsigned int temperature, max_temperature = 0;  // 温度
int temperature_parameters = 30;  // 温度参数
unsigned int temper[2], Temper_all = 0, Temper_average = 0;  // 平均温度
bit is_mesure_temp, is_mesure_temp_ing;  // 测量温度触发, 是否在测量中
unsigned int num_2ms_mesure, num_mesure_s;  // 测量温度触发时间

unsigned char dat1 = 0x13,dat2 = 0x03,dat3 = 0x05;  // （16进制）ds1302写数据

bit is_key_s9_pressing;  // 按键按下未松开
unsigned char num_s_key_pressing;  // 按键按下时间

// Record记录
unsigned int is_mesure_temp_num, mesure_h, mesure_m;  // 测量次数


// 警报
bit is_temp_more_mesure;  // 大于温度参数
bit f_100ms_L4;  // L4亮灭时间
unsigned int num_2ms_L4;  // 多少个2ms

unsigned int count_f_NE555 = 0, dat_f = 0;  // NE555计数
unsigned int count_2ms_NE555 = 0;
unsigned int temp_f,temp_t;
bit f;
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
	TR1 = 0;
	P0 = 0xff & temp_Led_P0;  // 清除其他地方对P0的值（特别是数码管一直在刷新），并且结合LED前一次的状态
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L0 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
	P2 &= 0x1f;
	
	stat_led[0] = 1;
}
void LED0_Off() {
	TR1 = 0;
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L0 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
	P2 &= 0x1f;
	
	stat_led[0] = 0;
}
// ======================LED1====================
void LED1_On() {
	TR1 = 0;
	P0 = 0xff & temp_Led_P0;  // 清除其他地方对P0的值（特别是数码管一直在刷新），并且结合LED前一次的状态
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L1 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
	P2 &= 0x1f;
	
	stat_led[1] = 1;
}
void LED1_Off() {
	TR1 = 0;
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L1 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
	P2 &= 0x1f;
	
	stat_led[1] = 1;
}
// ======================LED2====================
void LED2_On() {
	TR1 = 0;
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L2 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
	P2 &= 0x1f;
}
void LED2_Off() {
	TR1 = 0;
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L2 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
	P2 &= 0x1f;
}
// ======================LED3====================
void LED3_On() {
	TR1 = 0;
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L3 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
	P2 &= 0x1f;
}
void LED3_Off() {
	TR1 = 0;
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L3 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
	P2 &= 0x1f;
}
// ======================LED4====================
void LED4_On() {
	TR1 = 0;
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L4 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
	P2 &= 0x1f;
}
void LED4_Off() {
	TR1 = 0;
	P0 = 0xff & temp_Led_P0 & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L4 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
	P2 &= 0x1f;
}
// ======================LED5====================
void LED5_On() {
	TR1 = 0;
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L5 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
	P2 &= 0x1f;
}
void LED5_Off() {
	TR1 = 0;
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L5 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
	P2 &= 0x1f;
}
// ======================LED6====================
void LED6_On() {
	TR1 = 0;
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L6 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
	P2 &= 0x1f;
	
	stat_led[6] = 1;
}
void LED6_Off() {
	TR1 = 0;
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L6 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
	P2 &= 0x1f;
	
	stat_led[6] = 0;
}
// ======================LED7====================
void LED7_On() {
	TR1 = 0;
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L7 = 0;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
	P2 &= 0x1f;
	
	stat_led[7] = 1;
}
void LED7_Off() {
	TR1 = 0;
	P0 = 0xff & temp_Led_P0;
	P2 = (P2 & 0x1f) | 0x80;  // LED控制开启
	L7 = 1;  // 要将P0放在这两句代码之间，避免错乱
	temp_Led_P0 = P0;
	TR1 = 1;
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
									if(0)  // 根据需要的条件
									{
										P0 = type1[n6]&0x7f;  // 加入小数点
									}
									else
										P0 = type1[n6];
									P2 &= 0x1f;break;
		case 6:P2 = (P2 & 0x1f) | 0xe0;
									if(!dis_model_s5 && dis_model_s4 == 1 && !is_mesure_temp)  // 根据需要的条件
									{
										P0 = type1[n7]&0x7f;  // 加入小数点
									}
									else
										P0 = type1[n7];
									P2 &= 0x1f;break;
		case 7:P2 = (P2 & 0x1f) | 0xe0;P0 = type1[n8];P2 &= 0x1f;break;
	}
	if(++i==8)
		i = 0;
}

//============================================按键==============================================
void Delay_Keys(uint t)
{
	while(t--);
}
//============================================矩阵按键============================================
sfr P4 = 0xc0;
sbit P44 = P4^4;
sbit P42 = P4^2;

uchar Scan_Keys()
{
	char H,L;
	
	TR0 = 0;
	
	P3 =  0xcf;P44 = 0;P42 = 0;H = (~P3) & 0x0f;
	if(H == 0) return 0;
	Delay_Keys(100);
	P3 =  0xcf;P44 = 0;P42 = 0;H = (~P3) & 0x0f;
	if(H == 0) return 0;
	P3 =  0xf0;P44 = 1;P42 = 1;L = (~P3) & 0x30;
	if(P44 == 0) L |= 0x80;
	if(P42 == 0) L |= 0x40;
	
	TR0 = 1;
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

sbit R1 = P3^3;
sbit R2 = P3^2;
sbit R3 = P3^0;
sbit C1 = P4^4;
sbit C2 = P4^2;
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
//============================================Ds1302初始化============================================
void My_Write_Ds1302()
{
	Write_Ds1302_Byte(0x8e,0x00);  // 解除写保护
	Write_Ds1302_Byte(0x84,dat1);//时
	Write_Ds1302_Byte(0x82,dat2);//分
	Write_Ds1302_Byte(0x80,dat3);//秒
	
	Write_Ds1302_Byte(0x8e,0x80);  // 使能写保护
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
	TMOD = (TMOD & 0xf0) | 0x05;
	TH0 = 0;
	TL0 = 0;
	
	TR0 = 1;
}
//============================================定时器T1初始化=======================================
void Init_T1()
{
	TMOD = (TMOD & 0x0f) | 0x10;  // 定时
	TH1 = (65536 - 2000) / 256;
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
	if (!is_mesure_temp && dis_model_s4 == 0) {  // 时间显示界面
		LED0_On();  // L1亮
	}
	else {
		LED0_Off();  // L1灭
	}
	
	if (!is_mesure_temp && dis_model_s4 == 1) {  // 时间显示界面
		LED1_On();  // L2亮
	}
	else {
		LED1_Off();  // L2灭
	}
	if (is_mesure_temp) {  // 时间显示界面
		LED2_On();  // L3亮
	}
	else {
		LED2_Off();  // L3灭
	}
	
	// 报警指示灯
	if (is_temp_more_mesure == 1) {
		if (f_100ms_L4) LED3_Off();
		else LED3_On();
	}
	else LED3_Off();  // L4灭
	
	
	LED6_Off();
	// =========================================== 代码结束 ======================================
}

void main()
{
	Init_Sys();
	
	Init_T1();
//	TMOD = (TMOD & 0x0f) | 0x00;//定时
	Init_T0();
	Init_18b20();
	Read_Temperature();

	My_Write_Ds1302();
	
	while(1)
	{
//		LED_control();
//		Scan_Keys_2x2();
        if(f_2ms == 1)
        {
            f_2ms = 0;			
			LED_control();
			// =======================按键开始========================
			/*根据需要选择对应的按键类型*/
//			Change_Keys_Alone();  // 1.独立按键扫描(1)
//			ScanKeys_Alone();     // 2.独立按键扫描(2)
			
			if (!is_mesure_temp)  // 测量NE555时避免和数码管冲突
			{
				TR1 = 0;
				Change_Keys();        // 3.矩阵按键扫描
				TR1 = 1;
			}
			
			/*根据需要添加对应按键*/
			if (GN == 4) {  // S4
				// =========================================== 代码开始 ======================================
				
				dis_model_s4 ++ ;
				if (dis_model_s4 > 2) dis_model_s4 = 0;
				// =========================================== 代码结束 ======================================
				while(Scan_Keys());
				// =========================================== 代码开始 ======================================
				
				// =========================================== 代码结束 ======================================
			}
			
			if (GN == 5) {  // S5
				// =========================================== 代码开始 ======================================
				
				dis_model_s5 ++ ;
				if (dis_model_s5 > 2) dis_model_s5 = 0;
				// =========================================== 代码结束 ======================================
				while(Scan_Keys());
				// =========================================== 代码开始 ======================================

				// =========================================== 代码结束 ======================================
			}
			if (GN == 8) {  // S8
				// =========================================== 代码开始 ======================================

				if (dis_model_s4 == 2 && !is_mesure_temp) {  // 参数设置界面
					temperature_parameters ++ ;
					if (temperature_parameters > 99) temperature_parameters = 99;
				}
				// =========================================== 代码结束 ======================================
				while(Scan_Keys());
				// =========================================== 代码开始 ======================================

				// =========================================== 代码结束 ======================================
			}
			if (GN == 9) {  // S9
				// =========================================== 代码开始 ======================================

				if (dis_model_s4 == 2 && !is_mesure_temp) {  // 参数设置界面
					temperature_parameters -- ;
					if (temperature_parameters < 0) temperature_parameters = 0;
				}
				if (dis_model_s5 == 2 && !is_mesure_temp) {  // 时间回转界面
					is_key_s9_pressing = 1;  // 按键按下未松开
				}
				// =========================================== 代码结束 ======================================
				while(Scan_Keys());
				// =========================================== 代码开始 ======================================

				LED4_Off();
				
				is_key_s9_pressing = 0;  // 按键按下松开
				num_s_key_pressing = 0;
				
				if (f_key_long) {  // 长按转换模式
					is_mesure_temp_num = 0;
					f_key_long = 0;
				}
				// =========================================== 代码结束 ======================================
			}
			
			if (GN == 7) {  // S7
				// =========================================== 代码开始 ======================================
				// =========================================== 代码结束 ======================================
				while(Scan_Keys());
				// =========================================== 代码开始 ======================================
				if (!is_mesure_temp_ing) {
					num_2ms_mesure = 0;
					num_mesure_s = 0;
					
					is_mesure_temp_ing = 1;
					is_mesure_temp = 1;
					is_mesure_temp_num ++ ;  // 触发次数
				}
				// =========================================== 代码结束 ======================================
			}
			// =======================按键结束========================
						
			// =========================================== 测试代码开始 ======================================			
			// =======================1.温度读取开始========================
			
			// =======================1.温度读取结束========================
			
			
			
			// =========================================== 测试代码结束 ======================================
			
			// =========================================== 代码开始 ======================================
			if (f_200ms_ds1302) {
				f_200ms_ds1302 = 0;
				My_Read_Ds1302();
			}
			
			if (is_mesure_temp) {  // 触发测量 
				
				TR0 = 1;
				
				mesure_h = ds1302_h, mesure_m = ds1302_s;
				
//				TR1 = 0;
				temperature = Read_Temperature();
//				TR1 = 1;
				temper[(is_mesure_temp_num-1) % 10] = temperature;
				
				if (max_temperature < temperature && temperature < 8500)
					max_temperature = temperature;  // 最高温度
				
				Temper_all = 0;
				

				if (is_mesure_temp_num < 10) {
					unsigned char i;
					for (i = 0; i < is_mesure_temp_num; i ++ ) {
						Temper_all += temper[i];
					}
					Temper_average = Temper_all / is_mesure_temp_num;
				}
				else {
					unsigned char i;
					for (i = 0; i < 10; i ++ ) {
						Temper_all += temper[i];
					}
					Temper_average = Temper_all / 10;
				}
			}
			
			if ((temperature_parameters * 10) < temperature) {
					is_temp_more_mesure = 1;
				}
			else 
				is_temp_more_mesure = 0;
			
			if (!is_mesure_temp) {  // 没有触发温湿度测量
				if (dis_model_s4 == 0) {  // 时间显示
					num1=ds1302_h/10;num2=ds1302_h%10;num3=11;
					num4=ds1302_m/10;num5=ds1302_m%10;num6=11;
					num7=ds1302_s/10;num8=ds1302_s%10;
					
					dis_model_s5 = 0;
				}
				else if (dis_model_s4 == 1) {  // 回转
					
					if (dis_model_s5 == 0) {
						num1=12;num2=10;
						num3=max_temperature/100;num4=max_temperature%100/10;
						num5=max_temperature % 10;
						num6=Temper_average/ 100;num7=Temper_average % 100 / 10;num8=Temper_average%10;
					}
					if (dis_model_s5 == 1) {  // 
						num1=13;num2=10;num3=10;num4=10;
						num4=11;num5=11;num6=11;
						num7=11;num8=11;
					}
					if (dis_model_s5 == 2) {  // F
						num1=14;num2=is_mesure_temp_num / 10;num3=is_mesure_temp_num % 10;
						num4=mesure_h / 10;num5=mesure_h % 10;
						num6=11;
						num7=mesure_m / 10;num8=mesure_m % 10;
					}

				}
				else if (dis_model_s4 == 2) {  // P温度参数
					num1=15;
//					num2=10;num3=10;num4=10;
//					num4=10;num5=10;num6=10;
					
					num2=dat_f / 10000;num3=(dat_f % 10000) / 1000;num4=(dat_f % 1000) / 100;
					num5=(dat_f % 100) / 10;num6=dat_f % 10;
					
					num7=temperature_parameters/10;num8=temperature_parameters%10;
				}
			}
			else {   // 触发温湿度测量
				num1=16;num2=10;num3=10;num4=10;
				num4=temperature/100;num5=temperature%100/10;
				num6=11;
				num7=(dat_f % 100) / 10;num8=dat_f % 10;
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
	
	num_2ms ++ ;
	
	if (num_2ms == 500) {
		f_1s = 1;
		num_2ms = 0;
	}
	
	if (f_1s == 1) {
		f_1s = 0;
		
		if (f_time) num_s ++ ;
		
		// 按键长按2s
		if (is_key_s9_pressing) {
			num_s_key_pressing ++ ;
			if (num_s_key_pressing == 2) {
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
	
	// 时间读取刷新
	num_2ms_ds1302 ++ ;
	if (num_2ms_ds1302 == 100) {
		f_200ms_ds1302 = 1;
		num_2ms_ds1302 = 0;
	}
	
	// 测量模式
	if (is_mesure_temp) {
		num_2ms_mesure ++ ;
		if (num_2ms_mesure == 500) {
			num_mesure_s ++ ;
			num_2ms_mesure = 0;
		}
		if (num_mesure_s >= 3) {
			num_mesure_s = 0;
			is_mesure_temp = 0;  // 测量结束
			is_mesure_temp_ing = 0;  // 测量结束，可用开始下一次测量
			
			dis_model_s4 = 1;
			dis_model_s5 = 0;
		}
	}
	
	// L4管闪烁
	num_2ms_L4 ++ ;
	if (num_2ms_L4 == 50) {
		num_2ms_L4 = 0;
		f_100ms_L4 = ~f_100ms_L4;
	}
	
//	++ count_2ms_NE555;
//	if( count_2ms_NE555 == 500)
//	{
//		dat_f = (TH0 << 8) | TL0;
//		count_2ms_NE555 = 0;
//		
//		TH0 = 0;
//		TL0 = 0;
//	}
	
	if (is_mesure_temp)
	{
//		if(++count_2ms_NE555 == 500)
//		{
//			count_2ms_NE555 = 0;
//			dat_f = (TH0 << 8) | TL0;
//			TH0 = 0;
//			TL0 = 0;
//		}
	}
	else 
		TR0 = 0;
	/***************************************************
	与时间相关的变量的更新放在里面
	注意：不要把消耗过长时间的代码（如for循环等）放在这里，容易造成错乱
	***************************************************/
	// =========================================== 代码开始 ======================================
	
	// =========================================== 代码结束 ======================================
	
	Display_SMG(num1,num2,num3,num4,num5,num6,num7,num8);//2ms刷新数码管
}