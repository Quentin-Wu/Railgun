#include "hcsr04.h"
#include "delay.h"
#include "myuart.h"

//ʹ�ö�ʱ���ж�3
void Timer3IntHandler(void);

u16 msHcCount = 0;
u16 timer_init_count = 0;  //��������ֵ

//1ms
void My_Timer3_Init(void)
{		
	//ʹ������
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
	//�����Լ���ģʽ
  TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC);
	
	//���ü�����Χ
	TimerLoadSet(TIMER3_BASE, TIMER_A, (SysCtlClockGet()/1000)-1); //1ms
	
	//��ϵͳ��ʹ�ܶ�ʱ���ж�
	IntEnable(INT_TIMER3A);
	//������ĳ����ʱ�����ж�����
	TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT); 
	//�����ж����ȼ�
	IntPrioritySet(INT_TIMER3A, 3);
	//ע���жϴ�����
	TimerIntRegister(TIMER3_BASE, TIMER_BOTH, Timer3IntHandler); 
	//�رն�ʱ��
  TimerDisable(TIMER3_BASE, TIMER_A);
}

void Timer3IntHandler(void)
{
	msHcCount++;
	// �����ʱ���жϱ�־λ
	TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
}

void Hcsr04_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_TRIG);
	GPIOPinTypeGPIOOutput(GPIO_PORT_BASE_TRIG,GPIO_PIN_TRIG);
	GPIOPinWrite(GPIO_PORT_BASE_TRIG,GPIO_PIN_TRIG, 0);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_ECHO);
	GPIOPinTypeGPIOInput(GPIO_PORT_BASE_ECHO,GPIO_PIN_ECHO);
//	GPIOPadConfigSet(GPIO_PORT_BASE_ECHO, GPIO_PIN_ECHO , GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD); 
	
	My_Timer3_Init();
}

/************���ص㶨ʱ��******/
static void OpenTimerForHc(){
	msHcCount = 0;
	TimerLoadSet(TIMER3_BASE, TIMER_A, (SysCtlClockGet()/1000)-1);
	TimerEnable(TIMER3_BASE, TIMER_A);
	timer_init_count = TimerValueGet(TIMER3_BASE, TIMER_A) /80 ;
}

static void CloseTimerForHc(){
	TimerDisable(TIMER3_BASE, TIMER_A);
}

/********��ȡ��ʱ��ʱ��us********************/
u32 GetEchoTimer(void){
	u32 t = 0;
	t = msHcCount*1000; //�õ�us
//	printf("msHcCount: %d\n",msHcCount);
	t += (1000- (TimerValueGet(TIMER3_BASE, TIMER_A) /80)); //�õ�us
//	printf("TimerValue: %d\n",(1000- (TimerValueGet(TIMER3_BASE, TIMER_A) /80)));
//	t += timer_init_count;
//	printf("timer_init_count: %d\n",timer_init_count);
//	TimerLoadSet(TIMER3_BASE, TIMER_A, 0);
	delay_ms(50);
	msHcCount = 0;
	return t;
}

/*******���������*****/
float Hcsr04GetLength(void){
	u32 t= 0;
	double lengthTemp = 0;
	
	GPIOPinWrite(GPIO_PORT_BASE_TRIG,GPIO_PIN_TRIG, GPIO_PIN_TRIG); //���͸ߵ�ƽ
	delay_us(20);
	GPIOPinWrite(GPIO_PORT_BASE_TRIG,GPIO_PIN_TRIG, 0);
	
	while(GPIOPinRead(GPIO_PORT_BASE_ECHO, GPIO_PIN_ECHO) != GPIO_PIN_ECHO);
	OpenTimerForHc();
	while(GPIOPinRead(GPIO_PORT_BASE_ECHO, GPIO_PIN_ECHO) == GPIO_PIN_ECHO);
	CloseTimerForHc();
	
	t = GetEchoTimer();
	lengthTemp = (float)t*0.017;
//	printf("lengthTemp:%f\n\n",lengthTemp);

	return (float)lengthTemp;
}





