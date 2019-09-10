#include "launch.h"
#include "led.h"
#include "myuart.h"
#include "delay.h"
#include "mypwm.h"
#include "math.h"
#include "screen.h"
//发射模块
/***************************************************************************************************************************************************************************************************************************/
float Launch_Length_Kp = 0.5;  //没用得
bool Is_Launch = 0;

void Launch_TimerIntHandler(void);
void Charge_TimerIntHandler(void);

void Launch_Init(uint32_t xms)
{
	//初始化继电器
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_Charge);
	GPIOPinTypeGPIOOutput(GPIO_PORT_Charge_BASE,GPIO_PIN_Charge);
	GPIOPadConfigSet(GPIO_PORT_Charge_BASE,GPIO_PIN_Charge,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_OD); 
	//低电平输出
	GPIOPinWrite(GPIO_PORT_Charge_BASE,GPIO_PIN_Charge, GPIO_PIN_Charge);
	
	//初始化放电
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_Launch);
	GPIOPinTypeGPIOOutput(GPIO_PORT_Launch_BASE,GPIO_PIN_Launch);
	GPIOPadConfigSet(GPIO_PORT_Launch_BASE,GPIO_PIN_Launch,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_OD); 
	//高电平输出
	GPIOPinWrite(GPIO_PORT_Launch_BASE,GPIO_PIN_Launch, GPIO_PIN_Launch);
	
	//初始化定时器
	Launch_Timer_Init(20);
	Charge_Timer_Init(xms);
}

//充电
void Charge_Timer_Init(uint32_t xms)
{		
	//使能外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_Charge);
	//周期性计数模式
  TimerConfigure(Charge_Timer_BASE, TIMER_CFG_PERIODIC);
	
	//设置计数范围
	TimerLoadSet(Charge_Timer_BASE, TIMER_Charge_Timer, 80000*xms);
	
	//在系统层使能定时器中断
	IntEnable(INT_TIMER_Charge_Timer);
	//允许处理某个定时器的中断请求
	TimerIntEnable(Charge_Timer_BASE, TIMER_Charge_Timer_TIMEOUT); 
	//设置中断优先级
	IntPrioritySet(INT_TIMER_Charge_Timer, INT_TIMER_Charge_Timer);
	//注册中断处理函数
	TimerIntRegister(Charge_Timer_BASE, TIMER_Charge_Timer, Charge_TimerIntHandler); 
	//关闭定时器
  TimerDisable(Charge_Timer_BASE, TIMER_Charge_Timer);
//	TimerEnable(Launch_Timer_BASE, TIMER_Launch_Timer);
}

void Charge_TimerIntHandler(void)
{
	// 清除定时器中断标志位
	TimerIntClear(Charge_Timer_BASE, TIMER_Charge_Timer_TIMEOUT);
	LED_RED_TUGGEL;
	
	//高电平输出
	GPIOPinWrite(GPIO_PORT_Charge_BASE,GPIO_PIN_Charge, GPIO_PIN_Charge);
	//关闭定时器
	TimerDisable(Charge_Timer_BASE, TIMER_Charge_Timer);
}

//充电函数
void Charge_Now(uint32_t xms)
{
	//设置计数范围
	TimerLoadSet(Charge_Timer_BASE, TIMER_Charge_Timer, 80000*xms);
	//打开定时器
	TimerEnable(Charge_Timer_BASE, TIMER_Charge_Timer);
	LED_RED_TUGGEL;
	//低电平
	GPIOPinWrite(GPIO_PORT_Charge_BASE, GPIO_PIN_Charge, 0);
}

