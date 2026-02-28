/*
 * stmm32f407xx_gpio.c
 *
 *  Created on: Aug 28, 2024
 *      Author: Lenovo
 */

#include "stm32f407xx_gpio_driver.h"


//Peripheral clock setup API
void GPIO_PeriClckInit(GPIO_reg *pGPIOx, uint8_t EnorDi) {
    if (EnorDi == ENABLE) {
        if (pGPIOx == GPIOA) {
            GPIOA_PERI_CLOCK_ENABLE();
        } else if (pGPIOx == GPIOB) {
            GPIOB_PERI_CLOCK_ENABLE();
        } else if (pGPIOx == GPIOC) {
            GPIOC_PERI_CLOCK_ENABLE();
        } else if (pGPIOx == GPIOD) {
            GPIOD_PERI_CLOCK_ENABLE();
        } else if (pGPIOx == GPIOE) {
            GPIOE_PERI_CLOCK_ENABLE();
        } else if (pGPIOx == GPIOF) {
            GPIOF_PERI_CLOCK_ENABLE();
        } else if (pGPIOx == GPIOG) {
            GPIOG_PERI_CLOCK_ENABLE();
        } else if (pGPIOx == GPIOH) {
            GPIOH_PERI_CLOCK_ENABLE();
        } else {
            GPIOI_PERI_CLOCK_ENABLE();
        }
    } else {
        if (pGPIOx == GPIOA) {
            GPIOA_PERI_CLOCK_DISABLE();
        } else if (pGPIOx == GPIOB) {
            GPIOB_PERI_CLOCK_DISABLE();
        } else if (pGPIOx == GPIOC) {
            GPIOC_PERI_CLOCK_DISABLE();
        } else if (pGPIOx == GPIOD) {
            GPIOD_PERI_CLOCK_DISABLE();
        } else if (pGPIOx == GPIOE) {
            GPIOE_PERI_CLOCK_DISABLE();
        } else if (pGPIOx == GPIOF) {
            GPIOF_PERI_CLOCK_DISABLE();
        } else if (pGPIOx == GPIOG) {
            GPIOG_PERI_CLOCK_DISABLE();
        } else if (pGPIOx == GPIOH) {
            GPIOH_PERI_CLOCK_DISABLE();
        } else {
            GPIOI_PERI_CLOCK_DISABLE();
        }
    }
}



