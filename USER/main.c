#include "LongZhou.h"
#include "delay.h"
#include "sys.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"	
#include "usmart.h"	 
#include "rtc.h" 
#include  "flash.h"
# include "stdlib.h"
#include "beep.h"
//#include "usart.h"


#define SIZE sizeof(TEXT_Buffer)
#define SYS_DATA_ADDR           0 

#define NOq          1  
#define SNq          2 
#define stunumberq   3 
#define nameq        4 
#define timeq        5 
#define flagq        6 
#define w_dateq      7 
#define hourq        8 
#define minq         9 
#define secq        10 
#define min1q        11 
#define hour1q        12 

void UART_PutChar(USART_TypeDef* USARTx, uint8_t Data);  
void UART_PutStr (USART_TypeDef* USARTx, uint8_t *str); 
void ff(void);
void uart1_init(u32 bound);
void uart3_init(u32 bound);
void Attendance(void);
void Register(void);
void minf(void);

/*全局变量*/
unsigned char to[30];
unsigned char po[3];
unsigned char CT[2];//卡类型
unsigned char SN[4]; //卡号
unsigned char OSN[4]={0,0,0,0}; //旧卡号
//unsigned char P[16]; 
unsigned char RFID[16];			//存放RFID 
unsigned char lxl_bit=0;
unsigned char card1_bit=0;
unsigned char card2_bit=0;
unsigned char card3_bit=0;
unsigned char card4_bit=0;
unsigned char total=0;
//int flagg=0; 
u8 TEXT_Buffer[25];
char name[150][6];
u16 len;	
u16 Number=0;
u16 TempNumber;
u8 add;
u8 t=0,t1,divide;
u16 i;
u8 Atten[20]={"Attendance"};
u8 Regi[20]={"Register"};
u8 ok[20]={"yu"};
u8 ojbk[20]={"ojbk"};
u32 timesec;

unsigned char status;
//unsigned char lxl[4]={6,109,250,186};
//unsigned char card_1[4]={66,193,88,0};
//unsigned char card_2[4]={66,191,104,0};
//char card_3[100];
//unsigned char card_4[4]={126,252,248,12};
u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};
u8 datatemp[SIZE];

u8 NOO[1];         //Number的缓存
u8 NO[1];
u8 time[1];
u8 flag[1];
u8 w_date[1];
u8 hour[1];
u8 min[1];
u8 sec[1];
u8 hour1[1];
u8 min1[1];
u8 temphour;
u8 tempmin;
u8 tempsec;
u32 temptimesec;

unsigned char pValue[4]={0x02,0x00,0x00,0x00};
unsigned char RFID1[16]={0x01,0x00,0x00,0x00,0x00,0x00,0xff,0x07,0x80,0x29,0xff,0xff,0xff,0xff,0xff,0xff};
//unsigned char RFID2[16]={0x01,0x02,0xff,0x55,0xff,0x65,0xff,0x07,0x80,0x29,0xff,0xff,0xff,0xff,0xff,0xff};
struct user
{
	u8 NO;
 u8 SN[4];
	u8 stunumber[24];
	u8 name[10];
	u8 time;
	u8 min1;
	u8 hour1;
	u8 flag;
	u8 w_date;
	u8 hour;
	u8 min;
	u8 sec;
}pas[280];


void beep(u8 ty)
{
	if(!ty)
	{
    BEEP=0;  
		delay_ms(100);//延时300ms
	  BEEP=1;	
	  delay_ms(100);//延时300ms
	}
	else
	{
    BEEP=0;  
		delay_ms(100);//延时300ms
	  BEEP=1;	
	  delay_ms(100);//延时300ms  
		BEEP=0;  
		delay_ms(100);//延时300ms
	  BEEP=1;	
	}
//    delay_ms(40);//延时300ms
//	  BEEP=1; 
//    delay_ms(50);//延时300ms
//	  BEEP=0;		
//	  delay_ms(2000);//延时300ms
}

