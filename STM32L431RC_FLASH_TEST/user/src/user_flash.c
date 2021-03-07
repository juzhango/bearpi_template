//
// Created by luozw on 2021/3/7.
//

#include "user_flash.h"
#include "user_debug.h"
/* 获取地址所在的bank */
#if 0
uint32_t Flash_GetBank(uint32_t faddr)
{
    if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_MEM_MODE_Pos) == 0)
    {
        if (faddr < (FLASH_BASE + FLASH_BANK_SIZE))
            return FLASH_BANK_1;
        else
            return FLASH_BANK_1;
    }
    else
    {
        if (faddr < (FLASH_BASE + FLASH_BANK_SIZE)) // bank swap
            return FLASH_BANK_1;
        else
            return FLASH_BANK_1;
    }
}
#endif

/***************************************************************
* 函数名称: Flash_GetPage
* 说    明: 获得传入地址所在Page
* 参    数: faddr,Flash地址
* 返 回 值: Bank值
***************************************************************/
static uint32_t Flash_GetPage(uint32_t faddr)
{
    if (faddr < (FLASH_BASE + FLASH_BANK_SIZE))
        return (faddr - FLASH_BASE) / FLASH_PAGE_SIZE;
    else
        return (faddr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
}

/***************************************************************
* 函数名称: Flash_ReadOneWord
* 说    明: 从指定地址中读取一个32位的数据
* 参    数: faddr,Flash地址
* 返 回 值: Flash地址内的一个32位的数据
***************************************************************/
uint32_t Flash_ReadOneWord(uint32_t faddr)
{
    return *(__IO uint32_t *)faddr;
}



void Flash_ErasePages(uint32_t faddr, uint32_t nbpage)
{
    if(nbpage == 0 || faddr > FLASH_END || faddr < FLASH_BASE)
    {
        return;
    }

    FLASH_EraseInitTypeDef flash_EraseInit;
    uint32_t err;

    flash_EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    flash_EraseInit.Page = Flash_GetPage(faddr);
    flash_EraseInit.NbPages = nbpage;

    HAL_FLASH_Unlock();
    if(HAL_FLASHEx_Erase(&flash_EraseInit, &err) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return;
    }
}
/***************************************************************
* 函数名称:   Flash_WriteOneWord
* 说    明:  向指定地址中写入一个32位的数据
* 参    数:  faddr,Flash地址
*		    fdata32,Flash地址内的一个32位的数据
* 返 回 值: 写FLASH返回的状态
***************************************************************/
uint8_t Flash_WriteOneWord(uint32_t faddr,uint32_t fdata32)
{
    if(faddr > FLASH_END || faddr < FLASH_BASE)										//非法地址
    {
        return HAL_ERROR;
    }

    uint8_t FLASH_STATUS;
    uint64_t fdata64;

    HAL_FLASH_Unlock();
    fdata64 = fdata32|0xffffffff00000000uL;
    FLASH_STATUS = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,faddr,fdata64);//写入数据
    HAL_FLASH_Lock();

    return FLASH_STATUS;
}

/***************************************************************
* 函数名称: Flash_WriteDoubleWord
* 说    明: 向指定地址中写入一个双字（64位）数据
* 参    数: faddr,Flash地址
*		    fdata64,Flash地址内的一个双字（64位）数据
* 返 回 值: 写FLASH返回的状态
***************************************************************/
uint8_t Flash_WriteDoubleWord(uint32_t faddr, uint64_t fdata64)
{
    if(faddr > FLASH_END || faddr < FLASH_BASE)			//非法地址
    {
        return HAL_ERROR;
    }
    uint8_t FLASH_STATUS;

    HAL_FLASH_Unlock();									//解锁FLASH
    FLASH_STATUS = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,faddr,fdata64);//写入数据
    HAL_FLASH_Lock();									//上锁
    return FLASH_STATUS;
}


/***************************************************************
* 函数名称: Flash_Write32
* 说    明: 向指定地址中写入fdataNum个32位的数据
* 参    数: faddr,Flash起始地址
*			fdata,Flash地址内的32位的数据指针
*			fdata32Num,要写入的字（32位）数的数量
* 返 回 值: 无
***************************************************************/
void Flash_Write32(uint32_t faddr,uint32_t *fdata32, uint32_t fdata32Num)
{
    uint64_t fdata64;
    HAL_FLASH_Unlock();						//解锁FLASH
    if(faddr<FLASH_END)						//写入数据地址小于STM32L431RC的flash地址最大值
    {
        for (uint32_t i = 0; i < fdata32Num / 2; i++)
        {
            fdata64 = *(uint64_t*)fdata32;
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, faddr, fdata64) != HAL_OK)
            {									//如果出现错误
                HAL_FLASH_Lock(); 				//锁定FLASH
                return;
            }
            faddr += 8;     //4字节表示32位
            fdata32 += 2;   //地址向后偏移两个uint32_t宽度
        }
        if ((fdata32Num & 0x1) == 1)       //传入奇数个32位数据
        {
            uint64_t fdata64 = *(uint64_t*)faddr;  //此时fdata64高32位数据为0;

            //搞32位数据或上原来存在的值，低32位存传入的最后一个数据
            fdata64 = (fdata64 & 0xffffffff00000000uL) | (*fdata32);

            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, faddr, fdata64) != HAL_OK)
            {										//如果出现错误
                HAL_FLASH_Lock(); 					//锁定FLASH
                return;
            }
        }
    }
    HAL_FLASH_Lock();
}


