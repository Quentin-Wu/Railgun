#ifndef __MYQEI_H
#define __MYQEI_H

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/qei.h"
#include "driverlib/pin_map.h"


void Myqei_Init(void);
uint32_t Myqei_Position_Get(void);

#endif


