#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart3.h"
#include <string.h>

//ALIENTEK ̽����STM32F407������ ʵ��4
//����ͨ��ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);		//��ʱ��ʼ�� 
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	//usart3_init(115200);
	//usart2_init(115200);
	while(1)
	{
		u1_printf("ף���2021�������!%d%c\n",232,'c');
//		u2_printf("ף���2021�������!%d%c\n\r\n",12,'c');
//		u3_printf("ף���2021�������!%d%c\n\r\n",12,'c');
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

