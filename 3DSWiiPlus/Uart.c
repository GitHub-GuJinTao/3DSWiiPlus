#include <STRING.H>
#include "Uart.h"


bit busy = 0;	// ����æ��־
static xdata u8 sendBuf[0x100];	// ���ͻ���
static xdata u8 recvBuf[0x100];	// ���ջ���
static u8 sendP = 0;
static u8 sendBufP = 0;
static u8 recvP = 0;
static u8 recvBufP = 0;


static void Uart() interrupt 4 using 1{
	if(RI){
		RI = 0;		// ���RIλ
		//P0 = SBUF;	// P0��ʾ��������
		//P22 = RB8;	// P2.2��ʾУ��λ
		recvBuf[recvBufP++] = SBUF;	// ��UART���ݽ��ռĴ����ж����ݣ�д�뻺��
	}
	if(TI){
		TI = 0;		// ���TIλ
		busy = 0;	// ��æ��־
	}
}

static void Uart_Flush(){
	if(sendP != sendBufP){
		while(busy);	// �ȴ�ǰ������ݷ������
		busy = 1;
		SBUF = sendBuf[sendP++];	// д���ݵ�UART���ݷ��ͼĴ���
	}
}

// ��Ƶ30M
void Uart_Init(){
	P_SW1 &= 0x3F;	// ����1��TX/RXλ��P3.1/P3.0
	PCON &= 0x3F;	// �����ʲ����٣�SCON.6����Ϊ֡�����⹦��
	SCON = 0x50;	// 8λ���ݣ��ɱ䲨���ʣ�����żУ��
	AUXR |= 0x40;	// ��ʱ��1ʱ��ΪFosc����1T
	AUXR &= 0xFE;	// ����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;	// �����ʱ��1ģʽλ
	TMOD |= 0x20;	// �趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xF8;		// �趨��ʱ��ֵ��������115200��
	TH1 = 0xF8;		// �趨��ʱ����װֵ
	ET1 = 0;		// ��ֹ��ʱ��1�ж�
	TR1 = 1;		// ������ʱ��1
	ES = 1;			// ʹ�ܴ����ж�
    EA = 1;			// ȫ���жϴ�
}

void Uart_Send(u8 value){
	sendBuf[sendBufP++] = value;
	Uart_Flush();
}

void Uart_SendBuf(void* p, u8 len){
	u8 remains = 0x100 - sendBufP;
	if(remains >= len)
		memcpy(sendBuf + sendBufP, p, len);
	else{
		memcpy(sendBuf + sendBufP, p, remains);
		memcpy(sendBuf, (u8*)p + remains, len - remains);
	}
	sendBufP += len;
	Uart_Flush();
}

void Uart_Recv(u8* value){
	if(recvP != recvBufP)
		*value = recvBuf[recvBufP++];
	else
		*value = 0;
}

void Uart_RecvBuf(void* p, u8* len){
	if(recvP < recvBufP){
		*len = recvBufP - recvP;
		memcpy(p, recvBuf + recvP, recvBufP - recvP);
		recvP = recvBufP;
	}else if(recvP > recvBufP){
		u8 remains = 0x100 - recvP;
		memcpy(p, recvBuf + recvP, remains);
		memcpy((u8*)p + remains, recvBuf, recvBufP - remains);
		*len = 0x100 + recvBufP - recvP;
	}else{
		*len = 0;
	}
}

void Uart_String(const char* str){
	Uart_SendBuf(str, strlen(str));
}

void Uart_Dec(u8 val){
	u8 i = 0;
	u8 strBuf[4];
	Dec2String(val, strBuf);
	Uart_String(strBuf);
}

void Uart_Hex(u8 val){
	u8 i = 0;
	u8 strBuf[3];
	Hex2String(val, strBuf);
	Uart_String(strBuf);
}