//Init and Deinit of ports
void GPIO_init(GPIO_Handle *pGPIOHandle)
{

	//Enable the clock
	GPIO_PeriClckInit(pGPIOHandle->pGPIOx, ENABLE);

    // 1. Configure mode of GPIO pin
    // The below in a non-interrupt mode

    uint32_t temp = 0;
    if (pGPIOHandle->GPIO_PinConf.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {

        temp = (pGPIOHandle->GPIO_PinConf.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConf.GPIO_PinNumber));
        pGPIOHandle->pGPIOx->PortModeRegister&=~(0x3 << (2 * pGPIOHandle->GPIO_PinConf.GPIO_PinNumber) );//Clearing the bits
        pGPIOHandle->pGPIOx->PortModeRegister|=temp;//Setting the bits
    }
    else
    {
    	//Peripheral specific side
    	//1. Enable the registers accordingly
    	if(pGPIOHandle->GPIO_PinConf.GPIO_PinMode==GPIO_MODE_IT_FT){

    		//Enable the FTSR register
    		EXTI->EXTI_FTSR |= (1 << pGPIOHandle->GPIO_PinConf.GPIO_PinNumber);
    		//Clear the RTSR bit
    		EXTI->EXTI_RTSR &= ~(1 << pGPIOHandle->GPIO_PinConf.GPIO_PinNumber);

    	}else if(pGPIOHandle->GPIO_PinConf.GPIO_PinMode==GPIO_MODE_IT_RT){
    		//Enable the RTSR register
    		EXTI->EXTI_RTSR |= (1 << pGPIOHandle->GPIO_PinConf.GPIO_PinNumber);
    		//Clear the FTSR bit
    		EXTI->EXTI_FTSR &= ~(1<< pGPIOHandle->GPIO_PinConf.GPIO_PinNumber);
    	}else if(pGPIOHandle->GPIO_PinConf.GPIO_PinMode==GPIO_MODE_IT_RFT){
    		//Enable the FTSR and RTSR register
    		EXTI->EXTI_FTSR |= (1<< pGPIOHandle->GPIO_PinConf.GPIO_PinNumber);

    		EXTI->EXTI_RTSR |= (1<< pGPIOHandle->GPIO_PinConf.GPIO_PinNumber);
    	}
    	//2. Configure the GPIO port selection in SYSCFG_EXTICR
    	uint8_t temp1, temp2;
    	temp1= (pGPIOHandle->GPIO_PinConf.GPIO_PinNumber)/4;
    	temp2= (pGPIOHandle->GPIO_PinConf.GPIO_PinNumber)%4;
    	uint8_t portcode= GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
    	SYS_PERI_CLOCK_ENABLE();
    	SYS_CONFIG->SYSCFG_EXTICR[temp1]= portcode << (temp2 * 4);


    	//3. Enable the exti interrupt delivery using IMR
    	EXTI->EXTI_IMR |=(1<< pGPIOHandle->GPIO_PinConf.GPIO_PinNumber);
    }
    temp = 0;

    // 2. Configure speed mode in GPIO pin
    temp = (pGPIOHandle->GPIO_PinConf.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConf.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->PortOutSpeedRegister&=~(0x3 << (2 * pGPIOHandle->GPIO_PinConf.GPIO_PinNumber) );//Clearing the bits
    pGPIOHandle->pGPIOx->PortOutSpeedRegister |= temp;//Setting the bits
    temp = 0;

    // 3. Configure the Pull up and Pull down register
    temp = (pGPIOHandle->GPIO_PinConf.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConf.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->PortPullUpPulldownRegister&=~(0x3 << (2* pGPIOHandle->GPIO_PinConf.GPIO_PinNumber) );//Clearing the bits
    pGPIOHandle->pGPIOx->PortPullUpPulldownRegister |= temp;//setting the bits
    temp = 0;

    // 4. Configure output type
    temp = (pGPIOHandle->GPIO_PinConf.GPIO_PinOpType << pGPIOHandle->GPIO_PinConf.GPIO_PinNumber);
    pGPIOHandle->pGPIOx->PortOutTypeRegister&=~(0x1 << pGPIOHandle->GPIO_PinConf.GPIO_PinNumber);//Clearing the bits
    pGPIOHandle->pGPIOx->PortOutTypeRegister |= temp;//Setting the bits
    temp = 0;

    // Configure alternate functionality
    if (pGPIOHandle->GPIO_PinConf.GPIO_PinMode == GPIO_MODE_ALTFN)
    {

        // Configure alternate functionality
    	uint8_t temp1, temp2;
    	temp1 = pGPIOHandle->GPIO_PinConf.GPIO_PinNumber / 8;
    	temp2 = pGPIOHandle->GPIO_PinConf.GPIO_PinNumber % 8;
    	pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2));//Clearing the bits
    	pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConf.GPIO_PinAltFunMode << (4 * temp2));
    }
}
//GPIO de-initialize function
void GPIO_Deinit(GPIO_reg *pGPIOx)
{
    if (pGPIOx == GPIOA) {
        GPIOA_REG_RESET();
    } else if (pGPIOx == GPIOB) {
        GPIOB_REG_RESET();
    } else if (pGPIOx == GPIOC) {
        GPIOC_REG_RESET();
    } else if (pGPIOx == GPIOD) {
        GPIOD_REG_RESET();
    } else if (pGPIOx == GPIOE) {
        GPIOE_REG_RESET();
    } else if (pGPIOx == GPIOF) {
        GPIOF_REG_RESET();
    } else if (pGPIOx == GPIOG) {
        GPIOG_REG_RESET();
    } else if (pGPIOx == GPIOH) {
        GPIOH_REG_RESET();
    } else {
        GPIOI_REG_RESET();
    }

}

//Data read and write in ports and pin
uint8_t GPIO_ReadFromInputPin(GPIO_reg *pGPIOx, uint8_t PinNumber)
{

	uint8_t value;

	value= (uint8_t)((pGPIOx->PortInDataRegister >> PinNumber) & 0x00000001);
	return value;
}
uint16_t GPIO_ReadFromInputPort(GPIO_reg *pGPIOx)
{
	uint16_t value;

	value= (uint16_t)((pGPIOx->PortInDataRegister));
	return value;

}

void GPIO_WriteFromOutputPin(GPIO_reg *pGPIOx, uint8_t PinNumber, uint8_t Value)
{

	if(Value==GPIO_PIN_SET){
		(pGPIOx->PortOutDataRegister|=(1<<PinNumber));

	}else{

		(pGPIOx->PortOutDataRegister&=~(1<<PinNumber));
	}


}

void GPIO_WriteFromOutputPort(GPIO_reg *pGPIOx, uint16_t Value)
{
    Value= pGPIOx->PortOutDataRegister;
}

void GPIO_ToggleOutputPin(GPIO_reg *pGPIOx, uint8_t PinNumber)
{
   pGPIOx->PortOutDataRegister^=(1<<PinNumber);
}

//IRQ configuration and ISR handling
//This is used for NVIC controlling
//Processor specific
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{

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

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
   uint8_t iprx= IRQNumber/4;
   uint8_t iprx_section= IRQPriority % 4;
   uint8_t shiftamount= (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
   *(NVIC_PR_BASEADDR + (iprx))|= (IRQPriority << shiftamount);
}

void GPIO_IRQHandling(uint8_t PinNumber)
{
	//Clear the exti PR register corresponding to the pin number
	if(EXTI->EXTI_PR & (1 << PinNumber )) {
		//Clear
		EXTI->EXTI_PR |= (1 << PinNumber);
	}

}


