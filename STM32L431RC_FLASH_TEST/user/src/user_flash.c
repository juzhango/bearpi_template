//
// Created by luozw on 2021/3/7.
//

#include "user_flash.h"
#include "user_debug.h"
/* ��ȡ��ַ���ڵ�bank */
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
* ��������: Flash_GetPage
* ˵    ��: ��ô����ַ����Page
* ��    ��: faddr,Flash��ַ
* �� �� ֵ: Bankֵ
***************************************************************/
static uint32_t Flash_GetPage(uint32_t faddr)
{
    if (faddr < (FLASH_BASE + FLASH_BANK_SIZE))
        return (faddr - FLASH_BASE) / FLASH_PAGE_SIZE;
    else
        return (faddr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
}

/***************************************************************
* ��������: Flash_ReadOneWord
* ˵    ��: ��ָ����ַ�ж�ȡһ��32λ������
* ��    ��: faddr,Flash��ַ
* �� �� ֵ: Flash��ַ�ڵ�һ��32λ������
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
* ��������:   Flash_WriteOneWord
* ˵    ��:  ��ָ����ַ��д��һ��32λ������
* ��    ��:  faddr,Flash��ַ
*		    fdata32,Flash��ַ�ڵ�һ��32λ������
* �� �� ֵ: дFLASH���ص�״̬
***************************************************************/
uint8_t Flash_WriteOneWord(uint32_t faddr,uint32_t fdata32)
{
    if(faddr > FLASH_END || faddr < FLASH_BASE)										//�Ƿ���ַ
    {
        return HAL_ERROR;
    }

    uint8_t FLASH_STATUS;
    uint64_t fdata64;

    HAL_FLASH_Unlock();
    fdata64 = fdata32|0xffffffff00000000uL;
    FLASH_STATUS = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,faddr,fdata64);//д������
    HAL_FLASH_Lock();

    return FLASH_STATUS;
}

/***************************************************************
* ��������: Flash_WriteDoubleWord
* ˵    ��: ��ָ����ַ��д��һ��˫�֣�64λ������
* ��    ��: faddr,Flash��ַ
*		    fdata64,Flash��ַ�ڵ�һ��˫�֣�64λ������
* �� �� ֵ: дFLASH���ص�״̬
***************************************************************/
uint8_t Flash_WriteDoubleWord(uint32_t faddr, uint64_t fdata64)
{
    if(faddr > FLASH_END || faddr < FLASH_BASE)			//�Ƿ���ַ
    {
        return HAL_ERROR;
    }
    uint8_t FLASH_STATUS;

    HAL_FLASH_Unlock();									//����FLASH
    FLASH_STATUS = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,faddr,fdata64);//д������
    HAL_FLASH_Lock();									//����
    return FLASH_STATUS;
}


/***************************************************************
* ��������: Flash_Write32
* ˵    ��: ��ָ����ַ��д��fdataNum��32λ������
* ��    ��: faddr,Flash��ʼ��ַ
*			fdata,Flash��ַ�ڵ�32λ������ָ��
*			fdata32Num,Ҫд����֣�32λ����������
* �� �� ֵ: ��
***************************************************************/
void Flash_Write32(uint32_t faddr,uint32_t *fdata32, uint32_t fdata32Num)
{
    uint64_t fdata64;
    HAL_FLASH_Unlock();						//����FLASH
    if(faddr<FLASH_END)						//д�����ݵ�ַС��STM32L431RC��flash��ַ���ֵ
    {
        for (uint32_t i = 0; i < fdata32Num / 2; i++)
        {
            fdata64 = *(uint64_t*)fdata32;
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, faddr, fdata64) != HAL_OK)
            {									//������ִ���
                HAL_FLASH_Lock(); 				//����FLASH
                return;
            }
            faddr += 8;     //4�ֽڱ�ʾ32λ
            fdata32 += 2;   //��ַ���ƫ������uint32_t���
        }
        if ((fdata32Num & 0x1) == 1)       //����������32λ����
        {
            uint64_t fdata64 = *(uint64_t*)faddr;  //��ʱfdata64��32λ����Ϊ0;

            //��32λ���ݻ���ԭ�����ڵ�ֵ����32λ�洫������һ������
            fdata64 = (fdata64 & 0xffffffff00000000uL) | (*fdata32);

            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, faddr, fdata64) != HAL_OK)
            {										//������ִ���
                HAL_FLASH_Lock(); 					//����FLASH
                return;
            }
        }
    }
    HAL_FLASH_Lock();
}


