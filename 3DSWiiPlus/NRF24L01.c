#include <STRING.H>
#include "NRF24L01.h"
#include "SPI.h"
#include "Uart.h"


//#define DEBUG


// ��������
sbit NRF24L01_CS	= P3^4;	// Ƭѡ����Ƭ��->nRF24L01��
sbit NRF24L01_CE	= P3^5;	// ʹ�ܣ���Ƭ��->nRF24L01��
sbit NRF24L01_IRQ	= P3^6;	// �жϣ�nRF24L01->��Ƭ����

// ����ָ��
#define READ_REG	0x00	// �����üĴ�������5λΪ�Ĵ�����ַ
#define WRITE_REG	0x20	// д���üĴ�������5λΪ�Ĵ�����ַ��ֻ�е���ʹ���ģʽ�ɲ���
#define RD_RX_PLOAD	0x61	// ��RX��Ч���ݣ�1~32�ֽڡ�����֮��FIFO�Ĵ����е����ݱ���������ڽ���ģʽ
#define WR_TX_PLOAD	0xA0	// дTX��Ч���ݣ�1~32�ֽڡ����ڷ���ģʽ
#define FLUSH_TX	0xE1	// ���TX FIFO�Ĵ���������ģʽ����
#define FLUSH_RX	0xE2	// ���RX FIFO�Ĵ���������ģʽ���á�����Ӧ���źŵ�ʱ��Ҫ��������Ϊ����ʹӦ���źŲ�����������
#define REUSE_TX_PL	0xE3	// ����ʹ����һ�����ݣ�CEΪ�ߵĹ����У����ݰ������ϵ����·��䡣�ڷ��͹����б����ֹʹ����
#define NOP			0xFF	// �ղ���������������״̬�Ĵ���

// ����Ĵ�����ַ
#define CONFIG		0x00	// ���üĴ���
#define EN_AA		0x01	// ʹ���Զ�Ӧ����
#define EN_RXADDR	0x02	// ���յ�ַ����
#define SETUP_AW	0x03	// ���õ�ַ��ȣ���������ͨ����
#define SETUP_RETR	0x04	// �����Զ��ط�
#define RF_CH		0x05	// ��Ƶͨ��
#define RF_SETUP	0x06	// ��Ƶ�Ĵ���
#define STATUS		0x07	// ״̬�Ĵ���
#define OBSERVE_TX	0x08	// ���ͼ��Ĵ���
#define CD			0x09	// �ز����
#define RX_ADDR_P0	0x0A	// ����ͨ��0���յ�ַ����󳤶�:5���ֽڣ���д���ֽڣ���д�ֽ�������SETUP_AW�趨��
#define RX_ADDR_P1	0x0B	// ����ͨ��1���յ�ַ����󳤶�:5���ֽڣ���д���ֽڣ���д�ֽ�������SETUP_AW�趨��
#define RX_ADDR_P2	0x0C	// ����ͨ��2���յ�ַ������ֽڿ����á����ֽڲ��ֱ�����RX_ADDR_P1[39:8]���
#define RX_ADDR_P3	0x0D	// ����ͨ��3���յ�ַ������ֽڿ����á����ֽڲ��ֱ�����RX_ADDR_P1[39:8]���
#define RX_ADDR_P4	0x0E	// ����ͨ��4���յ�ַ������ֽڿ����á����ֽڲ��ֱ�����RX_ADDR_P1[39:8]���
#define RX_ADDR_P5	0x0F	// ����ͨ��5���յ�ַ������ֽڿ����á����ֽڲ��ֱ�����RX_ADDR_P1[39:8]���
#define TX_ADDR		0x10	// ���͵�ַ����д���ֽڣ�������ǿ��ShockBurst TMģʽ��RX_ADDR_P0��˵�ַ���
#define RX_PW_P0	0x11	// ��������ͨ��0��Ч���ݿ�ȣ�1��32�ֽڣ�
#define RX_PW_P1	0x12	// ��������ͨ��1��Ч���ݿ�ȣ�1��32�ֽڣ�
#define RX_PW_P2	0x13	// ��������ͨ��2��Ч���ݿ�ȣ�1��32�ֽڣ�
#define RX_PW_P3	0x14	// ��������ͨ��3��Ч���ݿ�ȣ�1��32�ֽڣ�
#define RX_PW_P4	0x15	// ��������ͨ��4��Ч���ݿ�ȣ�1��32�ֽڣ�
#define RX_PW_P5	0x16	// ��������ͨ��5��Ч���ݿ�ȣ�1��32�ֽڣ�
#define FIFO_STATUS	0x17	// FIFO״̬�Ĵ���


// ���ͽ������ݿ�ȶ���
#define TX_ADDR_WIDTH	5	// 5�ֽڵĵ�ַ���
#define RX_ADDR_WIDTH	5	// 5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH	8	// 8�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH	8	// 8�ֽڵ��û����ݿ��


