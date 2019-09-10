#include "usmart.h"
#include "usmart_str.h"

#include "led.h"
#include "delay.h"
#include "mypwm.h"
#include "launch.h"
#include "openmv.h"

extern void mode_change(uint8_t modee);
extern void change_mode_distance(uint16_t mode_distancee);
extern void change_mode_angle_distance(int16_t mode_anglee, uint16_t mode_distancee);

//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif		   
	(void*)M_X_PWM,	"void M_X_PWM(uint32_t width)",
	(void*)M_Y_PWM,	"void M_Y_PWM(uint32_t width)",
	(void*)Launch_Now,  "void Launch_Now(uint32_t xms)",
//	(void*)M_X_Angle_Set, "void M_X_Angle_Set(int16_t x_angle)",
//	(void*)M_Y_Angle_Set, "void M_Y_Angle_Set(int16_t y_angle)",
//	(void*)Launch_Length, "void Launch_Length(uint32_t cm)",
//	(void*)Reload_Run, "void Reload_Run(uint32_t width)",
	(void*)Reload,  "void Reload(void)",
//	(void*)Charge_Now, "void Charge_Now(uint32_t xms)",
	(void*)Openmv_Change_K, "void Openmv_Change_K( uint32_t openmv_k)",
//	(void*)Reload_Charge_Launch, "void Reload_Charge_Launch(float angel)",
//	(void*)Reload_Charge_Launch_Length, "void Reload_Charge_Launch_Length(uint16_t length)",
//	(void*)Length_Angel, "void Length_Angel(int16_t angel, uint16_t length)",
	(void*)mode_change, "void mode_change(uint8_t modee)",
	(void*)change_mode_distance, "void change_mode_distance(uint16_t mode_distancee)",
	(void*)change_mode_angle_distance, "void change_mode_angle_distance(int16_t mode_anglee, uint16_t mode_distancee)",
};	


//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};


