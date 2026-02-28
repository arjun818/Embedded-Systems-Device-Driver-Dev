/*
 * 004spi_testing.c
 *
 *  Created on: Sep 6, 2024
 *      Author: Lenovo
 */
#include "stm32f407xx.h"
#include "string.h"

void Spi2Init(){
	SPI_Handle SPI2handle;
	SPI2handle.pSPIx=SPI2;
	SPI2handle.spi_pin_config.SPI_BusConfig=SPI_BUS_CONFIG_FD;
	SPI2handle.spi_pin_config.SPI_Devicemode= SPI_DEVICE_MODE_MASTER;
	SPI2handle.spi_pin_config.SPI_Speed= SPI_CLK_SPEED_DIV2;//Generates serial clk of 8 Mhz
	SPI2handle.spi_pin_config.SPI_DFF=SPI_DFF_8BITS;
	SPI2handle.spi_pin_config.SPI_CPOL=SPI_CPOL_HIGH;
	SPI2handle.spi_pin_config.SPI_CPHA= SPI_CPHA_LOW;
	SPI2handle.spi_pin_config.SPI_SSM=SPI_SSM_EN;//SSM enabled for NSS for software slave management

	SPI_init(&SPI2handle);

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

//	//MISO init-->14th pin
//	SPIpins.GPIO_PinConf.GPIO_PinNumber=GPIO_PIN_NO_14;
//	GPIO_init(&SPIpins);
//
//	//NSS init->12th pin
//	SPIpins.GPIO_PinConf.GPIO_PinNumber=GPIO_PIN_NO_12;
//	GPIO_init(&SPIpins);
}



int main()
{

	char userdata[]="Hello World";


	//This function is used to initialize GPIO pins behave as SPI2 pins
	Spi2GpioInit();

	//This function is used initialize the SPI2 peripheral registers
	Spi2Init();

	//This makes NSS bit internally high
	Spi_SSIConfig(SPI2, ENABLE);

	//Enable the SPI2 peripheral register
	SPI_PeripheralControl(SPI2,ENABLE);

	SPI_SendData(SPI2,(uint8_t*)userdata ,strlen(userdata));

    //To check if SPI has pending data to send
	while(SPI_GetFLagStatus(SPI2, SPI_SR_BSY ));

	//Disable the SPI2 peripheral register
	SPI_PeripheralControl(SPI2,DISABLE);

	while(1);

	return 0;

}
