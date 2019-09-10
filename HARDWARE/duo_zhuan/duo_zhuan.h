#ifndef __DUO_ZHUAN_H
#define __DUO_ZHUAN_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"

//¶¨Ê±Æ÷
#define SYSCTL_PERIPH_Duo_Zhuan     	SYSCTL_PERIPH_TIMER3
#define Duo_Zhuan_BASE								TIMER3_BASE
#define TIMER_Duo_Zhuan  							TIMER_A
#define INT_TIMER_Duo_Zhuan 					INT_TIMER3A
#define TIMER_Duo_Zhuan_TIMEOUT				TIMER_TIMA_TIMEOUT



void Duo_Zhuan_Init(uint32_t xms);

#endif
