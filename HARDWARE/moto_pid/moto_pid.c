#include  "moto_pid.h"
#include "cesu.h"
#include "mypwm.h"
#include "myuart.h"

//uint32_t warebuf[8];
int16_t mout1,mout2,mout3,mout4;
int16_t moutmin=0, moutmax=900;

//moto 速度pid 大概250ms处理一次比较合适
//输入四个马达的速度
void Moto_Speed_Pid(int8_t sp1, int8_t sp2, int8_t sp3, int8_t sp4)
{
	static int16_t ek1,ek2,ek3,ek4 = 0;
	static int16_t ek1p,ek2p,ek3p,ek4p =0;
	static float   kp=60, kd=100;
	
	//小于0 先反转 再pid
	if (sp1 < 0) {sp1 = -sp1; M1_Ward(0);} else  M1_Ward(1);
	if (sp2 < 0) {sp2 = -sp2; M2_Ward(0);} else  M2_Ward(1);
	if (sp3 < 0) {sp3 = -sp3; M3_Ward(0);} else  M3_Ward(1);
	if (sp4 < 0) {sp4 = -sp4; M4_Ward(0);} else  M4_Ward(1);
	
//	warebuf[0] = M1_Speed_Get();
//	warebuf[1] = M2_Speed_Get();
//	warebuf[2] = M2_Speed_Get();
//	warebuf[3] = M3_Speed_Get();
//	ek1 = sp1 - warebuf[0];
//	ek2 = sp2 - warebuf[1];
//	ek3 = sp3 - warebuf[2];
//	ek4 = sp4 - warebuf[3];
	
	//pid
	mout1 +=  kp*(ek1 - ek1p);
	mout2 +=  kp*(ek2 - ek2p);
	mout3 +=  kp*(ek3 - ek3p);
	mout4 +=  kp*(ek4 - ek4p);
	
	//限幅
	if (mout1 <= moutmin) mout1 = moutmin;  if (mout1 > moutmax) mout1 = moutmax;
	if (mout2 <= moutmin) mout2 = moutmin;  if (mout2 > moutmax) mout2 = moutmax;
	if (mout3 <= moutmin) mout3 = moutmin;  if (mout3 > moutmax) mout3 = moutmax;
	if (mout4 <= moutmin) mout4 = moutmin;  if (mout4 > moutmax) mout4 = moutmax;
	
	Moto_Pwm_Out(mout1, mout2, mout3, mout4);
//	vcan_sendware((uint8_t *)warebuf,sizeof(warebuf)); 
}

void Moto_Pwm_Out(int32_t sp1, int32_t sp2, int32_t sp3, int32_t sp4)
{
//	//小于0 先反转 再输出
//	if (sp1 < 0) {sp1 = -sp1; M1_Ward(0);} else  M1_Ward(1);
//	if (sp2 < 0) {sp2 = -sp2; M2_Ward(0);} else  M2_Ward(1);
//	if (sp3 < 0) {sp3 = -sp3; M3_Ward(0);} else  M3_Ward(1);
//	if (sp4 < 0) {sp4 = -sp4; M4_Ward(0);} else  M4_Ward(1);
	
	M1_PWM(sp1);
	M2_PWM(sp2);
	M3_PWM(sp3);
	M4_PWM(sp4);
}











