#ifndef MAIN_H_
#define MAIN_H_


#include "Utility.h"


typedef enum{
	Module_None = 0,
	Module_Connect,
	Module_Countdown,
	Module_Generic,
	Module_ConfigMenu,
	Module_KeyMapping,
	Module_AnalogMapping,
	Module_AnalogCalibration,
	Module_TurboSetting,
	Module_DeviceSetting,
	
	Module_Max
}enumModuleType;


extern enumModuleType currentModule;


typedef enum{
	JOYSTICK_NOT_FOUND,
	RESTART,
	ENTER_SETUP_WITHIN_3_SECS,
	TITLE_SETUP,
	MENU_KEY_MAPPING,
	MENU_ANALOG_MAPPING,
	MENU_ANALOG_CALIBRATION,
	MENU_TURBO_SETTING,
	MENU_DEVICE_SETTING,
	DESC_1,
	TITLE_KEY_MAPPING,
	KEY_WII,
	KEY_CTR,
	KEY_WII_LEFT,
	KEY_WII_RIGHT,
	KEY_WII_UP,
	KEY_WII_DOWN,
	KEY_WII_A,
	KEY_WII_B,
	KEY_WII_X,
	KEY_WII_Y,
	KEY_WII_L,
	KEY_WII_R,
	KEY_WII_ZL,
	KEY_WII_ZR,
	KEY_WII_PLUS,
	KEY_WII_MINUS,
	KEY_WII_HOME,
	KEY_CTR_LEFT,
	KEY_CTR_RIGHT,
	KEY_CTR_UP,
	KEY_CTR_DOWN,
	KEY_CTR_A,
	KEY_CTR_B,
	KEY_CTR_X,
	KEY_CTR_Y,
	KEY_CTR_L,
	KEY_CTR_R,
	KEY_CTR_ZL,
	KEY_CTR_ZR,
	KEY_CTR_START,
	KEY_CTR_SELECT,
	KEY_CTR_HOME,
	KEY_CTR_NONE,
	DESC_2,
	TITLE_ANALOG_MAPPING,
	ANALOG_SWAP,
	ANALOG_NEGATIVE_X,
	ANALOG_NEGATIVE_Y,
	ANALOG_NO,
	ANALOG_YES,
	DESC_3,
	TITLE_ANALOG_CALIBRATION,
	CALIBRATION_THRESTHOLD,
	CALIBRATION_OFFSET,
	DESC_4,
	TITLE_TURBO_SETTING,
	TURBO_KEY_ENABLE,
	TURBO_KEY_DISABLE,
	TURBO_SPEED,
	TURBO_WII,
	TURBO_WII_NONE,
	TURBO_WII_LEFT,
	TURBO_WII_RIGHT,
	TURBO_WII_UP,
	TURBO_WII_DOWN,
	TURBO_WII_A,
	TURBO_WII_B,
	TURBO_WII_X,
	TURBO_WII_Y,
	TURBO_WII_L,
	TURBO_WII_R,
	TURBO_WII_ZL,
	TURBO_WII_ZR,
	TURBO_WII_PLUS,
	TURBO_WII_MINUS,
	TURBO_WII_HOME,
	TURBO_10,
	TURBO_15,
	TURBO_20,
	TURBO_25,
	TURBO_30,
	DESC_5,
	TITLE_DEVICE_SETTING,
	DEVICE_BRIGHTNESS,
	DEVICE_CONTRAST,
	DEVICE_BIAS,
	DEVICE_1_9,
	DEVICE_1_7,
	DESC_6,
}enumStrings;


#endif	/* MAIN_H_ */
