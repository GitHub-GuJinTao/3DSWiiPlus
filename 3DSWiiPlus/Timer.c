#include "Timer.h"


static vu16 countdown = 0;
static vu16 countdownBackup = 0;
static Func_TimerCallback timerCallback = 0;


static void Timer0_Interrupt() interrupt 1 using 1{
	countdown--;
	if(countdown == 0){
		countdown = countdownBackup;
		if(timerCallback){
			timerCallback();
		}
	}
}


void Timer_Init(){
	AUXR &= 0x7F;		// ��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		// ��ʱ��Ϊ16λ�Զ���װ
	TL0 = 0x3C;			// ���ö�ʱ��ֵ
	TH0 = 0xF6;			// ���ö�ʱ��ֵ
	TF0 = 0;			// ���TF0��־
	ET0 = 1;			// ������ʱ��0�ж�
	
    EA = 1;				// �����ж��ܿ���
}

void Timer_SetInterval(u16 ms){
	countdown = ms;
	countdownBackup = ms;
}

void Timer_SetCallback(Func_TimerCallback callback){
	timerCallback = callback;
}

void Timer_Start(){
	TR0 = 1;
}

void Timer_Stop(){
	TR0 = 0;
}
