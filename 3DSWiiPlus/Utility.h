#ifndef UTILITY_H_
#define UTILITY_H_


#include "typedef.h"
#include "STC15W4K.h"


#define KEY_ITEM_COUNT			15


void Delay10us(u8 us);	// ��ʱx*10΢��
void Delay(u16 ms);	// ��ʱx����
void SoftReset();
u8 Dec2String(u8 dec, u8* strBuf);
u8 Hex2String(u8 hex, u8* strBuf);


#endif	/* UTILITY_H_ */
