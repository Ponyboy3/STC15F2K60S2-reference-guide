#include "onewire.h"

//单总线内部延时函数
void Delay_OneWire(unsigned int t)  
{
	char i;
	while(t--)
		for(i=12;i!=0;i--);
}

//单总线写操作
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//单总线读操作
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

//DS18B20初始化
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
  	Delay_OneWire(10); 
    initflag = DQ;     
  	Delay_OneWire(5);
  
  	return initflag;
}

//=============DS18b20温度读取
void Init_18b20()
{
	init_ds18b20( );
	Write_DS18B20(0xcc);
	Write_DS18B20(0x4e);		
	Write_DS18B20(0x00);	
	Write_DS18B20(0x00);	
	Write_DS18B20(0x7f);
}
unsigned int Read_Temp()
{
	unsigned int temp;
	unsigned char H,L;
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe); //读取寄存器
	L = Read_DS18B20();//低字节
	H = Read_DS18B20();//高字节
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);//启动温度转换
	//temp = (H << 4) + (L>>4);//只留整数
	//temp = ((H<<8)+L)*5/8;//一位小数
	//temp >>= 4;//去除小数部分
	temp = ((H<<8)+L)*25/4;//两位小数
	return temp;
}