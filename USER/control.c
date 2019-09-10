#include "control.h"
#include "mypwm.h"
#include "myuart.h"
#include "led.h"
#include "launch.h"
#include "delay.h"
#include "duo_zhuan.h"
#include "openmv.h"
#include "screen.h"


extern uint8_t Duo_Zhuan_direction ;
extern uint16_t Duo_Zhuan_x;
extern uint16_t openmv[7];

uint8_t mode = 0;   //�����־λ
extern uint16_t middle_x ;  //�����ֵ
extern uint16_t middle_y ;

uint16_t mode_distance = 0;     
int16_t mode_angle = 40;       //���ÿ���ʵ�ֵĳ�ֵ  �ȴ���ֵ�ı� ��ִ��

extern uint16_t Duo_Zhuan_x;   //�������Ѱ��ʱ�� ������׼���� x ��Ӧ�� pwm

void mode_0(void)
{
	M_X_PWM(middle_x);
	M_Y_PWM(middle_y);
}
//������ܹ�����������ڿ�
void mode_1(void)
{
	sand_txt_time(2);          //��ʾʱ������
	sand_txt_time(0);          //��ʾʱ�俪ʼ
	Reload_Charge_Launch_Length(250);
	mode = 0;                   //������� �л�mode Ϊ��
}


void mode_2(void)
{
	M_X_PWM(middle_x);														//������xλ������
	while( mode_distance == 0 );    							//�ȴ����þ��벻Ϊ0
	sand_txt_time(2);          //��ʾʱ������
	sand_txt_time(0);          //��ʾʱ�俪ʼ
	Reload_Charge_Launch_Length(mode_distance); 	//���÷���̶����Ⱥ���
	sand_txt_time(1);          //��ʾʱ����ͣ
	mode = 0;										 							    //������� �л�mode Ϊ��
}

void mode_3(void)
{
	while( mode_distance == 0 || mode_angle == 40 );    //�ȴ����þ��벻Ϊ0 �� �ǶȲ�Ϊ40
	sand_txt_time(2);          //��ʾʱ������
	sand_txt_time(0);          //��ʾʱ�俪ʼ
	Length_Angel(mode_angle, mode_distance);						//���÷���̶����Ⱥ��� �ͽǶ�
	sand_txt_time(1);          //��ʾʱ����ͣ
	mode = 0;										    										//������� �л�mode Ϊ��
}


void mode_up_1(void)
{
	sand_txt_time(2);          //��ʾʱ������
	sand_txt_time(0);          //��ʾʱ�俪ʼ
	TimerEnable(Duo_Zhuan_BASE, TIMER_Duo_Zhuan);       //�ȴ�x��ת��
	
	IntEnable(INT_UART1);
	UARTIntEnable(UART1_BASE,UART_INT_RX |UART_INT_RT);

	while(openmv[2] != 140 && openmv[2] != 141 && openmv[2] != 139 &&  openmv[2] != 142 && openmv[2] != 138 || openmv[4] ==0);
	
	IntDisable(INT_UART1);
	UARTIntDisable(UART1_BASE,UART_INT_RX |UART_INT_RT); //�ر�openmv�ж�
	TimerDisable(Duo_Zhuan_BASE, TIMER_Duo_Zhuan);       //�ر�x��ת��
	
	printf("%d\n",openmv[4] + 70);

	Length_Angel(M_X_Angle_Get(Duo_Zhuan_x), openmv[4] + 70);						//���÷���̶����Ⱥ��� �ͽǶ�
	
	mode = 0;
}

void mode_up_2(void)
{
	Duo_Zhuan_x = 1000 ;
	M_Y_Angle_Set(2);
	TimerEnable(Duo_Zhuan_BASE, TIMER_Duo_Zhuan);       //�ȴ�x��ת��

	Reload();					//װ��
	Charge_Now(3000); //���
	
	delay_ms(3000);
	
	sand_txt_time(2);          //��ʾʱ������
	sand_txt_time(0);          //��ʾʱ�俪ʼ
	IntEnable(INT_UART1);
	UARTIntEnable(UART1_BASE,UART_INT_RX |UART_INT_RT);

	while(openmv[2] != 200 && openmv[2] != 201 && openmv[2] !=202  && openmv[2] != 203 && openmv[2] != 204);
//	M_Y_Angle_Set(25);
	M_Y_Angle_Set(20);
	delay_ms(500);
	IntDisable(INT_UART1);
	UARTIntDisable(UART1_BASE,UART_INT_RX |UART_INT_RT);  //�ر�openmv�ж�
	
	sand_txt_time(1);          //��ʾʱ����ͣ
	Launch_Now(6000); //����
	delay_ms(2000);  //��ʱ2s
	delay_ms(4000);
	delay_ms(1000);
	mode = 0;
}

void mode_up_3(void)
{
	 M_Y_Angle_Set(30);
	for(int i = 0; i<=10; i++)
	{
		Reload();
		delay_ms(100);
		Charge_Now(1000);
		delay_ms(1100);
		Launch_Now(1000);
		delay_ms(1100);
	}
}

//
void mode_up_2_Init(void)
{
	M_X_Angle_Set(-30);
	M_Y_Angle_Set(0);
}

void mode_up_22(void)
{
	Duo_Zhuan_x = 1000 ;
	M_Y_Angle_Set(2);
	TimerEnable(Duo_Zhuan_BASE, TIMER_Duo_Zhuan);       //�ȴ�x��ת��

	Reload();					//װ��
	Charge_Now(3000); //���
	
	delay_ms(3000);
	
	sand_txt_time(2);          //��ʾʱ������
	sand_txt_time(0);          //��ʾʱ�俪ʼ
	IntEnable(INT_UART1);
	UARTIntEnable(UART1_BASE,UART_INT_RX |UART_INT_RT);

	while(Duo_Zhuan_direction ==1 );

	while(openmv[2] != 90 && openmv[2] != 91 && openmv[2] !=92  && openmv[2] != 93 && openmv[2] != 94);
//	M_Y_Angle_Set(25);
	M_Y_Angle_Set(20);
	delay_ms(500);
	IntDisable(INT_UART1);
	UARTIntDisable(UART1_BASE,UART_INT_RX |UART_INT_RT);  //�ر�openmv�ж�
	
	sand_txt_time(1);          //��ʾʱ����ͣ
	Launch_Now(6000); //����
	delay_ms(2000);  //��ʱ2s
	delay_ms(4000);
	delay_ms(1000);
	mode = 0;
}







