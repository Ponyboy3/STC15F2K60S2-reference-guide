#include <STC15F2K60S2.H>
#include <ds1302.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int 
	
uchar num1,num2,num3,num4,num5,num6,num7,num8;
bit f_2ms;
uint t_2ms;
char hour,minute,second;




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

void smg_display(char n1,char n2,char n3,char n4,char n5,char n6,char n7,char n8)
{
	char i;
	P0=0xff;
	P2=(P2&0x1f)|0xc0;
	P0=type2[i];
	P2=(P2&0x1f);
	switch(i)
	{
		case 0:P2=(P2&0x1f)|0xe0; P0=type1[n1]; P2=(P2&0x1f);break;
		case 1:P2=(P2&0x1f)|0xe0; P0=type1[n2]; P2=(P2&0x1f);break;		
		case 2:P2=(P2&0x1f)|0xe0; P0=type1[n3]; P2=(P2&0x1f);break;		
		case 3:P2=(P2&0x1f)|0xe0; P0=type1[n4]; P2=(P2&0x1f);break;		
		case 4:P2=(P2&0x1f)|0xe0; P0=type1[n5]; P2=(P2&0x1f);break;		
		case 5:P2=(P2&0x1f)|0xe0; P0=type1[n6]; P2=(P2&0x1f);break;		
		case 6:P2=(P2&0x1f)|0xe0; P0=type1[n7]; P2=(P2&0x1f);break;		
		case 7:P2=(P2&0x1f)|0xe0; P0=type1[n8]; P2=(P2&0x1f);break;		
		
	}
	if(++i==8)
		i=0;
}
//////////////////////////1302
void xie_ds1302()
{
	Write_Ds1302_Byte( 0x8e,0x00 );
	Write_Ds1302_Byte( 0x84,0x23 );
	Write_Ds1302_Byte( 0x82,0x59 );
	Write_Ds1302_Byte( 0x80,0x55 );
}
void du_ds1302()
{
	hour=Read_Ds1302_Byte( 0x85 );   hour=(hour>>4)*10+(hour&0x0f);
	minute=Read_Ds1302_Byte( 0x83 ); minute=(minute>>4)*10+(minute&0x0f);
	second=Read_Ds1302_Byte( 0x81 ); second=(second>>4)*10+(second&0x0f);
}
char change_bcd(char dat)
{
	char dat1,dat2;
	dat1=dat/10;
	dat2=dat%10;
	dat2=dat1*16+dat2;
	return dat2;	
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

void main()
{
  initial_all();	
	time1_int1();
	xie_ds1302();
	//Write_Ds1302_Byte( 0x80,change_bcd(20) );
	while(1)
	{
	 if(f_2ms==1)
	 {
		 f_2ms=0;
	   
		 if(++t_2ms==250)
		 {
			 t_2ms=0;
			 du_ds1302();
		 }
		 
	   num1=hour/10;num2=hour%10;
		 num3=11;
		 num4=minute/10;num5=minute%10;
		 num6=11;
		 num7=second/10;num8=second%10;

	 }
	}
}



void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;		
	
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	f_2ms=1;
	

}














