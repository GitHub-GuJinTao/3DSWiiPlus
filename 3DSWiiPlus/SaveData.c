#include <STRING.H>
#include "SaveData.h"
#include "IAP.h"
#include "Uart.h"


#define SECTOR	121	// IAP15W4K61S4����121������Ϊ���һ������
#define SECTOR_ADDR(x)	(((u16)(x)) << 9)


structSaveData xdata saveData;


void SaveData_Init(){
	// ����ļ�ͷ�Ƿ�Ϸ������Ϸ��Ļ����ʽ���浵
	u8 i = 0;
	for(;i<sizeof(saveData.signature);++i){
		saveData.signature[i] = IAP_ReadByte(SECTOR_ADDR(SECTOR) + i);
	}
	if(strcmp(saveData.signature, "3DSWii+")){
		memset(&saveData, 0, sizeof(saveData));
		strcpy(saveData.signature, "3DSWii+");
		SaveData_Flush();
	}
}

void SaveData_Read(){
	u8 i = 0;
	for(;i<sizeof(saveData);++i){
		*((u8*)&saveData + i) = IAP_ReadByte(SECTOR_ADDR(SECTOR) + i);
	}
	for(i=0;i<KEY_ITEM_COUNT;++i){
		// ��֤��ֵ�ض���3DS��ֵ��Χ��
		if(saveData.keyMapping[i] > KEY_ITEM_COUNT - 1)saveData.keyMapping[i] = KEY_ITEM_COUNT - 1;
		if(saveData.keyMapping[i] < 0)saveData.keyMapping[i] = KEY_ITEM_COUNT - 1;
	}
	// ��֤ҡ�˱ض�ӳ��Ϊ��������
	saveData.analogMapping &= 1;
	// ��֤ҡ�˵�������ض�ҪôΪ��ҪôΪ��
	saveData.reverseAnalogX &= 1;
	saveData.reverseAnalogY &= 1;
	// ��֤���������ض���3DS��ֵ��Χ�ڣ�����Ƶ�����30Hz
	if(saveData.turboEnableKey > KEY_ITEM_COUNT)saveData.turboEnableKey = 0;
	if(saveData.turboEnableKey < 0)saveData.turboEnableKey = 0;
	if(saveData.turboDisableKey > KEY_ITEM_COUNT)saveData.turboDisableKey = 0;
	if(saveData.turboDisableKey < 0)saveData.turboDisableKey = 0;
	if(saveData.turboFreq > 5 - 1)saveData.turboFreq = 5 - 1;
	if(saveData.turboFreq < 0)saveData.turboFreq = 0;
	// ��֤��ֵ��ƫ�����ض�Ϊ����
	saveData.analogRatioL &= 0x80;
	saveData.analogOffsetL &= 0x80;
	saveData.analogRatioR &= 0x80;
	saveData.analogOffsetR &= 0x80;
}

void SaveData_Flush(){
	u8 i = 0;
	IAP_EraseSector(SECTOR_ADDR(SECTOR));
	for(;i<sizeof(saveData);++i){
		IAP_ProgramByte(SECTOR_ADDR(SECTOR) + i, *((u8*)&saveData + i));
	}
}

void SaveData_Dump(){
	Uart_SendBuf(&saveData, sizeof(saveData));
}

void SaveData_Restore(){
	u8 len = 0;
	Uart_RecvBuf(&saveData, &len);
	SaveData_Flush();
}
