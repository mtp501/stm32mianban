#include "pbdata.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
			
u32 longkey=0;
extern uint8_t RFLRState;			
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3;//KEY0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA4

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
		else if(KEY3==0)return KEY3_PRES;
	}else if(KEY1==1&&KEY2==1&&KEY3==1)key_up=1; 	    
 	return 0;// �ް�������
}



u8 scan_key(void)
{	
	if(KEY0==0) //���°���
	{   
		longkey++;
	}
	else 
	{  // �ɿ�����
		if((longkey >= 3) && (longkey <= 100) )// 15ms - 1s
			{    
				longkey = 0;
//				if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13)==Bit_RESET)
//				{
//					GPIO_SetBits(GPIOC,GPIO_Pin_13);	
//				}
//				else
//				{
//					GPIO_ResetBits(GPIOC,GPIO_Pin_13);
//				}
//				
//				Information_BACK();//�ظ�����״̬
			} 
		else if(longkey >= 300) // 2s
			{ 
				longkey = 0;
				F_data[0]=0xFF;
				Write_FlashData(); //дһ�����ݽ���F_data[0]��ֹ�´��ϵ����ʼ��
			} 
		else 
			{
				longkey = 0;
				return 0;// �ް�������// ��Ϊ�Ŷ�������״̬Ҳ��Ϊ�ް�������
			}
	}
}
