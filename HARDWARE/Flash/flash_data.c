#include "pbdata.h"
/*����д�����ܹ�6��
��0λ��оƬ�����ϵ��ж�λ  
��1λ��״̬λ
��2λ�����ڵ��ַ��
��3λ�����ڵ��ַ��λ

*/
u8 F_data[4]={0,0,0,0};
void Read_FlashData(void)  
{
	//д֮ǰ�Ȱ����ݶ�������  Ҫ����ҳ�Ͳ�������
	u8 x=0,y=0;
	for(x=0;x<4;x++)  
	{
	    F_data[x]=(*(__IO uint8_t*)(FLASH_ADR+y));
		y=y+4;
	}
}

void Write_FlashData(void)
{
	//����֯�����ݺ���д��
	u8 z=0,w=0;
	
	FLASH_Unlock();  //����
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);  //�����־λ
	FLASH_ErasePage(FLASH_ADR);  //Ҫ����ҳ����ʼ��ַ
	
	for(z=0;z<4;z++)	//д����
	{
		FLASH_ProgramWord(FLASH_ADR+w,F_data[z]);
		w=w+4;
	}

	FLASH_Lock();	//����
}

