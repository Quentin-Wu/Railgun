#include "delay.h"

//ʱ��Ƶ��Ϊ80Mhz

void delay_us(uint16_t xus)
{
	ROM_SysCtlDelay(xus*(80/3));
}


void delay_ms(uint16_t xms)
{
	ROM_SysCtlDelay(xms*(80000/3));
}