void Flashwrite(u16 num,u8 mode)
{
//		SPI_Flash_Write((u8*)TEXT_Buffer,i,SIZE);						//想flash写 已经准备好的数据
	if(mode==0)
	{
	 NO[0]=pas[num].NO;
	 SPI_Flash_Write((u8*)TEXT_Buffer,(num)*100,1);						//想flash写 已经准备好的数据
	 SPI_Flash_Write((u8*)NO,(num)*100,sizeof(NO));						//想flash写 已经准备好的数据

	 SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+11),4);						//想flash写 已经准备好的数据
	 SPI_Flash_Write((u8*)pas[num].SN,(num*100+11),sizeof(pas[num].SN));						//想flash写 已经准备好的数据

	 delay_us(1);
	 SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+21),24);						//想flash写 已经准备好的数据
	 SPI_Flash_Write((u8*)pas[num].stunumber,(num*100+21),sizeof(pas[num].stunumber));						//想flash写 已经准备好的数据

	 SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+31),10);						//想flash写 已经准备好的数据
	 SPI_Flash_Write((u8*)pas[num].name,(num*100+31),sizeof(pas[num].name));						//想flash写 已经准备好的数据
	 
	 time[0]=pas[num].time;
	 SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+41),1);						//想flash写 已经准备好的数据
	 SPI_Flash_Write((u8*)time,(num*100+41),sizeof(time));						//想flash写 已经准备好的数据
	 
	 flag[0]=pas[num].flag;
	 SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+42),1);						//想flash写 已经准备好的数据
	 SPI_Flash_Write((u8*)flag,(num*100+42),sizeof(flag));						//想flash写 已经准备好的数据
	 
	 w_date[0]=pas[num].w_date;
	 SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+43),1);						//想flash写 已经准备好的数据
	 SPI_Flash_Write((u8*)w_date,(num*100+43),sizeof(w_date));						//想flash写 已经准备好的数据
	 
	 hour[0]=pas[num].hour;
	 SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+44),1);						//想flash写 已经准备好的数据
	 SPI_Flash_Write((u8*)hour,(num*100+44),sizeof(hour));						//想flash写 已经准备好的数据
	 
	 min[0]=pas[num].min;
	 SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+45),1);						//想flash写 已经准备好的数据
	 SPI_Flash_Write((u8*)min,(num*100+45),sizeof(min));						//想flash写 已经准备好的数据
	 
	 sec[0]=pas[num].sec;
	 SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+46),1);						//想flash写 已经准备好的数据
	 SPI_Flash_Write((u8*)sec,(num*100+46),sizeof(sec));						//想flash写 已经准备好的数据
	 	 
		 min1[0]=pas[num].min1;
	 SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+47),1);						//想flash写 已经准备好的数据
	 SPI_Flash_Write((u8*)min1,(num*100+47),sizeof(min1));						//想flash写 已经准备好的数据
	
  	hour1[0]=pas[num].hour1;
	 SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+48),1);						//想flash写 已经准备好的数据
	 SPI_Flash_Write((u8*)hour1,(num*100+48),sizeof(hour1));						//想flash写 已经准备好的数据
	 

//	 NOO[0]=Number;
//	 SPI_Flash_Write((u8*)TEXT_Buffer,(num*10+47),SIZE);						//想flash写 已经准备好的数据
//	 SPI_Flash_Write((u8*)NOO,(num*10+47),sizeof(NOO));						//想flash写 已经准备好的数据
 }
	else
	{
		switch (mode)
		{	case 1:
			{
			 NO[0]=pas[num].NO;
			 SPI_Flash_Write((u8*)TEXT_Buffer,(num)*100,1);						//想flash写 已经准备好的数据
			 SPI_Flash_Write((u8*)NO,(num)*100,sizeof(NO));						//想flash写 已经准备好的数据
			}
			break;
				case 2:
			{
			 SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+11),4);						//想flash写 已经准备好的数据
			 SPI_Flash_Write((u8*)pas[num].SN,(num*100+11),sizeof(pas[num].SN));						//想flash写 已经准备好的数据
			}
			break;
				case 3:
			{
			 SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+21),24);						//想flash写 已经准备好的数据
			 SPI_Flash_Write((u8*)pas[num].stunumber,(num*100+21),sizeof(pas[num].stunumber));						//想flash写 已经准备好的数据
			}
			break;
				case 4:
			{
			SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+31),10);						//想flash写 已经准备好的数据
				delay_us(1);
			SPI_Flash_Write((u8*)pas[num].name,(num*100+31),sizeof(pas[num].name));						//想flash写 已经准备好的数据
			}
			break;
				case 5:
			{
			time[0]=pas[num].time;
			SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+41),1);						//想flash写 已经准备好的数据
			SPI_Flash_Write((u8*)time,(num*100+41),sizeof(time));						//想flash写 已经准备好的数据
			}
			break;
				case 6:
			{
			 flag[0]=pas[num].flag;
			 SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+42),1);						//想flash写 已经准备好的数据
			 SPI_Flash_Write((u8*)flag,(num*100+42),sizeof(flag));						//想flash写 已经准备好的数据
			}
			break;
				case 7:
			{
			 w_date[0]=pas[num].w_date;
			 SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+43),1);						//想flash写 已经准备好的数据
			 SPI_Flash_Write((u8*)w_date,(num*100+43),sizeof(w_date));						//想flash写 已经准备好的数据
			}
			break;
				case 8:
			{
			hour[0]=pas[num].hour;
			SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+44),1);						//想flash写 已经准备好的数据
			SPI_Flash_Write((u8*)hour,(num*100+44),sizeof(hour));						//想flash写 已经准备好的数据
			}
			break;
				case 9:
			{
			min[0]=pas[num].min;
			SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+45),1);						//想flash写 已经准备好的数据
			SPI_Flash_Write((u8*)min,(num*100+45),sizeof(min));						//想flash写 已经准备好的数据
			}
			break;
				case 10:
			{
			sec[0]=pas[num].sec;
			SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+46),1);						//想flash写 已经准备好的数据
			SPI_Flash_Write((u8*)sec,(num*100+46),sizeof(sec));						//想flash写 已经准备好的数据
			}
			break;
						case 11:
			{
			min1[0]=pas[num].min1;
			SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+47),1);						//想flash写 已经准备好的数据
			SPI_Flash_Write((u8*)sec,(num*100+47),sizeof(min1));						//想flash写 已经准备好的数据
			}
			break;
						case 12:
			{
			hour1[0]=pas[num].hour1;
			SPI_Flash_Write((u8*)TEXT_Buffer,(num*100+48),1);						//想flash写 已经准备好的数据
			SPI_Flash_Write((u8*)sec,(num*100+48),sizeof(hour1));						//想flash写 已经准备好的数据
			}
			break;
		}
	}
}

