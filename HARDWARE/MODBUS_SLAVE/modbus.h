#ifndef _MODBUS_H
#define _MODBUS_H
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 	 
#include "timer.h"
#include "platform.h"
#include "radio.h"
#include "modbus.h"
#include "sx1276-LoRa.h"
#include "string.h"
#include "sx1276.h"
#include "crc.h"
#include "../../radio/sx1276-Hal.h"

#define CRSSI_EN        1u                            //�ź�ǿ�ȵĺ궨�壬���˱�����1�����ڿɴ�ӡ���ôν�������ʱ���ź�ǿ��
#define SNR_EN          1u                            //����ȵĺ궨�壬���˱�����1�����ڿɴ�ӡ���ôν�������ʱ�������
#define LNA_EN          0u                            //�ź�����ĺ궨�壬���˱�����1�����ڿɴ�ӡ���ôν�������ʱ���ź�����
#define PARAMETER_SETTING_EN    1u                    //����������Ϣ��־λ�����˱�����1�����ڿɴ�ӡ�����õ�һЩ���� 

#define RX_BUF_SIZE     255                           //����������ݵ�����ֽ���
#define TX_BUF_SIZE     255                           //���巢�����ݵ�����ֽ���
extern u8  RXData[RX_BUF_SIZE];				          //������������������
extern u8  TXData[TX_BUF_SIZE];				          //������������������

extern tRadioDriver *Radio;

void Modules_Init(void);
void Modbus_02_Solve(void);
void Modbus_01_Solve(void);
void Modbus_05_Solve(void);
void Modbus_15_Solve(void);
void Modbus_03_Solve(void);
void Modbus_06_Solve(void);
void Modbus_16_Solve(void);

void SendData(u8 *txdata,u16 datalen);
void ReciveData(void);

#endif
