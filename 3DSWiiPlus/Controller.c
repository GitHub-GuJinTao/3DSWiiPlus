#include <STRING.H>
#include "Controller.h"
#include "I2C.h"
#include "WiiClassicController.h"
#include "NRF24L01.h"


static u8 xdata isConnected = 0;	// bit0�����Ƿ��������ӣ�bit1�����Ƿ���������


void Controller_Init(){
	isConnected = 0;
	
	I2C_EnumerateAllDevices(0, 0x7F);
	if(I2C_CheckSlaveValidity(WII_CLASSIC_CONTROLLER_ID)){
		WiiClassicController_Init();
		isConnected |= (1 << 0);
	}else{
		nRF24L01_Init();
		if(nRF24L01_Check()){
			u8 buf[8];
			u8 i;
			nRF24L01_RX_Mode();
			// ���Խ���255��
			for(i=0;i<0xFF;++i){
				if(nRF24L01_Recv(buf) == 0 && strcmp(buf, "3DSWii+") == 0){
					isConnected |= (1 << 1);
					break;
				}
				Delay10us(10);	// ������ΪɶҪ�ӳ�100us����
			}
		}
	}
}

void Controller_Read(){
	if(isConnected & (1 << 0)){
		WiiClassicController_Read();
	}
	if(isConnected & (1 << 1)){
		nRF24L01_Recv(WiiClassicController_GetPtr());
	}
}

bit Controller_IsConnected(){
	return isConnected == 0 ? 0 : 1;
}
