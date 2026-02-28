/*
 * 002LED_BUTTON.c
 *
 *  Created on: Aug 31, 2024
 *      Author: Lenovo
 */

#include <stdint.h>
#include "stm32f407xx.h"


#define HIGH 1
#define LOW  0
#define BTN_PRESSED HIGH


void delay(void)
{
	for(uint32_t i=0; i<500000/2; i++);
}



int main(void)
{
	GPIO_Handle GPIOLed, GPIOBtn;//Creation of structure variables
	//LED configuration
	GPIOLed.pGPIOx=GPIOD;
	GPIOLed.GPIO_PinConf.GPIO_PinNumber= GPIO_PIN_NO_12;
	GPIOLed.GPIO_PinConf.GPIO_PinMode=GPIO_MODE_OUT;
	GPIOLed.GPIO_PinConf.GPIO_PinSpeed=GPIO_OP_TYPE_HS;
	GPIOLed.GPIO_PinConf.GPIO_PinOpType=GPIO_OP_TYPE_OD;//1. GPIO_OP_TYPE_PP, 2. GPIO_OP_TYPE_OD
	GPIOLed.GPIO_PinConf.GPIO_PinPuPdControl=GPIO_PIN_PU;//1. GPIO_NO_PUPD, 2.GPIO_PIN_PU

	GPIO_PeriClckInit(GPIOD,ENABLE);
	GPIO_init(&GPIOLed);


	//Button configuration
	GPIOBtn.pGPIOx=GPIOA;
	GPIOBtn.GPIO_PinConf.GPIO_PinNumber= GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConf.GPIO_PinMode=GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConf.GPIO_PinSpeed=GPIO_OP_TYPE_HS;
	GPIOBtn.GPIO_PinConf.GPIO_PinPuPdControl=GPIO_NO_PUPD ;

	GPIO_PeriClckInit(GPIOA,ENABLE);
	GPIO_init(&GPIOBtn);

	while(1)
	{
		if(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)==BTN_PRESSED)
		{
			delay();// delay for button de-bouncing
			GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
		}
	}

}
