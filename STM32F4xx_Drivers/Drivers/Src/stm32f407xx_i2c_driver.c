/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: Jan 27, 2025
 *      Author: Lenovo
 */
#include "stm32f407xx_i2c_driver.h"
#include<stdint.h>

static void I2C_GenerateStartCondition(I2C_reg *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_reg *pI2Cx, uint8_t SlaveAddr);
static void I2C_ExecuteAddressPhaseRead(I2C_reg *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_Handle *pI2CHandle);
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle *pI2CHandle);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle *pI2CHandle);


//Helper functions
static void I2C_GenerateStartCondition(I2C_reg *pI2Cx) {
	pI2Cx->I2C_CR1 |= (1 << I2C_CR1_START);
}
void I2C_GenerateStopCondition(I2C_reg *pI2Cx) {
	pI2Cx->I2C_CR1 |= (1 << I2C_CR1_STOP);
}

static void I2C_ExecuteAddressPhaseWrite(I2C_reg *pI2Cx, uint8_t SlaveAddr) {
	SlaveAddr <<= 1;
	SlaveAddr &= ~(1); // SlaveAddr is slave address + r/w bit=0
	pI2Cx->I2C_DR = SlaveAddr;
}
static void I2C_ExecuteAddressPhaseRead(I2C_reg *pI2Cx, uint8_t SlaveAddr) {
	SlaveAddr <<= 1;
	SlaveAddr |= 1;
	pI2Cx->I2C_DR = SlaveAddr;
}
static void I2C_ClearADDRFlag(I2C_Handle *pI2CHandle) {
	uint32_t dummy_read;
	//Check for device mode
	if (pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL)) {
		//Master mode
		if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX) {
			if (pI2CHandle->RxSize == 1) {

				//First disable the ack
				I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);

				//Clear the ARR
				dummy_read = pI2CHandle->pI2Cx->I2C_SR1;
				dummy_read = pI2CHandle->pI2Cx->I2C_SR2;
				(void) dummy_read;
			}
		} else {
			//Clear the ARR
			dummy_read = pI2CHandle->pI2Cx->I2C_SR1;
			dummy_read = pI2CHandle->pI2Cx->I2C_SR2;
			(void) dummy_read;
		}
	} else {
		//slave mode

		//Clear the ARR
		dummy_read = pI2CHandle->pI2Cx->I2C_SR1;
		dummy_read = pI2CHandle->pI2Cx->I2C_SR2;
		(void) dummy_read;
	}
}
void I2c_SlaveEnableDisableCallbackEvents(I2C_reg *pI2Cx,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITEVTEN);
		pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITBUFEN);
		pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITERREN);
	}else
	{
		pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITEVTEN);
		pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITBUFEN);
		pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITERREN);
	}
}

void I2C_CloseSendData(I2C_Handle *pI2CHandle)
{
	//Disable ITBUFEN Control Bit
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Disable ITEVFEN Control Bit
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen = 0;

}


void I2C_CloseReceiveData(I2C_Handle *pI2CHandle)
{
	//Disable ITBUFEN Control Bit
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Disable ITEVFEN Control Bit
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxLen = 0;
	pI2CHandle->RxSize = 0;
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
	}
}

void I2C_PeriClckInit(I2C_reg *pI2Cx, uint8_t EnorDi) {
	if (EnorDi == ENABLE) {
		if (pI2Cx == I2C1) {
			I2C1_PERI_CLOCK_ENABLE();
		} else if (pI2Cx == I2C2) {
			I2C2_PERI_CLOCK_ENABLE();
		} else {
			I2C3_PERI_CLOCK_ENABLE();
		}
	} else {
		if (pI2Cx == I2C1) {
			I2C1_PERI_CLOCK_DISABLE();
		} else if (pI2Cx == I2C2) {
			I2C2_PERI_CLOCK_DISABLE();
		} else {
			I2C3_PERI_CLOCK_DISABLE();
		}
	}
}

