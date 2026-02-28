/*
 * i2c_driver.h
 *
 *  Created on: Jan 27, 2025
 *      Author: Lenovo
 */

#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_
#include "stm32f407xx.h"


//Configuration structure for I2Cx peripheral
typedef struct{
	uint32_t I2C_SCLSpeed;
	uint8_t	 I2C_DeviceAddress;
	uint8_t	 I2C_ACKControl;
	uint8_t I2C_FMDutyCycle;
}I2C_Config;

//Handle structure for I2Cx peripheral
typedef struct{
	I2C_reg *pI2Cx;
	I2C_Config I2C_Config;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxRxState;
	uint8_t DevAddr;
	uint32_t RxSize;
	uint8_t  Sr;
}I2C_Handle;

//@I2C_SCLSpeed
#define I2C_SCL_SPEED_SM		100000
#define I2C_SCL_SPEED_FM2K		200000
#define I2C_SCL_SPEED_FM4K		400000

//@I2C_ACKControl
#define I2C_ACK_ENABLE			1
#define I2C_ACK_DISABLE			0

//@I2C_FMDutyCycle
#define I2C_FM_DUTY_2			0
#define I2C_FM_DUTY_16_9		1

#define I2C_DISABLE_SR					RESET
#define I2C_ENABLE_SR					SET

//I2C related to status flag definitions
#define I2C_TXE_FLAG                               	(1 << I2C_SR1_TXE)
#define I2C_RXNE_FLAG                              	(1 << I2C_SR1_RXNE)
#define I2C_SB_FLAG								   	(1 << I2C_SR1_SB)
#define I2C_ADDR_FLAG							   	(1<< I2C_SR1_ADDR)
#define I2C_BTF_FLAG							   	(1<< I2C_SR1_BTF)
#define I2C_STOPF_FLAG							   	(1<< I2C_SR1_STOPF)
#define I2C_ARLO_FLAG							   	(1<< I2C_SR1_ARLO)
#define I2C_AF_FLAG							   		(1<< I2C_SR1_AF)
#define I2C_OVR_FLAG							   	(1<< I2C_SR1_OVR)
#define I2C_TIMEOUT_FLAG							(1<< I2C_SR1_TIMEOUT)

#define I2C_READY									0
#define I2C_BUSY_IN_RX								1
#define I2C_BUSY_IN_TX								2

#define I2C_EV_TX_CMPLT								0
#define I2C_EV_STOP									1
#define I2C_EV_RX_CMPLT								2
#define I2C_ERROR_BERR								3
#define I2C_ERROR_ARLO								4
#define I2C_ERROR_AF								5
#define I2C_ERROR_OVR								6
#define I2C_ERROR_TIMEOUT							7
#define I2C_EV_DATA_REQ								8
#define I2C_EV_DATA_RCV								9
/***********Prototypes for the functions or APIs used in I2C driver **************/


//Peripheral clock setup
void I2C_PeriClckInit(I2C_reg *pI2Cx,uint8_t EnorDi);

//I2C peripheral control API
void I2C_PeripheralControl(I2C_reg *pI2Cx,uint8_t EnorDi);
void I2C_ManageAcking(I2C_reg *pI2Cx,uint8_t EnorDi);


//Inint and deinit API
void I2C_init(I2C_Handle *pI2CHandle);
void I2C_Deinit(I2C_reg *pI2Cx);

//Data send and receive
void I2C_MasterSendData(I2C_Handle *pI2CHandle, uint8_t *pTxbuffer, uint32_t len, uint8_t SlaveAddr, uint8_t Sr);
void I2C_MasterReceiveData(I2C_Handle *pI2CHandle, uint8_t *pRxbuffer, uint32_t len, uint8_t SlaveAddr, uint8_t Sr);

//Data send and receive using interrupt
uint8_t I2C_MasterSendDataIT(I2C_Handle *pI2CHandle, uint8_t *pTxbuf, uint32_t len, uint8_t SlaveAddr, uint8_t Sr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle *pI2CHandle, uint8_t *pRxbuf, uint32_t len, uint8_t SlaveAddr, uint8_t Sr);


//Flag status
uint8_t I2C_GetFLagStatus(I2C_reg *pI2Cx, uint32_t FlagName);


//I2C interrupt & handler
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

void I2C_EV_IRQHandling(I2C_Handle *pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle *pI2CHandle);

//Application callback
void I2C_ApplicationEventCallback(I2C_Handle *pI2CHandle,uint8_t AppEv);

void I2C_CloseSendData(I2C_Handle *pI2CHandle);
void I2C_CloseReceiveData(I2C_Handle *pI2CHandle);

void I2C_SlaveSendData(I2C_reg *pI2C, uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_reg *pI2C);


void I2C_GenerateStopCondition(I2C_reg *pI2Cx);
void I2c_SlaveEnableDisableCallbackEvents(I2C_reg *pI2Cx,uint8_t EnorDi);

#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */
