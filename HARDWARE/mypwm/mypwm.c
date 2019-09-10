#include "mypwm.h"
#include "myuart.h"
#include "math.h"

uint16_t middle_x = 1560;  //舵机中值
uint16_t middle_y = 1480;
float X_P = 10;         //舵机pwm与角度比例
float Y_P = 10;

void  M_X_Init(uint32_t freq, uint32_t width)
{
	//设置PWM时钟为系统时钟的8分频
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//使能PWM1外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_M1);
	
	//配置引脚为PWM功能
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM1);
	GPIOPinConfigure(GPIO_M1_M1);
	GPIOPinTypePWM(GPIO_BASE_M1, GPIO_PIN_M1);
	
	//配置 PWM1 Generator0·发生器
  PWMGenConfigure(PWM_BASE_M1, PWM_GEN_M1, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//配置 PWM1 Generator0 周期
  PWMGenPeriodSet(PWM_BASE_M1, PWM_GEN_M1, FREQ_PWM*freq - 1);  //100us
	//配置 PWM1 Generator0 占空比
  PWMPulseWidthSet(PWM_BASE_M1, PWM_OUT_M1, WIDTH_PWM*width - 1); //us
	
	//使能 PWM1 输出
  PWMOutputState(PWM_BASE_M1, PWM_OUT_BIT_M1, true);
  //使能 PWM1 发生器模块
  PWMGenEnable(PWM_BASE_M1, PWM_GEN_M1);
}

void  M_Y_Init(uint32_t freq, uint32_t width)
{
	//设置PWM时钟为系统时钟的8分频
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//使能PWM1外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_M2);
	
	//配置引脚为PWM功能
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM2);
	GPIOPinConfigure(GPIO_M2_M2);
	GPIOPinTypePWM(GPIO_BASE_M2, GPIO_PIN_M2);
	
	//配置 PWM1 Generator0·发生器
  PWMGenConfigure(PWM_BASE_M2, PWM_GEN_M2, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//配置 PWM1 Generator0 周期
  PWMGenPeriodSet(PWM_BASE_M2, PWM_GEN_M2, FREQ_PWM*freq - 1);  //100us
	//配置 PWM1 Generator0 占空比
  PWMPulseWidthSet(PWM_BASE_M2, PWM_OUT_M2, WIDTH_PWM*width - 1); //us
	
	//使能 PWM1 输出
  PWMOutputState(PWM_BASE_M2, PWM_OUT_BIT_M2, true);
  //使能 PWM1 发生器模块
  PWMGenEnable(PWM_BASE_M2, PWM_GEN_M2);
}

//带宽设置
/*************************************************************************************************************************************************/
void M_X_PWM(uint32_t width)
{
	 PWMPulseWidthSet(PWM_BASE_M1, PWM_OUT_M1, WIDTH_PWM*width- 1); //us
}

void M_Y_PWM(uint32_t width)
{
	 PWMPulseWidthSet(PWM_BASE_M2, PWM_OUT_M2, WIDTH_PWM*width - 1); //us
}

void MyPWM_Init(void)
{
	M_X_Init(20, middle_x);
  M_Y_Init(20, middle_y);
}

//输出角度
void M_X_Angle_Set(int16_t x_angle)
{
//	if(fabs(x_angle) >= 31) return;
	M_X_PWM(middle_x + x_angle * X_P);
	printf("%f\n",middle_x +x_angle * X_P);
}

void M_Y_Angle_Set(int16_t y_angle)
{
//	if(fabs(y_angle) >= 45) return;
	M_Y_PWM((middle_y + y_angle * Y_P));
	printf("%f\n", middle_y +y_angle * Y_P);
}

//输入pwm
//获取当前角度
float M_X_Angle_Get(uint32_t x_pwm)
{
	return (x_pwm - middle_x) / X_P ;
}

float M_Y_Angle_Get(uint32_t y_pwm)
{
	return (y_pwm - middle_y) / Y_P ;
}




