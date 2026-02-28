/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Sep 5, 2024
 *      Author: Lenovo
 */

/**********API definitions for the prototypes provided in SPI driver.h***********/
#include "stm32f407xx_spi_driver.h"
#include<stdint.h>
static void spi_txe_interrupt_handle(SPI_Handle *pSPIHandle);
static void spi_rnxe_interrupt_handle(SPI_Handle *pSPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle *pSPIHandle);


uint8_t SPI_GetFLagStatus(SPI_reg *pSPIx, uint32_t FlagName){
	if(pSPIx->SPI_SR & FlagName){
		return FLAG_SET;
	}else{

	return FLAG_RESET;
	}
}


void SPI_PeriClckInit(SPI_reg *pSPIx,uint8_t EnorDi){
	if (EnorDi == ENABLE) {
	        if (pSPIx == SPI1) {
	        	SPI1_PERI_CLOCK_ENABLE();
	        } else if(pSPIx == SPI2) {
	        	SPI2_PERI_CLOCK_ENABLE();
	        } else if(pSPIx == SPI3) {
	        	SPI3_PERI_CLOCK_ENABLE();
	        } else {
	        	SPI4_PERI_CLOCK_ENABLE();
	        }

	    } else {
	        if (pSPIx == SPI1) {
	        	SPI1_PERI_CLOCK_DISABLE();
	        } else if(pSPIx == SPI2) {
	        	SPI2_PERI_CLOCK_DISABLE();
	        } else if(pSPIx == SPI3) {
	        	SPI3_PERI_CLOCK_DISABLE();
	        } else {
	        	SPI4_PERI_CLOCK_DISABLE();
	        }
	    }


}

void SPI_init(SPI_Handle *pSPIHandle){

	uint32_t tempreg=0;

	//Enabling clock
	SPI_PeriClckInit(pSPIHandle->pSPIx, ENABLE);

	//1. Configure the Device Configure mode in CR1 register
	tempreg|= pSPIHandle->spi_pin_config.SPI_Devicemode<<SPI_CR1_MSTR;

	//2. Configure the Bus configure mode in CR1 register
	if(pSPIHandle->spi_pin_config.SPI_BusConfig==SPI_BUS_CONFIG_FD){
		//Bidi mode should be cleared
		tempreg &= ~(1<<SPI_CR1_BIDIMODE);

	}else if(pSPIHandle->spi_pin_config.SPI_BusConfig==SPI_BUS_CONFIG_HD){
		//Bidi mode should be set
		tempreg |= (1<<SPI_CR1_BIDIMODE);
	}else if(pSPIHandle->spi_pin_config.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RX_ONLY){
		//Bidi mode should be cleared
		tempreg &= ~(1<<SPI_CR1_BIDIMODE);
		//RX bit only should be set
		tempreg |=(1<<SPI_CR1_RXONLY);
	}
	//3. Configure SPI serial clk speed (baud rate)
	tempreg |= pSPIHandle->spi_pin_config.SPI_Speed <<SPI_CR1_BR;

	//4. Configure the DFF
	tempreg |= pSPIHandle->spi_pin_config.SPI_DFF <<SPI_CR1_DFF;
	//5. Configure the CPOL
	tempreg |= pSPIHandle->spi_pin_config.SPI_CPOL<<SPI_CR1_CPOL;
	//6. Configure the CPHA
	tempreg |= pSPIHandle->spi_pin_config.SPI_CPHA<<SPI_CR1_CPHA;

	//7. Configure the SPI_SSM
	tempreg |= pSPIHandle->spi_pin_config.SPI_SSM << SPI_CR1_SSM ;
	//Put tempreg values to CR1 register
	pSPIHandle->pSPIx->SPI_CR1= tempreg;

}

void SPI_Deinit(SPI_reg *pSPIx){
    if (pSPIx == SPI1) {
    	SPI1_REG_RESET();
    } else if (pSPIx == SPI2) {
    	SPI2_REG_RESET();
    } else if (pSPIx == SPI3) {
    	SPI3_REG_RESET();
    } else {
    	SPI4_REG_RESET();
    }
}

//This a bloacking call function or known as polling type code
void SPI_SendData(SPI_reg *pSPIx, uint8_t *pTxBuffer, uint32_t Len){

	while(Len >0){
		//1. Wait until TXE is set
		while(SPI_GetFLagStatus(pSPIx, SPI_TXE_FLAG)==FLAG_RESET);

		//2. Check the DFF bit CR1 bit
		if((pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF))){
			//16 Bit data
			//1. Load data into the in to the DR
			pSPIx->SPI_DR= *((uint16_t*)pTxBuffer);
			Len--;
			Len--;
			(uint16_t*)pTxBuffer++;//Incrementing the address for 16 bit data

		}else{
			//8 Bit data
			pSPIx->SPI_DR= *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}

}

