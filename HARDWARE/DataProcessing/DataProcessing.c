#include "pbdata.h"
extern u8 Addr[2],Beacon[2];
extern uint16_t  crc_value;

void Registration(void);
/****************************************************************************
* 名    称：DATA_Processing()
* 功    能：数据处理函数
* 入口参数：无
* 出口参数：无
* 说    明：处理服务器发来的数据功能请求和任务实现
* 调用方法：无 
****************************************************************************/ 
void DATA_Processing(void)  //主机无线收到的信息处理
{
//	u8 i;
//	if(RXData[4]==Addr[0] && RXData[5]==Addr[1])  //查看地址是不是自己地址
//	{
//		if(RXData[8]==0xBB) //节点回复主机信息
//		{
//			for(i=0;i<RXData[2];i++)   //发送数据
//			{
//				USART_SendData(USART1,RXData[i]);
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	
//			}
//		}
//	}
//	
//	if(RXData[8]==0xCC)
//	{
//		for(i=0;i<RXData[2];i++)   //发送数据
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
//		crc_value=RadioComputeCRC(USART_RX_BUF,5,CRC_TYPE_IBM); //CRC校验根据协议重新写===============================================
//		TXData[10] =crc_value>>8;
//		TXData[11]=crc_value;
//		TXData[12]=0x0D;
//		TXData[13]=0x0A;
//		RFLRState=RFLR_STATE_TX_START;
//	}		
}

/****************************************************************************
* 名    称：Information_Back()
* 功    能：数据处理后的状态反馈函数
* 入口参数：无
* 出口参数：无
* 说    明：当数据处理函数处理完数据后反馈给服务器的信息函数
* 调用方法：无 
****************************************************************************/ 
void Information_BACK(void)  //反馈数据处理
{
	
		TXData[0]=Beacon[0];
		TXData[1]=Beacon[1];  //父节点的信标码
		TXData[2]=0x28;
		TXData[3]=0xA5;  //帧头
		TXData[4]=0x10;  //数据长度
		TXData[5]=Addr[0];
		TXData[6]=Addr[1];  //本机的地址
		TXData[7]=0x03;     //三开面板03
		TXData[8]=0xBB;     //反馈信息BB

		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==Bit_RESET)
		{TXData[9]=0xFF;}
		else{TXData[9]=0x00;}  //1通道状态
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==Bit_RESET)
		{TXData[10]=0xFF;}
		else{TXData[10]=0x00;}  //2通道状态
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==Bit_RESET)
		{TXData[11]=0xFF;}
		else{TXData[11]=0x00;}  //3通道状态
		
		crc_value=RadioComputeCRC(TXData,(TXData[4]-9),CRC_TYPE_IBM); //CRC校验根据协议重新写===============================================
		TXData[12] =crc_value>>8;
		TXData[13]=crc_value;
		TXData[14]=0x5A;
		TXData[15]=0x29;
		RFLRState=RFLR_STATE_TX_START;
}



/****************************************************************************
* 名    称：start_power_supply()
* 功    能：上电后的状态获取以及恢复
* 入口参数：无
* 出口参数：无
* 说    明：当电源板第一次上电或断电后上电获取上次状态运行
* 调用方法：无 
****************************************************************************/ 
void start_power_supply(void)   //上电初始化后的首次给个状态
{
	Read_FlashData(); //单片机第一次上电先读一下数据
}



