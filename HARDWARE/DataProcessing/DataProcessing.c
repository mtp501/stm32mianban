#include "pbdata.h"
extern u8 Addr[2],Beacon[2];
extern uint16_t  crc_value;

void Registration(void);
/****************************************************************************
* ��    �ƣ�DATA_Processing()
* ��    �ܣ����ݴ�����
* ��ڲ�������
* ���ڲ�������
* ˵    ����������������������ݹ������������ʵ��
* ���÷������� 
****************************************************************************/ 
void DATA_Processing(void)  //���������յ�����Ϣ����
{
//	u8 i;
//	if(RXData[4]==Addr[0] && RXData[5]==Addr[1])  //�鿴��ַ�ǲ����Լ���ַ
//	{
//		if(RXData[8]==0xBB) //�ڵ�ظ�������Ϣ
//		{
//			for(i=0;i<RXData[2];i++)   //��������
//			{
//				USART_SendData(USART1,RXData[i]);
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	
//			}
//		}
//	}
//	
//	if(RXData[8]==0xCC)
//	{
//		for(i=0;i<RXData[2];i++)   //��������
//		{
//			USART_SendData(USART1,RXData[i]);
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	
//		}
//		TXData[0]=0x28;
//		TXData[1]=0xA5;
//		TXData[2]=RXData[2];
//		TXData[3]=0x00;
//		TXData[4]=RXData[6];
//		TXData[5]=RXData[7];
//		TXData[6]=Addr[0];
//		TXData[7]=Addr[1];
//		TXData[8]=0xDD;  
//		TXData[9]=0x00;
//		crc_value=RadioComputeCRC(USART_RX_BUF,5,CRC_TYPE_IBM); //CRCУ�����Э������д===============================================
//		TXData[10] =crc_value>>8;
//		TXData[11]=crc_value;
//		TXData[12]=0x0D;
//		TXData[13]=0x0A;
//		RFLRState=RFLR_STATE_TX_START;
//	}		
}

/****************************************************************************
* ��    �ƣ�Information_Back()
* ��    �ܣ����ݴ�����״̬��������
* ��ڲ�������
* ���ڲ�������
* ˵    ���������ݴ��������������ݺ���������������Ϣ����
* ���÷������� 
****************************************************************************/ 
void Information_BACK(void)  //�������ݴ���
{
	
		TXData[0]=Beacon[0];
		TXData[1]=Beacon[1];  //���ڵ���ű���
		TXData[2]=0x28;
		TXData[3]=0xA5;  //֡ͷ
		TXData[4]=0x10;  //���ݳ���
		TXData[5]=Addr[0];
		TXData[6]=Addr[1];  //�����ĵ�ַ
		TXData[7]=0x03;     //�������03
		TXData[8]=0xBB;     //������ϢBB

		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==Bit_RESET)
		{TXData[9]=0xFF;}
		else{TXData[9]=0x00;}  //1ͨ��״̬
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==Bit_RESET)
		{TXData[10]=0xFF;}
		else{TXData[10]=0x00;}  //2ͨ��״̬
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==Bit_RESET)
		{TXData[11]=0xFF;}
		else{TXData[11]=0x00;}  //3ͨ��״̬
		
		crc_value=RadioComputeCRC(TXData,(TXData[4]-9),CRC_TYPE_IBM); //CRCУ�����Э������д===============================================
		TXData[12] =crc_value>>8;
		TXData[13]=crc_value;
		TXData[14]=0x5A;
		TXData[15]=0x29;
		RFLRState=RFLR_STATE_TX_START;
}



/****************************************************************************
* ��    �ƣ�start_power_supply()
* ��    �ܣ��ϵ���״̬��ȡ�Լ��ָ�
* ��ڲ�������
* ���ڲ�������
* ˵    ��������Դ���һ���ϵ��ϵ���ϵ��ȡ�ϴ�״̬����
* ���÷������� 
****************************************************************************/ 
void start_power_supply(void)   //�ϵ��ʼ������״θ���״̬
{
	Read_FlashData(); //��Ƭ����һ���ϵ��ȶ�һ������
}