void I2C_PeripheralControl(I2C_reg *pI2Cx, uint8_t EnorDi) {
	if (EnorDi == ENABLE) {
		pI2Cx->I2C_CR1 |= (1 << I2C_CR1_PE); //Setting the I2C enable bit
	} else {
		pI2Cx->I2C_CR1 &= ~(1 << I2C_CR1_PE); //Clearing the I2C enable bit
	}
}

void I2C_init(I2C_Handle *pI2CHandle) {
	uint32_t temp_reg = 0;

	//Enable the clock for i2cx peripheral;
	I2C_PeriClckInit(pI2CHandle->pI2Cx, ENABLE);

	//1. Ack control bit
	temp_reg |= pI2CHandle->I2C_Config.I2C_ACKControl << 10;
	pI2CHandle->pI2Cx->I2C_CR1 = temp_reg;

	//2. configure the FREQ field of CR2
	temp_reg = 0;
	temp_reg |= RCC_GetPCLK1Value() / 1000000U;
	pI2CHandle->pI2Cx->I2C_CR2 = (temp_reg & 0x3F);

	//3. Program the device own address
	temp_reg=0;
	temp_reg |= (pI2CHandle->I2C_Config.I2C_DeviceAddress << 1);
	temp_reg |= (1 << 14);
	pI2CHandle->pI2Cx->I2C_OAR1 = temp_reg;

	//4. CCR Calculation (Clock Control Register)
	uint16_t ccr_value = 0;
	temp_reg = 0;
	if (pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM) {
		// std mode
		ccr_value = RCC_GetPCLK1Value()
				/ (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		temp_reg |= (ccr_value & 0xFFF);
	} else {
		// fast mode
		temp_reg |= (1 << 15);
		temp_reg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);
		if (pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2) {
			ccr_value = RCC_GetPCLK1Value()
					/ (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		} else {
			ccr_value = RCC_GetPCLK1Value()
					/ (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		}
		temp_reg |= (ccr_value & 0xFFF);
	}
	pI2CHandle->pI2Cx->I2C_CCR = temp_reg;

	//5. Configuring T-rise afterwards
	if (pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM) {
		// std mode

		temp_reg = (RCC_GetPCLK1Value() / 1000000U) + 1;

	} else {
		// fast mode
		temp_reg = ((RCC_GetPCLK1Value() * 300) / 1000000000U) + 1;
	}
	pI2CHandle->pI2Cx->I2C_TRISE = (temp_reg & 0x3F);
}

uint8_t I2C_GetFLagStatus(I2C_reg *pI2Cx, uint32_t FlagName) {
	if (pI2Cx->I2C_SR1 & FlagName) {
		return FLAG_SET;
	} else {
		return FLAG_RESET;
	}
}
void I2C_MasterSendData(I2C_Handle *pI2CHandle, uint8_t *pTxbuffer,
		uint32_t len, uint8_t SlaveAddr, uint8_t Sr) {
	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that start generation is completed by checking the SB flag in the SR1
	//Note: Until SB is cleared SCL will be stretched (pulled to low)
	while (!I2C_GetFLagStatus(pI2CHandle->pI2Cx, I2C_SB_FLAG));

	//3. Send the address of the slave with r/w bit set w(0) (total 8 bts)
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, SlaveAddr);

	//4. Confirm that address phase is completed by checking the ADDR flag in the SR1
	while (!I2C_GetFLagStatus(pI2CHandle->pI2Cx, I2C_ADDR_FLAG));

	//5. Clear the ADDR flag according to its software sequence
	I2C_ClearADDRFlag(pI2CHandle);

	//6. Send data until len becomes 0
	while (len > 0) {
		while (!I2C_GetFLagStatus(pI2CHandle->pI2Cx, I2C_TXE_FLAG));
		pI2CHandle->pI2Cx->I2C_DR = *pTxbuffer;
		pTxbuffer++;
		len--;
	}
	//7. When len becomes zero wait for TXE=1 and BTF=1 before generating the STOP condition
	//Note: TXE=1, BTF=1 means that both SR and DR are empty and next transmission should begin
	//when BTF=1 SCL will be stretched  (pulled to low)
	while (!I2C_GetFLagStatus(pI2CHandle->pI2Cx, I2C_TXE_FLAG));

	while (!I2C_GetFLagStatus(pI2CHandle->pI2Cx, I2C_BTF_FLAG));
	//8. Generate STOP condition and master need not to wait for the completion of stop condition.
	// Note: Generating STOP, automatically clears the BTF
	if (Sr == I2C_DISABLE_SR) {
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

	}
}
void I2C_ManageAcking(I2C_reg *pI2Cx, uint8_t EnorDi) {
	if (EnorDi == I2C_ACK_ENABLE) {
		//enable ack
		pI2Cx->I2C_CR1 |= (1 << I2C_CR1_ACK);
	} else {
		//Disable ack
		pI2Cx->I2C_CR1 &= ~(1 << I2C_CR1_ACK);
	}
}
void I2C_MasterReceiveData(I2C_Handle *pI2CHandle, uint8_t *pRxbuffer,
		uint32_t len, uint8_t SlaveAddr, uint8_t Sr) {
	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that start generation is completed by checking the SB flag  in the SR1
	//Note: Untill SB is cleared SCL will be stretched (pulled to Low)
	while (!I2C_GetFLagStatus(pI2CHandle->pI2Cx, I2C_SB_FLAG))
		;

	//3. Send the address of the slave with r/w bit set to R(1) (total 8 bits)
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, SlaveAddr);

	//4. Wait until address phase is completed by checking the ADDR flag in the SR1
	while (!I2C_GetFLagStatus(pI2CHandle->pI2Cx, I2C_ADDR_FLAG))
		;

	//procedure to only 1 byte from slave
	if (len == 1) {
		//Disable acking
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

		//clear the Addr flag
		I2C_ClearADDRFlag(pI2CHandle);

		//wait untill RXNE becomes
		while (!I2C_GetFLagStatus(pI2CHandle->pI2Cx, I2C_RXNE_FLAG))
			;

		if (Sr == I2C_DISABLE_SR) {
			//Generate stop condition
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		}
		//read data in to buffer
		*pRxbuffer = pI2CHandle->pI2Cx->I2C_DR;
	}
	//procedure to read data from slave when len>1
	if (len > 1) {
		//Clear the Addr flag
		I2C_ClearADDRFlag(pI2CHandle);

		//read the data until len becomes zero
		for (uint32_t i = len; i > 0; i--) {
			//wait until RXNE becomes 1
			while (!I2C_GetFLagStatus(pI2CHandle->pI2Cx, I2C_RXNE_FLAG))
				;

			if (i == 2) {	//if last 2 bytes are remaining
				//Disable acking
				I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

				if (Sr == I2C_DISABLE_SR) {
					I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

				}
			}
			//read the data from data register in to buffer
			*pRxbuffer = pI2CHandle->pI2Cx->I2C_DR;

			//increment the buffer address
			pRxbuffer++;
		}
	}
	//re-enable acking
	if (pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE) {
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
	}
}
void I2C_SlaveSendData(I2C_reg *pI2C, uint8_t data){
	pI2C->I2C_DR = data;
}
uint8_t I2C_SlaveReceiveData(I2C_reg *pI2C){
	return (uint8_t) pI2C->I2C_DR;
}
void I2C_Deinit(I2C_reg *pI2Cx) {
	if (pI2Cx == I2C1) {
		I2C1_REG_RESET();
	} else if (pI2Cx == I2C2) {
		I2C2_REG_RESET();
	} else {
		I2C3_REG_RESET();
	}
}
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi) {
	if (EnorDi == ENABLE) {
		if (IRQNumber <= 31) {
			//program the ISR0 register
			*NVIC_ISR0 |= (1 << IRQNumber);
		} else if (IRQNumber > 31 && IRQNumber < 64) {
			//Program the ISR1 register
			*NVIC_ISR1 |= (1 << (IRQNumber % 32));
		} else if (IRQNumber >= 64 && IRQNumber < 96) {
			//Program the ISR2 register
			*NVIC_ISR3 |= (1 << (IRQNumber % 64));

		}
	} else {

		if (IRQNumber <= 31) {
			//program the IC0 register
			*NVIC_ICR0 |= (1 << IRQNumber);
		} else if (IRQNumber > 31 && IRQNumber < 64) {
			//Program the ICR1 register
			*NVIC_ICR1 |= (1 << (IRQNumber % 32));
		} else if (IRQNumber >= 64 && IRQNumber < 96) {
			//Program the ICR2 register
			*NVIC_ICR3 |= (1 << (IRQNumber % 64));

		}
	}
}
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority) {
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQPriority % 4;
	uint8_t shiftamount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASEADDR + (iprx)) |= (IRQPriority << shiftamount);

}
uint8_t I2C_MasterSendDataIT(I2C_Handle *pI2CHandle, uint8_t *pTxbuf,
		uint32_t len, uint8_t SlaveAddr, uint8_t Sr) {
	uint8_t busystate = pI2CHandle->TxRxState;

	if ((busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX)) {
		pI2CHandle->pTxBuffer = pTxbuf;
		pI2CHandle->TxLen = len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;
		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITERREN);
	}

	return busystate;
}
uint8_t I2C_MasterReceiveDataIT(I2C_Handle *pI2CHandle, uint8_t *pRxbuf,
		uint32_t len, uint8_t SlaveAddr, uint8_t Sr) {

	uint8_t busystate = pI2CHandle->TxRxState;

	if ((busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX)) {
		pI2CHandle->pRxBuffer = pRxbuf;
		pI2CHandle->RxLen = len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = len; //Rx-size is used in the ISR code to manage the data reception
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITERREN);
	}
	return busystate;
}

