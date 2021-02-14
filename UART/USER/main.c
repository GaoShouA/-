#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart3.h"
#include <string.h>

//ALIENTEK 探索者STM32F407开发板 实验4
//串口通信实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化 
	uart_init(115200);	//串口初始化波特率为115200
	//usart3_init(115200);
	//usart2_init(115200);
	while(1)
	{
		u1_printf("祝大家2021新年快乐!%d%c\n",232,'c');
//		u2_printf("祝大家2021新年快乐!%d%c\n\r\n",12,'c');
//		u3_printf("祝大家2021新年快乐!%d%c\n\r\n",12,'c');
//		u3_printf("%d\r\n",121);
//		printf("%c\r\n",'c');
		//printf(arr);
		//printf("hello world\r");
		delay_ms(1000);
//		if(USART_RX_STA&0x8000)
//		{
//			printf(USART_RX_BUF);
//			USART_RX_STA=0;
//			memset(USART_RX_BUF,0,200);
//		}
	}
}

