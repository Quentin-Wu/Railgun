#ifndef __LAUNCH_H
#define __LAUNCH_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"


//继电器 PD3
//三极管基极 PD2
//Timer1
//PWM PA6


//控制口 充电
#define SYSCTL_PERIPH_GPIO_Charge			SYSCTL_PERIPH_GPIOD
#define GPIO_PORT_Charge_BASE			    GPIO_PORTD_BASE
#define GPIO_PIN_Charge      					GPIO_PIN_3

//控制口  放电
#define SYSCTL_PERIPH_GPIO_Launch			  SYSCTL_PERIPH_GPIOD
#define GPIO_PORT_Launch_BASE			 	    GPIO_PORTD_BASE
#define GPIO_PIN_Launch      				  	GPIO_PIN_2

//定时器
#define SYSCTL_PERIPH_Launch_Timer     	SYSCTL_PERIPH_TIMER1
#define Launch_Timer_BASE								TIMER1_BASE
#define TIMER_Launch_Timer  						TIMER_A
#define INT_TIMER_Launch_Timer 					INT_TIMER1A
#define TIMER_Launch_Timer_TIMEOUT			TIMER_TIMA_TIMEOUT

//定时器
#define SYSCTL_PERIPH_Charge     			SYSCTL_PERIPH_TIMER2
#define Charge_Timer_BASE							TIMER2_BASE
#define TIMER_Charge_Timer 						TIMER_A
#define INT_TIMER_Charge_Timer   			INT_TIMER2A
#define TIMER_Charge_Timer_TIMEOUT		TIMER_TIMA_TIMEOUT

//重装载PWM
#define GPIO_Reload									GPIO_PA6_M1PWM2
#define SYSCTL_PERIPH_Reload				SYSCTL_PERIPH_PWM1
#define SYSCTL_PERIPH_GPIOReload		SYSCTL_PERIPH_GPIOA
#define GPIO_BASE_Reload			 			GPIO_PORTA_BASE
#define GPIO_PIN_Reload							GPIO_PIN_6
#define PWM_BASE_Reload							PWM1_BASE
#define	PWM_GEN_Reload							PWM_GEN_1
#define PWM_OUT_Reload							PWM_OUT_2
#define PWM_OUT_BIT_Reload					PWM_OUT_2_BIT    

//初始化
void Launch_Init(uint32_t xms);          //发射模块初始化
void Launch_Timer_Init(uint32_t xms);    //发射定时器
void Charge_Timer_Init(uint32_t xms);   //充电定时器初始化

//重装载
void Reload_Init(uint32_t xms);
void Reload_Run(uint32_t width);
void Reload(void);

//发射函数
void Launch_Now(uint32_t xms);
void Launch_Length(uint32_t cm);

//充电函数
void Charge_Now(uint32_t xms);

//装载 充电  发射
void Reload_Charge_Launch(float angel);
void Reload_Charge_Launch_Length(uint16_t length);

void Length_Angel(int16_t angel, uint16_t length);


#endif


