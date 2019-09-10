#include "screen.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "control.h"
#include "openmv.h"
#include "mypwm.h"
#include "launch.h"
#include "delay.h"



extern uint16_t middle_x ;  //舵机中值
extern uint16_t middle_y ;

/*************************************************************************
**用的是uart2
**显示时间函数
**void sand_txt_time(uint16_t t1)				//t为0是开始   t为1是停止  t为2是停止清零
**显示距离
**void sand_distance(uint16_t d)		
**显示角度
**void sand_angle(uint16_t a)		
*/

int num_screen = 0;
uint8_t  startBit_Screen = 0;
uint8_t  endBit_Screen = 0;
uint16_t intpurscreenString[80] = {0};
uint16_t SccreenBit = 0;
int flag_read_op4;
extern uint8_t mode;
extern void mode_change(uint8_t modee);
extern void change_mode_distance(uint16_t mode_distancee);
extern void change_mode_angle_distance(int16_t mode_anglee, uint16_t mode_distancee);
extern uint16_t openmv[7];
extern void M_X_PWM(uint32_t width);
extern void M_Y_PWM(uint32_t width);
float angel_up_test ;
float angel_down_test ;


void txt_putbuff(char *buff, uint32_t len);
void set_long_angle(void);
uint16_t get_num(void);
void sand_middle_x(uint16_t d);
void sand_middle_y(uint16_t d);
void sand_k(uint16_t d)	;

//设置高度时间
typedef struct
{
	int r_long;
	double angle;
}Long_Angle;
Long_Angle long_angle;


//中断服务函数
void ScreenIntHandler(void)
{
		uint32_t flag = UARTIntStatus(SCREEN_GPIO_BASE,1);
		//清除中断标志
		UARTIntClear(SCREEN_GPIO_BASE,flag);

		SccreenBit =UARTCharGet(SCREEN_UART_BASE);	//读取接收到的数据

		
	
	//接受任务
		if(SccreenBit == '$' || SccreenBit =='^' || SccreenBit=='&' || SccreenBit=='t')
	    {
	      	startBit_Screen = 1;
					num_screen = 0;
	    }
	    if(startBit_Screen == 1)
	    {
	       	intpurscreenString[num_screen] = SccreenBit; 
					
	    }  
	    if (startBit_Screen == 1 && SccreenBit == '#') 
	    {		
					endBit_Screen = 1; 
					startBit_Screen = 0;
				
					
				if(intpurscreenString[0]=='$')
					{
							mode_change(intpurscreenString[1]-0x30);
							if(intpurscreenString[1]=='f')
							{
									Launch_Now(6000);
								delay_ms(2000);
								delay_ms(2000);
								delay_ms(2000);
							}
						
					}
					if(intpurscreenString[0]=='^')
					{
						uint16_t distance_2=set_distance();
						change_mode_distance(distance_2);
						
					}
					
					if(intpurscreenString[0]=='&')
					{
						set_long_angle();
						
						change_mode_angle_distance(long_angle.angle,long_angle.r_long);
					}
					
					if(intpurscreenString[0]=='t')
					{
						flag_read_op4=0;
						if(intpurscreenString[1]=='x')
						{

							M_X_PWM(get_num());
						}
						if(intpurscreenString[1]=='R')
						{
							middle_x=get_num();
							M_X_PWM(middle_x);
						}
						if(intpurscreenString[1]=='y')
						{
							 
							 M_Y_PWM(get_num());
						}
						if(intpurscreenString[1]=='r')
						{ 
							 middle_y=get_num();
						}
						if(intpurscreenString[1]=='k')
						{
							uint16_t kkkk;
							kkkk=get_num();
							Openmv_Change_K( kkkk);
								
						}
						if(intpurscreenString[1]=='K')
						{
							uint16_t kkkk;
							kkkk=get_num();
							Openmv_Change_K( kkkk);
								
						}
						
						if(intpurscreenString[1]=='o')
						{
							 	IntEnable(INT_UART1);
							  UARTIntEnable(UART1_BASE,UART_INT_RX |UART_INT_RT);
								flag_read_op4=1;
								printf("IntEnable(INT_UART1)");
								printf("flag_read_op4=%d",flag_read_op4);
								
						}
		        if(intpurscreenString[1]=='d')
						{
							 	IntDisable(INT_UART1);
								UARTIntDisable(UART1_BASE,UART_INT_RX |UART_INT_RT);
								flag_read_op4=0;
							  printf("IntDisable(INT_UART1)");
								printf("flag_read_op4=%d",flag_read_op4);
							  
						}
						if(intpurscreenString[1]=='s')
						{
							sand_middle_x(middle_x);
							sand_middle_y(middle_y);	
							IntEnable(INT_UART1);
							UARTIntEnable(UART1_BASE,UART_INT_RX |UART_INT_RT);
							sand_k(openmv[5]);
							printf("open(5)=%d",openmv[5]);
							IntDisable(INT_UART1);
							UARTIntDisable(UART1_BASE,UART_INT_RX |UART_INT_RT);
						}
					}else flag_read_op4=0;
	    }
			num_screen++;
			if(num_screen >= 80)
				{
					num_screen = 0;
					startBit_Screen = 0;
					endBit_Screen	= 0;
				}	 
}


