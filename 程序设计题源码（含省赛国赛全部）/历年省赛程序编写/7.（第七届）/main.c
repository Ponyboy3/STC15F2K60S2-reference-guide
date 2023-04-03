#include <STC15F2K60S2.h>
#include <onewire.h>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
	
sbit output=P3^4;
uchar code type1[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf,0xc6};
uchar code type2[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar num1,num2,num3,num4,num5,num6,num7,num8;
uchar GN;
uchar model;
uchar time=0;
uchar f_100us;
uchar t_100us;
uint t_1s,tt;
bit flag=0;




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
void time0_int0()
{
	EA=1;
	ET0=1;
	TR0=1;
	TMOD=(TMOD&0xf0)|0x01;
	TH0=(65536-100)/256;
	TL0=(65536-100)%256;		
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
	time0_int0();
	initial_18b20();
	while(1)
	{
		if(f_100us==1)
		{
	   f_100us=0;
		 key_change();	
			
     if(GN==1)
		 {
			model++;
      if(model==3)
        model=0;				
			 while(key_scan());
		 }
		 if(GN==2)
		 {
			 time=time+60;
			 if(time>120)
				 time=0;
			 while(key_scan());
		 }
		 if(GN==3)
		 {
			 output=0;
			 time=0;
			 while(key_scan());
		 }		 
		 if(GN==4)
		 {
       flag=~flag;
			 while(key_scan());
		 }		

						 if(flag==0)
						 {	
											 num1=11;num2=model+1;num3=11;
											 num4=10;
											 num5=time/1000;num6=time%1000/100;			
											 num7=time%100/10;num8=time%10;
						 }
						 if(flag==1)
						 {
											 num1=11;num2=4;num3=11;
											 num4=time/10;num5=time%10;
																 
							         if(++tt==2000)
											 {
												 tt=0;
												num6=rd_temperature( )/10;num7=rd_temperature( )%10;num8=12;  
											 }
											 			
						 }
			       if(++t_1s==10000)
						 {
							 t_1s=0;							 
							 if(time==0)
								 time=0;
							 else
								 time--;
						 }

 
													 
		switch(model)
		{
			case 0:							 
			      if(time!=0)
						{
             t_100us++;
			       if(t_100us==10)
						 {
							 t_100us=0;
							 output=1; 
						 }
             if(t_100us==2)
							 output=0;
            P0=0xff;
						P2=(P2&0x1f)|0x80;
						P0=0xfe;
						P2=(P2&0x1f);						 
					  }
						if(time==0)
						{
             output=0;
            P0=0xff;
						P2=(P2&0x1f)|0x80;
						P0=0xff;
						P2=(P2&0x1f); 							
						}	
             						
             break;
			case 1:
			      if(time!=0)
						{
             t_100us++;
			       if(t_100us==10)
						 {
							 t_100us=0;
							 output=1; 
						 }
             if(t_100us==3)
							 output=0;
            P0=0xff;
						P2=(P2&0x1f)|0x80;
						P0=0xfd;
						P2=(P2&0x1f); 						 
					  }
						if(time==0)
						{
             output=0;
            P0=0xff;
						P2=(P2&0x1f)|0x80;
						P0=0xff;
						P2=(P2&0x1f); 							
						}	
 						
             break;
			case 2:
			      if(time!=0)
						{
             t_100us++;
			       if(t_100us==10)
						 {
							 t_100us=0;
							 output=1; 
						 }
             if(t_100us==7)
							 output=0;
            P0=0xff;
						P2=(P2&0x1f)|0x80;
						P0=0xfb;
						P2=(P2&0x1f); 						 
					  }
						if(time==0)
						{
             output=0;
            P0=0xff;
						P2=(P2&0x1f)|0x80;
						P0=0xff;
						P2=(P2&0x1f); 							
						}	
 						
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

}

void cl_time0_int0() interrupt 1
{
	TH0=(65536-100)/256;
	TL0=(65536-100)%256;		
	
	f_100us=1;

}

