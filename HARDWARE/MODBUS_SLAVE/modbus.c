#include "pbdata.h"
extern tLoRaSettings LoRaSettings;
u32 Tx_Time_Start,Tx_Time_End;   //��¼����ʱ���õ�ʱ��
u32 Rx_Time_Start,Rx_Time_End;   //��¼���������õ�ʱ��
u8 data[255];                    //�����������ݣ���RXData������鲻ͬ���ǣ���������������֡ͷ������
u16 Rec_Len = 0;	             //������յ������ݰ�����
uint16_t  crc_value;             //����crcУ��ֵ
extern u8 Addr[2],Beacon[2]; 
extern u8 Reg[12];
//Ӳ��ģ���ʼ��
void Modules_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);           //�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	    	                                  //��ʱ������ʼ��
	uart_init(115200);	 	                                  //���ڳ�ʼ��Ϊ115200
 	LED_Init();		  			                              //��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					                              //��ʼ������
	TIM3_Int_Init(10,7199);                                   //��ʱʱ��Ϊ1ms���˶�ʱ�����жϴ���������sx1276����LoRa.c�ļ��������
    SX1276_Init();
}

//��������
//txdata��װ���㷢�����ݵ�����
//datalen�����ݵĳ���
void SendData(u8 *txdata,u16 datalen)                                               
{
	u8 i = 0; 
	
	for(i=0; i<datalen; i++)       //��Ҫ���͵�����װ��data�������
	{
		data[i] = txdata[i];                                 
	}	
	
	SX1276LoRaSetTxPacket(data,datalen);  //�������ݣ�ʵ���������ֻ�ǽ�data����������RFBuffer������飬
	                                        //�ٰ�RFLRState = RFLR_STATE_TX_INIT���ٽ���SX1276LoRaProcess�������
	                                        //����һϵ�д���
}

//��������
void ReciveData()
{
    u8 temp = 0;
	uint16_t  value; 
	SX1276LoRaGetRxPacket( RXData, &Rec_Len );             //�������ݣ�ʵ���������ֻ�ǰ�RFBuffer������飬�����յ������ݷŽ�
                                                           //RXData������飬���ҵõ��˴ν��յ������ݳ���Rec_Len���������������
													       //�ĺ�������SX1276LoRaProcess�������
	if(Rec_Len > 0 && RXData[5]==Addr[0] && RXData[6]==Addr[1])  //�жϽ������ݵĳ����Ƿ����0���Ҵ��������ݵ�ַ�ǲ����Լ�
	{
		crc_value=RXData[Rec_Len-4];
		crc_value<<=8;
		crc_value|=RXData[Rec_Len-3];
		value=RadioComputeCRC(RXData,Rec_Len-9,CRC_TYPE_IBM);
		if(crc_value==RadioComputeCRC(RXData,Rec_Len-9,CRC_TYPE_IBM)) //�ж����ݵ�У��ֵ�Ƿ���ȷ
		{
			Read_FlashData();//��һ��flash����  �ȴ��ж��ǲ����Լ����ڵ�
			if(RXData[8]==0xDD)
			{
				F_data[0]=0x55;  //д������֤���Լ�ע�����
				F_data[1]=0xFF;
				F_data[2]=RXData[0];
				F_data[3]=RXData[1];  //�����ű���
				
				Beacon[0]=RXData[0];
				Beacon[1]=RXData[1];  //��ȡ���ڵ��ű�
				Write_FlashData(); //�������ص�ַ��flash
				LedToggle();
			}
			if(RXData[8]==0xAA && RXData[9]==0x00 && F_data[0]==0x55)
			{
				GPIO_WriteBit( GPIOA, GPIO_Pin_2,Bit_SET);//������Դ
				//Information_BACK();//�ظ�����״̬
			}
			else if(RXData[8]==0xAA && RXData[9]==0xFF && F_data[0]==0x55)
			{
				GPIO_WriteBit( GPIOA, GPIO_Pin_2,Bit_RESET);//�رյ�Դ
				//Information_BACK();//�ظ�����״̬
			}

			if(RXData[8]==0xAA && RXData[10]==0x00 && F_data[0]==0x55)
			{
				GPIO_WriteBit( GPIOA, GPIO_Pin_1,Bit_SET);//������Դ
				//Information_BACK();//�ظ�����״̬
			}
			else if(RXData[8]==0xAA && RXData[10]==0xFF && F_data[0]==0x55)
			{
				GPIO_WriteBit( GPIOA, GPIO_Pin_1,Bit_RESET);//�رյ�Դ
				//Information_BACK();//�ظ�����״̬
			}

			if(RXData[8]==0xAA && RXData[11]==0x00 && F_data[0]==0x55)
			{
				GPIO_WriteBit( GPIOA, GPIO_Pin_0,Bit_SET);//������Դ
				//Information_BACK();//�ظ�����״̬
			}
			else if(RXData[8]==0xAA && RXData[11]==0xFF && F_data[0]==0x55)
			{
				GPIO_WriteBit( GPIOA, GPIO_Pin_0,Bit_RESET);//�رյ�Դ
				//Information_BACK();//�ظ�����״̬
			}
			Information_BACK();//�ظ�����״̬			
		}
		for(temp = 0; temp < Rec_Len; temp++)              //���RXData���飬�����Ų���Ӱ����һ�εĽ�������              
		{           
			RXData[temp]=0;
		}
		Rec_Len=0;                                         //������ݳ���			
	}
}


 
