#include "openmv.h"
#include "myuart.h"
#include "control.h"

uint16_t openmv[7]={0};

extern uint8_t printfff;   //Ĭ�Ϸ��͸�uart0
                           //��Ϊ0ʱ���͸� uart1 openmv  ʵ�ֿ��ƾ������ K
                           //�������ݺ�ǵøĻ�1

//RX    PC4
//TX    PC5

void UART1IntHandler(void);
//uart1
void Openmv_Init(void)
{
	//ʹ��UART����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	//ʹ��GPIO����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	//GPIO����ģʽ����
	GPIOPinConfigure(GPIO_PC4_U1RX);
	GPIOPinConfigure(GPIO_PC5_U1TX);
	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 |GPIO_PIN_5);
	//uart���� �����ʣ�115200 8λ 1λֹͣλ ��У��
	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(),
                    115200, (UART_CONFIG_WLEN_8 |UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	
	//UART����FIFO Ĭ��FIFO LevelΪ4/8 �Ĵ�����8�ֽں�����ж�
	//���ú����1λ�Ͳ����ж�
	UARTFIFODisable(UART1_BASE);
	//ʹ��UART0�ж�
//	IntEnable(INT_UART1);
	IntDisable(INT_UART1);
	//ʹ��UART0�����ж�
//	UARTIntEnable(UART1_BASE,UART_INT_RX |UART_INT_RT);
	UARTIntDisable(UART1_BASE,UART_INT_RX |UART_INT_RT);
	//UART�жϵ�ַע��
	UARTIntRegister(UART1_BASE,UART1IntHandler);
}

void UART1IntHandler(void)
{
	static uint16_t data = 0;
	
	uint32_t flag = UARTIntStatus(UART1_BASE,1);
	UARTIntClear(UART1_BASE,flag);
	
	data = UARTCharGet(UART1_BASE);
	Openmv_Receive_Data(data);
}

void Openmv_Receive_Data(uint16_t data)//����Openmv������������
{
	static uint8_t state = 0;	
	static uint8_t bit_number=0;	
	if(state==0&&data==0xAD)	//��һλУ��λ0xAD
	{
		state=1;
		openmv[bit_number++]=data;
	}
	else if(state==1&&data==0x18)		//�ڶ�λУ��λ18
	{
		state=2;
		openmv[bit_number++]=data;
	}
	else if(state==2)		
	{
		openmv[bit_number++]=data;
		if(bit_number>=6)
		{
			state=3;
		}
	}
	else if(state==3)		//����Ƿ���ܵ�������־
	{
		if(data == 0x5B)	//���һλ��ֹλ0x5B
		{
					state = 0;
					openmv[bit_number++]=data;
			printf("%d,   %d,   %d,   %d, %d  %d\n", openmv[0],openmv[1],openmv[2],openmv[3],openmv[4],openmv[5]);
		}
		else if(data != 0x5B)
		{
				 state = 0;
					bit_number=0;						
		}
	}    
	else
	{
		state = 0;
		bit_number=0;
	}
}

//�ı�openmv��� �� ���ع�ϵ�ı���
void Openmv_Change_K( uint32_t openmv_k)
{
	printfff = 0;
	printf("abcd,%d,efgh\n",openmv_k);
	printfff = 1;
}


