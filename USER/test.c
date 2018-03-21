#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "usmart.h" 
#include "sram.h"
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "timer.h"	  
#include "text.h"	 
#include "24tft.h"
#include "MM.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_conf.h"
#include "stdio.h"


//ALIENTEK战舰STM32开发板 实验40
//汉字显示 实验 
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司

void GPIO_Config(void);

int main(void)
{		
	u8 res,times=0;
	u32 j=0,k=0,q=0;
	u8 buf[4096],num[20];
	long lsector;
	Stm32_Clock_Init(9);		//系统时钟设置
	uart_init(72,115200);		//串口初始化为115200
	delay_init(72);	   	 		//延时初始化  
	GPIO_Config();
	ILI9325_CMO24_Initial();	
	SPILCD_Clear(0x00);
	LCD_PutString(0,0,"Read&Write");
	//LCD_PutString(60,16,"T");
	//while(1);
 	//usmart_dev.init(72);		//初始化USMART		
	TIM6_Int_Init(10000,7199);	//10Khz计数频率,1s中断一次
 	my_mem_init(SRAMIN);		//初始化内部内存池
 	while(SD_Init())			//检测不到SD卡
	{
		LCD_PutString(0,0,"Init fail");
	}
	while(1)
	{ 
		for(lsector=44700;lsector<46650;lsector=lsector+8)
	  {
	       res=SD_ReadDisk(buf,lsector,8);
		     if(res!=0) 
				 {
					 LCD_PutString(0,20,"read faila");
					 while(1);
				 }
			   res=SD_WriteDisk(buf,lsector,8);
			   if(res!=0) 
				 {
					 LCD_PutString(0,20,"write faila");
					 while(1);
				 }
	  }
		for(lsector=0;lsector<50000;lsector=lsector+8)
	  {
	     	 res=SD_ReadDisk(buf,lsector,8);
		     if(res!=0) 
				 {
					 LCD_PutString(0,20,"read failb");
					 while(1);
				 }
			   res=SD_WriteDisk(buf,lsector,8);
			   if(res!=0) 
				 {
					 LCD_PutString(0,20,"write failb");
					 while(1);
				 }
				 j++;
         sprintf(num, "%d", j*8);
				 LCD_PutString(0,40,num);
	  }
		LCD_PutString(0,20,"finish      ");
		while(1);
		j=0;
  }
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);

																	  //BL        //RS            CS          SCK          MISO         SDA    
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOA, GPIO_Pin_2);
// 	GPIO_SetBits(GPIOC, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);	 // turn off all led
}








