#include <STC15F2K60S2.H>
#include "iic.h"
#include "ds1302.h"  	
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int 
	
uchar code type1[]={ 	0xC0,
										 	0xF9,
										 	0xA4,
										 	0xB0,
										 	0x99,
										 	0x92,
										 	0x82,
										 	0xF8,
										 	0x80,
										 	0x90,0xff,0xbf};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
bit f_2ms;
uchar t_2ms;
uchar x_2ms;
uint tt_2ms;
bit flag;

uchar hour,minute,second;
uint temp_f;

uchar model=0;
bit f_time;
uchar t_model;
bit shanshuo;

bit v_model,vf_model;
uint v_temp;
bit v_hl=1;
uint V_h=2000,V_l=1000;
char a;


uchar f_model;
bit f_FT;
uint t_temp;


bit chaxun;
uchar type;
bit f_chaxun;
char b=0,c=0,d=0,e=0,n,m;

void Delay10ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 117;
	j = 184;
	do
	{
		while (--j);
	} while (--i);
}

void smg_display(char n1,char n2,char n3,char n4,char n5,char n6,char n7,char n8)
{
	char i;
	P0=0xff;
	P2=(P2&0x1f)|0xc0;
	P0=type2[i];
	P2=(P2&0x1f);
	switch(i)
	{
		case 0:P2=(P2&0x1f)|0xe0;P0=type1[n1];P2=(P2&0x1f);break;
		case 1:P2=(P2&0x1f)|0xe0;P0=type1[n2];P2=(P2&0x1f);break;
		case 2:P2=(P2&0x1f)|0xe0;P0=type1[n3];P2=(P2&0x1f);break;
		case 3:P2=(P2&0x1f)|0xe0;P0=type1[n4];P2=(P2&0x1f);break;
		case 4:P2=(P2&0x1f)|0xe0;P0=type1[n5];P2=(P2&0x1f);break;
		case 5:P2=(P2&0x1f)|0xe0;P0=type1[n6];P2=(P2&0x1f);break;
		case 6:P2=(P2&0x1f)|0xe0;P0=type1[n7];P2=(P2&0x1f);break;
		case 7:P2=(P2&0x1f)|0xe0;P0=type1[n8];P2=(P2&0x1f);break;
	}
	if(++i==8)
		i=0;
}
//=======================================key
uchar  key_scan()
{
	uchar H,L;
	P3=0xdf;P44=0;P42=0;H=(~P3)&0x0f;
	if(H==0) return 0;
	Delay10ms();
	P3=0xff;P44=0;P42=0;H=(~P3)&0x0f;
	if(H==0) return 0;	
	P3=0xf0;P44=1;P42=1;L=(~P3)&0x20;
	if(P44==0) L|=0x80;
	if(P42==0) L|=0x40;
	
	return L+H;	
}
void key_change()
{
	uchar D;
	D=key_scan();
	switch(D)
	{
	  case 0x88:GN=4;break;
	  case 0x84:GN=5;break;
	  case 0x82:GN=6;break;
	  case 0x81:GN=7;break;
	  case 0x48:GN=8;break;
		case 0x44:GN=9;break;
		case 0x42:GN=10;break;
		case 0x41:GN=11;break;
		case 0 :GN=0;break;
	}
}

