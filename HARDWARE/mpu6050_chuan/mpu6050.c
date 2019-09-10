#include "mpu6050.h"
#include "myuart.h"
#include "led.h"

unsigned char counter=0;
static unsigned char Temp[11];
float a[3],w[3],angle[3],T;

void UART1IntHandler(void);

 float warebuf[8];

//使用uart1
//RX  PB0
//TX  PB1
void MPU6050_Init(void)
{
	//使能UART外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	//使能GPIO外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	//GPIO复用模式配置
	GPIOPinConfigure(GPIO_PC4_U1RX);
	GPIOPinConfigure(GPIO_PC5_U1TX);
	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 |GPIO_PIN_5);
	//uart配置 波特率：115200 8位 1位停止位 无校验
	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(),
                    115200, (UART_CONFIG_WLEN_8 |UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	
	//UART禁用FIFO 默认FIFO Level为4/8 寄存器满8字节后产生中断
	//禁用后接收1位就产生中断
	UARTFIFODisable(UART1_BASE);
	//使能UART1中断
	IntEnable(INT_UART1);
	//使能UART1接收中断
	UARTIntEnable(UART1_BASE,UART_INT_RX |UART_INT_RT);
	//UART中断地址注册
	UARTIntRegister(UART1_BASE,UART1IntHandler);
	//设置中断优先级
	IntPrioritySet(INT_UART1, 0<<5);
}


void UART1IntHandler(void)
{
	uint32_t flag = UARTIntStatus(UART1_BASE,1);
	UARTIntClear(UART1_BASE,flag);
	
	Temp[counter] =UARTCharGet(UART1_BASE);	  //读取接收到的数据
	
	if(counter == 0 && Temp[0] != 0x55)  return;    //第 0 号数据不是帧头，跳过
//	if(counter == 1 && Temp[1] != 0x53)  return;  //第1号数据不为角度包，跳过
	counter++; 
	if(counter==11) //接收到 11 个数据
	{ 
		 counter=0; //重新赋值，准备下一帧数据的接收
		if(Temp[1] == 0x53 || Temp[1] == 0x52)
		{
			if(Temp[1] == 0x53) //标识这个包是角速度包
			{
				angle[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*180;   //X轴滚转角（x 轴）
				angle[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*180;   //Y轴俯仰角（y 轴）
				angle[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*180;   //Z轴偏航角（z 轴）
				
				warebuf[0] = angle[0];
				warebuf[1] = angle[1];
				vcan_sendware((uint8_t *)warebuf,sizeof(warebuf));
							LED_GREEN_TUGGEL;
			}
			else if(Temp[1] == 0x52) //标识这个包是角度包
			{
				w[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*2000;      //X轴角速度
				w[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*2000;      //Y轴角速度
				w[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*2000;      //Z轴角速度
			}
//			printf("X角度：%.2f  Y角度：%.2f  Z角度：%.2f  X速度：%.2f  Y速度：%.2f  Z速度：%.2f\r\n",angle[0],angle[1],angle[2],w[0],w[1],w[2]);

		}
	} 
}

















//	if(sign)
//      {  
//         memcpy(Temp,Re_buf,11);
//         sign=0;
//				
//         if(Re_buf[0]==0x55)       //检查帧头
//         {  
//            switch(Re_buf[1])
//            {
//               case 0x51: //标识这个包是加速度包
//                  a[0] = ((short)(Temp[3]<<8 | Temp[2]))/32768.0*16;      //X轴加速度
//                  a[1] = ((short)(Temp[5]<<8 | Temp[4]))/32768.0*16;      //Y轴加速度
//                  a[2] = ((short)(Temp[7]<<8 | Temp[6]))/32768.0*16;      //Z轴加速度
//                  T    = ((short)(Temp[9]<<8 | Temp[8]))/340.0+36.25;      //温度
////							 LED_BLUE(1);
//                  break;
//               case 0x52: //标识这个包是角速度包
//                  w[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*2000;      //X轴角速度
//                  w[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*2000;      //Y轴角速度
//                  w[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*2000;      //Z轴角速度
//                  T    = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;      //温度
////									LED_RED(1);
//                  break;
//               case 0x53: //标识这个包是角度包
//                  angle[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*180;   //X轴滚转角（x 轴）
//                  angle[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*180;   //Y轴俯仰角（y 轴）
//                  angle[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*180;   //Z轴偏航角（z 轴）
//                  T        = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;   //温度

//                  //printf("X轴角度：%.2f   Y轴角度：%.2f   Z轴角度：%.2f\r\n",angle[0],angle[1],angle[2]);
//                  break;
//               default:  break;
//            }
//			printf("X角度：%.2f  Y角度：%.2f  Z角度：%.2f  X速度：%.2f  Y速度：%.2f  Z速度：%.2f\r\n",angle[0],angle[1],angle[2],w[0],w[1],w[2]);
//         }
//			 }
	
	














