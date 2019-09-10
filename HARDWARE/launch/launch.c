#include "launch.h"
#include "led.h"
#include "myuart.h"
#include "delay.h"
#include "mypwm.h"
#include "math.h"
#include "screen.h"
//����ģ��
/***************************************************************************************************************************************************************************************************************************/
float Launch_Length_Kp = 0.5;  //û�õ�
bool Is_Launch = 0;

void Launch_TimerIntHandler(void);
void Charge_TimerIntHandler(void);

void Launch_Init(uint32_t xms)
{
	//��ʼ���̵���
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_Charge);
	GPIOPinTypeGPIOOutput(GPIO_PORT_Charge_BASE,GPIO_PIN_Charge);
	GPIOPadConfigSet(GPIO_PORT_Charge_BASE,GPIO_PIN_Charge,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_OD); 
	//�͵�ƽ���
	GPIOPinWrite(GPIO_PORT_Charge_BASE,GPIO_PIN_Charge, GPIO_PIN_Charge);
	
	//��ʼ���ŵ�
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_Launch);
	GPIOPinTypeGPIOOutput(GPIO_PORT_Launch_BASE,GPIO_PIN_Launch);
	GPIOPadConfigSet(GPIO_PORT_Launch_BASE,GPIO_PIN_Launch,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_OD); 
	//�ߵ�ƽ���
	GPIOPinWrite(GPIO_PORT_Launch_BASE,GPIO_PIN_Launch, GPIO_PIN_Launch);
	
	//��ʼ����ʱ��
	Launch_Timer_Init(20);
	Charge_Timer_Init(xms);
}

//���
void Charge_Timer_Init(uint32_t xms)
{		
	//ʹ������
	SysCtlPeripheralEnable(SYSCTL_PERIPH_Charge);
	//�����Լ���ģʽ
  TimerConfigure(Charge_Timer_BASE, TIMER_CFG_PERIODIC);
	
	//���ü�����Χ
	TimerLoadSet(Charge_Timer_BASE, TIMER_Charge_Timer, 80000*xms);
	
	//��ϵͳ��ʹ�ܶ�ʱ���ж�
	IntEnable(INT_TIMER_Charge_Timer);
	//������ĳ����ʱ�����ж�����
	TimerIntEnable(Charge_Timer_BASE, TIMER_Charge_Timer_TIMEOUT); 
	//�����ж����ȼ�
	IntPrioritySet(INT_TIMER_Charge_Timer, INT_TIMER_Charge_Timer);
	//ע���жϴ�����
	TimerIntRegister(Charge_Timer_BASE, TIMER_Charge_Timer, Charge_TimerIntHandler); 
	//�رն�ʱ��
  TimerDisable(Charge_Timer_BASE, TIMER_Charge_Timer);
//	TimerEnable(Launch_Timer_BASE, TIMER_Launch_Timer);
}

void Charge_TimerIntHandler(void)
{
	// �����ʱ���жϱ�־λ
	TimerIntClear(Charge_Timer_BASE, TIMER_Charge_Timer_TIMEOUT);
	LED_RED_TUGGEL;
	
	//�ߵ�ƽ���
	GPIOPinWrite(GPIO_PORT_Charge_BASE,GPIO_PIN_Charge, GPIO_PIN_Charge);
	//�رն�ʱ��
	TimerDisable(Charge_Timer_BASE, TIMER_Charge_Timer);
}

//��纯��
void Charge_Now(uint32_t xms)
{
	//���ü�����Χ
	TimerLoadSet(Charge_Timer_BASE, TIMER_Charge_Timer, 80000*xms);
	//�򿪶�ʱ��
	TimerEnable(Charge_Timer_BASE, TIMER_Charge_Timer);
	LED_RED_TUGGEL;
	//�͵�ƽ
	GPIOPinWrite(GPIO_PORT_Charge_BASE, GPIO_PIN_Charge, 0);
}

//����
/*************************************************************************************************************************************************************************************/
void Launch_Timer_Init(uint32_t xms)
{		
	//ʹ������
	SysCtlPeripheralEnable(SYSCTL_PERIPH_Launch_Timer);
	//�����Լ���ģʽ
  TimerConfigure(Launch_Timer_BASE, TIMER_CFG_PERIODIC);
	
	//���ü�����Χ
	TimerLoadSet(Launch_Timer_BASE, TIMER_Launch_Timer, 80000*xms);
	
	//��ϵͳ��ʹ�ܶ�ʱ���ж�
	IntEnable(INT_TIMER_Launch_Timer);
	//������ĳ����ʱ�����ж�����
	TimerIntEnable(Launch_Timer_BASE, TIMER_Launch_Timer_TIMEOUT); 
	//�����ж����ȼ�
	IntPrioritySet(INT_TIMER_Launch_Timer, 0);
	//ע���жϴ�����
	TimerIntRegister(Launch_Timer_BASE, TIMER_Launch_Timer, Launch_TimerIntHandler); 
	//�رն�ʱ��
  TimerDisable(Launch_Timer_BASE, TIMER_Launch_Timer);
//	TimerEnable(Launch_Timer_BASE, TIMER_Launch_Timer);
}

void Launch_TimerIntHandler(void)
{
	// �����ʱ���жϱ�־λ
	TimerIntClear(Launch_Timer_BASE, TIMER_Launch_Timer_TIMEOUT);
	LED_RED_TUGGEL;
	
	//�ŵ���� �͵�ƽ�Ͽ�
	GPIOPinWrite(GPIO_PORT_Launch_BASE,GPIO_PIN_Launch, GPIO_PIN_Launch);
	//�رն�ʱ��
	TimerDisable(Launch_Timer_BASE, TIMER_Launch_Timer);
}

//���亯��
void Launch_Now(uint32_t xms)
{
	//���ü�����Χ
	TimerLoadSet(Launch_Timer_BASE, TIMER_Launch_Timer, 80000*xms);
	//�򿪶�ʱ��
	TimerEnable(Launch_Timer_BASE, TIMER_Launch_Timer);
	LED_RED_TUGGEL;
	//�ߵ�ƽ ����
	GPIOPinWrite(GPIO_PORT_Launch_BASE,GPIO_PIN_Launch, 0);
}



//����һ�� �̶��Ƕȣ� ��������ʱ��
//�������
void Launch_Length(uint32_t cm)
{
		Launch_Now(cm * Launch_Length_Kp);
}

//�Զ�װ��ģ��
/*****************************************************************************************************************************************************************************************************************/
void Reload_Init(uint32_t xms)
{
	//����PWMʱ��Ϊϵͳʱ�ӵ�8��Ƶ
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//ʹ��PWM1����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_Reload);
	
	//��������ΪPWM����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOReload);
	GPIOPinConfigure(GPIO_Reload);
	GPIOPinTypePWM(GPIO_BASE_Reload, GPIO_PIN_Reload);
	
	//���� PWM1 Generator0��������
  PWMGenConfigure(PWM_BASE_Reload, PWM_GEN_Reload, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//���� PWM1 Generator0 ����
  PWMGenPeriodSet(PWM_BASE_Reload, PWM_GEN_Reload, 1250*20 - 1);  //100us
	//���� PWM1 Generator0 ռ�ձ�
  PWMPulseWidthSet(PWM_BASE_Reload, PWM_OUT_Reload, 1.25*xms - 1); //us
	
	//ʹ�� PWM1 ���
  PWMOutputState(PWM_BASE_Reload, PWM_OUT_BIT_Reload, true);
  //ʹ�� PWM1 ������ģ��
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
	sand_txt_time(1);          //��ʾʱ����ͣ
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















