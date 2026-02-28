/*
 * 003Button_Interrupt.c
 *
 *  Created on: Sep 2, 2024
 *      Author: Lenovo
 */

/*
 * 002LED_BUTTON.c
 *
 *  Created on: Aug 31, 2024
 *      Author: Lenovo
 */

#include <stdint.h>
#include "stm32f407xx.h"
#include<string.h>


#define HIGH 1
#define BTN_PRESSED HIGH




void delay(void)
{
	for(uint32_t i=0; i<500000/2; i++);
}



int main(void)
{
	GPIO_Handle GPIOLed, GPIOBtn;//Creation of structure variables
	memset(&GPIOLed, 0, sizeof(GPIOLed));
	memset(&GPIOBtn, 0, sizeof(GPIOBtn));
	//LED configuration
	GPIOLed.pGPIOx=GPIOD;
	GPIOLed.GPIO_PinConf.GPIO_PinNumber= GPIO_PIN_NO_12;
	GPIOLed.GPIO_PinConf.GPIO_PinMode=GPIO_MODE_OUT;
	GPIOLed.GPIO_PinConf.GPIO_PinSpeed=GPIO_OP_TYPE_LS ;
	GPIOLed.GPIO_PinConf.GPIO_PinOpType=GPIO_OP_TYPE_OD;//1. GPIO_OP_TYPE_PP, 2. GPIO_OP_TYPE_OD
	GPIOLed.GPIO_PinConf.GPIO_PinPuPdControl=GPIO_NO_PUPD;//1. GPIO_NO_PUPD, 2.GPIO_PIN_PU

	GPIO_PeriClckInit(GPIOD,ENABLE);
	GPIO_init(&GPIOLed);


	//Button configuration
	GPIOBtn.pGPIOx=GPIOA;
	GPIOBtn.GPIO_PinConf.GPIO_PinNumber= GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConf.GPIO_PinMode=GPIO_MODE_IT_RT;
	GPIOBtn.GPIO_PinConf.GPIO_PinSpeed=GPIO_OP_TYPE_HS;
	GPIOBtn.GPIO_PinConf.GPIO_PinPuPdControl=GPIO_NO_PUPD;

	GPIO_PeriClckInit(GPIOA,ENABLE);
	GPIO_init(&GPIOBtn);

//    GPIO_WriteFromOutputPin(GPIOA, GPIO_PIN_NO_0, GPIO_PIN_RESET);
	//IRQ Configurations
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI0, NVIC_IRQ_PRIORITY_0);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0, ENABLE);



	while(1);

}
	//ISR implementation
void EXTI0_IRQHandler(void){

	delay();
	GPIO_IRQHandling(GPIO_PIN_NO_0);//Clear the pending register
	GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_12);
}







