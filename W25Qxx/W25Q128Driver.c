#include "W25Q128Driver.h"

/*
Dear visitor ,if you want use the library for other use
What you should do is to modify the function definition between BEGIN and END
Replace inner code with the correct code that fit your Platform and operation system
if you didn't use any system ,just leave the memory related function remain
the original code is on STM32 Platform 
you should choose similar function to replace it
the connectivity is SPI 
good luck
*/


#include "main.h"
#include <string.h>

#define READ_STATUS_1_REG 0x05
#define READ_STATUS_2_REG 0x35
#define READ_STATUS_3_REG 0x15
#define READ_MANU_AND_DEVICE_ID_REG 0x90
#define READ_UNIQUE_ID_REG 0x4B
#define WRITE_ENABLE_REG 0x06
#define WRITE_DISABLE_REG 0x04
#define DATA_READ_REG 0x03
#define PAGE_PROGRAM_REG 0x02
#define SECTOR_EARSE_REG 0x20
#define BLOCK_32KB_EARSE_REG 0x52
#define BLOCK_64KB_EARSE_REG 0xD8
#define CHIP_EARSE_REG 0xC7//0x60 ok too
#define EARSE_PROGRAM_SUSPEND_REG 0x75
#define EARSE_PROGRAM_RESUME_REG 0x7A
#define POWER_DOWN_REG 0xB9
#define RELEASE_DEVICE_ID_REG 0xAB
#define ENABLE_RESET_REG 0x66
#define RESET_DEVICE_REG 0x99

/********************************/
//Begin
/********************************/
//User Modified Part According various platform and operation system

//Dynamic Memory Allocate
#include <stdlib.h>
static void *Memclloc(size_t Size)
{
	return malloc(Size);
}


//Heap Memory Free
static void MemeFree(void* ptr)
{
	free(ptr);
}


//SPI Connectivity 
extern SPI_HandleTypeDef hspi1;
static void SPI_Send(uint8_t *Tx_Data_Ptr,uint16_t Tx_Size)
{
	HAL_SPI_Transmit(&hspi1,Tx_Data_Ptr,Tx_Size,10);
}


static void SPI_Recv(uint8_t *Rx_Data_Ptr,uint16_t Rx_Size)
{
	HAL_SPI_Receive(&hspi1,Rx_Data_Ptr,Rx_Size,10);
}

//Chip Select 
static void CS_Low(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
}

//Chip Release
static void CS_High(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
}

/********************************/
//End
/********************************/




//Transform interger to bool
static uint8_t To_Bool(uint32_t in)
{
	if(in!=0x00)
		return 1;
	else
		return 0;
}


//Read three status register
void W25Qxx_Read_Status(W25Qxx_obj* this)
{
	uint8_t tx_data[1]={READ_STATUS_1_REG};
	uint8_t rx_data[1];
	CS_Low();
	SPI_Send(tx_data,1);
	SPI_Recv(rx_data,1);
	CS_High();
	this->status_1_reg.BUSY_BIT=To_Bool(rx_data[0]&0x01);
	this->status_1_reg.WEL_BIT=To_Bool(rx_data[0]&0x02);
	this->status_1_reg.BP0_BIT=To_Bool(rx_data[0]&0x04);
	this->status_1_reg.BP1_BIT=To_Bool(rx_data[0]&0x08);
	this->status_1_reg.BP2_BIT=To_Bool(rx_data[0]&0x10);
	this->status_1_reg.TB_BIT=To_Bool(rx_data[0]&0x20);
	this->status_1_reg.SEC_BIT=To_Bool(rx_data[0]&0x40);
	tx_data[0]=READ_STATUS_2_REG;
	rx_data[0]=0;
	CS_Low();
	SPI_Send(tx_data,1);
	SPI_Recv(rx_data,1);
	CS_High();
	this->status_2_reg.SRL_BIT=To_Bool(rx_data[0]&0x01);
	this->status_2_reg.QE_BIT=To_Bool(rx_data[0]&0x02);
	this->status_2_reg.LB1_BIT=To_Bool(rx_data[0]&0x08);
	this->status_2_reg.LB2_BIT=To_Bool(rx_data[0]&0x10);
	this->status_2_reg.LB3_BIT=To_Bool(rx_data[0]&0x20);
	this->status_2_reg.CMP_BIT=To_Bool(rx_data[0]&0x40);
	this->status_2_reg.SBUS_BIT=To_Bool(rx_data[0]&0x80);
	tx_data[0]=READ_STATUS_3_REG;
	rx_data[0]=0;
	CS_Low();
	SPI_Send(tx_data,1);
	SPI_Recv(rx_data,1);
	CS_High();
	this->status_3_reg.WPS_BIT=To_Bool(rx_data[0]&0x04);
	this->status_3_reg.DRV2_BIT=To_Bool(rx_data[0]&0x20);
	this->status_3_reg.DRV1_BIT=To_Bool(rx_data[0]&0x40);
}


//Read Manufacturer ID and device ID
void W25Qxx_Read_ID(W25Qxx_obj* this)
{
	uint8_t tx_data[4]={READ_MANU_AND_DEVICE_ID_REG,0x00,0x00,0x00};
	uint8_t rx_data[2];
	CS_Low();
	SPI_Send(tx_data,4);
	SPI_Recv(rx_data,2);
	CS_High();
	this->Manu_ID=rx_data[0];
	this->Dev_ID=rx_data[1];
	tx_data[0]=READ_UNIQUE_ID_REG;
	uint8_t rx_2[12];
	CS_Low();
	SPI_Send(tx_data,1);
	SPI_Recv(rx_data,12);
	CS_High();
	this->Unique_ID=(uint64_t)((rx_2[4])|(rx_2[5]<<8)|(rx_2[6]<<16)|(rx_2[7]<<24)|(rx_2[8]<<32)|(rx_2[9]<<40)|(rx_2[10]<<48)|(rx_2[11]<<56));
}


