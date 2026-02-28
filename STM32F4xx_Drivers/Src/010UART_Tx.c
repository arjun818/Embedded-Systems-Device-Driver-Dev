/*
 * 010UART_Tx.c
 *
 *  Created on: May 24, 2025
 *      Author: Lenovo
 */

#include<stdio.h>

#include<string.h>

#include "stm32f407xx.h"

/*
 * PA2 = Tx
 * PA3 = Rx
 */

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

char msg[1024] = "Hello acha, amma and Udit... My UART is Txing\n\r";

USART_Handle usart2_handle;

void USART2_Init(void)
{
	usart2_handle.pUSARTx = USART2;
	usart2_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
	usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart2_handle.USART_Config.USART_Mode = USART_MODE_ONLY_TX;
	usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
	usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;

	USART_Init(&usart2_handle);
}

void 	USART2_GPIOInit(void)
{
	GPIO_Handle usart_gpios;

	usart_gpios.pGPIOx = GPIOA;
	usart_gpios.GPIO_PinConf.GPIO_PinMode = GPIO_MODE_ALTFN;
	usart_gpios.GPIO_PinConf.GPIO_PinOpType = GPIO_OP_TYPE_PP;
	usart_gpios.GPIO_PinConf.GPIO_PinPuPdControl = GPIO_PIN_PU;
	usart_gpios.GPIO_PinConf.GPIO_PinSpeed = GPIO_OP_TYPE_HS;
	usart_gpios.GPIO_PinConf.GPIO_PinAltFunMode =7;

	//USART2 TX
	usart_gpios.GPIO_PinConf.GPIO_PinNumber  = GPIO_PIN_NO_2;
	GPIO_init(&usart_gpios);

	//USART2 RX
	usart_gpios.GPIO_PinConf.GPIO_PinNumber = GPIO_PIN_NO_3;
	GPIO_init(&usart_gpios);


}

void GPIO_ButtonInit(void)
{
	GPIO_Handle GPIOBtn,GpioLed;

	//this is btn gpio configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConf.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConf.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConf.GPIO_PinSpeed = GPIO_OP_TYPE_HS;
	GPIOBtn.GPIO_PinConf.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_init(&GPIOBtn);

	//this is gpio configuration
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConf.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConf.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConf.GPIO_PinSpeed = GPIO_OP_TYPE_HS;
	GpioLed.GPIO_PinConf.GPIO_PinOpType = GPIO_OP_TYPE_OD;
	GpioLed.GPIO_PinConf.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_init(&GpioLed);

}


int main(void)
{

	GPIO_ButtonInit();

	USART2_GPIOInit();

    USART2_Init();

    USART_PeripheralControl(USART2,ENABLE);

    while(1)
    {
		//wait till button is pressed
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		USART_SendData(&usart2_handle,(uint8_t*)msg,strlen(msg));

    }

	return 0;
}
