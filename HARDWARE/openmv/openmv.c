#include "openmv.h"
#include "myuart.h"
#include "control.h"

uint16_t openmv[7]={0};

extern uint8_t printfff;   //默认发送给uart0
                           //当为0时发送给 uart1 openmv  实现控制距离测量 K
                           //发完数据后记得改回1

//RX    PC4
//TX    PC5

void UART1IntHandler(void);
//uart1
void Openmv_Init(void)
{
	//使能UART外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	//使能GPIO外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	//GPIO复用模式配置
	GPIOPinConfigure(GPIO_PC4_U1RX);
	GPIOPinConfigure(GPIO_PC5_U1TX);
	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 |GPIO_PIN_5);
	//uart配置 波特率：115200 8位 1位停止位 无校验
	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(),
                    115200, (UART_CONFIG_WLEN_8 |UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	
	//UART禁用FIFO 默认FIFO Level为4/8 寄存器满8字节后产生中断
	//禁用后接收1位就产生中断
	UARTFIFODisable(UART1_BASE);
	//使能UART0中断
//	IntEnable(INT_UART1);
	IntDisable(INT_UART1);
	//使能UART0接收中断
//	UARTIntEnable(UART1_BASE,UART_INT_RX |UART_INT_RT);
	UARTIntDisable(UART1_BASE,UART_INT_RX |UART_INT_RT);
	//UART中断地址注册
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

void Openmv_Receive_Data(uint16_t data)//接收Openmv传过来的数据
{
	static uint8_t state = 0;	
	static uint8_t bit_number=0;	
	if(state==0&&data==0xAD)	//第一位校验位0xAD
	{
		state=1;
		openmv[bit_number++]=data;
	}
	else if(state==1&&data==0x18)		//第二位校验位18
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
	else if(state==3)		//检测是否接受到结束标志
	{
		if(data == 0x5B)	//最后一位终止位0x5B
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

//改变openmv测距 与 像素关系的比例
void Openmv_Change_K( uint32_t openmv_k)
{
	printfff = 0;
	printf("abcd,%d,efgh\n",openmv_k);
	printfff = 1;
}


