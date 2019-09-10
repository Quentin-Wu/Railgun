#ifndef __MYPWM_H
#define __MYPWM_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"

/*******************************
 1, M1  ->PB4

 2, M2  ->PB5

*/

#define FREQ_PWM			1250       //ms
#define WIDTH_PWM     1.25			 //us

/*******************************************************************************************************************************************************************/
//四路pwm
#define GPIO_M1_M1							GPIO_PB4_M0PWM2
#define SYSCTL_PERIPH_M1				SYSCTL_PERIPH_PWM0
#define SYSCTL_PERIPH_GPIOM1		SYSCTL_PERIPH_GPIOB
#define GPIO_BASE_M1			 			GPIO_PORTB_BASE
#define GPIO_PIN_M1							GPIO_PIN_4
#define PWM_BASE_M1							PWM0_BASE
#define	PWM_GEN_M1							PWM_GEN_1
#define PWM_OUT_M1							PWM_OUT_2
#define PWM_OUT_BIT_M1					PWM_OUT_2_BIT       

#define GPIO_M2_M2							GPIO_PB5_M0PWM3
#define SYSCTL_PERIPH_M2				SYSCTL_PERIPH_PWM0
#define SYSCTL_PERIPH_GPIOM2		SYSCTL_PERIPH_GPIOB
#define GPIO_BASE_M2			 			GPIO_PORTB_BASE
#define GPIO_PIN_M2					 		GPIO_PIN_5
#define PWM_BASE_M2							PWM0_BASE
#define	PWM_GEN_M2							PWM_GEN_1
#define PWM_OUT_M2							PWM_OUT_3
#define PWM_OUT_BIT_M2					PWM_OUT_3_BIT    

/*******************************************************************************************************************************************************/
//初始化函数
void  MyPWM_Init(void);
void  M_X_Init(uint32_t freq, uint32_t width);
void  M_Y_Init(uint32_t freq, uint32_t width);

//PWM
void M_X_PWM(uint32_t width);
void M_Y_PWM(uint32_t width);

//设置角度输出
void M_X_Angle_Set(int16_t x_angle);
void M_Y_Angle_Set(int16_t y_angle);

//返回当前角度
float M_X_Angle_Get(uint32_t x_pwm);
float M_Y_Angle_Get(uint32_t y_pwm);



#endif

