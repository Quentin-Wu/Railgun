#include "duo_zhuan.h"
#include "mypwm.h" 
#include "led.h"

extern uint16_t middle_x;  //�����ֵ
extern int16_t X_P ;         //���pwm��Ƕȱ���

static uint16_t Duo_Zhuan_Max ;   //30��
static uint16_t Duo_Zhuan_Min ;   //-30��
 uint8_t Duo_Zhuan_direction ;
uint16_t Duo_Zhuan_x = 1000 ;

//����Զ�ת��
void Duo_ZhuanIntHandler(void);

void Duo_Zhuan_Init(uint32_t xms)
{		
	//ʹ������
	SysCtlPeripheralEnable(SYSCTL_PERIPH_Duo_Zhuan);
	//�����Լ���ģʽ
  TimerConfigure(Duo_Zhuan_BASE, TIMER_CFG_PERIODIC);
	
	//���ü�����Χ
	TimerLoadSet(Duo_Zhuan_BASE, TIMER_Duo_Zhuan, 80000*xms);
	
	//��ϵͳ��ʹ�ܶ�ʱ���ж�
	IntEnable(INT_TIMER_Duo_Zhuan);
	//������ĳ����ʱ�����ж�����
	TimerIntEnable(Duo_Zhuan_BASE, TIMER_Duo_Zhuan_TIMEOUT); 
	//�����ж����ȼ�
	IntPrioritySet(INT_TIMER_Duo_Zhuan, 1<<5);
	//ע���жϴ�����
	TimerIntRegister(Duo_Zhuan_BASE, TIMER_Duo_Zhuan, Duo_ZhuanIntHandler); 
	//�رն�ʱ��
  TimerDisable(Duo_Zhuan_BASE, TIMER_Duo_Zhuan);
//	TimerEnable(Duo_Zhuan_BASE, TIMER_Duo_Zhuan);
	
	Duo_Zhuan_Max = middle_x + 30 * 10;   //30��
	Duo_Zhuan_Min = middle_x - 30 * 10;   //-30��
}


void Duo_ZhuanIntHandler(void)
{
	TimerIntClear(Duo_Zhuan_BASE, TIMER_Duo_Zhuan_TIMEOUT);
	LED_RED_TUGGEL;
	
	//���С��min ��ʼ����
	if (Duo_Zhuan_x <= Duo_Zhuan_Min ) {Duo_Zhuan_direction = 0;  Duo_Zhuan_x = Duo_Zhuan_Min +1; }
	//���С��max ��ʼ��С
	if (Duo_Zhuan_x >= Duo_Zhuan_Max ) {Duo_Zhuan_direction = 1;  Duo_Zhuan_x = Duo_Zhuan_Max -1; }
	
	//���Ϊ1 ������� ��С
  if ( Duo_Zhuan_direction ) 
		Duo_Zhuan_x -= 4;
	else    //0 ����
		Duo_Zhuan_x += 4;
	
	M_X_PWM(Duo_Zhuan_x);
}
