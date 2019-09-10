#ifndef __MOTO_PID_H
#define __MOTO_PID_H

#include <stdint.h>
#include <stdbool.h>

void Moto_Speed_Pid(int8_t sp1, int8_t sp2, int8_t sp3, int8_t sp4);
void Moto_Pwm_Out(int32_t sp1, int32_t sp2, int32_t sp3, int32_t sp4);


#endif