void SPI_ReceiveData(SPI_reg *pSPIx, uint8_t *pRxBuffer, uint32_t Len){
	while(Len >0){
		//1. Wait until RXNE is set
		while(SPI_GetFLagStatus(pSPIx, SPI_RXNE_FLAG)==(uint8_t)FLAG_RESET);

		//2. Check the DFF bit CR1 bit
		if(pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF)){
			//16 Bit data
			//1. Load data from DR to Rx buffer
			*((uint16_t*)pRxBuffer)=pSPIx->SPI_DR;
			Len--;
			Len--;
			(uint16_t*)pRxBuffer++;//Incrementing the address for 16 bit data

		}else{
			//8 Bit data
			*(pRxBuffer)=pSPIx->SPI_DR;
			Len--;
			pRxBuffer++;
		}
	}

}

void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi){
	if(EnorDi==ENABLE){
		if(IRQNumber<=31) {
			//program the ISR0 register
			*NVIC_ISR0 |=(1<<IRQNumber);
		}
		else if(IRQNumber>31 && IRQNumber<64) {
			//Program the ISR1 register
			*NVIC_ISR1 |=(1<< (IRQNumber % 32) );
		}
		else if(IRQNumber>=64 && IRQNumber <96 ) {
			//Program the ISR2 register
			*NVIC_ISR3 |=(1<< (IRQNumber % 64) );

		}
	}
	else {

		if(IRQNumber<=31) {
			//program the IC0 register
			*NVIC_ICR0 |=(1<<IRQNumber);
		}
		else if(IRQNumber>31 && IRQNumber<64) {
			//Program the ICR1 register
			*NVIC_ICR1 |=(1<< (IRQNumber % 32) );
		}
		else if(IRQNumber>=64 && IRQNumber <96 ) {
			//Program the ICR2 register
			*NVIC_ICR3 |=(1 << (IRQNumber % 64) );

		}
	}

}

