#include "pbdata.h"

/****************************************************
函数名 WDG_Config
功能   看门狗配置
参数   无
返回值 无
喂狗   IWDG_ReloadCounter();
******************************************************/

//void WDG_Config(void)    
//{
//	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)    
//	{
//		RCC_ClearFlag();         //清除标志
//	}
//	
//	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);   //允许访问IWDG_PR 和IWDG_RLR寄存器
//	IWDG_SetPrescaler(IWDG_Prescaler_32);           //分频值
//	
//	IWDG_SetReload(40000/64);                       //重载值（32/64 = 0.5）也就是说在0.5s内必须喂狗，否则复位
//	IWDG_ReloadCounter();                           //是能重载值
//	
//	IWDG_Enable();                                  //使能看门狗
//}

void IWDG_Configuration(void)
{
	//使能寄存器 写功能
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//设置预分频 40K/64=0.625k 一个周期是 1.6ms
	IWDG_SetPrescaler(IWDG_Prescaler_64);
	//设置初值
	IWDG_SetReload(8000); //800*1.6ms=12.8S
	//喂狗
	IWDG_ReloadCounter();
	//使能独立看门狗
	IWDG_Enable();
}




