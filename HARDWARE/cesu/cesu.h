#ifndef __CESU_H
#define __CESU_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

void Cesu_Init(void);
uint8_t M1_Speed_Get(void);
uint8_t M2_Speed_Get(void);
uint8_t M3_Speed_Get(void);
uint8_t M4_Speed_Get(void);
			


#endif


