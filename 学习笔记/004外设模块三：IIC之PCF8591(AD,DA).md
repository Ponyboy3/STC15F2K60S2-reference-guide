#### 外设模块三：IIC之PCF8591(A/D,D/A)

1.导入头文件`iic.h`

```c
#include "iic.h"
```

2.A/D读取光敏电阻(RD1)或电压采集(RB2)数值

```c
unsigned char Read_ADC(unsigned char n)
{
	unsigned char temp;
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	if(n == 1)
	{
		IIC_SendByte(0x01);//读取光敏电阻
	}
	else
	{
		IIC_SendByte(0x01);//读取滑动变阻器
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
```

3.D/A写电压

```c
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
```