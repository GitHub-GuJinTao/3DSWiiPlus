#include <STRING.H>
#include "ModuleKeyMapping.h"
#include "main.h"
#include "LCM12864.h"
#include "SaveData.h"
#include "Controller.h"


static void Init(){
	SaveData_Read();
	
	if(1){
		u8 len = LCM12864_GetStrLenFromIdx(10, TITLE_KEY_MAPPING) - 1;
		LCM12864_FillRect(((128 - len) >> 1) - 1, 0, len + 2, 11, 1);
		LCM12864_DrawStringIdx(10, TITLE_KEY_MAPPING, (128 - len) >> 1, 1, 0);	// ����ӳ��
		LCM12864_DrawStringIdx(10, KEY_WII, 0, 12, 1);						// WII
		LCM12864_DrawString(10, "=", 61, 12, 1);
		LCM12864_DrawStringIdx(10, KEY_CTR, 89, 12, 1);						// CTR
	}
}

void ModuleKeyMapping(){
	s8 selectedIndex = 0;
	s16 offset = 128 << 4;	// ������
	s8 selectedWiiButton = 0;

	Init();
	while(1){
		if(1){
			u8 lenWII = LCM12864_GetStrLenFromIdx(10, KEY_WII_LEFT + selectedWiiButton) - 1;
			u8 lenCTR = LCM12864_GetStrLenFromIdx(10, KEY_CTR_LEFT + saveData.keyMapping[selectedWiiButton]) - 1;
			u16 len = LCM12864_GetStrLenFromIdx(10, DESC_2) - 1;
			LCM12864_FillRect(20, 12, 20, 10, 0);
			LCM12864_FillRect(109, 12, 20, 10, 0);
			LCM12864_DrawStringIdx(10, KEY_WII_LEFT + selectedWiiButton, 20 + ((20 - lenWII) >> 1), 12, 1);							// �� �� �� �� A B X Y L R ZL ZR + - H
			LCM12864_DrawStringIdx(10, KEY_CTR_LEFT + saveData.keyMapping[selectedWiiButton], 109 + ((20 - lenCTR) >> 1), 12, 1);	// �� �� �� �� A B X Y L R ZL ZR START SELECT HOME

			LCM12864_FillRect(0, 22, 128, 10, 0);
			LCM12864_DrawStringIdx(10, DESC_2, offset >> 4, 22, 1);	// ��/���л�WII�˰��� ��/���л�CTR�˰��� A�����趨���������ò˵� B�������趨ֱ�ӷ������ò˵�

			offset -= 8;
			if(offset < -(len << 4))offset = 128 << 4;
		}
		LCM12864_Refresh();
			
		Controller_Read();
		
		if(Controller_IsKeyDown(WII_CLASSIC_CONTROLLER_BUTTON_UP)){
			selectedWiiButton--;
			if(selectedWiiButton < 0)selectedWiiButton = KEY_ITEM_COUNT - 1;
		}
		if(Controller_IsKeyDown(WII_CLASSIC_CONTROLLER_BUTTON_DOWN)){
			selectedWiiButton++;
			if(selectedWiiButton > KEY_ITEM_COUNT - 1)selectedWiiButton = 0;
		}
		if(Controller_IsKeyDown(WII_CLASSIC_CONTROLLER_BUTTON_LEFT)){
			saveData.keyMapping[selectedWiiButton]--;
			if(saveData.keyMapping[selectedWiiButton] < 0)saveData.keyMapping[selectedWiiButton] = KEY_ITEM_COUNT;
		}
		if(Controller_IsKeyDown(WII_CLASSIC_CONTROLLER_BUTTON_RIGHT)){
			saveData.keyMapping[selectedWiiButton]++;
			if(saveData.keyMapping[selectedWiiButton] > KEY_ITEM_COUNT)saveData.keyMapping[selectedWiiButton] = 0;
		}
		if(Controller_IsKeyDown(WII_CLASSIC_CONTROLLER_BUTTON_A)){
			SaveData_Flush();
			
			currentModule = Module_ConfigMenu;
			return;
		}
		if(Controller_IsKeyDown(WII_CLASSIC_CONTROLLER_BUTTON_B)){
			currentModule = Module_ConfigMenu;
			return;
		}
	}
}
