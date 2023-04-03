#include <STC15F2K60S2.h>
#include <iic.h>
#include <onewire.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int 
	
uchar type1[]={ 	0xC0,
								 	0xF9,
								 	0xA4,
								 	0xB0,
								 	0x99,
								 	0x92,
								 	0x82,
								 	0xF8,
								 	0x80,
								 	0x90,0xff,0xc6,0x8c,0x88};
uchar type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar model=0;
uchar f_2ms;
uint t_2ms;

uint temp,temp1;
uint vary,vary1;
uchar temp_can=25;
bit dac_out=0;


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

void smg_display(uchar n1,uchar n2,uchar n3,uchar n4,uchar n5,uchar n6,uchar n7,uchar n8)
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
//////////////////
uchar key_scan()
{
 uchar H,L;
	P3=0xdf;P44=0;P42=0;H=(~P3)&0x0f;
	if(H==0) return 0;
	Delay10ms();
	P3=0xdf;P44=0;P42=0;H=(~P3)&0x0f;
	if(H==0) return 0;	
	P3=0xf0;P44=1;P42=1;L=(~P3)&0xf0;	
	if(P44==0)  L|=0x80;
	if(P42==0)  L|=0x40;	
	return H+L;	
}
void key_change()
{
	uchar D;
	D=key_scan();
	switch(D)
	{
		case 0x88:GN=4;break;
		case 0x84:GN=5;break;		
		case 0x48:GN=8;break;		
		case 0x44:GN=9;break;		
		case 0:GN=0;break;		
	}
}
///////////////////////////DA
void xie_dac(uchar D)
{
	IIC_Start( ); 
	IIC_SendByte(0x90+0);
	IIC_WaitAck( );
	IIC_SendByte(0x40);
	IIC_WaitAck( );	
	IIC_SendByte(D);
	IIC_WaitAck( );
	IIC_Stop( );
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
	P0=0x00;
	P2=(P2&0x1f);		
	P2=(P2&0x1f)|0xe0;
	P0=0xff;
	P2=(P2&0x1f);		
}
void main()
{
	initial_all();
	time1_int1();
	initial_18b20();
	while(1)
	{
	  if(f_2ms==1)
		{
	    f_2ms=0;
			key_change();
			
			if(GN==4)
			{
				model++;
				if(model==3)
					model=0;
				while(key_scan());
			}
			if(GN==5)
			{
        dac_out=~dac_out;
				while(key_scan());
			}			
			switch(model)
			{
				case 0:
					     if(++t_2ms>100)
							 {
								 t_2ms=0;
								 temp=rd_temperature(); 
							 }
				       num1=11;
							 num2=num3=num4=10;
				       num5=temp/1000;num6=temp%1000/100;num7=temp%100/10;num8=temp%10;
									    P0=0xff;
											P2=(P2&0x1f)|0x80;
											P0=0xfd;
											P2=(P2&0x1f);									 
				       break;
				case 1:
							if(GN==8)
							{
								if(temp_can==0)
									temp_can=0;
								else
									temp_can--;
								while(key_scan());
							}				       
							if(GN==9)
							{
								if(temp_can==99)
									temp_can=99;
								else
									temp_can++;
								while(key_scan());
							}				
				      num1=12;
							num2=num3=num4=num5=num6=10;
							num7=temp_can/10;num8=temp_can%10;
									    P0=0xff;
											P2=(P2&0x1f)|0x80;
											P0=0xfb;
											P2=(P2&0x1f);									
				      break;
				
				case 2:
								if(++t_2ms>100)
								 {
									 t_2ms=0;
									 temp=rd_temperature(); 
								 }
					      temp1=temp/100;
             		if(dac_out==0)
								 {
                   if(temp1 < temp_can)
									 {
										 xie_dac(0);
										 vary1=0;										 
									 }                     									     
									 else 
									 {
										xie_dac(255); 
										vary1=500;	
									 }
									    P0=0xff; 
											P2=(P2&0x1f)|0x80;
											P0=0xfe&0xf7;
											P2=(P2&0x1f);										 
								 }									
							  if(dac_out==1)
								{
									if(temp1 < 20)
									{
										xie_dac(51);vary1=100;
									}
									if(temp1 > 40)
									{
									  xie_dac(204);vary1=400;
									}
									if((temp1 > 20 || temp1 == 20) && (temp1 < 40 || temp1 == 40) )
									 {
										vary=(153*temp1)/20-102; 
										xie_dac(vary);
											vary1=vary;
											vary1=vary1*500/255;										 
									 }	
									     
											P2=(P2&0x1f)|0x80;
											P0=0xff&0xf7;
											P2=(P2&0x1f);									 
								}

								num1=13;
								num2=num3=num4=num5=10;
								num6=vary1/100;num7=vary1%100/10;num8=vary1%10;
//									    P0=0xff;
//											P2=(P2&0x1f)|0x80;
//											P0&=0xf7;
//											P2=(P2&0x1f);										
								break;	
	
			}//switch
			
			
			
			
			
			

			
			
			
			
			
	
	
	  }//f_2ms
	}
}
void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;	
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	f_2ms=1;
}












