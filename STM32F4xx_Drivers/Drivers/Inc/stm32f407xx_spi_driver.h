/*
 * stm32f407xx_spi_driver.h
 *
 *  Created on: Sep 5, 2024
 *      Author: Lenovo
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_
#include "stm32f407xx.h"

//Configuration structure for SPI
typedef struct{
	uint8_t SPI_Devicemode;//@SPI_Devicemode possible values
	uint8_t SPI_BusConfig;//@SPI_BusConfig possible values
	uint8_t SPI_Speed;//@SPI CLock configurations values
	uint8_t SPI_DFF;//@SPI_DFF mode values
	uint8_t SPI_CPOL;//@SPI_CPOL
	uint8_t SPI_CPHA;//@SPI_CPHA
	uint8_t SPI_SSM;//@SPI_SSM
}SPI_Pin_Config;

//Handle structure for SPI

typedef struct {
	SPI_reg *pSPIx;// This  SPI_reg type pointer holds the base address of SPI register
	SPI_Pin_Config spi_pin_config;
	uint8_t  *pTxBuffer;
	uint8_t  *pRxBuffer;
	uint32_t  TxLen;
	uint32_t  RxLen;
	uint8_t   TxState;
	uint8_t   RxState;


}SPI_Handle;

/******Define macros for the SPI pin utilized*******/
//@SPI_Devicemode possible values
#define SPI_DEVICE_MODE_MASTER                     1
#define SPI_DEVICE_MODE_SLAVE                      0

//@SPI_BusConfig possible values
#define SPI_BUS_CONFIG_FD                          1
#define SPI_BUS_CONFIG_HD                          2
#define SPI_BUS_CONFIG_SIMPLEX_RX_ONLY             3

//

//@SPI CLock configurations values
#define SPI_CLK_SPEED_DIV2                         0
#define SPI_CLK_SPEED_DIV4                         1
#define SPI_CLK_SPEED_DIV8                         2
#define SPI_CLK_SPEED_DIV16                        3
#define SPI_CLK_SPEED_DIV32                        4
#define SPI_CLK_SPEED_DIV64                        5
#define SPI_CLK_SPEED_DIV128                       6
#define SPI_CLK_SPEED_DIV256                       7


//@SPI_DFF mode values
#define SPI_DFF_8BITS                              0
#define SPI_DFF_16BITS                             1


//@SPI_CPOL
#define SPI_CPOL_HIGH                              1
#define SPI_CPOL_LOW                               0

////@SPI_CPHA
#define SPI_CPHA_HIGH                              1
#define SPI_CPHA_LOW                               0

//@SPI_SSM
#define SPI_SSM_EN                                 1
#define SPI_SSM_DI                                 0

//SPI related to status flag definitions
#define SPI_TXE_FLAG                               (1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG                              (1 << SPI_SR_RXNE)
#define SPI_BUSY_FLAG                              (1 << SPI_SR_BSY)

//Possible application states
#define SPI_READY								   0
#define SPI_BUSY_IN_RX							   1
#define SPI_BUSY_IN_TX							   2

//Possible SPI applications events
#define SPI_EVENT_TX_COMPLETE					   1
#define SPI_EVENT_RX_COMPLETE				       2
#define SPI_EVENT_OVR_ERR						   3
#define SPI_EVENT_CRC_ERR						   4


/***********Prototypes for the functions or APIs used in SPI driver **************/


//Peripheral clock setup
void SPI_PeriClckInit(SPI_reg *pSPIx,uint8_t EnorDi);

//SPI peripheral control API
void SPI_PeripheralControl(SPI_reg *pSPIx,uint8_t EnorDi);

//Inint and deinit API
void SPI_init(SPI_Handle *pSPIHandle);
void SPI_Deinit(SPI_reg *pSPIx);

//SSI configuration
void Spi_SSIConfig(SPI_reg *pSPIx,uint8_t EnorDi);

//Flag status
uint8_t SPI_GetFLagStatus(SPI_reg *pSPIx, uint32_t FlagName);

//SSOE configuration
void Spi_SSOEConfig(SPI_reg *pSPIx,uint8_t EnorDi);
//Data send and receive
void SPI_SendData(SPI_reg *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_reg *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

//Data send and receive IT
uint8_t SPI_SendDataIT(SPI_Handle *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataIT(SPI_Handle *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len);


//SPI interrupt & handler
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle *pSPIHandle);

//Application callback
void SPI_ApplicationEventCallback(SPI_Handle *pSPIHandle,uint8_t AppEv);

void SPI_ClearOVRFlag(SPI_reg *pSPIx);
void SPI_CloseTransmission(SPI_Handle *pSPIHandle);
void SPI_CloseReception(SPI_Handle *pSPIHandle);
//Other SPI management APIs


#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
