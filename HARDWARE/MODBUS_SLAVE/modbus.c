#include "pbdata.h"
extern tLoRaSettings LoRaSettings;
u32 Tx_Time_Start,Tx_Time_End;   //记录发送时间用的时间
u32 Rx_Time_Start,Rx_Time_End;   //记录接收数据用的时间
u8 data[255];                    //用来接收数据，和RXData这个数组不同的是，这个数组比它多了帧头的数据
u16 Rec_Len = 0;	             //保存接收到的数据包长度
uint16_t  crc_value;             //数据crc校验值
extern u8 Addr[2],Beacon[2]; 
extern u8 Reg[12];
//硬件模块初始化
void Modules_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);           //设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init();	    	                                  //延时函数初始化
	uart_init(115200);	 	                                  //串口初始化为115200
 	LED_Init();		  			                              //初始化与LED连接的硬件接口
	KEY_Init();					                              //初始化按键
	TIM3_Int_Init(10,7199);                                   //定时时间为1ms，此定时器的中断处理函数放在sx1276——LoRa.c文件的最后面
    SX1276_Init();
}

//发送数据
//txdata：装有你发送数据的数组
//datalen：数据的长度
void SendData(u8 *txdata,u16 datalen)                                               
{
	u8 i = 0; 
	
	for(i=0; i<datalen; i++)       //把要发送的数据装进data这个数组
	{
		data[i] = txdata[i];                                 
	}	
	
	SX1276LoRaSetTxPacket(data,datalen);  //发送数据，实际这个函数只是将data这个数组放入RFBuffer这个数组，
	                                        //再把RFLRState = RFLR_STATE_TX_INIT，再交给SX1276LoRaProcess这个函数
	                                        //进行一系列处理
}

//接收数据
void ReciveData()
{
    u8 temp = 0;
	uint16_t  value; 
	SX1276LoRaGetRxPacket( RXData, &Rec_Len );             //接收数据，实际这个函数只是把RFBuffer这个数组，即接收到的数据放进
                                                           //RXData这个数组，并且得到此次接收到的数据长度Rec_Len，真正起接收作用
													       //的函数还是SX1276LoRaProcess这个函数
	if(Rec_Len > 0 && RXData[5]==Addr[0] && RXData[6]==Addr[1])  //判断接收数据的长度是否大于0，且传来的数据地址是不是自己
	{
		crc_value=RXData[Rec_Len-4];
		crc_value<<=8;
		crc_value|=RXData[Rec_Len-3];
		value=RadioComputeCRC(RXData,Rec_Len-9,CRC_TYPE_IBM);
		if(crc_value==RadioComputeCRC(RXData,Rec_Len-9,CRC_TYPE_IBM)) //判断数据的校验值是否正确
		{
			Read_FlashData();//读一下flash数据  等待判断是不是自己父节点
			if(RXData[8]==0xDD)
			{
				F_data[0]=0x55;  //写个数据证明自己注册过了
				F_data[1]=0xFF;
				F_data[2]=RXData[0];
				F_data[3]=RXData[1];  //网关信标码
				
				Beacon[0]=RXData[0];
				Beacon[1]=RXData[1];  //获取父节点信标
				Write_FlashData(); //保存网关地址到flash
				LedToggle();
			}
			if(RXData[8]==0xAA && RXData[9]==0x00 && F_data[0]==0x55)
			{
				GPIO_WriteBit( GPIOA, GPIO_Pin_2,Bit_SET);//开启电源
				//Information_BACK();//回复网关状态
			}
			else if(RXData[8]==0xAA && RXData[9]==0xFF && F_data[0]==0x55)
			{
				GPIO_WriteBit( GPIOA, GPIO_Pin_2,Bit_RESET);//关闭电源
				//Information_BACK();//回复网关状态
			}

			if(RXData[8]==0xAA && RXData[10]==0x00 && F_data[0]==0x55)
			{
				GPIO_WriteBit( GPIOA, GPIO_Pin_1,Bit_SET);//开启电源
				//Information_BACK();//回复网关状态
			}
			else if(RXData[8]==0xAA && RXData[10]==0xFF && F_data[0]==0x55)
			{
				GPIO_WriteBit( GPIOA, GPIO_Pin_1,Bit_RESET);//关闭电源
				//Information_BACK();//回复网关状态
			}

			if(RXData[8]==0xAA && RXData[11]==0x00 && F_data[0]==0x55)
			{
				GPIO_WriteBit( GPIOA, GPIO_Pin_0,Bit_SET);//开启电源
				//Information_BACK();//回复网关状态
			}
			else if(RXData[8]==0xAA && RXData[11]==0xFF && F_data[0]==0x55)
			{
				GPIO_WriteBit( GPIOA, GPIO_Pin_0,Bit_RESET);//关闭电源
				//Information_BACK();//回复网关状态
			}
			Information_BACK();//回复网关状态			
		}
		for(temp = 0; temp < Rec_Len; temp++)              //清空RXData数组，这样才不会影响下一次的接收数据              
		{           
			RXData[temp]=0;
		}
		Rec_Len=0;                                         //清空数据长度			
	}
}


 
