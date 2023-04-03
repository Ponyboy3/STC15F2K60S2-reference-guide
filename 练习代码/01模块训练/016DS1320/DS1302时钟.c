#include "reg52.h"
#include "absacc.h"
#include "ds1302.h"
#include "smg_ca.h"

code unsigned char SMG_duanma[18] = 
		{0xc0,0xf9,0xa4,0xb0,0x99,0x92,
		 0x82,0xf8,0x80,0x90}; 

unsigned char code Write_DS1302_adrr[7] = {0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};
unsigned char code Read_DS1302_adrr[7] = {0x81,0x83,0x85,0x87,0x89,0x89,0x8d};
//20年4月18日，周六，23点59分24秒；
unsigned char Timer[7] = {0x24,0x59,0x23,0x18,0x04,0x06,0x20};

void DS1302_Config()
{
	char i;
	Write_Ds1302_Byte(0x8e,0x00);//保护
	for(i = 0;i < 7;i++)
	{
		Write_Ds1302_Byte(Write_DS1302_adrr[i],Timer[i]);
	}
	Write_Ds1302_Byte(0x8e,0x80);//保护
} 

void Read_DS1302_Time()
{
	char i;
	for(i = 0;i < 7;i++)
	{
		Timer[i] = Read_Ds1302_Byte(Read_DS1302_adrr[i]);
	}
	
}

void Display_DS1302()
{
	Display_SMG(SMG_duanma[Timer[2]/16],0);
	DelaySMG(100);
	Display_SMG(SMG_duanma[Timer[2]%16],1);
	DelaySMG(100);
	
	Display_SMG(0xbf,2);
	DelaySMG(100);
	
	Display_SMG(SMG_duanma[Timer[1]/16],3);
	DelaySMG(100);
	Display_SMG(SMG_duanma[Timer[1]%16],4);
	DelaySMG(100);
	
	Display_SMG(0xbf,5);
	DelaySMG(100);
	
	Display_SMG(SMG_duanma[Timer[0]/16],6);
	DelaySMG(100);
	Display_SMG(SMG_duanma[Timer[0]%16],7);
	DelaySMG(100);
	
	SelectHC573(6);
	P0 = 0xff;
	SelectHC573(7);
	P0 = 0xff;
}

void main()
{
	DS1302_Config();
	while(1)
	{
		Read_DS1302_Time();
		Display_DS1302();
	}
}