/***************************************************************
* 函数名称: Flash_Write64
* 说    明: 向指定地址中写入fdataNum个64位的数据
* 参    数: faddr,Flash起始地址
*			fdata,Flash地址内的64位的数据指针
*			fdata64Num，要写入的字（64位）数的数量
* 返 回 值: 无
***************************************************************/
void Flash_Write64(uint32_t faddr,uint64_t *fdata64, uint32_t fdata64Num)
{
    HAL_FLASH_Unlock();				//解锁FLASH
    if(faddr<FLASH_END)				//写入数据地址小于STM32L431RC的flash地址最大值
    {
        for (uint32_t i = 0; i < fdata64Num; i++)
        {

            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, faddr, *fdata64) != HAL_OK)
            {																						//如果写入异常
                HAL_FLASH_Lock(); 												//上锁FLASH
                return;
            }
            faddr += 8;
            fdata64 += 1;
        }
    }
    HAL_FLASH_Lock();															//上锁FLASH
}


/***************************************************************
* 函数名称: Flash_Read32
* 说    明: 从指定地址开始读出指定长度的数据
* 参    数: ReadAddr,起始地址
*			*pBuffer,数据指针
*			fdata32Num，字（32位）数
* 返 回 值: i, 读取几个数据
***************************************************************/
uint32_t Flash_Read32(uint32_t ReadAddr,uint32_t *fdata32,uint32_t fdata32Num)
{
    uint32_t i;
    for(i = 0; i < fdata32Num; i++)
    {
        if(ReadAddr > FLASH_END || ReadAddr < FLASH_BASE)		//地址越界
        {
            return i;
        }

        fdata32[i] = Flash_ReadOneWord(ReadAddr);	//读取4个字节
        ReadAddr+=4;
    }
    return i;
}


/***************************************************************
* 函数名称: FlashRead
* 说    明: 从指定地址开始读出指定长度的数据
* 参    数: ReadAddr,起始地址
*			*pBuffer，数据指针
*			fdata8Num,字（8位）数
* 返 回 值: 无
***************************************************************/
uint32_t Flash_ReadBytes(uint32_t faddr,uint8_t *fdata8,uint32_t fdata8Num)
{
    uint32_t i;

    for(i=0;i<fdata8Num;i++)
    {
        if(faddr > FLASH_END || faddr < FLASH_BASE)
        {
            return i;
        }
        fdata8[i] = *(__IO uint8_t *)faddr;		//读取1个字节
        faddr+=1;
    }
    return i;
}

void Flash_test()
{
    uint8_t rdata[] = "你好牛逼";
    uint64_t data64 = *(uint64_t *)rdata;

    const uint32_t addr = ADDR;
    Flash_ErasePages(ADDR,1);
    u_print("擦除1页，地址：%x\r\n", addr);


    Flash_WriteDoubleWord(ADDR,data64);
    u_print("地址：%x,双字写入:==%s==\r\n", addr,rdata);

    uint8_t rdata8[10];
    if(Flash_ReadBytes(addr,rdata8,8) == 8)
    {
        u_print("地址：%x,字节读取读取8个字节:==%s==\r\n", addr,rdata8);
    }
    else
    {
        u_print("地址：%x,读取8个字节失败\r\n", addr);
    }

    uint64_t dat64a[2] = { 0x1234567898765432,0x1416321298538754};
    uint32_t rdata32_[4];
    rdata32_[0] = dat64a[0];
    rdata32_[1] = dat64a[0]>>32;
    rdata32_[3] = dat64a[1];
    rdata32_[4] = dat64a[1]>>32;
    // 需要先擦除再写入
    Flash_ErasePages(ADDR,1);
    u_print("擦除1页，地址：%x\r\n", addr);
    Flash_Write64(addr,dat64a, 2);
    u_print("地址%x,写入%x,%x,%x,%x\r\n",addr,rdata32_[0],rdata32_[1],rdata32_[2],rdata32_[3]);




    uint32_t rdata32[4];
    Flash_Read32(addr,rdata32,4);
    u_print("地址：%x,字连续读取 ==%x, %x, %x, %x==\r\n", addr,rdata32[0],rdata32[1],rdata32[2],rdata32[3]);


}



