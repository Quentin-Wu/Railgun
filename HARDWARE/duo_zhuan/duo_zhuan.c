#include "duo_zhuan.h"
#include "mypwm.h" 
#include "led.h"

extern uint16_t middle_x;  //舵机中值
extern int16_t X_P ;         //舵机pwm与角度比例

static uint16_t Duo_Zhuan_Max ;   //30度
static uint16_t Duo_Zhuan_Min ;   //-30度
 uint8_t Duo_Zhuan_direction ;
uint16_t Duo_Zhuan_x = 1000 ;

//舵机自动转向
void Duo_ZhuanIntHandler(void);

void Duo_Zhuan_Init(uint32_t xms)
{		
	//使能外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_Duo_Zhuan);
	//周期性计数模式
  TimerConfigure(Duo_Zhuan_BASE, TIMER_CFG_PERIODIC);
	
	//设置计数范围
	TimerLoadSet(Duo_Zhuan_BASE, TIMER_Duo_Zhuan, 80000*xms);
	
	//在系统层使能定时器中断
	IntEnable(INT_TIMER_Duo_Zhuan);
	//允许处理某个定时器的中断请求
	TimerIntEnable(Duo_Zhuan_BASE, TIMER_Duo_Zhuan_TIMEOUT); 
	//设置中断优先级
	IntPrioritySet(INT_TIMER_Duo_Zhuan, 1<<5);
	//注册中断处理函数
	TimerIntRegister(Duo_Zhuan_BASE, TIMER_Duo_Zhuan, Duo_ZhuanIntHandler); 
	//关闭定时器
  TimerDisable(Duo_Zhuan_BASE, TIMER_Duo_Zhuan);
//	TimerEnable(Duo_Zhuan_BASE, TIMER_Duo_Zhuan);
	
	Duo_Zhuan_Max = middle_x + 30 * 10;   //30度
	Duo_Zhuan_Min = middle_x - 30 * 10;   //-30度
}


void Duo_ZhuanIntHandler(void)
{
	TimerIntClear(Duo_Zhuan_BASE, TIMER_Duo_Zhuan_TIMEOUT);
	LED_RED_TUGGEL;
	
	//如果小于min 开始增加
	if (Duo_Zhuan_x <= Duo_Zhuan_Min ) {Duo_Zhuan_direction = 0;  Duo_Zhuan_x = Duo_Zhuan_Min +1; }
	//如果小于max 开始减小
	if (Duo_Zhuan_x >= Duo_Zhuan_Max ) {Duo_Zhuan_direction = 1;  Duo_Zhuan_x = Duo_Zhuan_Max -1; }
	
	//如果为1 超过最大 减小
  if ( Duo_Zhuan_direction ) 
		Duo_Zhuan_x -= 4;
	else    //0 增大
		Duo_Zhuan_x += 4;
	
	M_X_PWM(Duo_Zhuan_x);
}