//Enable Write
void W25Qxx_Write_Enable(W25Qxx_obj* this)
{
	uint8_t tx_data[1]={WRITE_ENABLE_REG};
	CS_Low();
	SPI_Send(tx_data,1);
	CS_High();
}


//Disable Write
void W25Qxx_Write_Disable(W25Qxx_obj* this)
{
	uint8_t tx_data[1]={WRITE_DISABLE_REG};
	CS_Low();
	SPI_Send(tx_data,1);
	CS_High();
}


//Read Data From Flash
//Be Careful the size you choose must be less than the maximum of the type
void W25Qxx_Read_Data(W25Qxx_obj* this,uint32_t Add_24bit,uint8_t *rx_data,uint16_t Size)
{
	uint8_t tx_data[4]={DATA_READ_REG,(Add_24bit>>16)&0xFF,(Add_24bit>>8)&0xFF,Add_24bit&0xFF};
	CS_Low();
	SPI_Send(tx_data,4);
	SPI_Recv(rx_data,Size);
	CS_High();
}


//Write Data to the Given Address
void W25Qxx_Page_Write(W25Qxx_obj* this,uint32_t Add_24bit,uint8_t *write_data,uint16_t Size)
{
	uint8_t tx[4]={PAGE_PROGRAM_REG,(Add_24bit>>16)&0xFF,(Add_24bit>>8)&0xFF,Add_24bit&0xFF};
	uint8_t *tx_data=Memclloc(sizeof(uint8_t)*(8+Size));
	memcpy(tx_data,tx,sizeof(uint8_t)*4);
	memcpy(tx_data+4,write_data,sizeof(uint8_t)*Size);
	CS_Low();
	SPI_Send(tx_data,Size+4);
	CS_High();
	MemeFree(tx_data);
}


//Earse Sector
void W25Qxx_Sector_Earse(W25Qxx_obj* this,uint32_t Add_24bit)
{
	uint8_t tx_data[4]={SECTOR_EARSE_REG,(Add_24bit>>16)&0xFF,(Add_24bit>>8)&0xFF,Add_24bit&0xFF};
	CS_Low();
	SPI_Send(tx_data,4);
	CS_High();
}


//Earse 32KB Block
void W25Qxx_Block_32KB_Earse(W25Qxx_obj* this,uint32_t Add_24bit)
{
	uint8_t tx_data[4]={BLOCK_32KB_EARSE_REG,(Add_24bit>>16)&0xFF,(Add_24bit>>8)&0xFF,Add_24bit&0xFF};
	CS_Low();
	SPI_Send(tx_data,4);
	CS_High();
}

	
//Earse 64KB Block
void W25Qxx_Block_64KB_Earse(W25Qxx_obj* this,uint32_t Add_24bit)
{
	uint8_t tx_data[4]={BLOCK_64KB_EARSE_REG,(Add_24bit>>16)&0xFF,(Add_24bit>>8)&0xFF,Add_24bit&0xFF};
	CS_Low();
	SPI_Send(tx_data,4);
	CS_High();
}


//Earse the whole Chip
void W25Qxx_Chip_Earse(W25Qxx_obj* this,uint32_t Add_24bit)
{
	uint8_t tx_data[4]={CHIP_EARSE_REG,(Add_24bit>>16)&0xFF,(Add_24bit>>8)&0xFF,Add_24bit&0xFF};
	CS_Low();
	SPI_Send(tx_data,4);
	CS_High();
}


//Suspend the opeartion
void W25Qxx_Earse_Program_Suspend(W25Qxx_obj* this)
{
	uint8_t tx_data[1]={EARSE_PROGRAM_SUSPEND_REG};
	CS_Low();
	SPI_Send(tx_data,1);
	CS_High();
}


//Resume the operation
void W25Qxx_Earse_Program_Resume(W25Qxx_obj* this)
{
	uint8_t tx_data[1]={EARSE_PROGRAM_RESUME_REG};
	CS_Low();
	SPI_Send(tx_data,1);
	CS_High();
}


//Power Down the chip Until Be Released
void W25Qxx_Power_Down(W25Qxx_obj* this)
{
	uint8_t tx_data[1]={POWER_DOWN_REG};
	CS_Low();
	SPI_Send(tx_data,1);
	CS_High();
}


//Release the Power Down Chip
void W25Qxx_Release(W25Qxx_obj* this)
{
	uint8_t tx_data[1]={RELEASE_DEVICE_ID_REG};
	CS_Low();
	SPI_Send(tx_data,1);
	CS_High();
}


//Enable the Reset Function
void W25Qxx_Enable_Reset(W25Qxx_obj* this)
{
	uint8_t tx_data[1]={ENABLE_RESET_REG};
	CS_Low();
	SPI_Send(tx_data,1);
	CS_High();
}


//Reset the Device
void W25Qxx_Reset_Device(W25Qxx_obj* this)
{
	uint8_t tx_data[1]={RESET_DEVICE_REG};
	CS_Low();
	SPI_Send(tx_data,1);
	CS_High();
}