//====================================pcf8591
void initial_8591()
{
	IIC_Start( );
	IIC_SendByte(0x90+0); 
	IIC_WaitAck( ); 
	IIC_SendByte(0x3);
	IIC_WaitAck( ); 
	IIC_Stop( );
}
uint du_adc()
{
	uint D;
	IIC_Start( );
	IIC_SendByte(0x90+1); 
	IIC_WaitAck( ); 	
	D=IIC_RecByte( );
	IIC_SendAck(1); 
	IIC_Stop( );
	D=(D*5000.0)/255;
	return D;	
}
//=====================================24c02
void xie_24c02(uchar addr,uchar dat)
{
	IIC_Start( );
	IIC_SendByte(0xa0+0); 
	IIC_WaitAck( ); 
	IIC_SendByte(addr);	
	IIC_WaitAck( );
	IIC_SendByte(dat);
	IIC_WaitAck( );
	IIC_Stop( );
}
uchar du_24c02(uchar addr)
{
	uchar D;
	IIC_Start( );
	IIC_SendByte(0xa0+0); 
	IIC_WaitAck( ); 
	IIC_SendByte(addr);	
	IIC_WaitAck( );	
	
	IIC_Start( );
	IIC_SendByte(0xa0+1); 
	IIC_WaitAck( ); 	
	D=IIC_RecByte( );
	IIC_SendAck(1); 
	IIC_Stop( );
	
	return D;
}
//==================================1302
void xie_1302()
{
	Write_Ds1302_Byte( 0x8e,0x00 );
	Write_Ds1302_Byte( 0x84,0x23 );
	Write_Ds1302_Byte( 0x82,0x59 );
	Write_Ds1302_Byte( 0x80,0x55 );

}
void du_1302()
{
	hour=Read_Ds1302_Byte( 0x85 );   hour=(hour>>4)*10+(hour&0x0f);
	minute=Read_Ds1302_Byte( 0x83 ); minute=(minute>>4)*10+(minute&0x0f);
	second=Read_Ds1302_Byte( 0x81 ); second=(second>>4)*10+(second&0x0f);
}
uchar change_bcd(uchar D)
{
	uchar dat1,dat2;
  dat1=D/10;
  dat2=D%10;
  dat2=dat1*16+dat2;	
	
	return dat2;
}

