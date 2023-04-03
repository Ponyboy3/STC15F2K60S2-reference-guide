#include <STC15F2K60S2.h>
#include <onewire.h>
#include <ds1302.h>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 

uchar code type1[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar type3[]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar hour,minute,second;
uchar f_2ms;
uint t_2ms;
uint tt_2ms,ttt_2ms;
uchar t_1s,t_led;
uchar temper,temper_1=0;
uchar time_gap;
uchar f_time;
uchar over;
bit model;
bit shanshuo;
bit f_led;
uchar f_flag;
uchar a,b,c,p;


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

///////////////////////////////////////°´¼ü
uchar key_scan()
{
	uchar temp;
	temp=(~P3)&0x0f;
	if(temp==0) return 0;
	Delay10ms();
	temp=(~P3)&0x0f;
	if(temp==0) return 0;	
	
	return temp;	
}

void key_change()
{
	uchar D;
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
void xie_ds1302()
{
 	Write_Ds1302_Byte( 0x8e,0x00 );
 	Write_Ds1302_Byte( 0x84,0x23 );	
 	Write_Ds1302_Byte( 0x82,0x59 );	
 	Write_Ds1302_Byte( 0x80,0x50 );	
}
void du_ds1302()
{
	hour=Read_Ds1302_Byte( 0x85 );   hour=((hour>>4)*10+(hour&0x0f));
	minute=Read_Ds1302_Byte( 0x83 ); minute=((minute>>4)*10+(minute&0x0f));
	second=Read_Ds1302_Byte( 0x81 ); second=((second>>4)*10+(second&0x0f));	
}

///////////////////////////////////led
void led()
{
	if(b==1)
	{
		if(++t_led==25)
		{
			t_led=0;
			f_led=~f_led;
		
		if(f_led==0)
		{
			EA=0;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=0xfe;
			P2=(P2&0x1f);	
      EA=1;			
		}
		else if(f_led==1)
		{
			EA=0;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=0xff;
			P2=(P2&0x1f);
			EA=1;
		}
		}
		}
		if(b==0)
		{
			EA=0;
			P0=0xff;
			P2=(P2&0x1f)|0x80;
			P0=0xff;
			P2=(P2&0x1f);	
      EA=1;				
			
		}	
}

void time1_int1()
{
	EA=1;
	ET1=1;
	TR1=1;
	TMOD=(TMOD&0x0f)|0x10;
	TH1=(65536-2000/(120000/110592))/256;
	TL1=(65536-2000/(120000/110592))%256;	
}


void main()
{
	
	P2=(P2&0x1f)|0xa0;
	P0=0x0;
	P2=(P2&0x1f);	
	time1_int1();
	initial_ds18b20();
	c=rd_temperature( );
	while(1)
	{
    if(f_2ms==1)
		{
		  f_2ms=0;
			
			key_change();
			
			if(GN==1 && model==0)
			{
				f_time++;
				if(f_time==4)
					f_time=0;
				while(key_scan());				
			}
			if(GN==2)
			{
				model=1;
				while(key_scan());	
			}
			if(GN==3 && f_flag==1)
			{
				over=1;
				while(key_scan());	
			}			
			
		 
		 if(model==0)
		 {			 
			switch(f_time)
			{
				case 0:
					     time_gap=1;
				       num1=num2=num3=num4=num5=10;
				       num6=11;
				       num7=time_gap/10;num8=time_gap%10;				
				       break;
				case 1:
					     time_gap=5;
				       num1=num2=num3=num4=num5=10;
				       num6=11;
				       num7=time_gap/10;num8=time_gap%10;				
				       break;					
				case 2:
					     time_gap=30;
				       num1=num2=num3=num4=num5=10;
				       num6=11;
				       num7=time_gap/10;num8=time_gap%10;				
				       break;													
				case 3:
					     time_gap=60;
				       num1=num2=num3=num4=num5=10;
				       num6=11;
				       num7=time_gap/10;num8=time_gap%10;				
				       break;										
			   }
			 }//model==0
		 if(p==0)
		 {
			if(model==1)
			{
				if(a==0)
				{
					xie_ds1302();
					a=1;
				}
				du_ds1302();
				num1=hour/10;num2=hour%10;
				if(++t_2ms==500)
				{
					t_2ms=0;
					shanshuo=~shanshuo;
				}
				if(shanshuo==0)
					num3=num6=11;
				else if(shanshuo==1)
					num3=num6=10;
				num4=minute/10;num5=minute%10;				
				num7=second/10;num8=second%10;	

        switch(time_gap)
				{
					case 1:
							 if(f_flag==0)
							 {						
						     if(tt_2ms++==500)
								 {
									 tt_2ms=0;

									  type3[temper]=rd_temperature( );	 									 
									  temper++;
									 if(temper==10)
									 {
										temper=0; 
										f_flag=1; 
                    b=1;										 
									 }	
									}
							 }
					       break;
					case 5:
							 if(f_flag==0)
							 {						
						     if(++tt_2ms==500)
								 {
									 tt_2ms=0;
									 t_1s++;
									 if(t_1s==5)
									 {
										 t_1s=0;
											type3[temper]=rd_temperature( );	 
										 temper++;
										 if(temper==10)
										 {
											temper=0; 
											f_flag=1; 
                      b=1;											 
										 }
                   }										 
								 }
							 } 
					       break;								 
					case 30:
							 if(f_flag==0)
							 {						
						     if(++tt_2ms==500)
								 {
									 tt_2ms=0;
									 t_1s++;
									 if(t_1s==30)
									 {
										 t_1s=0;

											type3[temper]=rd_temperature( );	 
										
										 temper++;
										 if(temper==10)
										 {
											temper=0; 
											f_flag=1; 
                      b=1;											 
										 }
                   }										 
								 }
							 } 
					       break;	
					case 60:
							 if(f_flag==0)
							 {						
						     if(++tt_2ms==500)
								 {
									 tt_2ms=0;
									 t_1s++;
									 if(t_1s==60)
									 {
										 t_1s=0;
										 type3[temper]=rd_temperature( );	 										 
										 temper++;
										 if(temper==10)
										 {
											temper=0; 
											f_flag=1; 
                      b=1;											 
										 }
                   }										 
								 }
								} 
					       break;					
				}//switch

			}//model==1
		}	
			led();	
			if(f_flag==1)
			{
				if(over==1)
				{
				  b=0;p=1;
 				if(++ttt_2ms > 500)
				{
					 ttt_2ms=0;
					 temper_1++;
					 if(temper_1==10)
						 temper_1=0;	 
				} 					
					num1=11;
					num2=0;
					num3=temper_1;
					num4=num5=10;
					num6=11;
					num7=type3[temper_1]/10;
					num8=type3[temper_1]%10;	
        				
					if(GN==4)
					{
						model=0;
						f_flag=0;
						over=0;
						temper_1=0;p=0;
					}
								
			  }//over
			}							
		
		}//f_2ms
	}//while
}


void cl_time1_int1() interrupt 3
{
	TH1=(65536-2000/(120000/110592))/256;
	TL1=(65536-2000/(120000/110592))%256;		
	
	smg_display(num1,num2,num3,num4,num5,num6,num7,num8);
	
	f_2ms=1;
}