void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){
	   uint8_t iprx= IRQNumber/4;
	   uint8_t iprx_section= IRQPriority % 4;
	   uint8_t shiftamount= (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
	   *(NVIC_PR_BASEADDR + (iprx))|= (IRQPriority << shiftamount);


}


void SPI_IRQHandling(SPI_Handle *pSPIHandle){
	uint8_t temp1, temp2;
	//First lets check for TXE
	temp1= pSPIHandle->pSPIx->SPI_SR & (1 << SPI_SR_TXE);
	temp2= pSPIHandle->pSPIx->SPI_CR2 & (1 << SPI_CR2_TXEIE);

	if(temp1 && temp2){
		//Handle TXE
		spi_txe_interrupt_handle(pSPIHandle);
	}

	//Check for RXNE
	temp1= pSPIHandle->pSPIx->SPI_SR & (1 << SPI_SR_RXNE);
	temp2= pSPIHandle->pSPIx->SPI_CR2 & (1 << SPI_CR2_RXNEIE);

	if(temp1 && temp2){
		//Handle TXE
		spi_rnxe_interrupt_handle(pSPIHandle);
	}

	temp1= pSPIHandle->pSPIx->SPI_SR & (1 << SPI_SR_OVR);
	temp2= pSPIHandle->pSPIx->SPI_CR2 & (1 << SPI_CR2_ERRIE);

	//Chcek for over run error
	if(temp1 && temp2){
		//Handle TXE
		spi_ovr_err_interrupt_handle(pSPIHandle);
	}
}

void SPI_PeripheralControl(SPI_reg *pSPIx,uint8_t EnorDi){
	if(EnorDi==ENABLE){
		pSPIx->SPI_CR1 |= (1<<SPI_CR1_SPE);//Setting the SPI enable bit
	}else{
		pSPIx->SPI_CR1 &= ~(1<<SPI_CR1_SPE);//Clearing the SPI enable bit
	}
}


void Spi_SSIConfig(SPI_reg *pSPIx,uint8_t EnorDi){
	if(EnorDi==ENABLE){
		pSPIx->SPI_CR1 |= (1<<SPI_CR1_SSI);//Setting the SPI SSI enable bit
	}else{
		pSPIx->SPI_CR1 &= ~(1<<SPI_CR1_SSI);//Clearing the SPI SSI enable bit
	}
}

void Spi_SSOEConfig(SPI_reg *pSPIx,uint8_t EnorDi){
	if(EnorDi==ENABLE){
		pSPIx->SPI_CR2 |= (1<<SPI_CR2_SSOE);//Setting the SPI SSI enable bit
	}else{
		pSPIx->SPI_CR2 &= ~(1<<SPI_CR2_SSOE);//Clearing the SPI SSI enable bit
	}
}


uint8_t SPI_SendDataIT(SPI_Handle *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len){
	uint8_t state= pSPIHandle->TxState;

	if(state!=SPI_BUSY_IN_TX){
		//1. Save the Tx buffer address and Len information in some global variables
		pSPIHandle->pTxBuffer=pTxBuffer;
		pSPIHandle->TxLen=Len;

		//2. Mark the SPI state as busy in transmission so that
		//no other code can take over same peripheral until transmission is over
		pSPIHandle->TxState=SPI_BUSY_IN_TX;

		//3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->SPI_CR2 |=(1<< SPI_CR2_TXEIE);

		//4. Data transmission will be be handled by the ISR code ( will implement later)
	}
	return state;
}
uint8_t SPI_ReceiveDataIT(SPI_Handle *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len){
	uint8_t state= pSPIHandle->RxState;

	if(state!=SPI_BUSY_IN_RX){
		//1. Save the Tx buffer address and Len information in some global variables
		pSPIHandle->pRxBuffer=pRxBuffer;
		pSPIHandle->RxLen=Len;

		//2. Mark the SPI state as busy in transmission so that
		//no other code can take over same peripheral until transmission is over
		pSPIHandle->RxState=SPI_BUSY_IN_RX;

		//3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->SPI_CR2 |=(1<< SPI_CR2_RXNEIE);

		//4. Data transmission will be be handled by the ISR code ( will implement later)
	}
	return state;
}


static void spi_txe_interrupt_handle(SPI_Handle *pSPIHandle){
	if(pSPIHandle->pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF)){
				//16 Bit data
				//1. Load data into the in to the DR
				pSPIHandle->pSPIx->SPI_DR= *((uint16_t*)(pSPIHandle->pTxBuffer));
				pSPIHandle->TxLen--;
				pSPIHandle->TxLen--;
				(uint16_t*)pSPIHandle->pTxBuffer++;//Incrementing the address for 16 bit data

			}else{
				//8 Bit data
				//1. Load data into the in to the DR
				pSPIHandle->pSPIx->SPI_DR= *(pSPIHandle->pTxBuffer);
				pSPIHandle->TxLen--;
				pSPIHandle->pTxBuffer++;
			}if(!pSPIHandle->TxLen){
				//Tx len 0 info application Txing is over clsoing SPI transmission
				SPI_CloseTransmission(pSPIHandle);
//				SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_COMPLETE);
			}
}
static void spi_rnxe_interrupt_handle(SPI_Handle *pSPIHandle){
	if(pSPIHandle->pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF)){
				//16 Bit data
				//1. Load data into the in to the DR
				*((uint16_t*)(pSPIHandle->pRxBuffer))= pSPIHandle->pSPIx->SPI_DR;
				pSPIHandle->RxLen--;
				pSPIHandle->RxLen--;
				(uint16_t*)pSPIHandle->pRxBuffer--;//Incrementing the address for 16 bit data

			}else{
				//8 Bit data
				//1. Load data into the in to the DR
				*(pSPIHandle->pRxBuffer)= pSPIHandle->pSPIx->SPI_DR;
				pSPIHandle->RxLen--;
				pSPIHandle->pRxBuffer--;
			}if(!pSPIHandle->RxLen){
				//Tx len 0 info application Txing is over closing SPI transmission
				SPI_CloseReception(pSPIHandle);
//				SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_COMPLETE);
			}
}
static void spi_ovr_err_interrupt_handle(SPI_Handle *pSPIHandle){

	uint8_t temp;
	//1. Clear the ovr flag
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX){
		temp= pSPIHandle->pSPIx->SPI_DR;
		temp= pSPIHandle->pSPIx->SPI_SR;
	}
	(void)temp;
	//Inform the application
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}
void SPI_CloseTransmission(SPI_Handle *pSPIHandle){
					pSPIHandle->pSPIx->SPI_CR2 &= ~(1 << SPI_CR2_TXEIE);// Prevents IT from setting up of TXE flag
					pSPIHandle->pTxBuffer=NULL;
					pSPIHandle->TxLen=0;
					pSPIHandle->TxState= SPI_READY;
}
void SPI_CloseReception(SPI_Handle *pSPIHandle){
					pSPIHandle->pSPIx->SPI_CR2 &= ~(1 << SPI_CR2_RXNEIE);// Prevents IT from setting up of TXE flag
					pSPIHandle->pRxBuffer=NULL;
					pSPIHandle->RxLen=0;
					pSPIHandle->RxState= SPI_READY;
}

void SPI_ClearOVRFlag(SPI_reg *pSPIx){
	uint8_t temp;
	temp= pSPIx->SPI_DR;
	temp= pSPIx->SPI_SR;
	(void)temp;
}


__weak void SPI_ApplicationEventCallback(SPI_Handle *pSPIHandle,uint8_t AppEv){
	//This is weak implementation and application may override this function
}