static u8 code TX_ADDRESS[TX_ADDR_WIDTH] = {0x10, 0x10, 0x10, 0x10, 0x10};  // ����һ����̬���͵�ַ
static u8 sta;


/*
��ʼ��nRF24L01
*/
void nRF24L01_Init(){
	P3M0 &= 0x8F;
	P3M1 &= 0x8F;		// /CS��CE��IRQ����Ϊ׼˫��
	
	NRF24L01_CE = 0;	// RF24L01�������ģʽ
	NRF24L01_CS = 1;	// ��ֹSPI
	NRF24L01_IRQ = 1;	// �жϸ�λ
}

/*
��reg�Ĵ�����1�ֽ�
*/
u8 nRF24L01_SPI_Read(u8 reg){
	u8 reg_val;
  	NRF24L01_CS = 0;					// CS���ͣ�SPI��ʼ��������
  	SPI_SendAndReceive(reg);			// ѡ��Ĵ���
  	reg_val = SPI_SendAndReceive(0);	// �����üĴ�����ֵ
  	NRF24L01_CS = 1;					// CS���ߣ�SPI������ݴ���
  	return reg_val;						// ��������
}

/*
��reg�Ĵ���д1�ֽ�
*/
u8 nRF24L01_SPI_Write(u8 reg, u8 value){
	u8 status;
	NRF24L01_CS = 0;					// CS���ͣ�SPI��ʼ��������
	status = SPI_SendAndReceive(reg);	// ѡ��Ĵ���
	SPI_SendAndReceive(value);			// ����ֵд��üĴ���
	NRF24L01_CS = 1;					// CS���ߣ�SPI������ݴ���
	return status;						// ����nRF24L01��״̬
}


/*
��reg�Ĵ�������size���ֽڡ�ͨ�����ڶ�ȡ����ͨ�����ݻ����/���͵�ַ
*/
u8 nRF24L01_SPI_Read_Buf(u8 reg, u8* pBuf, u8 size){
	u8 status, i;
  	NRF24L01_CS = 0;						// CS���ͣ�SPI��ʼ��������
  	status = SPI_SendAndReceive(reg);		// ѡ��Ĵ���
  	for(i=0;i<size;++i)
		pBuf[i] = SPI_SendAndReceive(0);	// �����üĴ�����ֵ
  	NRF24L01_CS = 1;						// CS���ߣ�SPI������ݴ���
  	return status;							// ����nRF24L01��״̬
}

/*
��pBuf�����е�����д�뵽nRF24L01��ͨ������д�뷢��ͨ�����ݻ����/���͵�ַ
*/
u8 nRF24L01_SPI_Write_Buf(u8 reg, u8* pBuf, u8 size){
	u8 status, i;
	NRF24L01_CS = 0;						// CS�õͣ���ʼ��������
	status = SPI_SendAndReceive(reg);		// ѡ��Ĵ�����ͬʱ����״̬��
	for(i=0;i<size;++i)
		SPI_SendAndReceive(pBuf[i]);		// ����ֽ�д��nRF24L01
	NRF24L01_CS = 1;						// CS���ߣ��������ݴ���
	return status;							// ����״̬�Ĵ���
}

/*
����Ƿ����nRF24L01�豸
*/
bit nRF24L01_Check(){
	u8 buf[TX_ADDR_WIDTH];
	u8 i = 0;
	nRF24L01_SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADDR_WIDTH);	// д��5���ֽڵĵ�ַ.
	nRF24L01_SPI_Read_Buf(TX_ADDR, buf, TX_ADDR_WIDTH);						// ����д��ĵ�ַ
	for(i=0;i<TX_ADDR_WIDTH;++i)
		Uart_Hex(buf[i]);
	Uart_String("\r\n");
	return memcmp(TX_ADDRESS, buf, TX_ADDR_WIDTH) == 0 ? 1 : 0;
}

/*
��nRF24L01����Ϊ����ģʽ
*/
void nRF24L01_TX_Mode(){
	NRF24L01_CE = 0;															// �������ģʽ
  	nRF24L01_SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADDR_WIDTH);		// д�뷢�͵�ַ
  	nRF24L01_SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADDR_WIDTH);	// Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	nRF24L01_SPI_Write(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);					// ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
#ifdef DEBUG
  	nRF24L01_SPI_Write(WRITE_REG + EN_AA, 0x00);								// ��ֹ����ͨ��0�Զ�Ӧ��
  	nRF24L01_SPI_Write(WRITE_REG + EN_RXADDR, 0x01);							// ʹ�ܽ���ͨ��0
  	nRF24L01_SPI_Write(WRITE_REG + SETUP_RETR, 0x00);							// ��ֹ�Զ��ط�
