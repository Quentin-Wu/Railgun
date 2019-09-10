#include "myuart.h"
#include "led.h"

uint8_t printfff = 1;   //Ĭ�Ϸ��͸�uart0
                        //��Ϊ0ʱ���͸� uart1 openmv  ʵ�ֿ��ƾ������ K


void UART0IntHandler(void);

uint8_t USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t USART_RX_STA=0;       //����״̬���	

void My_Uart_Init(void)
{
	//ʹ��UART����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	//ʹ��GPIO����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	//GPIO����ģʽ����
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 |GPIO_PIN_1);
	//uart���� �����ʣ�115200 8λ 1λֹͣλ ��У��
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(),
                    115200, (UART_CONFIG_WLEN_8 |UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	
	//UART����FIFO Ĭ��FIFO LevelΪ4/8 �Ĵ�����8�ֽں�����ж�
	//���ú����1λ�Ͳ����ж�
	UARTFIFODisable(UART0_BASE);
	//ʹ��UART0�ж�
	IntEnable(INT_UART0);
	//ʹ��UART0�����ж�
	UARTIntEnable(UART0_BASE,UART_INT_RX |UART_INT_RT);
	//UART�жϵ�ַע��
	UARTIntRegister(UART0_BASE,UART0IntHandler);
	//�����ж����ȼ�
	IntPrioritySet(INT_UART0, 0);
	
}

void UART0IntHandler(void)
{
	//��ȡ�жϱ�־ ԭʼ�ж�״̬ �������жϱ�־
//	uint32_t flag = UARTIntStatus(UART0_BASE,0);
	//��ȡ�жϱ�־ ԭʼ�ж�״̬ �����жϱ�־
	uint32_t flag = UARTIntStatus(UART0_BASE,1);
	uint8_t	Res;

	//����жϱ�־
	UARTIntClear(UART0_BASE,flag);
	
	if(flag&UART_INT_RX)	//�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		//UARTCharsAvail()�ж�FIFO�Ƿ�������
//		while(UARTCharsAvail(UART0_BASE))
		Res =UARTCharGet(UART0_BASE);	//��ȡ���յ�������
	
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}
				}
			}
	}
}


///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	if (printfff)
	{
		/* ����һ���ֽ����ݵ����� */
		UARTCharPut(UART0_BASE, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (UARTCharsAvail(UART0_BASE));		
	
		return(ch);
	}
	else  //openmv
	{
		/* ����һ���ֽ����ݵ����� */
		UARTCharPut(UART1_BASE, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (UARTCharsAvail(UART1_BASE));		
	
		return(ch);
	}
}

//����ָ�����ȵ�����
void usart_putbuff(uint8_t *buff, uint32_t len)
{
//	LED_GREEN_DUBUG(ON);
	while(len--)
	{
		UARTCharPut(UART0_BASE,*buff);
		buff++;
	}
//	LED_GREEN_DUBUG(OFF);
}

//uint32_t warebuf[8];
//vcan_sendware((uint8_t *)warebuf,sizeof(warebuf)); 

//����ͨ��Э������λ����������
void vcan_sendware(uint8_t *wareaddr, uint32_t waresize)
{
	#define CMD_WARE												3 //3��ʾ����ʾ������1����ͷ��2CCD
	uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};
	uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};
	
	usart_putbuff(cmdf, sizeof(cmdf));
	usart_putbuff(wareaddr, waresize);
	usart_putbuff(cmdr, sizeof(cmdr));
}

