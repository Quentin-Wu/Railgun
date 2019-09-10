#include "mypwm.h"
#include "myuart.h"
#include "math.h"

uint16_t middle_x = 1560;  //�����ֵ
uint16_t middle_y = 1480;
float X_P = 10;         //���pwm��Ƕȱ���
float Y_P = 10;

void  M_X_Init(uint32_t freq, uint32_t width)
{
	//����PWMʱ��Ϊϵͳʱ�ӵ�8��Ƶ
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//ʹ��PWM1����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_M1);
	
	//��������ΪPWM����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM1);
	GPIOPinConfigure(GPIO_M1_M1);
	GPIOPinTypePWM(GPIO_BASE_M1, GPIO_PIN_M1);
	
	//���� PWM1 Generator0��������
  PWMGenConfigure(PWM_BASE_M1, PWM_GEN_M1, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//���� PWM1 Generator0 ����
  PWMGenPeriodSet(PWM_BASE_M1, PWM_GEN_M1, FREQ_PWM*freq - 1);  //100us
	//���� PWM1 Generator0 ռ�ձ�
  PWMPulseWidthSet(PWM_BASE_M1, PWM_OUT_M1, WIDTH_PWM*width - 1); //us
	
	//ʹ�� PWM1 ���
  PWMOutputState(PWM_BASE_M1, PWM_OUT_BIT_M1, true);
  //ʹ�� PWM1 ������ģ��
  PWMGenEnable(PWM_BASE_M1, PWM_GEN_M1);
}

void  M_Y_Init(uint32_t freq, uint32_t width)
{
	//����PWMʱ��Ϊϵͳʱ�ӵ�8��Ƶ
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//ʹ��PWM1����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_M2);
	
	//��������ΪPWM����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM2);
	GPIOPinConfigure(GPIO_M2_M2);
	GPIOPinTypePWM(GPIO_BASE_M2, GPIO_PIN_M2);
	
	//���� PWM1 Generator0��������
  PWMGenConfigure(PWM_BASE_M2, PWM_GEN_M2, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//���� PWM1 Generator0 ����
  PWMGenPeriodSet(PWM_BASE_M2, PWM_GEN_M2, FREQ_PWM*freq - 1);  //100us
	//���� PWM1 Generator0 ռ�ձ�
  PWMPulseWidthSet(PWM_BASE_M2, PWM_OUT_M2, WIDTH_PWM*width - 1); //us
	
	//ʹ�� PWM1 ���
  PWMOutputState(PWM_BASE_M2, PWM_OUT_BIT_M2, true);
  //ʹ�� PWM1 ������ģ��
  PWMGenEnable(PWM_BASE_M2, PWM_GEN_M2);
}

//��������
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

//����Ƕ�
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

//����pwm
//��ȡ��ǰ�Ƕ�
float M_X_Angle_Get(uint32_t x_pwm)
{
	return (x_pwm - middle_x) / X_P ;
}

float M_Y_Angle_Get(uint32_t y_pwm)
{
	return (y_pwm - middle_y) / Y_P ;
}




