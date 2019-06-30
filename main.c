#include<reg51.h>
#define out_port P1
#define uchar unsigned char
#define uint unsigned int
sbit test_port=P3^2;
uchar dat[6]={0};
uchar tab[4]={0x0f,0x03,0x0c,0x00};
uchar flag=0;
uint Htime,Ltime;
uchar zq=100;
uchar zk=78;
uchar index=0;
uint temp,temp0;
void delay(uint z)
{
	while(z--);
}
void main()
{
	TMOD=0x01;
	ET0=1;
	TR0=1;
	Htime=65536-(zq-zk)*100;
	Ltime=65536-zk*100;
	TH0=Htime/256;
	TL0=Htime%256;
	EX0=1;
	IT0=1;
	EA=1;
	out_port=tab[index];
	test_port=1;
	temp=Htime;
	temp0=Ltime;
	while(1);
}
void chang() interrupt 1
{
	if(flag==0)
	{
		out_port=0x00;
		TH0=Ltime/256;
		TL0=Ltime%256;
		flag=1;
	}
	else
	{
		out_port=tab[index];
		TH0=Htime/256;
		TL0=Htime%256;
		flag=0;
	}
}
void interr() interrupt 0
{
	uchar i,j,time;
	uint err;
	P1=0x00;
	delay(700);
	time=0;
	err=1000;
	while(!test_port&&err)
	{
		delay(1);
		err--;
	}
	if(test_port)
	{
		err=500;
		while(test_port&&err)
		{
			delay(1);
			err--;
		}
		if(!test_port)
		{
			for(i=0;i<4;i++)
			{
				for(j=0;j<8;j++)
				{
					err=60;
					while(!test_port&&err)
					{
						delay(1);
						err--;
					}
					err=50;
					while(err&&test_port)
					{
						time++;
						delay(10);
						err--;
						if(time>=30)
						return;
					}
					dat[i]>>=1;
					if(time>8)
					{
						dat[i]|=0x80;
					}
					time=0;
				}
			}
			if(dat[2]!=~dat[3])
			return;
		}
	}
	if(dat[2]==0x18)
	{
		index=0;
		Htime-=500;
		Ltime+=500;
	}
	else if(dat[2]==0x08)
	{
		index=2;
	}
	else if(dat[2]==0x5a)
	{
		index=1;
	}
	else if(dat[2]==0x1c)
	{
		index=4;
	}
	else if(dat[2]==0x52)
	{
		index=0;
		Htime+=500;
		Ltime-=500;
	}	
	for(i=0;i<6;i++)
	dat[i]=0;
}							
