#include "pbdata.h"
/*本次写数据总共6个
第0位：芯片初次上电判断位  
第1位：状态位
第2位：父节点地址高
第3位：父节点地址低位

*/
u8 F_data[4]={0,0,0,0};
void Read_FlashData(void)  
{
	//写之前先把数据读到缓存  要不整页就擦除掉了
	u8 x=0,y=0;
	for(x=0;x<4;x++)  
	{
	    F_data[x]=(*(__IO uint8_t*)(FLASH_ADR+y));
		y=y+4;
	}
}

void Write_FlashData(void)
{
	//等组织完数据后再写入
	u8 z=0,w=0;
	
	FLASH_Unlock();  //解锁
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);  //清除标志位
	FLASH_ErasePage(FLASH_ADR);  //要擦出页的起始地址
	
	for(z=0;z<4;z++)	//写数据
	{
		FLASH_ProgramWord(FLASH_ADR+w,F_data[z]);
		w=w+4;
	}

	FLASH_Lock();	//锁定
}

