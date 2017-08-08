/*
�����豸��Ҫ��IO�����£�
1��Ӳ��Uart�����ڴ������ء�
15��GPIO������ӳ�䵽3DS��13�������Լ���ҡ�˵�ZL/ZR����
2��GPIO������ģ��I2C��
4��Ӳ��PWM����������DAC��
1��Ӳ��SPI�����ڿ���LCD��nrf24L01+��
3��GPIO������LCD��/CS��CD��LED��
2��GPIO������nRF24L01+��/CS��CE��

��ϸ�����£�
	����	IO��	ģʽ	�ܽ�
Ӳ��Uart��
	RX		P3.0	׼˫��	18
	TX		P3.1	׼˫��	19
CTRInterface��
	LEFT	P0.0	��©	40
	RIGHT	P0.1	��©	41
	UP		P0.2	��©	42
	DOWN	P0.3	��©	43
	A		P0.4	��©	44
	B		P0.5	��©	1
	X		P0.6	��©	2
	Y		P0.7	��©	3
	L		P4.0	��©	17
	R		P4.1	��©	26
	ZL		P4.2	��©	27
	ZR		P4.3	��©	28
	STA		P4.4	��©	29
	SEL		P4.5	��©	38
	HOME	P4.6	��©	39
I2C��
	SDA		P3.2	��©	20
	SCL		P3.3	��©	21
PWM��
	PWM0	P3.7	ǿ����	25
	PWM1	P2.1	ǿ����	31
	PWM2	P2.2	ǿ����	32
	PWM3	P2.3	ǿ����	33
Ӳ��SPI��
	MOSI	P1.3	׼˫��	8
	MISO	P1.4	׼˫��	9
	SCLK	P1.5	׼˫��	10
LCD��
	/CS		P1.0	׼˫��	4
	CD		P1.1	׼˫��	5
	LED		P1.2	׼˫��	7
nRF24L01+��
	/CS		P3.4	׼˫��	22
	CE		P3.5	׼˫��	23
	IRQ		P3.6	׼˫��	24
*/


#include <INTRINS.h>
#include "main.h"
#include "Uart.h"
#include "I2C.h"
#include "IAP.h"
#include "PWM.h"
#include "Timer.h"
#include "SPI.h"
#include "CTRInterface.h"
#include "LCM12864.h"
#include "SaveData.h"
#include "Controller.h"
#include "ModuleConnect.h"
#include "ModuleCountdown.h"
#include "ModuleGeneric.h"
#include "ModuleConfigMenu.h"
#include "ModuleKeyMapping.h"
#include "ModuleAnalogMapping.h"
#include "ModuleAnalogCalibration.h"
#include "ModuleTurboSetting.h"
#include "ModuleDeviceSetting.h"


enumModuleType currentModule = Module_Connect;


static void Init(){
	P0M0 = 0;P0M1 = 0;
	P1M0 = 0;P1M1 = 0;
	P2M0 = 0;P2M1 = 0;
	P3M0 = 0;P3M1 = 0;
	P4M0 = 0;P4M1 = 0;	// ��������P�ھ�Ϊ������
	P0 = 0xFF;
	P1 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;
	P4 = 0xFF;			// ����P�ھ�����ߵ�ƽ
}

void main(){
	Init();
	Uart_Init();
	I2C_Init();
	PWM_Init();
	Timer_Init();
	SPI_Init();
	
	CTRInterface_Init();
	LCM12864_Init();
	LCM12864_Backlight(0);
	SaveData_Init();
	Controller_Init();
	
	while(1){
		LCM12864_Clear();
		
		switch(currentModule){
			case Module_Connect:			ModuleConnect();			break;
			case Module_Countdown:			ModuleCountdown();			break;
			case Module_Generic:			ModuleGeneric();			break;
			case Module_ConfigMenu:			ModuleConfigMenu();			break;
			case Module_KeyMapping:			ModuleKeyMapping();			break;
			case Module_AnalogMapping:		ModuleAnalogMapping();		break;
			case Module_AnalogCalibration:	ModuleAnalogCalibration();	break;
			case Module_TurboSetting:		ModuleTurboSetting();		break;
			case Module_DeviceSetting:		ModuleDeviceSetting();		break;
		}
	}
}
