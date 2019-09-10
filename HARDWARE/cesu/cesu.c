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
	// ����WTimer2ģ��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);
	// ����GPIO_D��Ϊ���岶׽��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	// ����GPIO��Ϊʹ��WTimer2��׽ģʽ
	GPIOPinConfigure(GPIO_PD0_WT2CCP0);
	GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_0);
	
	// Ϊ�ܽ�����������ģʽ�������½��أ�����Ϊ������
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU); 
	
	 // ����ʹ��WTimer2��TimerAģ��Ϊ���ش����Ӽ���ģʽ
	TimerConfigure(WTIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_COUNT_UP |TIMER_CFG_B_CAP_COUNT_UP);
	// ʹ��˫�ߴ���
	TimerControlEvent(WTIMER2_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES);
	
	// ���ü�����ΧΪ16λ
	TimerMatchSet(WTIMER2_BASE, TIMER_A, 0xffffffff-1);
	
	// ע���жϴ���������Ӧ�����¼�
	TimerIntRegister(WTIMER2_BASE, TIMER_A, WTimer2_A_Handler); 
	
	 // ϵͳ���жϿ�   
  IntMasterEnable();
	
	// ʱ���ж������ж��¼�ΪCaptureģʽ�б��ش�������������Ԥ��ֵ
	TimerIntEnable(WTIMER2_BASE, TIMER_CAPA_MATCH);
	
	// NVIC������ʱ��Aģ���ж�   
  IntEnable(INT_WTIMER2A);
	// ������׽ģ�� 
	TimerEnable(WTIMER2_BASE, TIMER_A);
}

//PD1
void WTimer2_B_Init(void)
{
	// ����WTimer2ģ��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);
	// ����GPIO_D��Ϊ���岶׽��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	// ����GPIO��Ϊʹ��WTimer2��׽ģʽ
	GPIOPinConfigure(GPIO_PD1_WT2CCP1);
	GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_1);
	
	// Ϊ�ܽ�����������ģʽ�������½��أ�����Ϊ������
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU); 
	
	// ����ʹ��Timer0��TimerBģ��Ϊ���ش����Ӽ���ģʽ 
  //	AB ͬʱʹ��ʱ��ͬʱ���� TimerConfigure�е�Сbug
	//

	//	TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_COUNT_UP);
	
	// ʹ��˫�ߴ���
	TimerControlEvent(WTIMER2_BASE, TIMER_B, TIMER_EVENT_BOTH_EDGES);

	// ���ü�����ΧΪ0~9
	TimerMatchSet(WTIMER2_BASE, TIMER_B, 0xffffffff-1);
	
	// ע���жϴ���������Ӧ�����¼�
	TimerIntRegister(WTIMER2_BASE, TIMER_B, WTimer2_B_Handler); 
	
	 // ϵͳ���жϿ�   
  IntMasterEnable();
	
	// ʱ���ж������ж��¼�ΪCaptureģʽ�б��ش�������������Ԥ��ֵ
	TimerIntEnable(WTIMER2_BASE, TIMER_CAPB_MATCH);
	
	// NVIC������ʱ��Bģ���ж�   
  IntEnable(INT_WTIMER2B);
	// ������׽ģ�� 
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
	// ����WTimer3ģ��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER3);
	// ����GPIO_D��Ϊ���岶׽��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	// ����GPIO��Ϊʹ��WTimer3��׽ģʽ
	GPIOPinConfigure(GPIO_PD2_WT3CCP0);
	GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_2);
	
	// Ϊ�ܽ�����������ģʽ�������½��أ�����Ϊ������
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU); 
	
	 // ����ʹ��WTimer3��TimerAģ��Ϊ���ش����Ӽ���ģʽ
	TimerConfigure(WTIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_COUNT_UP |TIMER_CFG_B_CAP_COUNT_UP);
	// ʹ��˫�ߴ���
	TimerControlEvent(WTIMER3_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES);
	
	// ���ü�����ΧΪ0~9
	TimerMatchSet(WTIMER3_BASE, TIMER_A, 0xffffffff-1);
	
	// ע���жϴ���������Ӧ�����¼�
	TimerIntRegister(WTIMER3_BASE, TIMER_A, WTimer3_A_Handler); 
	
	 // ϵͳ���жϿ�   
  IntMasterEnable();
	
	// ʱ���ж������ж��¼�ΪCaptureģʽ�б��ش�������������Ԥ��ֵ
	TimerIntEnable(WTIMER3_BASE, TIMER_CAPA_MATCH);
	
	// NVIC������ʱ��Aģ���ж�   
  IntEnable(INT_WTIMER3A);
	// ������׽ģ�� 
	TimerEnable(WTIMER3_BASE, TIMER_A);
}

//PD3
void WTimer3_B_Init(void)
{
	// ����WTimer3ģ��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER3);
	// ����GPIO_D��Ϊ���岶׽��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	// ����GPIO��Ϊʹ��WTimer3��׽ģʽ
	GPIOPinConfigure(GPIO_PD3_WT3CCP1);
	GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_3);
	
	// Ϊ�ܽ�����������ģʽ�������½��أ�����Ϊ������
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU); 
	
	// ����ʹ��Timer0��TimerBģ��Ϊ���ش����Ӽ���ģʽ 
  //	AB ͬʱʹ��ʱ��ͬʱ���� TimerConfigure�е�Сbug
	//
//	TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_COUNT_UP);
	
	// ʹ��˫�ߴ���
	TimerControlEvent(WTIMER3_BASE, TIMER_B, TIMER_EVENT_BOTH_EDGES);

	// ���ü�����ΧΪ0~9
	TimerMatchSet(WTIMER3_BASE, TIMER_B, 0xffffffff-1);
	
	// ע���жϴ���������Ӧ�����¼�
	TimerIntRegister(WTIMER3_BASE, TIMER_B, WTimer3_B_Handler); 
	
	 // ϵͳ���жϿ�   
  IntMasterEnable();
	
	// ʱ���ж������ж��¼�ΪCaptureģʽ�б��ش�������������Ԥ��ֵ
	TimerIntEnable(WTIMER3_BASE, TIMER_CAPB_MATCH);
	
	// NVIC������ʱ��Bģ���ж�   
  IntEnable(INT_WTIMER3B);
	// ������׽ģ�� 
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


