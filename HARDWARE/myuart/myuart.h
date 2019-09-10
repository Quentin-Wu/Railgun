#ifndef		__MYUART_H
#define		__MYUART_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"

#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"


#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  				
extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USART_RX_STA;         		//����״̬���	

void My_Uart_Init(void);
void vcan_sendware(uint8_t *wareaddr, uint32_t waresize);
void usart_putbuff(uint8_t *buff, uint32_t len);

#endif

