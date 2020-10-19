#ifndef __DataProcessing_H
#define __DataProcessing_H
#include "stdint.h"

#define false 0
#define true 1
	
extern uint8_t RFLRState;


void DATA_Processing(void);
void start_power_supply(void);
void Information_BACK(void);
#endif
