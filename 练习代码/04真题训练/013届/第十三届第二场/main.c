#include "reg52.h"
#include "iic.h"

#define uchar unsigned char
#define uint unsigned int

sbit TX = P1 ^ 0;
sbit RX = P1 ^ 1;

uchar code type1[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff, 0xc1, 0xc7, 0x8c, 0x88};
uchar code type2[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

uchar num1, num2, num3, num4, num5, num6, num7, num8;
bit f_2ms;
uint t_2ms, tt_2ms;
uchar GN;
uint VRB2, Vmax = 450, Vmin = 50;
uchar model;
uchar distance;
bit f_S5;
bit shanshuo;
bit f_cs;
uchar L_stat;
bit f_100ms;

void Display_Smg(uchar n1, uchar n2, uchar n3, uchar n4, uchar n5, uchar n6, uchar n7, uchar n8)
{
	uchar i;
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = type2[i];
	P2 &= 0x1f;
	P0 = 0xff;
	switch (i)
	{
	case 0:
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = type1[n1];
		P2 &= 0x1f;
		break;
	case 1:
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = type1[n2];
		P2 &= 0x1f;
		break;
	case 2:
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = type1[n3];
		P2 &= 0x1f;
		break;
	case 3:
		P2 = (P2 & 0x1f) | 0xe0;
		if (model == 2)
		{
			P0 = type1[n4] & 0x7f;
		}
		else
			P0 = type1[n4];
		P2 &= 0x1f;
		break;
	case 4:
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = type1[n5];
		P2 &= 0x1f;
		break;
	case 5:
		P2 = (P2 & 0x1f) | 0xe0;
		if (model == 0)
		{
			P0 = type1[n6] & 0x7f;
		}
		else
			P0 = type1[n6];
		P2 &= 0x1f;
		break;
	case 6:
		P2 = (P2 & 0x1f) | 0xe0;
		if (model == 2)
		{
			P0 = type1[n7] & 0x7f;
		}
		else
			P0 = type1[n7];
		P2 &= 0x1f;
		break;
	case 7:
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = type1[n8];
		P2 &= 0x1f;
		break;
	}
	if (++i == 8)
		i = 0;
}
void Delay_Keys(uint t)
{
	while (t--)
		;
}
uchar Scan_Keys()
{
	uchar d;
	d = (~P3) & 0x0f;
	if (d == 0)
		return 0;
	Delay_Keys(100);
	d = (~P3) & 0x0f;
	if (d == 0)
		return 0;
	return d;
}
void Change_Keys()
{
	switch (Scan_Keys())
	{
	case 8:
		GN = 1;
		break; // S4
	case 4:
		GN = 2;
		break;
	case 2:
		GN = 3;
		break;
	case 1:
		GN = 4;
		break;
	case 0:
		GN = 0;
		break;
	}
}
uint Du_ADc()
{
	uchar d;
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(0x03);
	I2CWaitAck();
	I2CStop();

	Delay_Keys(500);
	I2CStart();
	I2CSendByte(0x91);
	I2CWaitAck();
	d = I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
	if (d > 250)
		d = 250;
	return d * 2;
}
void Xie_DAc(uchar dat)
{
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(0x40);
	I2CWaitAck();
	I2CSendByte(dat);
	I2CWaitAck();
	I2CStop();
}
uchar Get_Distance()
{
	uchar d = 16;
	TX = 0;
	TR0 = 1;
	while (d--)
	{
		while (!TF0)
			;
		TX ^= 1;
		TH0 = (65536 - 12) / 256;
		TL0 = (65536 - 12) % 256;
		TF0 = 0;
	}
	TR0 = 0;
	TH0 = 0;
	TL0 = 0;
	TR0 = 1;
	while (RX && !TF0)
		;
	TR0 = 0;

	if (TF0 == 1)
	{
		TF0 = 0;
		return 999;
	}
	else
		return ((TH0 << 8) + TL0) * 0.017;
}

void Led_Control()
{
	P0 = 0xff;
	if (model == 0)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 & 0xfe; // L1亮
		P2 &= 0x1f;
	}
	else
	{

		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 | 0x01; ////L1灭
		P2 &= 0x1f;
	}
	if (model == 1)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 & 0xfd; // L2亮
		P2 &= 0x1f;
	}
	else
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 | 0x02; ////L2灭
		P2 &= 0x1f;
	}
	if (model == 2)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 & 0xfb; // L3亮
		P2 &= 0x1f;
	}
	else
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 | 0x04; // L3灭
		P2 &= 0x1f;
	}
	if (f_100ms == 1)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 & 0x7f;
		P2 &= 0x1f;
	}
	else
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = P0 | 0x80;
		P2 &= 0x1f;
	}
	L_stat = P0;
}