//初始化
void Screen_Init(void)
{
	//使能UART外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SCREEN);
	//使能GPIO外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SCREEN_GPIO);
//	//GPIO复用模式配置
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0xFF;
	
	GPIOPinConfigure(SCREEN_GPIO_RX);
	GPIOPinConfigure(SCREEN_GPIO_TX);
	GPIOPinTypeUART(SCREEN_GPIO_BASE, SCREEN_GPIO_PIN_RX |SCREEN_GPIO_PIN_TX);
	//uart配置 波特率：115200 8位 1位停止位 无校验
	UARTConfigSetExpClk(SCREEN_UART_BASE, SysCtlClockGet(),
                    SCREEN_BAUD, (UART_CONFIG_WLEN_8 |UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	
	//UART禁用FIFO 默认FIFO Level为4/8 寄存器满8字节后产生中断
	//禁用后接收1位就产生中断
	UARTFIFODisable(SCREEN_GPIO_BASE);
	//使能UART0中断
	IntEnable(SCREEN_INT_UART);
	//使能UART0接收中断
	UARTIntEnable(SCREEN_UART_BASE,UART_INT_RX |UART_INT_RT);
	//UART中断地址注册
	UARTIntRegister(SCREEN_UART_BASE,ScreenIntHandler);
	
	IntPrioritySet(SCREEN_INT_UART, 0<<5);
	
	
}

//写数组
void txt_putbuff(char *buff, uint32_t len)
{
	while(len--)
	{
		UARTCharPut(UART2_BASE,*buff);
		UARTCharPut(UART0_BASE,*buff);
		buff++;
	}
}

//显示时间函数
void sand_txt_time(uint16_t t1)				//t1为1s t2为0.1s
{
	
	char txt[9]={0};
			 txt[0]='v';
			 txt[1]='a';
			 txt[2]='0';
			 txt[3]='.';
			 txt[4]='v';
			 txt[5]='a';
			 txt[6]='l';
			 txt[7]='=';
			 txt[8]=t1+48;

	double Hex_end[4]={0};
				 Hex_end[0]=0xff;
				 Hex_end[1]=0xff;
				 Hex_end[2]=0xff;
		txt_putbuff((char*) txt,9);	
		UARTCharPut(UART2_BASE,Hex_end[0]);
		UARTCharPut(UART2_BASE,Hex_end[1]);
		UARTCharPut(UART2_BASE,Hex_end[1]);

}

//显示x
void sand_middle_x(uint16_t d)				
{
	
	int d1=d/1000+48,
		  d2 =(d/100)%10+48,
			d3=(d/10)%10+48,
			d4=(d%10)+48;
	char txt[11]={0};
			 txt[0]='n';
			 txt[1]='0';
			 txt[2]='.';
			 txt[3]='v';
			 txt[4]='a';
			 txt[5]='l';
			 txt[6]='=';
			 txt[7]=d1;
			 txt[8]=d2;
			 txt[9]=d3;
			 txt[10]=d4;
	double Hex_end[4]={0};
				 Hex_end[0]=0xff;
				 Hex_end[1]=0xff;
				 Hex_end[2]=0xff;
		txt_putbuff((char*) txt,11);	
		UARTCharPut(UART2_BASE,Hex_end[0]);
		UARTCharPut(UART2_BASE,Hex_end[1]);
		UARTCharPut(UART2_BASE,Hex_end[1]);
}

//显示y
void sand_middle_y(uint16_t d)				//t1为1s t2为0.1s
{ 
	int d1=d/1000+48,
		  d2 =(d/100)%10+48,
			d3=(d/10)%10+48,
			d4=(d%10)+48;
	char txt[11]={0};
			 txt[0]='n';
			 txt[1]='1';
			 txt[2]='.';
			 txt[3]='v';
			 txt[4]='a';
			 txt[5]='l';
			 txt[6]='=';
			 txt[7]=d1;
			 txt[8]=d2;
			 txt[9]=d3;
			 txt[10]=d4;
	double Hex_end[4]={0};
				 Hex_end[0]=0xff;
				 Hex_end[1]=0xff;
				 Hex_end[2]=0xff;
		txt_putbuff((char*) txt,11);	
		UARTCharPut(UART2_BASE,Hex_end[0]);
		UARTCharPut(UART2_BASE,Hex_end[1]);
		UARTCharPut(UART2_BASE,Hex_end[1]);

}

//k
void sand_k(uint16_t d)				//t1为1s t2为0.1s
{ 
	int d1=d/1000+48,
		  d2 =(d/100)%10+48,
			d3=(d/10)%10+48,
			d4=(d%10)+48;
	char txt[11]={0};
			 txt[0]='n';
			 txt[1]='2';
			 txt[2]='.';
			 txt[3]='v';
			 txt[4]='a';
			 txt[5]='l';
			 txt[6]='=';
			 txt[7]=d1;
			 txt[8]=d2;
			 txt[9]=d3;
			 txt[10]=d4;
	double Hex_end[4]={0};
				 Hex_end[0]=0xff;
				 Hex_end[1]=0xff;
				 Hex_end[2]=0xff;
		txt_putbuff((char*) txt,11);	
		UARTCharPut(UART2_BASE,Hex_end[0]);
		UARTCharPut(UART2_BASE,Hex_end[1]);
		UARTCharPut(UART2_BASE,Hex_end[1]);

}
//显示距离
void sand_distance(uint16_t d)				
{
	int d1=d/100+48,
		  d2 =(d/10)%10+48,
			d3=d%10+48;		
	char txt[12]={0};
			 txt[0]='t';
			 txt[1]='4';
			 txt[2]='.';
			 txt[3]='t';
			 txt[4]='x';
			 txt[5]='t';
			 txt[6]='=';
			 txt[7]='"';
			 txt[8]=d1;
			 txt[9]=d2;
			 txt[10]=d3;
			 txt[11]='"';
	double Hex_end[4]={0};
				 Hex_end[0]=0xff;
				 Hex_end[1]=0xff;
				 Hex_end[2]=0xff;
		txt_putbuff((char*) txt,12);

		UARTCharPut(UART2_BASE,Hex_end[0]);	
		UARTCharPut(UART2_BASE,Hex_end[1]);
		UARTCharPut(UART2_BASE,Hex_end[1]);

}

//显示角度
void sand_angle(uint16_t a)				
{
	int a1=a/100+48,
		  a2 =(a/10)%10+48,
			a3=a%10+48;		
	char txt[12]={0};
			 txt[0]='t';
			 txt[1]='a';
			 txt[2]='.';
			 txt[3]='t';
			 txt[4]='x';
			 txt[5]='t';
			 txt[6]='=';
			 txt[7]='"';
			 txt[8]=a1;
			 txt[9]=a2;
			 txt[10]=a3;
			 txt[11]='"';
	double Hex_end[4]={0};
				 Hex_end[0]=0xff;
				 Hex_end[1]=0xff;
				 Hex_end[2]=0xff;
		txt_putbuff((char*) txt,12);	
		UARTCharPut(UART2_BASE,Hex_end[0]);	
		UARTCharPut(UART2_BASE,Hex_end[1]);
		UARTCharPut(UART2_BASE,Hex_end[1]);

}
//设置距离
uint16_t set_distance()
{
		int findend=1;
		double distance;
		for(;intpurscreenString[findend]!='#';findend++)
		{
			distance=distance*10+intpurscreenString[findend]-48;
			printf("circle_num=%f\n",distance);
		}
		printf("circle_num_end=%f\n",distance);
		return distance;
}

//设置角度+长度
void set_long_angle()
{

		int findend1=1;
	  int findend2=0;
  	long_angle.angle=0;
		long_angle.r_long=0;
	  int flag_fu;
		for(;intpurscreenString[findend1]!='+';findend1++)
		{
			if(intpurscreenString[1]=='-')
			{
					flag_fu=1;
					
			}
			long_angle.angle=long_angle.angle*10+intpurscreenString[findend1]-48;
		}
		if(flag_fu==1)
		{
				long_angle.angle=-(long_angle.angle+300);
		}

		findend2=findend1+1;
		for(;intpurscreenString[findend2]!='#';findend2++)
		{
			long_angle.r_long=long_angle.r_long*10+intpurscreenString[findend2]-48;
		}	
		
}

//设置炮弹
uint16_t set_paodan()
{
		int findend=1;
		double distance;
		for(;intpurscreenString[findend]!='#';findend++)
		{
			distance=distance*10+intpurscreenString[findend]-48;
			printf("circle_num=%f\n",distance);
		}
		printf("circle_num_end=%f\n",distance);
		return distance;
}

uint16_t get_num()
{
	  int num;
		num=intpurscreenString[2]+intpurscreenString[3]*256;
		return num;
			
}

////设置中值
//double set_center()
//{
//	int findend=2;
//	int flag=1;
//	double center_up=0;
//	double center_down=0;
//	double set_center=0;
//	if(intpurscreenString[1]=='u')
//	{
//			for(;intpurscreenString[findend]!='#';findend++)
//			{

//				if(flag==0)
//				{
//						center_up=center_up*10+intpurscreenString[findend]-48;
//						center_up=center_up/10;	
//				}	
//				if(intpurscreenString[findend]=='.')
//				{
//						flag=0;
//				}
//				if(flag==1)
//				{
//					center_up=center_up*10+intpurscreenString[findend]-48;
//					
//				}
//			}
//			set_center= center_up;
//			//printf("cnter_up_end=%f\n",center_up);
//	}
//	if(intpurscreenString[1]=='d')
//	{
//			for(;intpurscreenString[findend]!='#';findend++)
//			{

//				if(flag==0)
//				{
//						center_down=center_down*10+intpurscreenString[findend]-48;
//						center_down=center_down/10;
//				}	
//				if(intpurscreenString[findend]=='.')
//				{
//						flag=0;
//				}
//				if(flag==1)
//				{
//					center_down=center_down*10+intpurscreenString[findend]-48;
//				}
//			}
//			//printf("cnter_up=%f",center_up);
//			set_center= center_down;
//	}
//	return set_center;
//}

////显示上中值
//void show_center_up(float a)
//{
//	int b,t1,t2,t3,t4;
//	b=a*10;
//	t1=(b/1000)+48;
//	t2=(b/100)%10+48;
//	t3=(b/10)%10+48;
//	t4=(b%10)+48;
//		char center_up[14]={0};
//					center_up[0]='t';
//					center_up[1]='u';
//					center_up[2]='.';
//					center_up[3]='t';
//					center_up[4]='x';
//					center_up[5]='t';
//					center_up[6]='=';
//					center_up[7]='"';
//					center_up[8]=t1;
//					center_up[9]=t2;
//					center_up[10]=t3;
//					center_up[11]='.';
//					center_up[12]=t4;
//					center_up[13]='"';
//	double Hex_end[4]={0};
//				 Hex_end[0]=0xff;
//				 Hex_end[1]=0xff;
//				 Hex_end[2]=0xff;
//		txt_putbuff((char*) center_up,14);
//		UARTCharPut(UART0_BASE,Hex_end[0]);		
//		UARTCharPut(UART2_BASE,Hex_end[0]);
//		UARTCharPut(UART0_BASE,Hex_end[1]);		
//		UARTCharPut(UART2_BASE,Hex_end[1]);
//		UARTCharPut(UART0_BASE,Hex_end[2]);
//		UARTCharPut(UART2_BASE,Hex_end[1]);

//}

////显示下中值
//void show_center_down(float a)
//{
//	int b,t1,t2,t3,t4;
//	b=a*10;
//	t1=(b/1000)+48;
//	t2=(b/100)%10+48;
//	t3=(b/10)%10+48;
//	t4=(b%10)+48;
//	char  center_down[14]={0};
//				center_down[0]='t';
//				center_down[1]='d';
//				center_down[2]='.';
//				center_down[3]='t';
//				center_down[4]='x';
//				center_down[5]='t';
//				center_down[6]='=';
//				center_down[7]='"';
//				center_down[8]=t1;
//				center_down[9]=t2;
//				center_down[10]=t3;
//				center_down[11]='.';
//				center_down[12]=t4;
//				center_down[13]='"';
//	double Hex_end[4]={0};
//				 Hex_end[0]=0xff;
//				 Hex_end[1]=0xff;
//				 Hex_end[2]=0xff;
//		txt_putbuff((char*) center_down,14);
//		UARTCharPut(UART0_BASE,Hex_end[0]);		
//		UARTCharPut(UART2_BASE,Hex_end[0]);
//		UARTCharPut(UART0_BASE,Hex_end[1]);		
//		UARTCharPut(UART2_BASE,Hex_end[1]);
//		UARTCharPut(UART0_BASE,Hex_end[2]);
//		UARTCharPut(UART2_BASE,Hex_end[1]);

//}
