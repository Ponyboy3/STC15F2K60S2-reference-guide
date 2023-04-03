#### 外设模块四：IIC之	EEPROM--24C02存储器

1.导入头文件

```c
#include "iic.h"
```

2.写入24C02

```c
void Xie_24C02(unsigned char addr,unsigned char dat)
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
```

3.从24C02读取数据

```c
unsigned char Du_24C02(unsigned char addr)
{
	unsigned char d;
    IIC_Start();
    IIC_SendByte(0xa0);
    IIC_WaitAck();
    IIC_SendByte(addr);
    IIC_WaitAck();
    IIC_Stop();
    
    IIC_Start();
    IIC_SendByte(0xa1);
    IIC_WaitAck();
    d = IIC_RecByte(); 
    IIC_SendAck(1);
    IIC_Stop();
    return d;
}
```

