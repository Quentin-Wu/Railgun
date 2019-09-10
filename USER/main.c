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

#include "mypwm.h"        //���PWM����			X ->PB4   Y ->PB5
#include "launch.h"       //�̵��� ��ʱ
#include "duo_zhuan.h"    //�������ת��     
#include "openmv.h"       //openmv���       uart1   RX ->PC4        TX  ->PC5      ��openmv Ӳ������ PC4 -> P4   PC5 -> P5
#include "control.h"
#include "screen.h"


extern uint8_t mode;							//�����־λ
extern uint16_t mode_distance;    //���þ���
extern int16_t mode_angle;				//���ýǶ�
extern int flag_read_op4;
extern uint16_t openmv[7];
extern uint16_t intpurscreenString[80];
//�ı�����ĺ��� ֻ�����һ��
//1~~6          0��Ӧ��ͣ�����м�
void mode_change(uint8_t modee)
{
//	MyPWM_Init();
	openmv[4] = 0;
	mode_distance = 0;     
  mode_angle = 40;       //���ÿ���ʵ�ֵĳ�ֵ  �ȴ���ֵ�ı� ��ִ��
	mode = modee;
	printf("mode = %d\n", mode);
}

//�ı�������ľ���  
//������������  ֻ�����һ��
void change_mode_distance(uint16_t mode_distancee)
{
	mode_distance = mode_distancee;
	printf("mode_distance = %d\n", mode_distance);
}

//�ı��������ĽǶ� �� ����  
//�������������  ֻ�����һ��
void change_mode_angle_distance(int16_t mode_anglee, uint16_t mode_distancee)
{
	mode_angle = mode_anglee;
	mode_distance = mode_distancee;
	printf("mode_angle = %d , mode_distance = %d  \n", mode_angle, mode_distance);
}


int main(void)
{
	//����ϵͳʱ��80Mhz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	LED_Init();
	My_Uart_Init();
	usmart_dev.init(80);		 //80MHz
	
	Launch_Init(0);  //�̵��� PD2  ������ PD4
	Openmv_Init();
	MyPWM_Init();
	Duo_Zhuan_Init(20);  //���X���Զ���ת  ����MyPWM_Init ��
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


