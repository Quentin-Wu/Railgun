#ifndef __OPENMV_H
#define __OPENMV_H

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

void Openmv_Init(void);
void Openmv_Receive_Data(uint16_t data);
void Openmv_Change_K( uint32_t openmv_k); //改变openmv测距 与 像素关系的比例 K




#endif