void Flashread(u16 num)
{
		SPI_Flash_Read(NO,(num*100),sizeof(NO));								//读取 存入在flash里的数据
		SPI_Flash_Read(pas[num].SN,((num*100)+11),sizeof(pas[num].SN));								//读取 存入在flash里的数据
		SPI_Flash_Read(pas[num].stunumber,((num*100)+21),sizeof(pas[num].stunumber));								//读取 存入在flash里的数据
		SPI_Flash_Read(pas[num].name,((num*100)+31),sizeof(pas[num].name));								//读取 存入在flash里的数据
		SPI_Flash_Read(time,((num*100)+41),sizeof(time));								//读取 存入在flash里的数据
		SPI_Flash_Read(flag,((num*100)+42),sizeof(flag));								//读取 存入在flash里的数据
		SPI_Flash_Read(w_date,((num*100)+43),sizeof(w_date));								//读取 存入在flash里的数据
	  SPI_Flash_Read(hour,((num*100)+44),sizeof(hour));			
  	SPI_Flash_Read(min,((num*100)+45),sizeof(min));								//读取 存入在flash里的数据
		SPI_Flash_Read(sec,((num*100)+46),sizeof(sec));								//读取 存入在flash里的数据	
	  SPI_Flash_Read(min1,((num*100)+47),sizeof(min1));								//读取 存入在flash里的数据
		SPI_Flash_Read(hour1,((num*100)+48),sizeof(hour1));								//读取 存入在flash里的数据	
		pas[num].NO=NO[0];
		pas[num].time=time[0];
		pas[num].flag=flag[0];
		pas[num].w_date=w_date[0];
		pas[num].hour=hour[0];
		pas[num].min=min[0];
		pas[num].sec=sec[0];
	  pas[num].min1=min1[0];
		pas[num].hour1=hour1[0];
}

void Flashtest()
{
while(SPI_Flash_ReadID()!=W25Q16)							//检测不到W25Q64
	  {
		delay_ms(500);
		delay_ms(500);
			printf("OK");
	  }
 printf("Flashtest\r\n");
//		flashWriteSysData(0);
//		flashGetSysData(0);
	 while(1)
	 {
		 
		 if(USART_RX_STA&0x8000)	   //判断是否接收完数据
		{					   
			len=USART_RX_STA&0x3f;//得到此次接收到的数据长度   前两位为状态位 所以要与去 
			  
			if(len==6)																   //PA0 
		 {
			 		 for(t=0;t<200;t++)
			 {				 
//				 SPI_Flash_Write((u8*)TEXT_Buffer,(t)*100,1);						//想flash写 已经准备好的数据
//				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+11),4);						//想flash写 已经准备好的数据
//				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+21),24);						//想flash写 已经准备好的数据
//				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+31),10);						//想flash写 已经准备好的数据
//				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+41),1);						//想flash写 已经准备好的数据 
//				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+42),1);						//想flash写 已经准备好的数据 
//				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+43),1);						//想flash写 已经准备好的数据 
//				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+44),1);						//想flash写 已经准备好的数据 
//				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+45),1);						//想flash写 已经准备好的数据 
//				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+46),1);						//想flash写 已经准备好的数据 	 
//				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+47),1);						//想flash写 已经准备好的数据
//				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+48),1);						//想flash写 已经准备好的数据 
			 SPI_Flash_Write((u8*)TEXT_Buffer,t,SIZE);						//想flash写 已经准备好的数据
          printf("Write OK");
				  SPI_Flash_Read(datatemp,t,SIZE);								//读取 存入在flash里的数据
       printf("Read OK   %d",i);
			printf("  %d  \r\n",datatemp);
				 delay_ms(20);
			 }
		 }

		if(len==4)																   //PA0 
		 {
			 printf("clear all\r\n");
			SPI_Flash_Write((u8*)TEXT_Buffer,60,1);						//想flash写 已经准备好的数据
		 for(t=0;t<Number;t++)
			 {				 
				 SPI_Flash_Write((u8*)TEXT_Buffer,(t)*100,1);						//想flash写 已经准备好的数据
				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+11),4);						//想flash写 已经准备好的数据
				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+21),24);						//想flash写 已经准备好的数据
				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+31),10);						//想flash写 已经准备好的数据
				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+41),1);						//想flash写 已经准备好的数据 
				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+42),1);						//想flash写 已经准备好的数据 
				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+43),1);						//想flash写 已经准备好的数据 
				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+44),1);						//想flash写 已经准备好的数据 
				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+45),1);						//想flash写 已经准备好的数据 
				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+46),1);						//想flash写 已经准备好的数据 	 
				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+47),1);						//想flash写 已经准备好的数据
				 SPI_Flash_Write((u8*)TEXT_Buffer,(t*100+48),1);						//想flash写 已经准备好的数据 
			//			 SPI_Flash_Write((u8*)TEXT_Buffer,i,SIZE);						//想flash写 已经准备好的数据
			//          printf("Write OK");
			//				  SPI_Flash_Read(datatemp,i,SIZE);								//读取 存入在flash里的数据
			//       printf("Read OK   %d",i);
			//			printf("  %d  \r\n",datatemp);
			//				 delay_ms(20);
			 }
			 				 printf("clear all ok\r\n");

		 }

