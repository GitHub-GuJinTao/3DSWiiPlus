#include <INTRINS.h>
#include "IAP.h"


/*
ÿ������Ϊ512�ֽ�
*/


/* ����IAPָ�� */
#define CMD_IDLE	0	// �ȴ�
#define CMD_READ	1	// �ֽڶ�ȡ
#define CMD_PROGRAM	2	// �ֽڱ��
#define CMD_ERASE	3	// ��������

/* ����ȴ�ʱ�� */
#define WAIT_TIME	0x00	// ϵͳʱ��>=30MHz
//#define WAIT_TIME	0x01	// ϵͳʱ��>=24MHz
//#define WAIT_TIME	0x02	// ϵͳʱ��>=20MHz
//#define WAIT_TIME	0x03	// ϵͳʱ��>=12MHz
//#define WAIT_TIME	0x04	// ϵͳʱ��>=6MHz
//#define WAIT_TIME	0x05	// ϵͳʱ��>=3MHz
//#define WAIT_TIME	0x06	// ϵͳʱ��>=2MHz
//#define WAIT_TIME	0x07	// ϵͳʱ��>=1MHz


/*
�ر�IAP
*/
void IAP_Idle(){
    IAP_CONTR = 0;		// �ر�IAP����
    IAP_CMD = 0;		// �������Ĵ���
    IAP_TRIG = 0;		// ��������Ĵ���
    IAP_ADDRH = 0x80;	// ����ַ���õ���IAP����
    IAP_ADDRL = 0;
}

/*
��IAP����ָ����ַ��1�ֽ�
*/
u8 IAP_ReadByte(u16 addr){
    u8 dat;							// ���ݻ�����
    IAP_CONTR = WAIT_TIME | 0x80;	// ��IAP���ܣ��������õȴ�ʱ��
    IAP_CMD = CMD_READ;				// ����IAP��ָ��Ϊ��
    IAP_ADDRL = addr;				// ���õ�ַ
    IAP_ADDRH = addr >> 8;
    IAP_TRIG = 0x5A;				// ����ָ��1��0x5A��
    IAP_TRIG = 0xA5;				// ����ָ��2��0xA5��
    _nop_();						// �ȴ��������
    dat = IAP_DATA;					// ��IAP����
    IAP_Idle();						// �ر�IAP
    return dat;
}

/*
д1�ֽ����ݵ�IAPָ����ַ
*/
void IAP_ProgramByte(u16 addr, u8 dat){
    IAP_CONTR = WAIT_TIME | 0x80;	// ��IAP���ܣ��������õȴ�ʱ��
    IAP_CMD = CMD_PROGRAM;			// ����IAP��ָ��Ϊ���
    IAP_ADDRL = addr;				// ���õ�ַ
    IAP_ADDRH = addr >> 8;
    IAP_DATA = dat;					// дIAP����
    IAP_TRIG = 0x5A;				// ����ָ��1��0x5A��
    IAP_TRIG = 0xA5;				// ����ָ��2��0xA5��
    _nop_();						// �ȴ��������
    IAP_Idle();						// �ر�IAP
}

/*
����ָ������
*/
void IAP_EraseSector(u16 addr){
    IAP_CONTR = WAIT_TIME | 0x80;	// ��IAP���ܣ��������õȴ�ʱ��
    IAP_CMD = CMD_ERASE;			// ����IAP��ָ��Ϊ����
    IAP_ADDRL = addr;				// ���õ�ַ
    IAP_ADDRH = addr >> 8;
    IAP_TRIG = 0x5A;				// ����ָ��1��0x5A��
    IAP_TRIG = 0xA5;				// ����ָ��2��0xA5��
    _nop_();						// �ȴ��������
    IAP_Idle();						// �ر�IAP
}
