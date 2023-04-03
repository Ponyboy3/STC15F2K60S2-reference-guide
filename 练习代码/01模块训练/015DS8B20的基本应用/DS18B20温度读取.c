#include "reg52.h"
#include "absacc.h"
#include "onewire.h"

unsigned char code SMGNoDot_CA[10] = 
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char code SMGDot_CA[10] = 
{0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};

unsigned int temp = 0;

void DelaySMG(unsigned int t)
{
	while(t--);
}

void DisplaySMG_Bit(unsigned char pos,unsigned char dat)
{
	XBYTE[0xE000] = 0xff;
	XBYTE[0xC000] = 0x01 << pos;
	XBYTE[0xE000] = dat;
}

void Display_All(unsigned char dat)
{
	XBYTE[0xc000] = 0xff;
	XBYTE[0xe000] = dat;
}

void DisplaySMG_Temp()
{
	DisplaySMG_Bit(7,SMGNoDot_CA[temp % 10]);
	DelaySMG(100);
	DisplaySMG_Bit(6,SMGDot_CA[(temp % 100) / 10]);
	DelaySMG(100);
	DisplaySMG_Bit(5,SMGNoDot_CA[temp / 100]);
	DelaySMG(100);
	
	DisplaySMG_Bit(4,0xff);
	DelaySMG(100);
	DisplaySMG_Bit(3,0xff);
	DelaySMG(100);
	DisplaySMG_Bit(2,0xff);
	DelaySMG(100);
	DisplaySMG_Bit(1,0xff);
	DelaySMG(100);
	DisplaySMG_Bit(0,0xff);
	DelaySMG(100);
	
	Display_All(0xff);
}

void Delay(unsigned int t)
{
	while(t--)
	{
		DisplaySMG_Temp();
	}
}
void Read_DS18B20_temp()
{
	unsigned char LSB,MSB;
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	
	Delay(1000);
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);
	
	LSB = Read_DS18B20();
	MSB = Read_DS18B20();
	
	temp = MSB;
	temp = (temp <<= 8 ) | LSB;
	//temp >>= 4;
	
	
	if((temp & 0xf800) == 0x0000)
	{
		temp >>= 4;
		temp = temp * 10;
		temp = temp + (LSB & 0x0f) * 0.625;
	}
	
}
void main()
{
	XBYTE[0x8000] = 0xff;
	while(1)
	{
		Read_DS18B20_temp();
		DisplaySMG_Temp();
	}
}