#include "pbdata.h"
#include "crc.h"

#define RX_BUF_SIZE     255                       //定义接收数据的最大字节数
#define TX_BUF_SIZE     255                       //定义发送数据的最大字节数

u8  RXData[RX_BUF_SIZE];				          //此数组用来接收数据
u8  TXData[TX_BUF_SIZE];//="我是主机!";		      //此数组用来发送数据

u8 Addr[2];  									  //通讯地址用芯片id的后2位
u8 Beacon[2];                                     //信标码
extern uint16_t  crc_value;             		  //数据crc校验值
extern uint8_t RFLRState;

void Get_ChipID(void);
void Registration(void);

//u8 crc[7]={0x01,0x02,0x03,0x04,0x05,0x06,0x07};

int main(void)
{	 
    Modules_Init();	                              //硬件模块初始化 
	Get_ChipID(); 								  //获取芯片的唯一ID  这里用作通讯地址
	printf("LoRa通信开始......\r\n");
    SX1276StartRx();                              //默认接收状态
	Read_FlashData(); //单片机第一次上电先读一下数据
	if(F_data[0]!=0x55)   //在第一次上电内存没数据情况下
	{
		Registration();								  //注册自己
    }
	else
	{
		Beacon[0]=F_data[2];
		Beacon[1]=F_data[3];  //获取父节点信标
	}
	while(1)
	{
//		int crcdata;
//		crcdata=stm32_crcbitbybitfast(crc, 7);
		switch( SX1276LoRaProcess() )
		{
			case RF_RX_DONE:                       //接收数据
                 ReciveData();
				 break;
			case RF_TX_DONE:                       //发送数据
                 SX1276StartRx();                  //发送成功后又进入接收状态      
				 break;
			case RF_TX_START:                      //开始发送数据
			     SendData(TXData,TXData[4]);
			     break;
		}	
	}
}

void Registration(void)
{	
	TXData[0]=Addr[0];
	TXData[1]=Addr[1];  //注册的时候不知道信标码先写自己地址
	TXData[2]=0x28;
	TXData[3]=0xA5;     //帧头数据
	TXData[4]=0x10;     //数据长度
	TXData[5]=Addr[0];
	TXData[6]=Addr[1];  //本机的地址这里用的是stm32的芯片ID
	TXData[7]=0X03;     //三开面板为03
	TXData[8]=0xCC;     //注册是CC
	TXData[9]=0xFF;     //目前1通道状态是关闭
	TXData[10]=0xFF;     //目前2通道状态是关闭
	TXData[11]=0xFF;     //目前3通道状态是关闭
	
	crc_value=RadioComputeCRC(TXData,TXData[4]-9,CRC_TYPE_IBM); //CRC校验根据协议重新写===============================================
	TXData[12] =crc_value>>8;
	TXData[13]=crc_value;
	TXData[14]=0x5A;
	TXData[15]=0x29;   //帧尾
	RFLRState=RFLR_STATE_TX_START;
}

void Get_ChipID(void)
{
#if 0
 u32 ChipUniqueID[3];
 //地址从小到大,先放低字节，再放高字节：小端模式
 //地址从小到大,先放高字节，再放低字节：大端模式
 ChipUniqueID[2] = *(__IO u32*)(0X1FFFF7E8);  // 低字节
 ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
 ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // 高字节
 printf("######## 芯片的唯一ID为: X-X-X  rn",ChipUniqueID[0],ChipUniqueID[1],ChipUniqueID[2]);
          //此条语句输出32位
#else   //调整了大小端模式，与ISP下载软件的一致
    u8 temp[12];   
    u32 temp0,temp1,temp2;
    temp0=*(__IO u32*)(0x1FFFF7E8);    //产品唯一身份标识寄存器（96位）
    temp1=*(__IO u32*)(0x1FFFF7EC);
    temp2=*(__IO u32*)(0x1FFFF7F0);
    temp[0] = (u8)(temp0 & 0x000000FF);
    temp[1] = (u8)((temp0 & 0x0000FF00)>>8);
    temp[2] = (u8)((temp0 & 0x00FF0000)>>16);
    temp[3] = (u8)((temp0 & 0xFF000000)>>24);
    temp[4] = (u8)(temp1 & 0x000000FF);
    temp[5] = (u8)((temp1 & 0x0000FF00)>>8);
    temp[6] = (u8)((temp1 & 0x00FF0000)>>16);
    temp[7] = (u8)((temp1 & 0xFF000000)>>24);
    temp[8] = (u8)(temp2 & 0x000000FF);
    temp[9] = (u8)((temp2 & 0x0000FF00)>>8);
    temp[10] = (u8)((temp2 & 0x00FF0000)>>16);
    temp[11] = (u8)((temp2 & 0xFF000000)>>24);
	
	Addr[0]=(u8)((temp2 & 0x0000FF00)>>8);
	Addr[1]=(u8)(temp2 & 0x000000FF);//这里用他来做通讯地址
    //printf("######## STM32芯片ID为: %.2X%.2X%.2X%.2X-%.2X%.2X%.2X%.2X-%.2X%.2X%.2X%.2X rn",
	//temp[0],temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],temp[7],temp[8],temp[9],temp[10],temp [11]); //串口打印出芯片ID
#endif
}

