#ifndef __XUNJI_H
#define __XUNJI_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

//ËÄÂ·Ñ­¼£
#define		SYSCTL_PERIPH_GPIO_XUN1			SYSCTL_PERIPH_GPIOB
#define   GPIO_PORT_BASE_XUN1					GPIO_PORTB_BASE
#define	  GPIO_PIN_XUN1								GPIO_PIN_2
#define   YIWEI1											2

#define		SYSCTL_PERIPH_GPIO_XUN2			SYSCTL_PERIPH_GPIOB
#define   GPIO_PORT_BASE_XUN2					GPIO_PORTB_BASE
#define	  GPIO_PIN_XUN2								GPIO_PIN_3
#define   YIWEI2											3

#define		SYSCTL_PERIPH_GPIO_XUN3			SYSCTL_PERIPH_GPIOE
#define   GPIO_PORT_BASE_XUN3					GPIO_PORTE_BASE
#define	  GPIO_PIN_XUN3								GPIO_PIN_0
#define   YIWEI3											0

#define		SYSCTL_PERIPH_GPIO_XUN4			SYSCTL_PERIPH_GPIOA
#define   GPIO_PORT_BASE_XUN4					GPIO_PORTA_BASE
#define	  GPIO_PIN_XUN4								GPIO_PIN_2
#define   YIWEI4											2

void Xunji_Init(void);
uint8_t Xunji_Read(void);

#endif

