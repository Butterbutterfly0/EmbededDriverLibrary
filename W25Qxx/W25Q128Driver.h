#ifndef __W25Q128DRIVER_H__
#define __W25Q128DRIVER_H__
#include <stdint.h>
typedef struct
{
	uint8_t BUSY_BIT:1;
	//When Device is busy ,the bit will be set to 1
	uint8_t WEL_BIT:1;
	//When Device is Write enabled ,the bit will be set to 1
	uint8_t BP0_BIT:1;
	uint8_t BP1_BIT:1;
	uint8_t BP2_BIT:1;
	//When Device is Write protected ,the bit will be set to 1.
	//By Writing Status register ,Can this bit be set to 1
	uint8_t TB_BIT:1;
	uint8_t SEC_BIT:1;
	uint8_t reserved:1;
}
__W25Qxx_Status_1_Register_t;

typedef struct
{
	uint8_t SRL_BIT:1;
	uint8_t QE_BIT:1;
	uint8_t reserved:1;
	uint8_t LB1_BIT:1;
	uint8_t LB2_BIT:1;
	uint8_t LB3_BIT:1;
	uint8_t CMP_BIT:1;
	uint8_t SBUS_BIT:1;
}
__W25Qxx_Status_2_Register_t;


typedef struct
{
	uint8_t _reserved:1;
	uint8_t __reserved:1;
	uint8_t WPS_BIT:1;
	uint8_t ___reserved:1;
	uint8_t ____reserved:1;
	uint8_t DRV2_BIT:1;
	uint8_t DRV1_BIT:1;
	uint8_t _____reserved:1;
}
__W25Qxx_Status_3_Register_t;

typedef struct W25Qxx
{
	__W25Qxx_Status_1_Register_t status_1_reg;
	__W25Qxx_Status_2_Register_t status_2_reg;
	__W25Qxx_Status_3_Register_t status_3_reg;
	uint8_t Manu_ID;
	uint8_t Dev_ID;
	uint64_t Unique_ID;
	
}W25Qxx_obj;


void W25Qxx_Read_Status(W25Qxx_obj* this);
void W25Qxx_Read_ID(W25Qxx_obj* this);
void W25Qxx_Write_Enable(W25Qxx_obj* this);
void W25Qxx_Write_Disable(W25Qxx_obj* this);
void W25Qxx_Read_Data(W25Qxx_obj* this,uint32_t Add_24bit,uint8_t *rx_data,uint16_t Size);
void W25Qxx_Page_Write(W25Qxx_obj* this,uint32_t Add_24bit,uint8_t *write_data,uint16_t Size);
void W25Qxx_Sector_Earse(W25Qxx_obj* this,uint32_t Add_24bit);
void W25Qxx_Block_32KB_Earse(W25Qxx_obj* this,uint32_t Add_24bit);
void W25Qxx_Block_64KB_Earse(W25Qxx_obj* this,uint32_t Add_24bit);
void W25Qxx_Chip_Earse(W25Qxx_obj* this,uint32_t Add_24bit);
void W25Qxx_Earse_Program_Suspend(W25Qxx_obj* this);
void W25Qxx_Earse_Program_Resume(W25Qxx_obj* this);
void W25Qxx_Power_Down(W25Qxx_obj* this);
void W25Qxx_Release(W25Qxx_obj* this);
void W25Qxx_Enable_Reset(W25Qxx_obj* this);
void W25Qxx_Reset_Device(W25Qxx_obj* this);

#endif