//		if(len==8)																  //PD2
//		{
//			printf("Read Flash");
//			for(i=0;i<(Number+1)*100;i++)
//			 {
//			 SPI_Flash_Read(datatemp,i,SIZE);								//读取 存入在flash里的数据
//       printf("Read OK");
//			printf("  %d  %s \r\n",i,datatemp);
//			 }
//		}
		
			if(len==5)																  //PD2
	  	{
				printf("clear time\r\n");
			 for(t=0;t<Number;t++)
				 {
					 pas[t].hour1=0;
					 pas[t].min1=0;
					 pas[t].time=0;
					 Flashwrite(t,0); 
					 Flashread(t);
				 }
				 printf("clear ok\r\n");
	  	}

				if(len==1)																  //PD2
	  	{
			printf("进入主菜单 \r\n\r\n");
      minf();
	  	}
		
			USART_RX_STA=0;			  //清零 虚拟寄存器		  为一下次采集做准备
		}
	 }							 
}

void Rtc()
{
      sprintf(to,"%d",calendar.w_year);
			USART_ClearFlag(USART3, USART_FLAG_TC);
			UART_PutStr(USART3, "t20.txt=\"");
			UART_PutStr(USART3, to);
			UART_PutStr(USART3, "\"");
			ff();delay_ms(4);	
			
			
					sprintf(to,"%d",calendar.w_month);
			USART_ClearFlag(USART3, USART_FLAG_TC);
			UART_PutStr(USART3, "t19.txt=\"");
			UART_PutStr(USART3, to);
			UART_PutStr(USART3, "\"");
			ff();delay_ms(4);	
			
					sprintf(to,"%d",calendar.w_date);
			USART_ClearFlag(USART3, USART_FLAG_TC);
			UART_PutStr(USART3, "t7.txt=\"");
			UART_PutStr(USART3, to);
			UART_PutStr(USART3, "\"");
			ff();delay_ms(4);	
			
					sprintf(to,"%d",calendar.hour);
			USART_ClearFlag(USART3, USART_FLAG_TC);
			UART_PutStr(USART3, "t8.txt=\"");
			UART_PutStr(USART3, to);
			UART_PutStr(USART3, "\"");
			ff();delay_ms(4);	
			
					sprintf(to,"%d",calendar.min);
			USART_ClearFlag(USART3, USART_FLAG_TC);
			UART_PutStr(USART3, "t9.txt=\"");
			UART_PutStr(USART3, to);
			UART_PutStr(USART3, "\"");
			ff();delay_ms(4);	
			
					sprintf(to,"%d",calendar.sec);
			USART_ClearFlag(USART3, USART_FLAG_TC);
			UART_PutStr(USART3, "t10.txt=\"");
			UART_PutStr(USART3, to);
			UART_PutStr(USART3, "\"");
			ff();delay_ms(4);	
}
void clear()
{

	USART_ClearFlag(USART3, USART_FLAG_TC);
	UART_PutStr(USART3, "t0.txt=\"");
	UART_PutStr(USART3, "\"");
	ff();delay_ms(4);	

	USART_ClearFlag(USART3, USART_FLAG_TC);
	UART_PutStr(USART3, "t1.txt=\"");
	UART_PutStr(USART3, "\"");
	ff();delay_ms(4);	

	USART_ClearFlag(USART3, USART_FLAG_TC);
	UART_PutStr(USART3, "t2.txt=\"");
	UART_PutStr(USART3, "\"");
	ff();delay_ms(4);	


	USART_ClearFlag(USART3, USART_FLAG_TC);
	UART_PutStr(USART3, "t3.txt=\"");
	UART_PutStr(USART3, "\"");	
	ff();delay_ms(4);	
	
		USART_ClearFlag(USART3, USART_FLAG_TC);
	UART_PutStr(USART3, "t14.txt=\"");
	UART_PutStr(USART3, "\"");
	ff();delay_ms(4);	

	USART_ClearFlag(USART3, USART_FLAG_TC);
	UART_PutStr(USART3, "t13.txt=\"");
	UART_PutStr(USART3, "\"");
	ff();delay_ms(4);	

	USART_ClearFlag(USART3, USART_FLAG_TC);
	UART_PutStr(USART3, "t11.txt=\"");
	UART_PutStr(USART3, "\"");	
	ff();delay_ms(4);	
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	UART_PutStr(USART3, "t25.txt=\"");
	UART_PutStr(USART3, "\"");	
	ff();delay_ms(4);	
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	UART_PutStr(USART3, "t29.txt=\"");
	UART_PutStr(USART3, "\"");	
	ff();delay_ms(4);	
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	UART_PutStr(USART3, "t28.txt=\"");
	UART_PutStr(USART3, "\"");	
	ff();delay_ms(4);	
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	UART_PutStr(USART3, "t12.txt=\"");
	UART_PutStr(USART3, "\"");	
	ff();delay_ms(4);	
}
void show(u8 t,u8 h)
{
	
			USART_ClearFlag(USART3, USART_FLAG_TC);
			UART_PutStr(USART3, "t14.txt=\"");
			UART_PutStr(USART3, pas[t].name);
			UART_PutStr(USART3, "\"");
			ff();delay_ms(4);	
					
			USART_ClearFlag(USART3, USART_FLAG_TC);
			UART_PutStr(USART3, "t13.txt=\"");
			UART_PutStr(USART3, pas[t].stunumber);
			UART_PutStr(USART3, "\"");
			ff();delay_ms(4);	
	
	    USART_ClearFlag(USART3, USART_FLAG_TC);
			UART_PutStr(USART3, "t12.txt=\"总时间\"");
			ff();delay_ms(4);		


	
			USART_ClearFlag(USART3, USART_FLAG_TC);
			UART_PutStr(USART3, "t25.txt=\"");
			sprintf(to,"%d",pas[t].hour1);
			UART_PutStr(USART3, to);
			UART_PutStr(USART3, "h ");
			sprintf(to,"%d",pas[t].min1);
			UART_PutStr(USART3, to);
			UART_PutStr(USART3, "m ");
			sprintf(to,"%d",pas[t].time);
			UART_PutStr(USART3, to);
			UART_PutStr(USART3, "s\"");
			ff();delay_ms(4);	
			
		if(h)
		{   
			USART_ClearFlag(USART3, USART_FLAG_TC);
			UART_PutStr(USART3, "t28.txt=\"本次时间\"");
			ff();delay_ms(4);		
			
			USART_ClearFlag(USART3, USART_FLAG_TC);
			UART_PutStr(USART3, "t29.txt=\"");
			sprintf(to,"%d",temphour);
			UART_PutStr(USART3, to);
			UART_PutStr(USART3, "h ");
			sprintf(to,"%d",tempmin);
			UART_PutStr(USART3, to);
			UART_PutStr(USART3, "m ");
			sprintf(to,"%d",tempsec);
			UART_PutStr(USART3, to);
			UART_PutStr(USART3, "s\"");
			ff();delay_ms(4);	
		}
//u8 temphour;
//u8 tempmin;
//u8 tempsec;
			
}

