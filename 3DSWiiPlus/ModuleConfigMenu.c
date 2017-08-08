#include <STRING.H>
#include "ModuleConfigMenu.h"
#include "main.h"
#include "LCM12864.h"
#include "Controller.h"
#include "Uart.h"


#define MENU_ITEM_COUNT	5


static void Init(){
	u8 len = LCM12864_GetStrLenFromIdx(10, TITLE_SETUP) - 1;
	LCM12864_FillRect(((128 - len) >> 1) - 1, 0, len + 2, 11, 1);
	LCM12864_FillRect(0, 12, 128, 1, 1);
	LCM12864_FillRect(0, 52, 128, 1, 1);
	LCM12864_DrawStringIdx(10, TITLE_SETUP, (128 - len) >> 1, 1, 0);	// ���ò˵�
}

void ModuleConfigMenu(){
	s8 selectedIndex = 0;
	s16 offset = 128 << 4;	// ������

	Init();
	while(1){
		if(1){
			s16 len;
			len = (s16)LCM12864_GetStrLenFromIdx(10, MENU_KEY_MAPPING + selectedIndex) - 1;
			LCM12864_FillRect(0, 27, 128, 10, 0);
			LCM12864_DrawStringIdx(10, MENU_KEY_MAPPING + selectedIndex, (128 - len) >> 1, 27, 1);	// ����ӳ��	ҡ��ӳ��	�����趨	ҡ��У��	�豸����

			len = (s16)LCM12864_GetStrLenFromIdx(10, DESC_1) - 1;
			LCM12864_FillRect(0, 54, 128, 10, 0);
			LCM12864_DrawStringIdx(10, DESC_1, offset >> 4, 54, 1);	// ��/��ѡ��˵��� Aȷ�� B�˳����ò˵�

			offset -= 8;
			if(offset < -(len << 4))offset = 128 << 4;
		}
		LCM12864_Refresh();
		
		Controller_Read();
		// ���¼�ѡ��A�������Ӧ�Ľ��棬B���˳���������������
		if(Controller_IsKeyDown(WII_CLASSIC_CONTROLLER_BUTTON_UP)){
			selectedIndex--;
			if(selectedIndex < 0)selectedIndex = MENU_ITEM_COUNT - 1;
		}
		if(Controller_IsKeyDown(WII_CLASSIC_CONTROLLER_BUTTON_DOWN)){
			selectedIndex++;
			if(selectedIndex >= MENU_ITEM_COUNT)selectedIndex = 0;
		}
		if(Controller_IsKeyDown(WII_CLASSIC_CONTROLLER_BUTTON_A)){
			currentModule = Module_KeyMapping + selectedIndex;
			return;
		}
		if(Controller_IsKeyDown(WII_CLASSIC_CONTROLLER_BUTTON_B)){
			currentModule = Module_Generic;
			return;
		}
	}
}
