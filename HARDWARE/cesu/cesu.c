#include  "cesu.h"
#include  "led.h"
#include  "math.h"

void WTimer2_A_Handler(void);  //PD0
void WTimer2_B_Handler(void);  //PD1
void WTimer3_A_Handler(void);  //PD2
void WTimer3_B_Handler(void);  //PD3

//PD0
void WTimer2_A_Init(void)
{
	// 启用WTimer2模块
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);
	// 启用GPIO_D作为脉冲捕捉脚
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	// 配置GPIO脚为使用WTimer2捕捉模式
	GPIOPinConfigure(GPIO_PD0_WT2CCP0);
	GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_0);
	
	// 为管脚配置弱上拉模式（捕获下降沿，配置为上拉）
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU); 
	
	 // 配置使用WTimer2的TimerA模块为边沿触发加计数模式
	TimerConfigure(WTIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_COUNT_UP |TIMER_CFG_B_CAP_COUNT_UP);
	// 使用双边触发
	TimerControlEvent(WTIMER2_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES);
	
	// 设置计数范围为16位
	TimerMatchSet(WTIMER2_BASE, TIMER_A, 0xffffffff-1);
	
	// 注册中断处理函数以响应触发事件
	TimerIntRegister(WTIMER2_BASE, TIMER_A, WTimer2_A_Handler); 
	
	 // 系统总中断开   
  IntMasterEnable();
	
	// 时钟中断允许，中断事件为Capture模式中边沿触发，计数到达预设值
	TimerIntEnable(WTIMER2_BASE, TIMER_CAPA_MATCH);
	
	// NVIC中允许定时器A模块中断   
  IntEnable(INT_WTIMER2A);
	// 启动捕捉模块 
	TimerEnable(WTIMER2_BASE, TIMER_A);
}

//PD1
void WTimer2_B_Init(void)
{
	// 启用WTimer2模块
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);
	// 启用GPIO_D作为脉冲捕捉脚
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	// 配置GPIO脚为使用WTimer2捕捉模式
	GPIOPinConfigure(GPIO_PD1_WT2CCP1);
	GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_1);
	
	// 为管脚配置弱上拉模式（捕获下降沿，配置为上拉）
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU); 
	
	// 配置使用Timer0的TimerB模块为边沿触发加计数模式 
  //	AB 同时使用时需同时配置 TimerConfigure有点小bug
	//

	//	TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_COUNT_UP);
	
	// 使用双边触发
	TimerControlEvent(WTIMER2_BASE, TIMER_B, TIMER_EVENT_BOTH_EDGES);

	// 设置计数范围为0~9
	TimerMatchSet(WTIMER2_BASE, TIMER_B, 0xffffffff-1);
	
	// 注册中断处理函数以响应触发事件
	TimerIntRegister(WTIMER2_BASE, TIMER_B, WTimer2_B_Handler); 
	
	 // 系统总中断开   
  IntMasterEnable();
	
	// 时钟中断允许，中断事件为Capture模式中边沿触发，计数到达预设值
	TimerIntEnable(WTIMER2_BASE, TIMER_CAPB_MATCH);
	
	// NVIC中允许定时器B模块中断   
  IntEnable(INT_WTIMER2B);
	// 启动捕捉模块 
	TimerEnable(WTIMER2_BASE, TIMER_B);
}

void WTimer2_A_Handler(void)
{
	TimerIntClear(WTIMER2_BASE, TIMER_CAPA_MATCH);
}

void WTimer2_B_Handler(void)
{
	TimerIntClear(WTIMER2_BASE, TIMER_CAPB_MATCH);
}

