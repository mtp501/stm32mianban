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

#define CRSSI_EN        1u                            //信号强度的宏定义，将此变量置1，串口可打印出该次接收数据时的信号强度
#define SNR_EN          1u                            //信噪比的宏定义，将此变量置1，串口可打印出该次接收数据时的信噪比
#define LNA_EN          0u                            //信号增益的宏定义，将此变量置1，串口可打印出该次接收数据时的信号增益
#define PARAMETER_SETTING_EN    1u                    //参数配置信息标志位，将此变量置1，串口可打印出配置的一些参数 

#define RX_BUF_SIZE     255                           //定义接收数据的最大字节数
#define TX_BUF_SIZE     255                           //定义发送数据的最大字节数
extern u8  RXData[RX_BUF_SIZE];				          //此数组用来接收数据
extern u8  TXData[TX_BUF_SIZE];				          //此数组用来发送数据

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