void time0()
{
	TMOD=(TMOD&0xf0)|0x05;
	TH0=TL0=0;
}
void time1_int1()
{
	EA=1;
	ET1=1;
	TR1=1;
	TMOD=(TMOD&0x0f)|0x10;
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;	
}
void initial_all()
{
	P2=(P2&0x1f)|0xa0;
	P0=0x00;
	P2=(P2&0x1f);	
	P2=(P2&0x1f)|0x80;
	P0=0xff;
	P2=(P2&0x1f);	
	P2=(P2&0x1f)|0xc0;
	P0=0xff;
	P2=(P2&0x1f);	
	P2=(P2&0x1f)|0xe0;
	P0=0xff;
	P2=(P2&0x1f);	
}
//////////////////////////////////////时钟模块
void time_mk()
{
		if(GN==7)
		{
     model=1;        v_model=0;f_model=0;chaxun=0;
		 t_model=0;	//为下次进入调节时间做准备
		 f_time=1; //时分秒走起来
     while(key_scan());
		}			
		 
		if(model==1)//时钟显示
		{
			 if(GN==4)
			 {
				 f_time=0;
				 t_model++;
				 if(t_model==4)
					 t_model=1; 
					while(key_scan());
			 }
		
     if(f_time)
		 {			 
			 du_1302();
			 num1=hour/10;num2=hour%10;num3=11;
			 num4=minute/10;num5=minute%10;num6=11;
			 num7=second/10;num8=second%10;
		 }
		 else
		 {
		 
			 switch(t_model)
			 {
				 case 1:
											 if(GN==11)
											 {
                         if(hour==23) hour=23; else hour++;
												  while(key_scan());
											 }
											 if(GN==10)
											 {
                          if(hour==0) hour=0; else hour--;	 
												   while(key_scan());
											 }					 
					       Write_Ds1302_Byte( 0x84,change_bcd(hour) );//写入时间
								 if(shanshuo)
								 {
                   num1=hour/10;num2=hour%10;
								 }
                 else	
								 {
                    num1=10;num2=10;
								 }									 
								 num3=11;
								 num4=minute/10;num5=minute%10;num6=11;
								 num7=second/10;num8=second%10;					 
				        break;
				 case 2:
											 if(GN==11)
											 {
                         if(minute==59) minute=59; else minute++;  while(key_scan());
											 }
											 if(GN==10)
											 {
                          if(minute==0) minute=0; else minute--;	  while(key_scan());
											 }					 
					       Write_Ds1302_Byte( 0x82,change_bcd(minute) );//写入时间
								 if(shanshuo)
								 {
                   num4=minute/10;num5=minute%10;
								 }
                 else	
								 {
                    num4=10;num5=10;
								 }									 
								 num3=11;
								 num1=hour/10;num2=hour%10;num6=11;
								 num7=second/10;num8=second%10;					 
				        break;				 
				 case 3:
											 if(GN==11)
											 {
                         if(second==59) second=59; else second++;   while(key_scan());
											 }
											 if(GN==10)
											 {
                          if(second==0) second=0; else second--;	   while(key_scan()); 
											 }					 
					       Write_Ds1302_Byte( 0x80,change_bcd(second) );//写入时间
								 if(shanshuo)
								 {
                   num7=second/10;num8=second%10;
								 }
                 else	
								 {
                    num7=10;num8=10;
								 }									 
								 num3=11;
								 num1=hour/10;num2=hour%10;num6=11;
								 num4=minute/10;num5=minute%10;					 
				        break;					 
				 
				 
			 }//switch
		 }

		}		//shizhong 	
}
void volt_mk() //////////电压模块
{
		if(GN==6)
		{
      v_model=1;          model=0;f_model=0;chaxun=0;
			vf_model=0;//实时测量电压
		  a=0;       //保存一次电压阈值
			while(key_scan());
		}			
								 if(a==0) //保存电压阈值
								 {
									 xie_24c02(4,V_h/100);
									 Delay10ms();
									 xie_24c02(5,V_l/100);
									 Delay10ms();
									 a=1;
								 }

		 if(v_model==1)
		 {
			 if(GN==4)
			 {
          vf_model=1;
				  v_hl=~v_hl;
					while(key_scan());				 
			 }
			if(vf_model==0)
			{				
			 if(++x_2ms==40)
			 {
				 x_2ms=0;
				 v_temp=du_adc();
			 }
			 num1=11;num2=1;num3=11;
			 num4=10;
			 num5=v_temp/1000;num6=v_temp%1000/100;num7=v_temp%100/10;num8=v_temp%10;
		 } 
			else
			{
        if(v_hl==0)
				{
											 if(GN==11)
											 {
                         if(V_h>4999) V_h=5000; else V_h+=500;   while(key_scan());
											 }
											 if(GN==10)
											 {
                          if(V_h==0) V_h=0; else V_h-=500;	  while(key_scan());
											 }	
          if(shanshuo)
					{						
					num1=V_h/1000;num2=V_h%1000/100;num3=V_h%100/10;num4=V_h%10;
					}
					else
					{
						num1=num2=num3=num4=10;						
					}
					num5=V_l/1000;num6=V_l%1000/100;num7=V_l%100/10;num8=V_l%10;
				}
        else
				{
											 if(GN==11)
											 {
                         if(V_l>5000) V_l=5000; else V_l+=500;   while(key_scan());
											 }
											 if(GN==10)
											 {
                          if(V_l==0) V_l=0; else V_l-=500;	   while(key_scan());
											 }	
          if(shanshuo)
					{						
					num5=V_l/1000;num6=V_l%1000/100;num7=V_l%100/10;num8=V_l%10;
					}
					else
					{
						num5=num6=num7=num8=10;						
					}
					num1=V_h/1000;num2=V_h%1000/100;num3=V_h%100/10;num4=V_h%10;						
				}
			}				
		 }	

}
//==========================================频率测量
void f_mk()
{
	if(GN==5)
	{
		f_model=1;          model=0;v_model=0;chaxun=0;
		while(key_scan());
	}
	
	if(f_model==1)
	{
		if(GN==4)
		{
			f_FT=~f_FT;
			while(key_scan());
		}
		if(f_FT==0) //显示频率
		{
			num1=11;num2=2;num3=11;
			num4=temp_f/10000;num5=temp_f%10000/1000;num6=temp_f%1000/100;num7=temp_f%100/10;num8=temp_f%10;
		}
		else  //显示周期
		{
			num1=11;num2=2;num3=11;
			t_temp=1000000/temp_f;
			num4=t_temp/10000;num5=t_temp%10000/1000;num6=t_temp%1000/100;num7=t_temp%100/10;num8=t_temp%10;
		}	
	}
}

