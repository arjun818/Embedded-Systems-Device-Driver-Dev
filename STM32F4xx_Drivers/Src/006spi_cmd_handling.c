/*
 * 004spi_testing.c
 *
 *  Created on: Sep 6, 2024
 *      Author: Lenovo
 */
#include "stm32f407xx.h"
#include <string.h>
#include <stdint.h>

//Command code
#define COMMAND_LED_CTRL       0x50
#define COMMAND_SENSOR_READ    0x51
#define COMMAND_LED_READ       0x52
#define COMMAND_PRINT          0x53
#define COMMAND_ID_READ        0x54


#define LED_ON                 1
#define LED_OFF				   0

//Arduino analog pin
#define ANALOG_PIN0            0
#define ANALOG_PIN1            1
#define ANALOG_PIN2            2
#define ANALOG_PIN3            3
#define ANALOG_PIN4            4

//Arduino LED on pin number 9
#define LED_PIN                9


void delay(void)
{
	for(uint32_t i=0; i<500000/2; i++);
}

void GpioBtnInit(){
	GPIO_Handle GPIOBtn;//Creation of structure variables

	//Button configuration
	GPIOBtn.pGPIOx=GPIOA;
	GPIOBtn.GPIO_PinConf.GPIO_PinNumber= GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConf.GPIO_PinMode=GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConf.GPIO_PinSpeed=GPIO_OP_TYPE_HS;
	GPIOBtn.GPIO_PinConf.GPIO_PinPuPdControl=GPIO_NO_PUPD ;

	GPIO_init(&GPIOBtn);
}

void Spi2Init(){
	SPI_Handle SPI2handle;
	SPI2handle.pSPIx=SPI2;
	SPI2handle.spi_pin_config.SPI_BusConfig=SPI_BUS_CONFIG_FD;
	SPI2handle.spi_pin_config.SPI_Devicemode= SPI_DEVICE_MODE_MASTER;
	SPI2handle.spi_pin_config.SPI_Speed= SPI_CLK_SPEED_DIV8;//Generates serial clk of 2 Mhz
	SPI2handle.spi_pin_config.SPI_DFF=SPI_DFF_8BITS;
	SPI2handle.spi_pin_config.SPI_CPOL=SPI_CPOL_LOW;
	SPI2handle.spi_pin_config.SPI_CPHA= SPI_CPHA_LOW;
	SPI2handle.spi_pin_config.SPI_SSM=SPI_SSM_DI;//SSM disabled hardware slave management

	SPI_init(&SPI2handle);

}

uint8_t SPI_VerifyResponse(uint8_t ackbyte){

	if(ackbyte==(uint8_t)0xf5){
		//Ack
		return 1;
	}else{
		return 0;
		//Nack
	}
}

void Spi2GpioInit(void){
	GPIO_Handle SPIpins;
	SPIpins.pGPIOx=GPIOB;
	SPIpins.GPIO_PinConf.GPIO_PinMode=GPIO_MODE_ALTFN;
	SPIpins.GPIO_PinConf.GPIO_PinAltFunMode=5;
	SPIpins.GPIO_PinConf.GPIO_PinOpType=GPIO_OP_TYPE_PP;
	SPIpins.GPIO_PinConf.GPIO_PinPuPdControl=GPIO_NO_PUPD;
	SPIpins.GPIO_PinConf.GPIO_PinSpeed=GPIO_OP_TYPE_HS;

	//SCLK init-->13th pin
	SPIpins.GPIO_PinConf.GPIO_PinNumber=GPIO_PIN_NO_13;
	GPIO_init(&SPIpins);

	//MOSI init-->15Th pin
	SPIpins.GPIO_PinConf.GPIO_PinNumber=GPIO_PIN_NO_15;
	GPIO_init(&SPIpins);

	//MISO init-->14th pin
	SPIpins.GPIO_PinConf.GPIO_PinNumber=GPIO_PIN_NO_14;
	GPIO_init(&SPIpins);
//
	//NSS init->12th pin
	SPIpins.GPIO_PinConf.GPIO_PinNumber=GPIO_PIN_NO_12;
	GPIO_init(&SPIpins);
}



int main()
{
	uint8_t dummy_write= 0xff;
	uint8_t dummy_read;

	//This function enables the button
	GpioBtnInit();

	//This function is used to initialize GPIO pins behave as SPI2 pins
	Spi2GpioInit();

	//This function is used initialize the SPI2 peripheral registers
	Spi2Init();

	//This function is used to  initialize SPI2 as SSOE
	Spi_SSOEConfig(SPI2, ENABLE);

	while(1){

	while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));//The loop exits when button is pressed

	delay();// delay for button de-bouncing

	//Enable the SPI2 peripheral register
	SPI_PeripheralControl(SPI2,ENABLE);

	//1. COMMAND_LED_CTRL   pin_no.    value
	uint8_t commandcode= COMMAND_LED_CTRL;
	uint8_t ackbyte;
	uint8_t args[2];


	//Send the command
	SPI_SendData(SPI2, &commandcode, 1);

	//Do dummy read to clear of the RXNE
	SPI_ReceiveData(SPI2, &dummy_read, 1);

	//Send some dummy byte to fetch response of the slave
	SPI_SendData(SPI2, &dummy_write, 1);

	//Read the ackbyte received
	SPI_ReceiveData(SPI2, &ackbyte, 1);

	//Function to verify ackbyte
	if(SPI_VerifyResponse(ackbyte)){
		//Send arguments
		args[0]=LED_PIN;
		args[1]=LED_ON;
		SPI_SendData(SPI2, args, 2);
	}
	//EOF COMMAND_LED_CTRL

	//2. COMMAND_SENSOR_READ   analog pin number    1
	while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));//The loop exits when button is pressed

	delay();// delay for button de-bouncing
	commandcode= COMMAND_SENSOR_READ;


	//Send the command
	SPI_SendData(SPI2, &commandcode, 1);

	//Do dummy read to clear of the RXNE
	SPI_ReceiveData(SPI2, &dummy_read, 1);

	//Send some dummy byte to fetch response of the slave
	SPI_SendData(SPI2, &dummy_write, 1);

	//Read the ackbyte received
	SPI_ReceiveData(SPI2, &ackbyte, 1);

	//Function to verify ackbyte
	if(SPI_VerifyResponse(ackbyte)){
		//Send arguments
		args[0]=ANALOG_PIN0;
		SPI_SendData(SPI2, args, 1);
		//Insert a delay for the slave to be ready with the data;
		delay();

		//Do dummy read to clear of the RXNE
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//Send some dummy byte to fetch response of the slave
		SPI_SendData(SPI2, &dummy_write, 1);

		uint8_t analog_read;
		SPI_ReceiveData(SPI2, &analog_read, 1);
	}

    //To check if SPI has pending data to send
	while(SPI_GetFLagStatus(SPI2, SPI_SR_BSY ));

	//Disable the SPI2 peripheral register
	SPI_PeripheralControl(SPI2,DISABLE);
}
	return 0;

}