//PD2
void WTimer3_A_Init(void)
{
	// 启用WTimer3模块
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER3);
	// 启用GPIO_D作为脉冲捕捉脚
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	// 配置GPIO脚为使用WTimer3捕捉模式
	GPIOPinConfigure(GPIO_PD2_WT3CCP0);
	GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_2);
	
	// 为管脚配置弱上拉模式（捕获下降沿，配置为上拉）
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU); 
	
	 // 配置使用WTimer3的TimerA模块为边沿触发加计数模式
	TimerConfigure(WTIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_COUNT_UP |TIMER_CFG_B_CAP_COUNT_UP);
	// 使用双边触发
	TimerControlEvent(WTIMER3_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES);
	
	// 设置计数范围为0~9
	TimerMatchSet(WTIMER3_BASE, TIMER_A, 0xffffffff-1);
	
	// 注册中断处理函数以响应触发事件
	TimerIntRegister(WTIMER3_BASE, TIMER_A, WTimer3_A_Handler); 
	
	 // 系统总中断开   
  IntMasterEnable();
	
	// 时钟中断允许，中断事件为Capture模式中边沿触发，计数到达预设值
	TimerIntEnable(WTIMER3_BASE, TIMER_CAPA_MATCH);
	
	// NVIC中允许定时器A模块中断   
  IntEnable(INT_WTIMER3A);
	// 启动捕捉模块 
	TimerEnable(WTIMER3_BASE, TIMER_A);
}

//PD3
void WTimer3_B_Init(void)
{
	// 启用WTimer3模块
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER3);
	// 启用GPIO_D作为脉冲捕捉脚
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	// 配置GPIO脚为使用WTimer3捕捉模式
	GPIOPinConfigure(GPIO_PD3_WT3CCP1);
	GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_3);
	
	// 为管脚配置弱上拉模式（捕获下降沿，配置为上拉）
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU); 
	
	// 配置使用Timer0的TimerB模块为边沿触发加计数模式 
  //	AB 同时使用时需同时配置 TimerConfigure有点小bug
	//
//	TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_COUNT_UP);
	
	// 使用双边触发
	TimerControlEvent(WTIMER3_BASE, TIMER_B, TIMER_EVENT_BOTH_EDGES);

	// 设置计数范围为0~9
	TimerMatchSet(WTIMER3_BASE, TIMER_B, 0xffffffff-1);
	
	// 注册中断处理函数以响应触发事件
	TimerIntRegister(WTIMER3_BASE, TIMER_B, WTimer3_B_Handler); 
	
	 // 系统总中断开   
  IntMasterEnable();
	
	// 时钟中断允许，中断事件为Capture模式中边沿触发，计数到达预设值
	TimerIntEnable(WTIMER3_BASE, TIMER_CAPB_MATCH);
	
	// NVIC中允许定时器B模块中断   
  IntEnable(INT_WTIMER3B);
	// 启动捕捉模块 
	TimerEnable(WTIMER3_BASE, TIMER_B);
}

void WTimer3_A_Handler(void)
{
	TimerIntClear(WTIMER3_BASE, TIMER_CAPA_MATCH);
}

void WTimer3_B_Handler(void)
{
	TimerIntClear(WTIMER3_BASE, TIMER_CAPB_MATCH);
}


uint8_t M1_Speed_Get(void)
{
	static uint32_t satuation, speed, speed1=0;
	speed = TimerValueGet(WTIMER2_BASE, TIMER_A) - satuation;
	satuation = TimerValueGet(WTIMER2_BASE, TIMER_A);
  if (speed > 40 ) speed = speed1;
	speed1 = speed;
  return speed;
}

uint8_t M2_Speed_Get(void)
{
	static uint32_t satuation, speed, speed1=0;
	speed = TimerValueGet(WTIMER2_BASE, TIMER_B) - satuation;
	satuation = TimerValueGet(WTIMER2_BASE, TIMER_B);
  if (speed > 40 ) speed = speed1;
	speed1 = speed;
  return speed;
}

uint8_t M3_Speed_Get(void)
{
	static uint32_t satuation, speed, speed1=0;
	speed = TimerValueGet(WTIMER3_BASE, TIMER_A) - satuation;
	satuation = TimerValueGet(WTIMER3_BASE, TIMER_A);
  if (speed > 40 ) speed = speed1;
	speed1 = speed;
  return speed;
}

uint8_t M4_Speed_Get(void)
{
	static uint32_t satuation, speed, speed1=0;
	speed = TimerValueGet(WTIMER3_BASE, TIMER_A) - satuation;
	satuation = TimerValueGet(WTIMER3_BASE, TIMER_A);
  if (speed > 40 ) speed = speed1;
	speed1 = speed;
  return speed;
}



void Cesu_Init(void)
{
	WTimer2_A_Init();
	WTimer2_B_Init();
	WTimer3_A_Init();
	WTimer3_B_Init();
}


