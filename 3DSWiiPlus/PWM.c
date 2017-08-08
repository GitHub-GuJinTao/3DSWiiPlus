#include "PWM.h"


// Ԥ����Ĵ�����ַ
static const u16 code addr_PWM_T1[] = {0xff00, 0xff10, 0xff20, 0xff30, 0xff40, 0xff50};
static const u16 code addr_PWM_T2[] = {0xff02, 0xff12, 0xff22, 0xff32, 0xff42, 0xff52};
static const u16 code addr_PWM_CR[] = {0xff04, 0xff14, 0xff24, 0xff34, 0xff44, 0xff54};


void PWM_Init(){
	P3M0 |= 0x80;
	P3M1 &= 0x7F;	// P3.7����Ϊǿ����
	P2M0 |= 0x0E;
	P2M1 &= 0xF1;	// P2.1��P2.2��P2.3����Ϊǿ����
	P_SW2 |= 0x80;	// ʹ�ܷ���XSFR��PWM�ļĴ���λ��XSFR����
    PWMCFG = 0x00;	// ����PWM�������ʼ��ƽΪ�͵�ƽ
	PWMCR = 0x00;	// �ر�PWM�����������ж�
	PWMCKS = 0x00;	// ѡ��PWM��ʱ��Ϊϵͳʱ��/(0+1)
	PWMC = 0x100;	// ����PWM����Ϊ256
	P_SW2 &= ~0x80;	// ��ֹ����XSFR
}

void PWM_Start(){
	P_SW2 |= 0x80;	// ʹ�ܷ���XSFR��PWM�ļĴ���λ��XSFR����
	PWMCR |= 0x80;	// ����PWM
	P_SW2 &= ~0x80;	// ��ֹ����XSFR
}

void PWM_Stop(){
	P_SW2 |= 0x80;	// ʹ�ܷ���XSFR��PWM�ļĴ���λ��XSFR����
	PWMCR &= ~0x80;	// ֹͣPWM
	P_SW2 &= ~0x80;	// ��ֹ����XSFR
}

void PWM_SetChannelStatus(u8 channel){
	P_SW2 |= 0x80;				// ʹ�ܷ���XSFR��PWM�ļĴ���λ��XSFR����
	channel &= 0x30;			// ��֤channel��6·��Χ֮��
	channel |= (PWMCR & 0xC0);	// ����PWMCR��ֵ
	PWMCR = channel;			// ��ֵ��PWMCR
	P_SW2 &= ~0x80;				// ��ֹ����XSFR
}

void PWM_SetDuty(u8 channel, u8 duty){
	P_SW2 |= 0x80;									// ʹ�ܷ���XSFR��PWM�ļĴ���λ��XSFR����
	*((vu16 xdata*)addr_PWM_T1[channel]) = 0;
	*((vu16 xdata*)addr_PWM_T2[channel]) = duty;
	*((vu16 xdata*)addr_PWM_CR[channel]) = 0x00;	// ѡ��PWM�ܽ�ΪĬ�ϣ��ر�PWM�ж�
	P_SW2 &= ~0x80;									// ��ֹ����XSFR
}
