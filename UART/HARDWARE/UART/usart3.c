#include "delay.h"
#include "usart3.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	  
#include "usart.h"


//���ڷ��ͻ�����
u8 USART2_TX_BUF[USART2_MAX_SEND_LEN];
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN];

#ifdef USART3_RX_EN   								//���ʹ���˽���   	 
 
//���ڽ��ջ�����
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�. 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.


//ͨ���жϽ�������2���ַ�֮���ʱ������100ms�������ǲ���һ������������.
//���2���ַ����ռ������100ms,����Ϊ����1����������.Ҳ���ǳ���100msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���	  
u16 USART3_RX_STA = 0;
void USART3_IRQHandler(void)//����3�жϷ������
{
    u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.��������Ͳ�ȥ����
    OSIntEnter();
#endif
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res =USART_ReceiveData(USART3);//(USART1->DR);	//��ȡ���յ�������

        if((USART3_RX_STA&0x8000)==0)//����δ���
        {
            if(USART3_RX_STA&0x4000)//���յ���0x0d
            {
                if(Res!=0x0a)USART3_RX_STA=0;//���մ���,���¿�ʼ
                else USART3_RX_STA|=0x8000;	//���������
            }
            else //��û�յ�0X0D
            {
                if(Res==0x0d)USART3_RX_STA|=0x4000;
                else
                {
                    USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
                    USART3_RX_STA++;
                    if(USART3_RX_STA>(USART_REC_LEN-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����
                }
            }
        }
    }
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
    OSIntExit();
#endif
}


  
#endif	

#ifdef USART2_RX_EN   								//���ʹ���˽���   	
u16 USART2_RX_STA = 0;					
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
//ucos���嶼ɾ��
void USART2_IRQHandler(void)//����2�жϷ������
{
	u8 Res;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res =USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������

        if((USART2_RX_STA&0x8000)==0)//����δ���
        {
            if(USART2_RX_STA&0x4000)//���յ���0x0d
            {
                if(Res!=0x0a)USART2_RX_STA=0;//���մ���,���¿�ʼ
                else USART2_RX_STA|=0x8000;	//���������
            }
            else //��û�յ�0X0D
            {
                if(Res==0x0d)USART2_RX_STA|=0x4000;
                else
                {
                    USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
                    USART2_RX_STA++;
                    if(USART2_RX_STA>(USART_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����
                }
            }
        }
    }
}

#endif
void usart2_init(u32 bound)
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��

	USART_DeInit(USART2);  //��λ����2

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA2����ΪUSART2

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3; 	//GPIOA2��GPIOA3��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 				//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 				//����
	GPIO_Init(GPIOA,&GPIO_InitStructure); 						//��ʼ��GPIOA2����GPIOA3

	USART_InitStructure.USART_BaudRate = bound;										//������ 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); 			//��ʼ������2
	USART_Cmd(USART2, ENABLE);               			//ʹ�ܴ���2 
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		//�����ж�2   

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1 �����Լ��߼��༭����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

//��ʼ��IO ����3
//bound:������	  
void usart3_init(u32 bound)
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��

	USART_DeInit(USART3);  //��λ����3

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10����ΪUSART3	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; 	//GPIOB11��GPIOB10��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 				//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 				//����
	GPIO_Init(GPIOB,&GPIO_InitStructure); 						//��ʼ��GPIOB11����GPIOB10

	USART_InitStructure.USART_BaudRate = bound;										//������ 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); 			//��ʼ������3
	USART_Cmd(USART3, ENABLE);               			//ʹ�ܴ��� 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);		//�����ж�   

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	USART3_RX_STA=0;					//����
}


//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u2_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;//������һ����ָ��
	memset(USART2_TX_BUF,'\0',USART2_MAX_SEND_LEN);
	va_start(ap,fmt);//
	vsprintf((char*)USART2_TX_BUF,fmt,ap);//��ʽ���������Ĳ���
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);//�˴η������ݵĳ���
	
	for(j=0;j<i;j++)//ѭ����������
	{
		//�ȴ��ϴδ������ 
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
		//�������ݵ�����3 
		USART_SendData(USART2,(uint8_t)USART2_TX_BUF[j]);
	}
	
}

//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u3_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;//������һ����ָ��
	memset(USART3_TX_BUF,'\0',USART3_MAX_SEND_LEN);
	va_start(ap,fmt);//
	vsprintf((char*)USART3_TX_BUF,fmt,ap);//��ʽ���������Ĳ���
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);//�˴η������ݵĳ���
	
	for(j=0;j<i;j++)//ѭ����������
	{
		//�ȴ��ϴδ������ 
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
		//�������ݵ�����3 
		USART_SendData(USART3,(uint8_t)USART3_TX_BUF[j]);
	}
	
}

