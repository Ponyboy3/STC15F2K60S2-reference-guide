#include <STC15F2K60S2.h>
#include <onewire.h>
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int
	
uchar code type1[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar type3[]={0,0,0,0 };
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar VA;
uchar t_max=30,t_min=20;
bit model=0;
bit f_led1,f_led2,f_led3;
uchar temperature,t;
uchar f_2ms,t_2ms;
uint tt_2ms;
uchar a=0;


void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}

void smg_display(uchar n1,uchar n2,uchar n3,uchar n4,uchar n5,uchar n6,uchar n7,uchar n8)
{
	uchar i;
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
	i++;
	if(i==8)
		i=0;
}
uchar key_scan()
{
	uchar H,L;
	P3=0x0f;P44=0;P42=0;H=(~P3)&0x0f;
	if(H==0) return 0;
	Delay10ms();
	P3=0x0f;P44=0;P42=0;H=(~P3)&0x0f;
	if(H==0) return 0;	
	P3=0xf0;P44=1;P42=1;L=(~P3)&0x20;	
	if(P44==0) L|=0x80;
	if(P42==0) L|=0x40;
	return (H+L);
	
}
void key_change()
{
	uchar D;
	D=key_scan();
	switch(D)
	{
		case 0x88:VA=9;break;
		case 0x84:VA=6;break;		
		case 0x82:VA=3;break;		
		case 0x81:VA=0;break;		
		case 0x48:GN=20;break;	////////…Ë÷√20
		case 0x44:VA=7;break;
		case 0x42:VA=4;break;
		case 0x41:VA=1;break;
		case 0x28:GN=21;break;///////«Â¡„21
		case 0x24:VA=8;break;
		case 0x22:VA=5;break;
		case 0x21:VA=2;break;		
		case 0 :GN=22;VA=22;break;		
	}	
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

void main()
{
	P2=(P2&0x1f)|0x80;
	P0=0xff;
	P2=(P2&0x1f);	
	P2=(P2&0x1f)|0xa0;
	P0=0x00;
	P2=(P2&0x1f);		
	time1_int1();	
	initial_18b20();
	temperature=rd_temperature( );
	while(1)
	{
    if(f_2ms==1)
		{
		 f_2ms=0;
		 key_change();
		if(a==0)
		{			
		 if(GN==20)
		 {
      model=~model;
      while(key_scan());
		 }			 
	 }
     if(model==0)
		 {
			 temperature=rd_temperature( );
			 num1=num3=11;
			 if(temperature < t_min)
			 {
				num2=0; 
				  EA=0;
					P0=0x00;
					P2=(P2&0x1f)|0xa0;
					P0=0x00;
					P2=(P2&0x1f);
					EA=1;				 
//				if(++tt_2ms>120)
//				{
//         tt_2ms=0;
//				 f_led1=~f_led1;	
//				}				
				if(f_led1==0)
				{
				  EA=0;
					P0=0xff;
					P2=(P2&0x1f)|0x80;
					P0=0xfe;
					P2=(P2&0x1f);
					EA=1;
				}
        if(f_led1==1)
				{
				  EA=0;
					P0=0xff;
					P2=(P2&0x1f)|0x80;
					P0=0xff;
					P2=(P2&0x1f);
					EA=1;
				 }
			 				 
			}
			   
			 if(((temperature>t_min)||(temperature==t_min)) && (temperature<t_max)||((temperature==t_max)))
			 {
				 num2=1; 
				  EA=0;
					P0=0x00;
					P2=(P2&0x1f)|0xa0;
					P0=0x00;
					P2=(P2&0x1f);
					EA=1;				 
//				if(++tt_2ms>50)
//				{
//         tt_2ms=0;
//				 f_led1=~f_led1;	
//				}				
				if(f_led2==0)
				{
				  EA=0;
					P0=0xff;
					P2=(P2&0x1f)|0x80;
					P0=0xfe;
					P2=(P2&0x1f);
					EA=1;
				}
        if(f_led2==1)
				{
				  EA=0;
					P0=0xff;
					P2=(P2&0x1f)|0x80;
					P0=0xff;
					P2=(P2&0x1f);
					EA=1;
				 }				  
			 }
			  			 
			 if(temperature>t_max)
			 {
				 num2=2;
				  EA=0;
					P0=0x00;
					P2=(P2&0x1f)|0xa0;
					P0=0x10;
					P2=(P2&0x1f);
					EA=1;				 
//				if(++tt_2ms>20)
//				{
//         tt_2ms=0;
//				 f_led1=~f_led1;	
//				}				
				if(f_led3==0)
				{
				  EA=0;
					P0=0xff;
					P2=(P2&0x1f)|0x80;
					P0=0xfe;
					P2=(P2&0x1f);
					EA=1;
				}
        if(f_led3==1)
				{
				  EA=0;
					P0=0xff;
					P2=(P2&0x1f)|0x80;
					P0=0xff;
					P2=(P2&0x1f);
					EA=1;
				 }				 
			 }
			   			 
			 num4=num5=num6=10;
			 num7=temperature/10;num8=temperature%10;
		 }//model=0;
		 
     if(model==1)
		 {
			 num1=11;num6=11;num4=num5=10;
			 if(VA!=22 )
			 {
				 type3[t]=VA;
				 while(key_scan());
				 t++;
				 if(t==4)
					 t=0;	
			 }
			 if(GN==21)
			 {
				 type3[0]=0;type3[1]=0;type3[2]=0;type3[3]=0; 
				 t=0;
				 while(key_scan());
			 }
			 num2=type3[0];num3=type3[1];
			 num7=type3[2];num8=type3[3];
			 t_max=type3[0]*10+type3[1];
			 t_min=type3[2]*10+type3[3];
			 
			 if(t_max< t_min)
			 {
				 a=1;
				 EA=0;
				 P0=0xff;
				 P2=(P2&0x1f)|0x80;
				 P0=0xfd;
				 P2=(P2&0x1f);					 
				 EA=1; 
			 }
			 if(t_max> t_min)
			 {
				 a=0;
				 EA=0;
				 P0=0xff;
				 P2=(P2&0x1f)|0x80;
				 P0=0xff;
				 P2=(P2&0x1f);					 
				 EA=1; 
			 }			 
		 }//model=1;		 
		 	
	  }//f_2ms
	}
}

void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;	
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	
f_2ms=1;	
	     if(temperature < t_min)
			 {
				if(++tt_2ms>400)
				{
         tt_2ms=0;
				 f_led1=~f_led1;	
				}					 				 
			 }
			 if(((temperature>t_min)||(temperature==t_min)) && (temperature<t_max)||((temperature==t_max)))
			 {
				if(++tt_2ms>200)
				{
         tt_2ms=0;
				 f_led2=~f_led2;	
				}				 				 
			 }
        if(temperature>t_max)
				{
				if(++tt_2ms>100)
				{
         tt_2ms=0;
				 f_led3=~f_led3;	
				}											
				}
			
}