void minf()
{
while(1)
		{
	   clear();	
			Rtc();
		if(USART_RX_STA&0x8000)
			{	
				len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度	
		  		for(t=0;t<=(len-1);t++)
				{			
					if(USART_RX_BUF[t]!=Atten[t])
						t=len;
					if(t==(len-1))
					{
						Attendance();
					}
				}
						
  				for(t=0;t<=7;t++)
				{			
					if(USART_RX_BUF[t]!=Regi[t])
						t=8;
					if(t==(7))
					{
						Register();
					
					}
				}
				if(len==2)																   //PA0 
		 {	 
//			 printf("%d  ",Number);
		
			 for(t=0;t<Number;t++)
			 {
	//			 Flashwrite(t,0);
//				 Flashread(t);
				 printf("%d  ",pas[t].NO);
				 printf("%s  ",pas[t].name);
				 printf("%s  ",pas[t].stunumber);
				 printf("%d  ",pas[t].w_date);
				 printf("%d  ",pas[t].hour);
				 printf("%d  ",pas[t].min);
				 printf("%d  ",pas[t].sec);
				 printf("%d  ",pas[t].flag);
				
		 		 printf("%d  ",pas[t].hour1);
				 printf("%d  ",pas[t].min1); 
				 printf("%d  ",pas[t].time);
					 for(t1=0;t1<4;t1++)
				 {
			    printf("%d ",pas[t].SN[t1]);
				 }
				 printf(" \r\n");
			 }
		 }
		 	if(len==7)																   //PA0 
		 {
  Flashtest();
		 }
				USART_RX_STA=0;
			}
		}
}
int main(void)
{


	
	Stm32_Clock_Init(9);
	delay_init();
	BEEP_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart1_init(115200);	 //串口初始化为9600
	uart3_init(9600);	 //串口初始化为9600
	RTC_Init();	  			//RTC初始化
	InitRc522();
	SPI_Flash_Init();  	 //SPI FLASH 初始化
// BEEP=0;	

	
//	USART_ClearFlag(USART1, USART_FLAG_TC);
//  USART_SendData(USART3,  0xff );//向串口3发送数据
//	while(1)
//		{	
//			printf("fgsg");
//			               delay_ms(4);	

////		UART_PutStr(USART3, "1242");
//		}
	
//	  Flashtest();
//					 Flashread(0); 
    		
		SPI_Flash_Read(NOO,60,sizeof(NOO));								//读取 存入在flash里的数据	
		printf("%d   ",NOO[0]);
    Number=NOO[0];
			printf("%d\r\n",Number);
		for(t=0;t<Number;t++)
			 {
			 Flashread(t);
			 }
	Attendance();
//   minf();
			
}

