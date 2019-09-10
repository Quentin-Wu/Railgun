#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"

#include "led.h"
#include "myuart.h"
#include "delay.h"
#include "usmart.h"

#include "mypwm.h"        //舵机PWM控制			X ->PB4   Y ->PB5
#include "launch.h"       //继电器 定时
#include "duo_zhuan.h"    //舵机左右转动     
#include "openmv.h"       //openmv相关       uart1   RX ->PC4        TX  ->PC5      与openmv 硬件链接 PC4 -> P4   PC5 -> P5
#include "control.h"
#include "screen.h"


extern uint8_t mode;							//任务标志位
extern uint16_t mode_distance;    //设置距离
extern int16_t mode_angle;				//设置角度
extern int flag_read_op4;
extern uint16_t openmv[7];
extern uint16_t intpurscreenString[80];
//改变任务的函数 只需调用一次
//1~~6          0对应暂停返回中间
void mode_change(uint8_t modee)
{
//	MyPWM_Init();
	openmv[4] = 0;
	mode_distance = 0;     
  mode_angle = 40;       //设置可能实现的初值  等待初值改变 才执行
	mode = modee;
	printf("mode = %d\n", mode);
}

//改变任务二的距离  
//任务二界面调用  只需调用一次
void change_mode_distance(uint16_t mode_distancee)
{
	mode_distance = mode_distancee;
	printf("mode_distance = %d\n", mode_distance);
}

//改变任务三的角度 及 距离  
//任务三界面调用  只需调用一次
void change_mode_angle_distance(int16_t mode_anglee, uint16_t mode_distancee)
{
	mode_angle = mode_anglee;
	mode_distance = mode_distancee;
	printf("mode_angle = %d , mode_distance = %d  \n", mode_angle, mode_distance);
}


int main(void)
{
	//配置系统时钟80Mhz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	LED_Init();
	My_Uart_Init();
	usmart_dev.init(80);		 //80MHz
	
	Launch_Init(0);  //继电器 PD2  三极管 PD4
	Openmv_Init();
	MyPWM_Init();
	Duo_Zhuan_Init(20);  //舵机X轴自动旋转  放在MyPWM_Init 后
	Reload_Init(1576);
	Screen_Init();
	while(1)
	{
		switch(mode)
		{
			case 0: mode_0();   break;
			case 1: mode_1();  	break;
			case 2: mode_2();  	break;
			case 3: mode_3();  	break;
			case 4: mode_up_1();  	break;
			case 5: mode_up_2();  	break;
			case 6: mode_up_3();  	break;
			case 7: mode_up_2_Init(); break;
			case 8: mode_up_22();  break;
			default:  break;
		}
		if(flag_read_op4==1)
		{
				sand_distance(openmv[4]);
			
		}
//				delay_ms(500);
//		
		
		

//		printf("%d ",intpurscreenString[0]);
//		printf("%d ",intpurscreenString[1]);
//		printf("%d ",intpurscreenString[2]);
//		printf("%d ",intpurscreenString[3]);	
//		printf("%d ",intpurscreenString[4]);
//		printf("%d ",intpurscreenString[5]);
//		printf("%d ",intpurscreenString[6]);
//		printf("%d ",intpurscreenString[7]);
//		printf("%d ",intpurscreenString[8]);
//		printf("%d\n ",intpurscreenString[9]);

	}
}


