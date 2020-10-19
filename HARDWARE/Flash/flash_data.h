#ifndef  _FLASH_DATA_H
#define  _FLASH_DATA_H
#include "modbus.h"
//#define FLASH_ADR 0x0800F000    //ตฺ60าณ
#define FLASH_ADR 0x0800FC00

extern u8 F_data[4];

void Read_FlashData(void);
void Write_FlashData(void);

#endif