///================================查询模式
void chaxun_mk()
{
		
	if(GN==9)
	{
		chaxun=1;           f_model=0;model=0;v_model=0;
		m=n=0;
		while(key_scan());
	}
	if(chaxun==1)
	{
		if(GN==4)
		{
			f_chaxun=~f_chaxun;	n=m=0;	//为读写一次做准备，防止闪烁		
			while(key_scan());
		}
		if(f_chaxun==0) //查询事件标号
		{
			if(n==0)
			{
			num1=num2=num3=num4=num5=num6=10;
			num7=du_24c02(6)/10;
			num8=du_24c02(6)%10;
       n=1;
			}				
		}
	 if(f_chaxun==1) //查询事件时间
		{
			if(m==0)
			{
			num1=du_24c02(7)/10;num2=du_24c02(7)%10;
			num3=11;
			num4=du_24c02(8)/10;num5=du_24c02(8)%10;
			num6=11;			
			num7=du_24c02(9)/10;num8=du_24c02(9)%10;
       m=1;
			}				
		}	
	}
	
	
	if(b==0)
	{
		if(v_temp > V_h)	 //高于阈值触发
		{
//			d=0;
			b=1;//仅触发一次变量
			c=1;	//保存数据变量
		}
	}
	if(c==1)
	{
		type=1;
		xie_24c02(6,type);
		Delay10ms();
		xie_24c02(7,hour);
		Delay10ms();
		xie_24c02(8,minute);
		Delay10ms();		
		xie_24c02(9,second);
		Delay10ms();
    c=0; n=m=0;		
	}
///==============================
	if(d==0)      //低于阈值触发
	{
		if(v_temp < V_l)
		{
//			b=0;
			d=1;e=1;
		}		
	}

  if(e==1)
	{
	 type=0;	
		xie_24c02(6,type);
		Delay10ms();
		xie_24c02(7,hour);
		Delay10ms();
		xie_24c02(8,minute);
		Delay10ms();		
		xie_24c02(9,second);
		Delay10ms();	
    e=0;	n=m=0;	
	}	
	
	if(v_temp<V_h && v_temp > V_l)  //非触发条件变量清零
	{
		b=d=0;n=m=0;	
	}

}

void main()
{
	initial_all();
	time0();
	time1_int1();
	initial_8591();
	
	xie_1302();
	V_h=du_24c02(4)*100;
	V_l=du_24c02(5)*100;
  num1=num2=num3=num4=num5=num6=num7=num8=10;
	while(1)
	{
	 if(f_2ms)
	 {
	  f_2ms=0;
	 if(++tt_2ms > 500)
	  {
		 tt_2ms=0;
		 shanshuo=~shanshuo;
	  }
	  key_change();//扫描按键
		 
		time_mk(); //时间显示
	  volt_mk(); //电压显示
    f_mk();    //频率显示
		 
		chaxun_mk(); //查询显示

	 }//f_2ms
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;		
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	
	f_2ms=1;
	
	if(++t_2ms==250)
	{
		t_2ms=0;
		flag=~flag;
		if(flag)
		{
			TH0=TL0=0;
			TR0=1;		
		}
		else
		{
			TR0=0;
			temp_f=((TH0<<8)+TL0)*2;
		}
	}
	
}