//发射
/*************************************************************************************************************************************************************************************/
void Launch_Timer_Init(uint32_t xms)
{		
	//使能外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_Launch_Timer);
	//周期性计数模式
  TimerConfigure(Launch_Timer_BASE, TIMER_CFG_PERIODIC);
	
	//设置计数范围
	TimerLoadSet(Launch_Timer_BASE, TIMER_Launch_Timer, 80000*xms);
	
	//在系统层使能定时器中断
	IntEnable(INT_TIMER_Launch_Timer);
	//允许处理某个定时器的中断请求
	TimerIntEnable(Launch_Timer_BASE, TIMER_Launch_Timer_TIMEOUT); 
	//设置中断优先级
	IntPrioritySet(INT_TIMER_Launch_Timer, 0);
	//注册中断处理函数
	TimerIntRegister(Launch_Timer_BASE, TIMER_Launch_Timer, Launch_TimerIntHandler); 
	//关闭定时器
  TimerDisable(Launch_Timer_BASE, TIMER_Launch_Timer);
//	TimerEnable(Launch_Timer_BASE, TIMER_Launch_Timer);
}

void Launch_TimerIntHandler(void)
{
	// 清除定时器中断标志位
	TimerIntClear(Launch_Timer_BASE, TIMER_Launch_Timer_TIMEOUT);
	LED_RED_TUGGEL;
	
	//放电完成 低电平断开
	GPIOPinWrite(GPIO_PORT_Launch_BASE,GPIO_PIN_Launch, GPIO_PIN_Launch);
	//关闭定时器
	TimerDisable(Launch_Timer_BASE, TIMER_Launch_Timer);
}

//发射函数
void Launch_Now(uint32_t xms)
{
	//设置计数范围
	TimerLoadSet(Launch_Timer_BASE, TIMER_Launch_Timer, 80000*xms);
	//打开定时器
	TimerEnable(Launch_Timer_BASE, TIMER_Launch_Timer);
	LED_RED_TUGGEL;
	//高电平 触发
	GPIOPinWrite(GPIO_PORT_Launch_BASE,GPIO_PIN_Launch, 0);
}



//方案一： 固定角度， 调整发射时间
//发射距离
void Launch_Length(uint32_t cm)
{
		Launch_Now(cm * Launch_Length_Kp);
}

//自动装载模块
/*****************************************************************************************************************************************************************************************************************/
void Reload_Init(uint32_t xms)
{
	//设置PWM时钟为系统时钟的8分频
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//使能PWM1外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_Reload);
	
	//配置引脚为PWM功能
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOReload);
	GPIOPinConfigure(GPIO_Reload);
	GPIOPinTypePWM(GPIO_BASE_Reload, GPIO_PIN_Reload);
	
	//配置 PWM1 Generator0·发生器
  PWMGenConfigure(PWM_BASE_Reload, PWM_GEN_Reload, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//配置 PWM1 Generator0 周期
  PWMGenPeriodSet(PWM_BASE_Reload, PWM_GEN_Reload, 1250*20 - 1);  //100us
	//配置 PWM1 Generator0 占空比
  PWMPulseWidthSet(PWM_BASE_Reload, PWM_OUT_Reload, 1.25*xms - 1); //us
	
	//使能 PWM1 输出
  PWMOutputState(PWM_BASE_Reload, PWM_OUT_BIT_Reload, true);
  //使能 PWM1 发生器模块
  PWMGenEnable(PWM_BASE_Reload, PWM_GEN_Reload);
}

void Reload_Run(uint32_t width)
{
	PWMPulseWidthSet(PWM_BASE_Reload, PWM_OUT_Reload, 1.25*width - 1);
}

void Reload(void)
{
	Reload_Run(1576);
	delay_ms(150);
	Reload_Run(1300);
	delay_ms(150);
	Reload_Run(1576);
}

void Reload_Charge_Launch(float angel)
{
	if( fabs(angel) >= 45 ) return;
	M_Y_Angle_Set(angel);
		Reload();
		delay_ms(100);
		Charge_Now(3000);
		delay_ms(3100);
	Is_Launch = 1;
		Launch_Now(3000);
	sand_txt_time(1);          //显示时间暂停
		delay_ms(3000);
}

void Reload_Charge_Launch_Length(uint16_t length)
{
	uint16_t angel = (length-200) * 0.2 +10;
	Reload_Charge_Launch(angel);
}

void Length_Angel(int16_t angel, uint16_t length)
{
	M_X_Angle_Set(angel);
	Reload_Charge_Launch_Length(length);
	printf("angel %d,   length %d", angel,length);
}















