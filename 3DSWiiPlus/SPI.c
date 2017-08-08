#include "SPI.h"


#define USE_HARDWARE_SPI


// ÿ��SPI�豸��/CSҪ�ڸ��Ե�.c�е�������
#ifndef USE_HARDWARE_SPI
sbit SPI_MOSI	= P1^3;
sbit SPI_MISO	= P1^4;
sbit SPI_CLK	= P1^5;
#endif


void SPI_Init(){
#ifdef USE_HARDWARE_SPI
	P_SW1 &= 0xF3;	// ��Ӳ��SPI���õ���һ��ܽţ�P1.2��P1.3��P1.4��P1.5��
#endif
	P1M0 &= 0xC7;
	P1M1 &= 0xC7;	// ��SPI_CLK��SPI_MOSI��SPI_MISO���ó�׼˫��
#ifdef USE_HARDWARE_SPI
	SPDAT = 0;		// ��ʼ��SPI������
	SPSTAT = 0xC0;	// ��ʼ��SPI״̬
	SPCTL = 0xDC;	// ����Ӳ��SPI������ģʽ����λ�ȷ����½��ط��������ؽ��գ�CLKƵ��Ϊ1/4��Ƶ
#endif
}

u8 SPI_SendAndReceive(u8 value){
#ifdef USE_HARDWARE_SPI
	SPDAT = value;				// ��仺����
	while(!(SPSTAT & 0x80));	// �ȴ��������
	SPSTAT = 0xC0;				// ���־λ
	return SPDAT;				// ���شӻ����͹���������
#else
	u8 i;
	for(i=0;i<8;++i){
		SPI_CLK = 0;
		SPI_MOSI = value & 0x80;
		SPI_CLK = 1;
		value <<= 1;
		value |= SPI_MISO;
	}
	return value;
#endif
}