/***************************************************************
* ��������: Flash_Write64
* ˵    ��: ��ָ����ַ��д��fdataNum��64λ������
* ��    ��: faddr,Flash��ʼ��ַ
*			fdata,Flash��ַ�ڵ�64λ������ָ��
*			fdata64Num��Ҫд����֣�64λ����������
* �� �� ֵ: ��
***************************************************************/
void Flash_Write64(uint32_t faddr,uint64_t *fdata64, uint32_t fdata64Num)
{
    HAL_FLASH_Unlock();				//����FLASH
    if(faddr<FLASH_END)				//д�����ݵ�ַС��STM32L431RC��flash��ַ���ֵ
    {
        for (uint32_t i = 0; i < fdata64Num; i++)
        {

            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, faddr, *fdata64) != HAL_OK)
            {																						//���д���쳣
                HAL_FLASH_Lock(); 												//����FLASH
                return;
            }
            faddr += 8;
            fdata64 += 1;
        }
    }
    HAL_FLASH_Lock();															//����FLASH
}


/***************************************************************
* ��������: Flash_Read32
* ˵    ��: ��ָ����ַ��ʼ����ָ�����ȵ�����
* ��    ��: ReadAddr,��ʼ��ַ
*			*pBuffer,����ָ��
*			fdata32Num���֣�32λ����
* �� �� ֵ: i, ��ȡ��������
***************************************************************/
uint32_t Flash_Read32(uint32_t ReadAddr,uint32_t *fdata32,uint32_t fdata32Num)
{
    uint32_t i;
    for(i = 0; i < fdata32Num; i++)
    {
        if(ReadAddr > FLASH_END || ReadAddr < FLASH_BASE)		//��ַԽ��
        {
            return i;
        }

        fdata32[i] = Flash_ReadOneWord(ReadAddr);	//��ȡ4���ֽ�
        ReadAddr+=4;
    }
    return i;
}


/***************************************************************
* ��������: FlashRead
* ˵    ��: ��ָ����ַ��ʼ����ָ�����ȵ�����
* ��    ��: ReadAddr,��ʼ��ַ
*			*pBuffer������ָ��
*			fdata8Num,�֣�8λ����
* �� �� ֵ: ��
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
        fdata8[i] = *(__IO uint8_t *)faddr;		//��ȡ1���ֽ�
        faddr+=1;
    }
    return i;
}

void Flash_test()
{
    uint8_t rdata[] = "���ţ��";
    uint64_t data64 = *(uint64_t *)rdata;

    const uint32_t addr = ADDR;
    Flash_ErasePages(ADDR,1);
    u_print("����1ҳ����ַ��%x\r\n", addr);


    Flash_WriteDoubleWord(ADDR,data64);
    u_print("��ַ��%x,˫��д��:==%s==\r\n", addr,rdata);

    uint8_t rdata8[10];
    if(Flash_ReadBytes(addr,rdata8,8) == 8)
    {
        u_print("��ַ��%x,�ֽڶ�ȡ��ȡ8���ֽ�:==%s==\r\n", addr,rdata8);
    }
    else
    {
        u_print("��ַ��%x,��ȡ8���ֽ�ʧ��\r\n", addr);
    }

    uint64_t dat64a[2] = { 0x1234567898765432,0x1416321298538754};
    uint32_t rdata32_[4];
    rdata32_[0] = dat64a[0];
    rdata32_[1] = dat64a[0]>>32;
    rdata32_[3] = dat64a[1];
    rdata32_[4] = dat64a[1]>>32;
    // ��Ҫ�Ȳ�����д��
    Flash_ErasePages(ADDR,1);
    u_print("����1ҳ����ַ��%x\r\n", addr);
    Flash_Write64(addr,dat64a, 2);
    u_print("��ַ%x,д��%x,%x,%x,%x\r\n",addr,rdata32_[0],rdata32_[1],rdata32_[2],rdata32_[3]);




    uint32_t rdata32[4];
    Flash_Read32(addr,rdata32,4);
    u_print("��ַ��%x,��������ȡ ==%x, %x, %x, %x==\r\n", addr,rdata32[0],rdata32[1],rdata32[2],rdata32[3]);


}



