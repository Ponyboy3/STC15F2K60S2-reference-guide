#include "reg52.h"

sbit L1 = P0^0;
sbit S7 = P3^0;

unsigned char count = 0;
unsigned char pwm_dut = 0;

void Init_T0_PWM()
{
	TMOD = 0x01;
	TH0 = (65536 - 100) / 256;
	TL0 = (65536 - 100) % 256;
	
	ET0 = 1;
	EA = 1;
}

void Service_T0() interrupt 1
{
	TH0 = (65536 - 100) / 256;
	TL0 = (65536 - 100) % 256;
	
	count++;
	if(count <= pwm_dut)
	{
		L1 = 0;
	}
	else if(count < 100)
	{
		L1 = 1;
	}
	else if(count == 100)
	{
		L1 = 0;
		count = 0;
	}
}

void Delay(unsigned int t)
{
	while(t--);
}

unsigned char key_stat = 0;
void Scan_Key()
{
	if(S7 == 0)
	{
		Delay(100);
		if(S7 == 0)
		{
			switch(key_stat)
			{
				case 0:
					L1 = 0;
					key_stat = 1;
					TR0 = 1;
					pwm_dut = 10;
				break;
				
				case 1:
					L1 = 0;
					key_stat = 2;
					pwm_dut = 50;
				break;
				
				case 2:
					L1 = 0;
					key_stat = 3;
					pwm_dut = 100;
				break;
				
				case 3:
					L1 = 1;
					TR0 = 0;
					key_stat = 0;
				break;
			}
			
			while(S7 == 0);
		}
	}
}

void Init_Sys()
{
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0xff;
	P2 &= 0x1f;
	
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;
}

void main()
{
	Init_Sys();
	Init_T0_PWM();
	while(1)
	{
		Scan_Key();
	}
}