#include "pbdata.h"
#include "crc.h"

#define RX_BUF_SIZE     255                       //����������ݵ�����ֽ���
#define TX_BUF_SIZE     255                       //���巢�����ݵ�����ֽ���

u8  RXData[RX_BUF_SIZE];				          //������������������
u8  TXData[TX_BUF_SIZE];//="��������!";		      //������������������

u8 Addr[2];  									  //ͨѶ��ַ��оƬid�ĺ�2λ
u8 Beacon[2];                                     //�ű���
extern uint16_t  crc_value;             		  //����crcУ��ֵ
extern uint8_t RFLRState;

void Get_ChipID(void);
void Registration(void);

//u8 crc[7]={0x01,0x02,0x03,0x04,0x05,0x06,0x07};

int main(void)
{	 
    Modules_Init();	                              //Ӳ��ģ���ʼ�� 
	Get_ChipID(); 								  //��ȡоƬ��ΨһID  ��������ͨѶ��ַ
	printf("LoRaͨ�ſ�ʼ......\r\n");
    SX1276StartRx();                              //Ĭ�Ͻ���״̬
	Read_FlashData(); //��Ƭ����һ���ϵ��ȶ�һ������
	if(F_data[0]!=0x55)   //�ڵ�һ���ϵ��ڴ�û���������
	{
		Registration();								  //ע���Լ�
    }
	else
	{
		Beacon[0]=F_data[2];
		Beacon[1]=F_data[3];  //��ȡ���ڵ��ű�
	}
	while(1)
	{
//		int crcdata;
//		crcdata=stm32_crcbitbybitfast(crc, 7);
		switch( SX1276LoRaProcess() )
		{
			case RF_RX_DONE:                       //��������
                 ReciveData();
				 break;
			case RF_TX_DONE:                       //��������
                 SX1276StartRx();                  //���ͳɹ����ֽ������״̬      
				 break;
			case RF_TX_START:                      //��ʼ��������
			     SendData(TXData,TXData[4]);
			     break;
		}	
	}
}

void Registration(void)
{	
	TXData[0]=Addr[0];
	TXData[1]=Addr[1];  //ע���ʱ��֪���ű�����д�Լ���ַ
	TXData[2]=0x28;
	TXData[3]=0xA5;     //֡ͷ����
	TXData[4]=0x10;     //���ݳ���
	TXData[5]=Addr[0];
	TXData[6]=Addr[1];  //�����ĵ�ַ�����õ���stm32��оƬID
	TXData[7]=0X03;     //�������Ϊ03
	TXData[8]=0xCC;     //ע����CC
	TXData[9]=0xFF;     //Ŀǰ1ͨ��״̬�ǹر�
	TXData[10]=0xFF;     //Ŀǰ2ͨ��״̬�ǹر�
	TXData[11]=0xFF;     //Ŀǰ3ͨ��״̬�ǹر�
	
	crc_value=RadioComputeCRC(TXData,TXData[4]-9,CRC_TYPE_IBM); //CRCУ�����Э������д===============================================
	TXData[12] =crc_value>>8;
	TXData[13]=crc_value;
	TXData[14]=0x5A;
	TXData[15]=0x29;   //֡β
	RFLRState=RFLR_STATE_TX_START;
}

void Get_ChipID(void)
{
#if 0
 u32 ChipUniqueID[3];
 //��ַ��С����,�ȷŵ��ֽڣ��ٷŸ��ֽڣ�С��ģʽ
 //��ַ��С����,�ȷŸ��ֽڣ��ٷŵ��ֽڣ����ģʽ
 ChipUniqueID[2] = *(__IO u32*)(0X1FFFF7E8);  // ���ֽ�
 ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
 ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // ���ֽ�
 printf("######## оƬ��ΨһIDΪ: X-X-X  rn",ChipUniqueID[0],ChipUniqueID[1],ChipUniqueID[2]);
          //����������32λ
#else   //�����˴�С��ģʽ����ISP���������һ��
    u8 temp[12];   
    u32 temp0,temp1,temp2;
    temp0=*(__IO u32*)(0x1FFFF7E8);    //��ƷΨһ��ݱ�ʶ�Ĵ�����96λ��
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
	Addr[1]=(u8)(temp2 & 0x000000FF);//������������ͨѶ��ַ
    //printf("######## STM32оƬIDΪ: %.2X%.2X%.2X%.2X-%.2X%.2X%.2X%.2X-%.2X%.2X%.2X%.2X rn",
	//temp[0],temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],temp[7],temp[8],temp[9],temp[10],temp [11]); //���ڴ�ӡ��оƬID
#endif
}