void Init_T0()
{
	TMOD = (TMOD & 0xf0) | 0x01;
	TH0 = (65536 - 12) / 256;
	TL0 = (65536 - 12) % 256;
}
void Init_T1()
{
	TMOD = (TMOD & 0x0f) | 0x10;
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;

	EA = 1;
	ET1 = 1;
	TR1 = 1;
}
void Init_Sys()
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;
	P2 &= 0x1f;
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x00;
	P2 &= 0x1f;
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0xff;
	P2 &= 0x1f;
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0xff;
	P2 &= 0x1f;
}
void main()
{
	Init_Sys();
	Init_T1();
	Init_T0();
	while (1)
	{
		if (f_2ms == 1)
		{
			f_2ms = 0;
			Change_Keys();
			VRB2 = Du_ADc();
			if (GN == 1)
			{
				f_S5 = 0;
				f_cs = 0;
				model++;
				if (model > 2)
					model = 0;
				while (Scan_Keys())
					;
			}
			if (GN == 2) // S5
			{
				f_S5 = ~f_S5;
				while (Scan_Keys())
					;
			}
			switch (model)
			{
			case 0: // 电压界面
				num1 = 11;
				num2 = num3 = num4 = num5 = 10;
				num6 = VRB2 / 100;
				num7 = VRB2 / 10 % 10;
				num8 = VRB2 % 10;
				break;

			case 1: // 测量距离
				num1 = 12;
				num2 = num3 = num4 = num5 = 10;
				if ((VRB2 < Vmax) && (VRB2 > Vmin))
				{
					f_cs = 1;
					distance = Get_Distance();
					if (distance >= 100)
					{
						num6 = distance / 100;
						num7 = distance / 10 % 10;
						num8 = distance % 10;
					}
					else if (distance >= 10)
					{
						num6 = 10;
						num7 = distance / 10;
						num8 = distance % 10;
					}
					else
					{
						num6 = 10;
						num7 = 10;
						num8 = distance;
					}
					if (distance < 20)
						Xie_DAc(51);
					else if (distance > 80)
						Xie_DAc(255);
					else
					{
						Xie_DAc(51 * distance * 4 / 60 - 51 / 3);
					}
				}
				else
				{
					f_cs = 0;
					num6 = num7 = num8 = 14;
				}
				break;

			case 2: // 参数设置界面
				if (f_S5 == 0)
				{
					if (GN == 3)
					{
						Vmax += 50;
						if (Vmax > 500)
							Vmax = 50;
						while (Scan_Keys())
							;
					}
					if (GN == 4)
					{
						Vmax -= 50;
						if (Vmax < 50)
							Vmax = 500;
						while (Scan_Keys())
							;
					}

					if (++tt_2ms == 400)
					{
						tt_2ms = 0;
						shanshuo = ~shanshuo;
					}
					if (shanshuo == 0)
					{
						num4 = Vmax / 100;
						num5 = Vmax / 10 % 10;
					}
					else
					{
						num4 = 10;
						num5 = 10;
					}
					num7 = Vmin / 100;
					num8 = Vmin / 10 % 10;
				}
				else
				{
					if (GN == 3)
					{
						Vmin += 50;
						if (Vmin > 500)
							Vmin = 50;
						while (Scan_Keys())
							;
					}
					if (GN == 4)
					{
						Vmin -= 50;
						if (Vmin < 50)
							Vmin = 500;
						while (Scan_Keys())
							;
					}
					if (++tt_2ms == 400)
					{
						tt_2ms = 0;
						shanshuo = ~shanshuo;
					}
					if (shanshuo == 0)
					{
						num7 = Vmin / 100;
						num8 = Vmin / 10 % 10;
					}
					else
					{
						num7 = 10;
						num8 = 10;
					}
					num4 = Vmax / 100;
					num5 = Vmax / 10 % 10;
				}
				num1 = 13;
				num2 = num3 = num6 = 10;
				break;
			}
			Led_Control();
		}
	}
}
void Service_T1() interrupt 3
{
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;

	f_2ms = 1;
	Display_Smg(num1, num2, num3, num4, num5, num6, num7, num8);
	if (f_cs == 1)
	{
		t_2ms++;
	}
	else
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = L_stat | 0x80;
		P2 &= 0x1f;
	}
	if (t_2ms == 50)
	{
		t_2ms = 0;
		f_100ms = ~f_100ms;
	}
}