void Registerbranch1()
{
		
	if(status==MI_OK)//讀卡成功
	 {
			for(t=0;t<4;t++)
			  {		
					pas[Number].SN[t]=SN[t];
					Flashwrite(Number,SNq);
//						USART_ClearFlag(USART1, USART_FLAG_TC);
											sprintf(to,"%d",SN[t]);
						switch(t)
						{
							case 0: 
							{
								USART_ClearFlag(USART3, USART_FLAG_TC);
								UART_PutStr(USART3, "t0.txt=\"");
								UART_PutStr(USART3, to);
								UART_PutStr(USART3, "\"");
								ff();
								printf("你的卡号为： %s ",to);
							}
							break;
							case 1: 
							{
								USART_ClearFlag(USART3, USART_FLAG_TC);
								UART_PutStr(USART3, "t1.txt=\"");
								UART_PutStr(USART3, to);
								UART_PutStr(USART3, "\"");
								ff();
								printf("%s ",to);
							}
								break;
							case 2: 
							{
								USART_ClearFlag(USART3, USART_FLAG_TC);
								UART_PutStr(USART3, "t2.txt=\"");
								UART_PutStr(USART3, to);
								UART_PutStr(USART3, "\"");
								ff();
	    					printf("%s ",to);
							}
							break;
							case 3: 
							{
								USART_ClearFlag(USART3, USART_FLAG_TC);
								UART_PutStr(USART3, "t3.txt=\"");
								UART_PutStr(USART3, to);
								UART_PutStr(USART3, "\"");	
								ff();			
								printf("%s ",to);
							}
							break;
						}
               delay_ms(4);	
			  }		 
			while(1)
		 {
		   Rtc();
				if(USART_RX_STA&0x8000)
			{
				
				len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度	 &&(USART_RX_BUF[(10)]==ok[0])   

//          TEXT_Buffer[0]=Number[0];
//					printf("%d",len);
				
						if(len==1)
			 {
				 	 printf("进入主菜单 \r\n\r\n");
            minf();
			 }
				
			for(t=0;t<=(len-1);t++)
			{			
				if(USART_RX_BUF[t]==ok[0])
				{
					divide=t;
					t=len;
				}
			}
			
			if((USART_RX_BUF[(len-1)]==ojbk[3]))
			{
						printf("注册成功, ");
					printf(" 学号：");
					pas[Number].NO=Number;
			    Flashwrite(Number,NOq);
								for(t=0;t<24;t++)
				{
				pas[Number].stunumber[t]=0;
				}
						for(t=0;t<divide;t++)
				{		
					pas[Number].stunumber[t]=USART_RX_BUF[t];
					printf("%c", USART_RX_BUF[t]);					
				}
				Flashwrite(Number,stunumberq);
	//					printf("\r\n");
				printf("    名字：");
							for(t=0;t<10;t++)
				{
				pas[Number].name[t]=0;
				}
				for(t=divide+2,t1=0;t<(len-4);t++,t1++)
				{		
					pas[Number].name[t1]=USART_RX_BUF[t];
					printf("%c", USART_RX_BUF[t]);					
				}		
				Flashwrite(Number,nameq);
				printf("\r\n");
				printf("\r\n");
				Number++;
				NOO[0]=Number;
	       SPI_Flash_Write((u8*)TEXT_Buffer,60,1);						//想flash写 已经准备好的数据
	       SPI_Flash_Write((u8*)NOO,60,sizeof(NOO));						//想flash写 已经准备好的数据
			 		SPI_Flash_Read(NOO,60,sizeof(NOO));								//读取 存入在flash里的数据	
          Number=NOO[0];
				 	printf("进入主菜单 \r\n\r\n");
				minf();
			}
				USART_RX_STA=0;
			}
		
		 }
	 }
}

void Registerbranch2()
{
	 
	if(status==MI_OK)//讀卡成功
	 {
			for(t=0;t<4;t++)
			  {	
//					pas[Number].SN[t]=SN[t];
//				  Flashwrite(Number,SNq);
//						USART_ClearFlag(USART1, USART_FLAG_TC);
											sprintf(to,"%d",SN[t]);
						switch(t)
						{
							case 0: 
							{
								USART_ClearFlag(USART3, USART_FLAG_TC);
								UART_PutStr(USART3, "t0.txt=\"");
								UART_PutStr(USART3, to);
								UART_PutStr(USART3, "\"");
								ff();
								printf("你的卡号为： %s ",to);
							}
							break;
							case 1: 
							{
								USART_ClearFlag(USART3, USART_FLAG_TC);
								UART_PutStr(USART3, "t1.txt=\"");
								UART_PutStr(USART3, to);
								UART_PutStr(USART3, "\"");
								ff();
								printf("%s ",to);
							}
								break;
							case 2: 
							{
								USART_ClearFlag(USART3, USART_FLAG_TC);
								UART_PutStr(USART3, "t2.txt=\"");
								UART_PutStr(USART3, to);
								UART_PutStr(USART3, "\"");
								ff();
	    					printf("%s ",to);
							}
							break;
							case 3: 
							{
								USART_ClearFlag(USART3, USART_FLAG_TC);
								UART_PutStr(USART3, "t3.txt=\"");
								UART_PutStr(USART3, to);
								UART_PutStr(USART3, "\"");	
								ff();			
								printf("%s ",to);
								
							}
							break;
						}
               delay_ms(4);	
			  }
   		
			while(1)
		 {
			 Rtc();
				if(USART_RX_STA&0x8000)
			{
				
				len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度	 &&(USART_RX_BUF[(10)]==ok[0])   

//          TEXT_Buffer[0]=Number[0];
//					printf("%d",len);
				
					if(len==1)
			 {
					 printf("进入主菜单 \r\n\r\n");
						minf();
			 }
				
			for(t=0;t<=(len-1);t++)
			{			
				if(USART_RX_BUF[t]==ok[0])
				{
					divide=t;
					t=len;
				}
			}
			
			if((USART_RX_BUF[(len-1)]==ojbk[3]))
			{	
				
					printf("注册成功, ");
				printf(" 学号：");
				pas[TempNumber].NO=TempNumber;
				Flashwrite(TempNumber,NOq);
					for(t=0;t<24;t++)
			{
			pas[TempNumber].stunumber[t]=0;
			}
					for(t=0;t<divide;t++)
			{		
				pas[TempNumber].stunumber[t]=USART_RX_BUF[t];
				printf("%c", USART_RX_BUF[t]);					
			}
			Flashwrite(TempNumber,stunumberq);
//					printf("\r\n");
			printf("    名字：");
			for(t=0;t<10;t++)
			{
			pas[TempNumber].name[t]=0;
			}
			for(t=divide+2,t1=0;t<(len-4);t++,t1++)
			{		
				pas[TempNumber].name[t1]=USART_RX_BUF[t];
				printf("%c", USART_RX_BUF[t]);					
			}
			Flashwrite(TempNumber,nameq);
			printf("\r\n");	
			printf("\r\n");
			NOO[0]=Number;
	       SPI_Flash_Write((u8*)TEXT_Buffer,60,SIZE);						//想flash写 已经准备好的数据
	       SPI_Flash_Write((u8*)NOO,60,sizeof(NOO));						//想flash写 已经准备好的数据
			 		SPI_Flash_Read(NOO,60,sizeof(NOO));								//读取 存入在flash里的数据	
          Number=NOO[0];
				 	 printf("进入主菜单 \r\n\r\n");
      minf();
			}
				USART_RX_STA=0;
			}
		
		 }
	 }
}

