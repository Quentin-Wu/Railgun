#include "key.h"

volatile uint16_t keyvalue = 0;

void KEY_IntHandler(void)
{
	uint32_t IntStatus_raw;
	IntStatus_raw = GPIOIntStatus(GPIO_PORT_KEY_BASE, true);	//��ȡGPIOֵ
	
	if((IntStatus_raw & SW1)==SW1)  // SW1�������ж��¼�
	{
			keyvalue = 1;
			GPIOIntClear(GPIO_PORT_KEY_BASE,SW1);//���SW1
	}
	
	if((IntStatus_raw & SW2)==SW2)  // SW2�������ж��¼�
	{
			keyvalue = 2;
			GPIOIntClear(GPIO_PORT_KEY_BASE,SW2);//���SW2
	}

}

void KEY_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_KEY);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIO_KEY));
	
	//����PF0(SW2����)
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0xFF;
	
	GPIOPinTypeGPIOInput(GPIO_PORT_KEY_BASE, SW1 |SW2 );	
	GPIOPadConfigSet(GPIO_PORT_KEY_BASE, SW1 |SW2 , GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	
	//�ж� �½��ش���
	GPIOIntTypeSet(GPIO_PORT_KEY_BASE, SW1 |SW2 , GPIO_FALLING_EDGE );
	//ʹ���ж�
	GPIOIntEnable(GPIO_PORT_KEY_BASE, SW1 |SW2 );
	//ע���жϴ�����
	GPIOIntRegister(GPIO_PORT_KEY_BASE, KEY_IntHandler);
	//����жϱ�־λ
	GPIOIntClear(GPIO_PORT_KEY_BASE, SW1 |SW2 );

}