#else
  	nRF24L01_SPI_Write(WRITE_REG + EN_AA, 0x01);								// ʹ�ܽ���ͨ��0�Զ�Ӧ��
  	nRF24L01_SPI_Write(WRITE_REG + EN_RXADDR, 0x01);							// ʹ�ܽ���ͨ��0
  	nRF24L01_SPI_Write(WRITE_REG + SETUP_RETR, 0x0A);							// �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
#endif
  	nRF24L01_SPI_Write(WRITE_REG + RF_CH, 0x00);								// ѡ����Ƶͨ��Ϊ2.4GHz
  	nRF24L01_SPI_Write(WRITE_REG + RF_SETUP, 0x0F);								// ���ݴ�����2Mbps�����书��0dBm���������Ŵ�������
	NRF24L01_CE = 1;															// ����CE���������ģʽ
	Delay10us(1);																// �ȴ�10us��תΪ����ģʽ
}

/*
��nRF24L01����Ϊ����ģʽ
*/
void nRF24L01_RX_Mode(){
	NRF24L01_CE = 0;															// �������ģʽ
	nRF24L01_SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADDR_WIDTH);	// �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
	nRF24L01_SPI_Write(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);					// ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
#ifdef DEBUG
	nRF24L01_SPI_Write(WRITE_REG + EN_AA, 0x00);								// ���ý���ͨ��0�Զ�Ӧ��
	nRF24L01_SPI_Write(WRITE_REG + EN_RXADDR, 0x01);							// ʹ�ܽ���ͨ��0
#else
	nRF24L01_SPI_Write(WRITE_REG + EN_AA, 0x01);								// ʹ�ܽ���ͨ��0�Զ�Ӧ��
	nRF24L01_SPI_Write(WRITE_REG + EN_RXADDR, 0x01);							// ʹ�ܽ���ͨ��0
#endif
	nRF24L01_SPI_Write(WRITE_REG + RF_CH, 0x00);								// ѡ����Ƶͨ��Ϊ2.4GHz
	nRF24L01_SPI_Write(WRITE_REG + RF_SETUP, 0x0F);								// ���ݴ�����2Mbps�����书��0dBm���������Ŵ�������
  	nRF24L01_SPI_Write(WRITE_REG + CONFIG, 0x0F);								// �����жϣ�CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
	NRF24L01_CE = 1;															// ����CE���������ģʽ
}

/*
�������ݰ���CE=1��������10us����130us���������䣬����ֵ��0����ɹ�����0Ϊʧ�ܡ�����1����ﵽ����ط�����������ֵ��Ҫ���ֲ�
*/
u8 nRF24L01_Send(u8* pBuf){
	u8 ret;
	NRF24L01_CE = 0;															// �������ģʽ
	nRF24L01_SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADDR_WIDTH);		// д�뷢�͵�ַ
	nRF24L01_SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADDR_WIDTH);	// Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	nRF24L01_SPI_Write_Buf(WR_TX_PLOAD, pBuf, TX_PLOAD_WIDTH);					// д���ݰ���TX FIFO
  	nRF24L01_SPI_Write(WRITE_REG + CONFIG, 0x0E);								// �����жϣ�CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
	NRF24L01_CE = 1;															// ����CE���������豸
	while(NRF24L01_IRQ);														// �ȴ������ж�
	sta = nRF24L01_SPI_Read(STATUS);											// ��״̬�Ĵ���
	if(sta & (1 << 4)){															// ����ﵽ����ط�����
		nRF24L01_SPI_Write(FLUSH_TX, 0);										// ���TX������
		ret = 1;
	}
	if(sta & (1 << 5)){																// ������ݳɹ�����
		ret = 0;
	}
	nRF24L01_SPI_Write(WRITE_REG + STATUS, sta);								// ��״̬�Ĵ���
	return ret;
}

/*
�������ݰ�������ֵ��0����ɹ�����0Ϊʧ��
*/
u8 nRF24L01_Recv(u8* pBuf){
	u8 ret = 1;
	sta = nRF24L01_SPI_Read(STATUS);								// ��״̬�Ĵ���
	if(sta & (1 << 6)){
		//NRF24L01_CE = 0;
		nRF24L01_SPI_Read_Buf(RD_RX_PLOAD, pBuf, TX_PLOAD_WIDTH);	// ��RX FIFO��������
		ret = 0;
	}
	nRF24L01_SPI_Write(FLUSH_RX, 0);
	nRF24L01_SPI_Write(WRITE_REG + STATUS, sta);					// ���RX_DS�жϱ�־
	return ret;
}

/*
��ȡ�Ĵ���ֵ������ר��
*/
u8 nRF24L01_TestReg(u8 reg){
	return nRF24L01_SPI_Read(reg);
}
