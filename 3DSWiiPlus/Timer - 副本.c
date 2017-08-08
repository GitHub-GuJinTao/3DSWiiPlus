#include "Timer.h"


/*
30M��Ƶ�Ķ�ʱ��ʵ��̫���ˣ����ʹ������Ӳ����ʱ��T0��T2��Ͻ��ж�ʱ��
���У���ʱ��T2�Ķ�ʱʱ��㶨Ϊ25ms����ʱ��T0�Ķ�ʱʱ��Ϊ�ܶ�ʱʱ�� % 25ms��
����ܶ�ʱʱ��С��25ms����ֻ������ʱ��T0��T0�жϵ���֮�󴥷��ص�������
����ܶ�ʱʱ����ڵ���25ms������������ʱ��T2��ͨ��T2�ж��ۼ�ʱ�䣬ֱ��ʣ�ඨʱʱ�䲻��25ms��ʱ������T0����ʣ�µ�ʱ���ʱ������T0�жϴ����ص�������
����countdown����װcountdown��Ϊ8bit��������ʱʱ��Ϊ256*25ms=6400ms��
*/


static vu8 countdown = 0;
static vu8 countdownBackup = 0;
static Func_TimerCallback timerCallback = 0;


static void Timer0_Interrupt() interrupt 1 using 1{
	if(timerCallback){
		timerCallback();
		Timer_Start();
	}
}

static void Timer2_Interrupt() interrupt 12 using 1{
	countdown--;
	// ���countdown���⣬����װcountdown���رն�ʱ��2��������ʱ��0
	if(countdown == 0){
		countdown = countdownBackup;
		AUXR &= 0xEF;
		TR0 = 1;
	}
}


void Timer_Init(){
	AUXR &= 0x7F;		// ��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		// ��ʱ��Ϊ16λ�Զ���װ
	TF0 = 0;			// ���TF0��־
	ET0 = 1;			// ������ʱ��0�ж�
	
	AUXR &= 0xF3;		// T2Ϊ��ʱ����12Tģʽ
	INT_CLKO &= 0xFB;	// ������T2�������ʱ��
	IE2 |= 0x04;		// ������ʱ��2�ж�
	
    EA = 1;				// �����ж��ܿ���
}

// 12T��30MHz����Ƶ�£�16λ��ʱ��0���ʱʱ��Ϊ26.214ms�����Գ���25ms�Ķ�ʱʱ�䣬����Ҫcountdown����
void Timer_SetInterval(u16 ms){
	// ���㶨ʱ��0�ĳ�ֵ��countdown��
	u8 remain = ms % 25;
	// ���������25ms��������������T0ֻ��1��ʱ�Ӿ���������жϡ�����������΢�е��������㻹����
	if(remain == 0){
		TH0 = 0xFF;
		TL0 = 0xFF;
	}else{
		u16 value = 65536 - remain * 2500;
		TH0 = value >> 8;
		TL0 = value & 0xFF;
	}
	countdown = ms / 25;
	countdownBackup = countdown;
	// ��ʱ��2�Ķ�ʱʱ��㶨Ϊ25ms
	T2L = 0xDC;
	T2H = 0x0B;
}

void Timer_SetCallback(Func_TimerCallback callback){
	timerCallback = callback;
}

void Timer_Start(){
	// ���countdown�в��붨ʱ����������ʱ��2������ֱ��������ʱ��0
	if(countdown > 0){
		TR0 = 0;
		AUXR |= 0x10;
	}else{
		TR0 = 1;
		AUXR &= 0xEF;
	}
}

void Timer_Stop(){
	TR0 = 0;
	AUXR &= 0xEF;
}
