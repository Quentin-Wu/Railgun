#include "myqei.h"

void Myqei_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
	
	GPIOPinConfigure(GPIO_PC5_PHA1);
	GPIOPinConfigure(GPIO_PC6_PHB1);
	GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5 |GPIO_PIN_6);
	
	//使用A,B通道4个边沿计算位置
	QEIConfigure(QEI1_BASE, (QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET|
								QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 1560-1);
	
	QEIEnable(QEI1_BASE);
//	QEIPositionSet(QEI0_BASE,800);  //设置中值
}

uint32_t Myqei_Position_Get(void)
{
	return (QEIPositionGet(QEI1_BASE));
}
















