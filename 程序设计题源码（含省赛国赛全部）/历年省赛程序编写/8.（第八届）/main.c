#include <STC15F2K60S2.h>
#include <ds1302.h>
#include <onewire.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int 
	
uchar code type1[]={	0xC0,
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
uchar f_2ms;
uint t_2ms;
uint tt_2ms;
uint t_5s;
uint z_2ms;
uchar hour,minute,second;
uchar hour1=0,minute1=0,second1=0;
uchar model_s,model_n;
bit shans=0;
bit f_led;
uchar a=2,b,c;
char led;

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
uchar key_scan()
{
	char temp;
	temp=(~P3)&0x0f;
	if(temp==0) return 0;
	Delay10ms();
	temp=(~P3)&0x0f;
	if(temp==0) return 0;	
	return temp;
	
	
}
void key_change()
{
	char D;
	D=key_scan();
	switch(D)
	{
		case 8:GN=1;break;
		case 4:GN=2;break;
		case 2:GN=3;break;
		case 1:GN=4;break;
		case 0:GN=0;break;
	}
}
///////////////////////////////1302
void xie_1302()
{
	Write_Ds1302_Byte( 0x8e,0x00 );
	Write_Ds1302_Byte( 0x84,0x23 );	
	Write_Ds1302_Byte( 0x82,0x59 );	
	Write_Ds1302_Byte( 0x80,0x57 );	

}
void du_1302()
{
	hour=Read_Ds1302_Byte( 0x85 );   hour=(hour>>4)*10+(hour&0x0f);
	minute=Read_Ds1302_Byte( 0x83 ); minute=(minute>>4)*10+(minute&0x0f);
	second=Read_Ds1302_Byte( 0x81 ); second=(second>>4)*10+(second&0x0f);
}
uchar bcd_change(uchar D)
{
	uchar dat1,dat2;
	dat1=D/10;
	dat2=D%10;
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

void main()
{
	P2=(P2&0x1f)|0xa0;
	P0=0x00;
	P2=(P2&0x1f);	
	P2=(P2&0x1f)|0x80;
	P0=0xff;
	P2=(P2&0x1f);		
	time1_int1();
	initial_18b20();
	xie_1302();
	while(1)
	{
	 if(f_2ms==1)
	 {
	  f_2ms=0;
		key_change(); 
			if(++t_2ms==500)
			{
				t_2ms=0;
				shans=~shans;	
			}
		
				if(a==2)
				{
						du_1302();
						num1=hour/10;num2=hour%10;
						num3=11;
						num4=minute/10;num5=minute%10;
						num6=11;				
						num7=second/10;num8=second%10;
				}
		if(b==0)
		{			
		 if(GN==4 && model_n==0)
		 {
			 model_s++;
			 if(model_s==4)
				 model_s=0;
			 while(key_scan());
		 }
		 if(GN==3 && model_s==0)
		 {
			 model_n++;
			 if(model_n==4)
				 model_n=0;
			 while(key_scan());
		 }	
	 }



	 
		 switch(model_s)//时间调节
		 {
			 case 0:
				       if(a==1)
							 {
                 Write_Ds1302_Byte( 0x84,bcd_change(hour) );
                 Write_Ds1302_Byte( 0x82,bcd_change(minute) );
                 Write_Ds1302_Byte( 0x80,bcd_change(second) );	
                 a=2;
							 }								 	         
			         break;
			 case 1:
				     a=1;
						 if(GN==2)
						 {
							 if(hour==23)
								 hour=23;
							 else
								 hour++;
	             //Write_Ds1302_Byte( 0x84,bcd_change(hour) );								 
							 while(key_scan());
						 }				      
						 if(GN==1)
						 {
							 if(hour==0)
								 hour=0;
							 else
								 hour--;
							 //Write_Ds1302_Byte( 0x84,bcd_change(hour) );
							 while(key_scan());
						 }								 
			       if(shans==0)
						 {
							num1=hour/10; num2=hour%10;  
						 }
						 else
						 {
							 num1=10; num2=10; 
						 }
			       num3=11;
						 num4=minute/10;num5=minute%10;
			       num6=11;
						 num7=second/10;num8=second%10;			 
			       break;
			 case 2:
				     a=1;
						 if(GN==2)
						 {
							 if(minute==59)
								 minute=59;
							 else
								 minute++;
	            // Write_Ds1302_Byte( 0x82,bcd_change(minute) );								 
							 while(key_scan());
						 }				      
						 if(GN==1)
						 {
							 if(minute==0)
								 minute=0;
							 else
								 minute--;
							// Write_Ds1302_Byte( 0x82,bcd_change(minute) );
							 while(key_scan());
						 }								 
			       if(shans==0)
						 {
							num4=minute/10; num5=minute%10;  
						 }
						 else
						 {
							 num4=10; num5=10; 
						 }
			       num3=11;
						 num1=hour/10;num2=hour%10;
			       num6=11;
						 num7=second/10;num8=second%10;			 
			       break;	
			 case 3:
				     a=1;
						 if(GN==2)
						 {
							 if(second==59)
								 second=59;
							 else
								 second++;
	            // Write_Ds1302_Byte( 0x80,bcd_change(second) );								 
							 while(key_scan());
						 }				      
						 if(GN==1)
						 {
							 if(second==0)
								 second=0;
							 else
								 second--;
							// Write_Ds1302_Byte( 0x80,bcd_change(second) );
							 while(key_scan());
						 }								 
			       if(shans==0)
						 {
							num7=second/10; num8=second%10;  
						 }
						 else
						 {
							 num7=10; num8=10; 
						 }
			       num3=11;
						 num1=hour/10;num2=hour%10;
			       num6=11;
						 num4=minute/10;num5=minute%10;			 
			       break;						 
		 }//switch时间
		 
		 
		 switch(model_n)//时间调节
		 {
			 case 0:
                 if(a==3)
								 {
									 a=2; 
								 }
                 
								 	         
			         break;
			 case 1:
				     a=1;
						 if(GN==2)
						 {
							 if(hour1==23)
								 hour1=23;
							 else
								 hour1++;
	             //Write_Ds1302_Byte( 0x84,bcd_change(hour) );								 
							 while(key_scan());
						 }				      
						 if(GN==1)
						 {
							 if(hour1==0)
								 hour1=0;
							 else
								 hour1--;
							 //Write_Ds1302_Byte( 0x84,bcd_change(hour) );
							 while(key_scan());
						 }								 
			       if(shans==0)
						 {
							num1=hour1/10; num2=hour1%10;  
						 }
						 else
						 {
							 num1=10; num2=10; 
						 }
			       num3=11;
						 num4=minute1/10;num5=minute1%10;
			       num6=11;
						 num7=second1/10;num8=second1%10;			 
			       break;
			 case 2:
				     a=1;
						 if(GN==2)
						 {
							 if(minute1==59)
								 minute1=59;
							 else
								 minute1++;
	            // Write_Ds1302_Byte( 0x82,bcd_change(minute) );								 
							 while(key_scan());
						 }				      
						 if(GN==1)
						 {
							 if(minute1==0)
								 minute1=0;
							 else
								 minute1--;
							// Write_Ds1302_Byte( 0x82,bcd_change(minute) );
							 while(key_scan());
						 }								 
			       if(shans==0)
						 {
							num4=minute1/10; num5=minute1%10;  
						 }
						 else
						 {
							 num4=10; num5=10; 
						 }
			       num3=11;
						 num1=hour1/10;num2=hour1%10;
			       num6=11;
						 num7=second1/10;num8=second1%10;			 
			       break;	
			 case 3:
				     a=3;
						 if(GN==2)
						 {
							 if(second1==59)
								 second1=59;
							 else
								 second1++;
	            // Write_Ds1302_Byte( 0x80,bcd_change(second) );								 
							 while(key_scan());
						 }				      
						 if(GN==1)
						 {
							 if(second1==0)
								 second1=0;
							 else
								 second1--;
							// Write_Ds1302_Byte( 0x80,bcd_change(second) );
							 while(key_scan());
						 }								 
			       if(shans==0)
						 {
							num7=second1/10; num8=second1%10;  
						 }
						 else
						 {
							 num7=10; num8=10; 
						 }
			       num3=11;
						 num1=hour1/10;num2=hour1%10;
			       num6=11;
						 num4=minute1/10;num5=minute1%10;			 
			       break;						 
		 }//switch闹钟		 
		 
	/////////////////////////////闹钟
		 if(c==0)
		 {
				if(hour==hour1 && minute==minute1 && second==second1)		 
				{
					led=1;b=1;	
				}			 
		 }

		if((GN==1) || (GN==2) || (GN==3) || (GN==4))
		{
			 led=0;c=0;
			 while(key_scan());
			 b=0;
		}	
  if(led==1)
	{     c=1;
				if(++z_2ms==100)
					{
						z_2ms=0;
						f_led=~f_led;	
						
					if(f_led==0)
					{
					P0=0xff;
					P2=(P2&0x1f)|0x80;
					P0=0xfe;
					P2=(P2&0x1f);    
					}
					if(f_led==1)
					{
					P0=0xff;
					P2=(P2&0x1f)|0x80;
					P0=0xff;
					P2=(P2&0x1f);    
					}
			}	
    if(++t_5s==2500)
		{
			b=0;
			t_5s=0;
			led=0;
		}

	}	
	if(led==0)
	{  
		  
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=0xff;
			P2=(P2&0x1f); 
	}	
	
	 }//f_2ms;
	 
	}
}
void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000)/256;
	TL1=(65536-2000)%256;		
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	f_2ms=1;
	
	
}










