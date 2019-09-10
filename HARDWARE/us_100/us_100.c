#include "us_100.h"
#include "led.h"
#include "delay.h"

//���ڽ��� uart4
//PC4 -> Rx
//PC5 -> Tx

void UART4IntHandler(void);

void My_Uart4_Init(void)
{
	//ʹ��UART����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
	//ʹ��GPIO����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	//GPIO����ģʽ����
	GPIOPinConfigure(GPIO_PC4_U4RX);
	GPIOPinConfigure(GPIO_PC5_U4TX);
	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 |GPIO_PIN_5);
	//uart���� �����ʣ�115200 8λ 1λֹͣλ ��У��
	UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(),
                    9600, (UART_CONFIG_WLEN_8 |UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	
//	//UART����FIFO Ĭ��FIFO LevelΪ4/8 �Ĵ�����8�ֽں�����ж�
//	//���ú����1λ�Ͳ����ж�
//	UARTFIFODisable(UART4_BASE);
////	UARTFIFOEnable(UART4_BASE);
//	//ʹ��UART0�ж�
//	IntEnable(INT_UART4);
//	//ʹ��UART0�����ж�
//	UARTIntEnable(UART4_BASE,UART_INT_RX |UART_INT_RT);
//	//UART�жϵ�ַע��
//	UARTIntRegister(UART4_BASE,UART4IntHandler);
}

//�жϴ���
//void UART4IntHandler(void)
//{
//	uint32_t flag = UARTIntStatus(UART4_BASE,1);
//	int32_t a,b=0;
//	float length;
//	
//	UARTIntClear(UART4_BASE,flag);
//	
//	a = UARTCharGet(UART4_BASE);
//	b = UARTCharGet(UART4_BASE);
//	length = (float)(a*256 + b)/10; //��ȡFIFO�е����ݲ�����
//	
//	LED_GREEN_TUGGEL
//}


//���жϴ���
bool us_100_GetLength(float* length)
{
	int32_t a,b=0;
	//���Ͷ�ȡ�������� 0x55
	UARTCharPut(UART4_BASE, 0x55);	
	
	//���FIFO��������
	if(UARTCharsAvail(UART4_BASE) == true)
	{	
		a = UARTCharGet(UART4_BASE);
		b = UARTCharGet(UART4_BASE);
		*length = (float)(a*256 + b)/10; //��ȡFIFO�е����ݲ�����
		LED_GREEN_TUGGEL
		return 0;
	}
	else
	{	
		return 1;
	}
}










