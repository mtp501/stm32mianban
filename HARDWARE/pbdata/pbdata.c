#include "pbdata.h"

/****************************************************
������ WDG_Config
����   ���Ź�����
����   ��
����ֵ ��
ι��   IWDG_ReloadCounter();
******************************************************/

//void WDG_Config(void)    
//{
//	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)    
//	{
//		RCC_ClearFlag();         //�����־
//	}
//	
//	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);   //�������IWDG_PR ��IWDG_RLR�Ĵ���
//	IWDG_SetPrescaler(IWDG_Prescaler_32);           //��Ƶֵ
//	
//	IWDG_SetReload(40000/64);                       //����ֵ��32/64 = 0.5��Ҳ����˵��0.5s�ڱ���ι��������λ
//	IWDG_ReloadCounter();                           //��������ֵ
//	
//	IWDG_Enable();                                  //ʹ�ܿ��Ź�
//}

void IWDG_Configuration(void)
{
	//ʹ�ܼĴ��� д����
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//����Ԥ��Ƶ 40K/64=0.625k һ�������� 1.6ms
	IWDG_SetPrescaler(IWDG_Prescaler_64);
	//���ó�ֵ
	IWDG_SetReload(8000); //800*1.6ms=12.8S
	//ι��
	IWDG_ReloadCounter();
	//ʹ�ܶ������Ź�
	IWDG_Enable();
}