void Register(void)
{
		printf("注册前请刷卡！ ");
		printf("\r\n\r\n");	
	while(1)
	{
				if(USART_RX_STA&0x8000)
			{					
				len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
								if(len==1)
			 {
					 printf("进入主菜单 \r\n\r\n");
						minf();
			 }
				USART_RX_STA=0;
			}
			
		status = PcdRequest(PICC_REQALL,CT);/*尋卡*/
		if(status==MI_OK)//尋卡成功
		{
			 status=MI_ERR;
			 status = PcdAnticoll(SN);/*防冲撞*/
		}	
		
		if(status==MI_OK)
		{
			status=MI_ERR;
			status =PcdSelect(SN);			
		}

		if(status==MI_OK)//選卡成功
		{
			 status=MI_ERR;
			 status =PcdAuthState(0x60,0x06,KEY,SN);					
		}
	 					 
		 if(status==MI_OK)//驗證成功
		{
     
			for(t=0;t<Number;t++)
			 {
				 for(t1=0;t1<4;t1++)
				 {
					 if((pas[t].SN[t1]==SN[t1])&&(t1==3))
					 {   
						 TempNumber=t;
						 Registerbranch2();

					 }					 
				 }
			 }   
			 	  Registerbranch1();
		 }
			
	}
}

