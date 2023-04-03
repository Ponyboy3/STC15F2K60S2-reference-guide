#include <STC15F2K60S2.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int 
	
uchar code type1[]={
	                   	0xC0,
										 	0xF9,
										 	0xA4,
										 	0xB0,
										 	0x99,
										  0x92,
										 	0x82,
										  0xF8,
										 	0x80,
										 	0x90,0xff};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar f_2ms;
uint t_2ms;
bit flag=0;
uchar temp1,temp2;
bit FZ;


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

void time1_int1()
{
	EA=1;
	ET1=1;
	TR1=1;
	TMOD=(TMOD&0x0f)|0x10;
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;	
}
void led_display()
{
	if(++t_2ms==500)
	{
		t_2ms=0;
		flag=~flag;
		if(flag==1)
		{
			temp1++;
			FZ=~FZ;
			temp2++;
			if(temp2==10)
				temp2=0;
		}
		
		if(temp1==100)
			temp1=0;
	}
	if(flag==1)
	{
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0x7f;
		P2=(P2&0x1f);		
	}
	if(flag==0)
	{
		
		P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0=0xff;
		P2=(P2&0x1f);		
	}	
	if(FZ==0)
	{
		//P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0&=0xfe;
		P2=(P2&0x1f);				
	}
	if(FZ==1)
	{
		//P0=0xff;
		P2=(P2&0x1f)|0x80;
		P0&=0xfd;
		P2=(P2&0x1f);				
	}	
	
	
}

void main()
{
	P2=(P2&0x1f)|0xa0;
	P0=0x00;
	P2=(P2&0x1f);	
	P2=(P2&0x1f)|0x80;
	P0=0xff;
	P2=(P2&0x1f);		
	time1_int1();
	while(1)
	{
	  if(f_2ms==1)
		{
	   f_2ms=0;
	   led_display();

			num1=temp2;
			num2=num3=num4=num5=num6=10;
			num7=temp1/10;num8=temp1%10;
			

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