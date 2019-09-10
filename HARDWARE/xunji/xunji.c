#include "xunji.h"

void Xunji_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_XUN1);
	GPIOPinTypeGPIOInput(GPIO_PORT_BASE_XUN1, GPIO_PIN_XUN1);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_XUN2);
	GPIOPinTypeGPIOInput(GPIO_PORT_BASE_XUN2, GPIO_PIN_XUN2);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_XUN3);
	GPIOPinTypeGPIOInput(GPIO_PORT_BASE_XUN3, GPIO_PIN_XUN3);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_XUN4);
	GPIOPinTypeGPIOInput(GPIO_PORT_BASE_XUN4, GPIO_PIN_XUN4);
}

uint8_t Xunji_Read(void)
{
	uint8_t state;
	state  = GPIOPinRead(GPIO_PORT_BASE_XUN1, GPIO_PIN_XUN1)>>YIWEI1;
	state |= ((GPIOPinRead(GPIO_PORT_BASE_XUN2, GPIO_PIN_XUN2))>>YIWEI2)<<1;
	state |= ((GPIOPinRead(GPIO_PORT_BASE_XUN3, GPIO_PIN_XUN3))>>YIWEI3)<<2;
	state |= ((GPIOPinRead(GPIO_PORT_BASE_XUN4, GPIO_PIN_XUN4))>>YIWEI4)<<3;
	return state;
}






