static void I2C_MasterHandleTXEInterrupt(I2C_Handle *pI2CHandle){
	if(pI2CHandle->TxLen > 0)
		{
			//1. Load the data into DR
			pI2CHandle->pI2Cx->I2C_DR = *(pI2CHandle->pTxBuffer);

			//2. Decrement the TxLen
			pI2CHandle->TxLen--;

			//3. Increment the buffer address
			pI2CHandle->pTxBuffer++;
		}

}

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle *pI2CHandle){
	//data Reception
		if(pI2CHandle->RxSize == 1)
		{
			*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
			pI2CHandle->RxLen--;
		}

		if(pI2CHandle->RxSize > 1)
		{
			if(pI2CHandle->RxLen == 2)
			{
				//Clear ACK
				I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE	);
			}

			//Read DR
			*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
			pI2CHandle->pRxBuffer++;
			pI2CHandle->RxLen--;
		}

		if(pI2CHandle->RxLen == 0)
		{
			//Close I2C data reception and notify the application

			//1.Generate the stop condition
			if(pI2CHandle->Sr == I2C_DISABLE_SR)
			{
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
			}

			//2. Reset all the members of the Handle structure
			I2C_CloseReceiveData(pI2CHandle);

			//3. Notify the application
			I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);
		}

}
void I2C_EV_IRQHandling(I2C_Handle *pI2CHandle) {
	//Interrupt handling for both master and slave mode of a device

	uint32_t temp1, temp2, temp3;
	temp1 = pI2CHandle->pI2Cx->I2C_CR2 & (1 << I2C_CR2_ITEVTEN);
	temp2 = pI2CHandle->pI2Cx->I2C_CR2 & (1 << I2C_CR2_ITBUFEN); //**

	temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_SB);

	//1. Handle for interrupt generated by SB event
	//Note: SB flag is only applicable in master mode
	if (temp1 && temp3) {
		//SB flag is set
		//This flag is only applicable in master mode
		if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX) {
			I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		} else if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX) {
			I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}
	}
	//2. Handle for interrupt generated by ADDR event
	//Note: When master mode: Address is sent
	//      When slave mode: Address matched with own address
	temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_ADDR);
	if (temp1 && temp3) {
		//Addr flag is set
		I2C_ClearADDRFlag(pI2CHandle);
	}

	//3. Handle for interrupt generated by BTF(Byte Transfer Finished) event
	temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_BTF);
	if (temp1 && temp3) {
		//BTF flag is set
		if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX) {
			//Make sure if TXE is also set
			if (pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_TXE)) {
				//BTF, TXE = 1
				if (pI2CHandle->TxLen == 0) {
					//1. generate the stop condition
					if (pI2CHandle->Sr == I2C_DISABLE_SR) {

						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
					}
					//2. Reset all the member elements of the handle structure
					I2C_CloseSendData(pI2CHandle);

					//3. Notify the application about transmission being completed
					I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TX_CMPLT);
				}
			}

		} else if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX) {
			;
		}

	}
	//4. Handle for interrupt generated by STPF event
	//Note: Stop detection flag is applicable only slave mode: For master bits flag will never be set
	//The below code block will be executed by the master since STOPF will not set in master mode
	temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_STOPF);
	if (temp1 && temp3) {
		//Stop flag is set
		//Clear the Stop flag (Read SR1 and then write to CR1)
		pI2CHandle->pI2Cx->I2C_CR1 |= 0x0000;
		//Notify the application that STOP is generated by master
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_STOP);

	}
	//5. Handle for interrupt generated by TXE event
	temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_TXE);
	if (temp1 && temp2 && temp3) {
		//Check for device mode
		if (pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL)) {
			//TXE flag is set
			//Do the data transmission
			if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX) {
				I2C_MasterHandleTXEInterrupt(pI2CHandle);
			}else{
				//Slave Mode
				if(!(pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_TRA)))
				{
					I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_REQ);
				}
			}
		}
	}
	//6. Handle for interrupt generated by RXNE event
	temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_RXNE);
	if (temp1 && temp2 && temp3) {
		//Check for device mode
		if (pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL)) {
			//The device is in master mode
			//RXNE flag is set
			if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX) {

				I2C_MasterHandleRXNEInterrupt(pI2CHandle);
			}else{
				//Slave mode
				if(!(pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_TRA)))
				{
					I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_RCV);
				}
			}
		}
	}
}
void I2C_ER_IRQHandling(I2C_Handle *pI2CHandle) {
	uint32_t temp1, temp2;

	//Know the status of  ITERREN control bit in the CR2
	temp2 = (pI2CHandle->pI2Cx->I2C_CR2) & (1 << I2C_CR2_ITERREN);

	/***********************Check for Bus error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & (1 << I2C_SR1_BERR);
	if (temp1 && temp2) {
		//This is Bus error

		//Implement the code to clear the buss error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~(1 << I2C_SR1_BERR);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_BERR);
	}

	/***********************Check for arbitration lost error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & (1 << I2C_SR1_ARLO);
	if (temp1 && temp2) {
		//This is arbitration lost error

		//Implement the code to clear the arbitration lost error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~(1 << I2C_SR1_ARLO);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_ARLO);
	}

	/***********************Check for ACK failure  error************************************/

	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & (1 << I2C_SR1_AF);
	if (temp1 && temp2) {
		//This is ACK failure error

		//Implement the code to clear the ACK failure error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~(1 << I2C_SR1_AF);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_AF);
	}

	/***********************Check for Overrun/underrun error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & (1 << I2C_SR1_OVR);
	if (temp1 && temp2) {
		//This is Overrun/underrun

		//Implement the code to clear the Overrun/underrun error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~(1 << I2C_SR1_OVR);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_OVR);
	}

	/***********************Check for Time out error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & (1 << I2C_SR1_TIMEOUT);
	if (temp1 && temp2) {
		//This is Time out error

		//Implement the code to clear the Time out error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~(1 << I2C_SR1_TIMEOUT);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_TIMEOUT);
	}
}