void Attendance(void)
{ 
	 printf("考 勤 ！ ");
		printf("\r\n\r\n");	

	while(1)
	{
			 Rtc();
		clear();
		if(USART_RX_STA&0x8000)
		{					
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
				if((len==1)&&(pas[t].flag==0))
			 {
				 	 printf("进入主菜单 \r\n\r\n");
            minf();
			 }
				USART_RX_STA=0;
		}
		status = PcdRequest(PICC_REQALL,CT);/*尋卡*/

		if(status==MI_OK)//尋卡成功
		{
			 status=MI_ERR;
			 status = PcdAnticoll(SN);/*防冲撞*/
		}	
		if(status==MI_OK)
		{
			status=MI_ERR;
			status =PcdSelect(SN);			
		}

		if(status==MI_OK)//選卡成功
		{
			 status=MI_ERR;
			 status =PcdAuthState(0x60,0x06,KEY,SN);
		}
	 					 
		 if(status==MI_OK)//驗證成功
		 {
				 status=MI_ERR;
					 status= PcdRead(4,RFID);	
			  for(t=0;t<4;t++)
			  {	
//					pas[flag].SN[t]=SN[t];
											sprintf(to,"%d",SN[t]);
						switch(t)
						{
							case 0: 
							{
								USART_ClearFlag(USART3, USART_FLAG_TC);
								UART_PutStr(USART3, "t0.txt=\"");
								UART_PutStr(USART3, to);
								UART_PutStr(USART3, "\"");
								ff();
							}
							break;
							case 1: 
							{
								USART_ClearFlag(USART3, USART_FLAG_TC);
								UART_PutStr(USART3, "t1.txt=\"");
								UART_PutStr(USART3, to);
								UART_PutStr(USART3, "\"");
								ff();
							}
								break;
							case 2: 
							{
								USART_ClearFlag(USART3, USART_FLAG_TC);
								UART_PutStr(USART3, "t2.txt=\"");
								UART_PutStr(USART3, to);
								UART_PutStr(USART3, "\"");
								ff();
							}
							break;
							case 3: 
							{
								USART_ClearFlag(USART3, USART_FLAG_TC);
								UART_PutStr(USART3, "t3.txt=\"");
								UART_PutStr(USART3, to);
								UART_PutStr(USART3, "\"");	
								ff();
							}
							break;
						}
               delay_ms(4);	
			  }
  	 }
		 
		if(status==MI_OK)//讀卡成功
		 {    
//			 	for(t=0;t<Number;t++)
//			 { 
//				 for(t1=0;t1<4;t1++)
//				 {
//					 if((pas[t].SN[t1]==SN[t1])&&(t1==3))
//					 { 
//             pas[t].w_date=calendar.w_date;
//						 pas[t].hour=calendar.hour;
//						 pas[t].min=calendar.min;
//						 pas[t].sec=calendar.sec;
//					 }
//				 }
//			 }
			 		for(t=0;t<Number;t++)
			 { 
				 for(t1=0;t1<4;t1++)
				 {
					 if((pas[t].SN[t1]==SN[t1])&&(t1==3)&&(pas[t].flag==0))
					 { 
						 
						 if(calendar.w_date==pas[t].w_date)
						 {
						 timesec=((calendar.sec-pas[t].sec)+60*(calendar.min-pas[t].min)+3600*(calendar.hour-pas[t].hour));
						 }
             else
							 timesec=6;
             if((timesec>5)||(pas[t].w_date==0))
						 {				
      	     printf("进\r\n");								 
						 pas[t].w_date=calendar.w_date;
						 pas[t].hour=calendar.hour;
						 pas[t].min=calendar.min;
						 pas[t].sec=calendar.sec;
             pas[t].flag=1;
//						  printf("pas[t].hour:  %d\r\n",pas[t].hour);
//						 printf("pas[t].min:  %d\r\n",pas[t].min);
//						  printf("pas[t].sec:  %d\r\n",pas[t].sec);
//						temphour=0;
//						tempmin=0;
//						tempsec=0;
							 
							beep(0);
			   		  delay_ms(1);				 
							show(t,0);
							
							USART_ClearFlag(USART3, USART_FLAG_TC);
							UART_PutStr(USART3, "t11.txt=\"");
							UART_PutStr(USART3, "进");
							UART_PutStr(USART3, "\"");
							ff();delay_ms(4);	
							USART_ClearFlag(USART3, USART_FLAG_TC);
							UART_PutStr(USART3, "t11.pco=63504");
							ff();
//							 Flashwrite(t,flagq);
						 Flashwrite(t,0);
							 delay_ms(1000);	
						 }
					 }				
      					 
				 }
			 }  
			 
			 	for(t=0;t<Number;t++)
			 {
				 for(t1=0;t1<4;t1++)
				 {
					     if((pas[t].SN[t1]==SN[t1])&&(t1==3)&&(pas[t].flag==1))
					 {   
						 if(calendar.w_date==pas[t].w_date)
						 {
						 timesec=((calendar.sec-pas[t].sec)+60*(calendar.min-pas[t].min)+3600*(calendar.hour-pas[t].hour));
						 }
						 else
							 timesec=6;
//						 printf("timesec:  %d\r\n",timesec);
//						  printf("pas[t].hour:  %d\r\n",pas[t].hour);
//						 printf("pas[t].min:  %d\r\n",pas[t].min);
//						  printf("pas[t].sec:  %d\r\n",pas[t].sec);
						temptimesec=timesec;
					  temphour=0;
						tempmin=0;
						tempsec=0;
						
						 while(temptimesec>3600)
							{
							temptimesec-=3600;
							temphour++;
							}
							while(temptimesec>60)
							{
							temptimesec-=60;
							tempmin++;
							}	
							tempsec=temptimesec;
					
						 if(timesec>5)
						 {
							 	 printf("出\r\n");	
							 	timesec+=pas[t].time;
							 pas[t].time=0;	
							 while(timesec>3600)
								{
								timesec-=3600;
								pas[t].hour1++;
								}
							 while(timesec>60)
								{
								timesec-=60;
								pas[t].min1++;
								}		
								pas[t].time=timesec;
			         if(pas[t].min1>60)
							 {
							 pas[t].min1-=60;
							 pas[t].hour1++;
							 }
							 
						  	 beep(1);
			          delay_ms(1);	
							 show(t,1);	
							USART_ClearFlag(USART3, USART_FLAG_TC);
							UART_PutStr(USART3, "t11.txt=\"");
							UART_PutStr(USART3, "出");
							UART_PutStr(USART3, "\"");
							ff();delay_ms(4);	
							USART_ClearFlag(USART3, USART_FLAG_TC);
							UART_PutStr(USART3, "t11.pco=2024");
							ff();
								
						 pas[t].w_date=calendar.w_date;		  	 
						 pas[t].hour=calendar.hour;		  	 
						 pas[t].min=calendar.min;				   
						 pas[t].sec=calendar.sec;						
             pas[t].flag=0;					
							 Flashwrite(t,0);
				
							  delay_ms(1000);	
						 }				
					 }	
				 }
			 }

		 }	 
	}
}

void UART_PutChar(USART_TypeDef* USARTx, uint8_t Data)  
{  
	USART_SendData(USARTx, Data);  
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET){}  
} 
 
void UART_PutStr (USART_TypeDef* USARTx, uint8_t *str)    
{    
	while (0 != *str)    
	{    
		UART_PutChar(USARTx, *str);    
		str++;    
	}    
}   

void ff()
{  
  int i=0;
	do
		 {
			 USART_SendData(USART3,  0xff );//向串口1发送数据
			 i++;
		 }
		 while(i<=5000);
}   
