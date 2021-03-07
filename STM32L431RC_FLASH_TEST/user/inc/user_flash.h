//
// Created by luozw on 2021/3/7.
//

#ifndef STM32L431RC_FLASH_TEST_USER_FLASH_H
#define STM32L431RC_FLASH_TEST_USER_FLASH_H
#include "stm32l4xx_hal.h"

#define ADDR 0x0803f800	//写地址
void Flash_ErasePages(uint32_t faddr, uint32_t nbpage);

/***************************************************************
* 函数名称:   Flash_WriteOneWord
* 说    明:  向指定地址中写入一个32位的数据
* 参    数:  faddr,Flash地址
*		    fdata32,Flash地址内的一个32位的数据
* 返 回 值: 写FLASH返回的状态
***************************************************************/
uint8_t Flash_WriteOneWord(uint32_t faddr,uint32_t fdata32);

/***************************************************************
* 函数名称: Flash_WriteDoubleWord
* 说    明: 向指定地址中写入一个双字（64位）数据
* 参    数: faddr,Flash地址
*		    fdata64,Flash地址内的一个双字（64位）数据
* 返 回 值: 写FLASH返回的状态
***************************************************************/
uint8_t Flash_WriteDoubleWord(uint32_t faddr, uint64_t fdata64);

/***************************************************************
* 函数名称: Flash_Write32
* 说    明: 向指定地址中写入fdataNum个32位的数据
* 参    数: faddr,Flash起始地址
*			fdata,Flash地址内的32位的数据指针
*			fdata32Num,要写入的字（32位）数的数量
* 返 回 值: 无
***************************************************************/
void Flash_Write32(uint32_t faddr,uint32_t *fdata32, uint32_t fdata32Num);

/***************************************************************
* 函数名称: Flash_Write64
* 说    明: 向指定地址中写入fdataNum个64位的数据
* 参    数: faddr,Flash起始地址
*			fdata,Flash地址内的64位的数据指针
*			fdata64Num，要写入的字（64位）数的数量
* 返 回 值: 无
***************************************************************/
void Flash_Write64(uint32_t faddr,uint64_t *fdata64, uint32_t fdata64Num);

void Flash_test(void);
#endif //STM32L431RC_FLASH_TEST_USER_FLASH_H
