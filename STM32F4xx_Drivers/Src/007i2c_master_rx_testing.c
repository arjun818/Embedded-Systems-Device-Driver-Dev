/*
 * 006i2c_master_tx_testing.c
 *
 *  Created on: Jan 28, 2025
 *      Author: Lenovo
 */

#include "stm32f407xx.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define SLAVE_ADDR			0x68
#define MY_ADDR				0x61
I2C_Handle I2C1Handle;

extern void initialise_monitor_handles();

//Receive buffer
uint8_t rcv_buffer[32];

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

void I2C1Init(void){
	I2C1Handle.pI2Cx= I2C1;
	I2C1Handle.I2C_Config.I2C_ACKControl=I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress=MY_ADDR;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle= I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed= I2C_SCL_SPEED_SM;

	I2C_init(&I2C1Handle);


}
void I2C1GpioInit(void){
	GPIO_Handle I2Cpins;
	I2Cpins.pGPIOx=GPIOB;
	I2Cpins.GPIO_PinConf.GPIO_PinMode=GPIO_MODE_ALTFN;
	I2Cpins.GPIO_PinConf.GPIO_PinAltFunMode=4;
	I2Cpins.GPIO_PinConf.GPIO_PinOpType=GPIO_OP_TYPE_OD;
	I2Cpins.GPIO_PinConf.GPIO_PinPuPdControl=GPIO_PIN_PU;
	I2Cpins.GPIO_PinConf.GPIO_PinSpeed=GPIO_OP_TYPE_HS;

	//SCL
	I2Cpins.GPIO_PinConf.GPIO_PinNumber=GPIO_PIN_NO_6;
	GPIO_init(&I2Cpins);

	//SDA
	I2Cpins.GPIO_PinConf.GPIO_PinNumber=GPIO_PIN_NO_7;
	GPIO_init(&I2Cpins);
}
int main(void){
	uint8_t commandcode;
	uint8_t len;

	initialise_monitor_handles();

	printf("Application is running\n");

	//Initialize button functionality
	GpioBtnInit();

	//1. I2C pins initializations
	I2C1GpioInit();

	//2. I2C peripheral initialization
	I2C1Init();

	//3. Enable the peripheral
	I2C_PeripheralControl(I2C1, ENABLE);

	//Ack bit is made 1 after PE=1
	I2C_ManageAcking(I2C1, I2C_ACK_ENABLE);

	//Wait for button press
	while(1){
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));//The loop exits when button is pressed

		delay();// delay for button de-bouncing

		commandcode=0x51;

		I2C_MasterSendData(&I2C1Handle, &commandcode,1, SLAVE_ADDR,I2C_ENABLE_SR);

		I2C_MasterReceiveData(&I2C1Handle, &len, 1, SLAVE_ADDR, I2C_ENABLE_SR);

		commandcode=0x52;

		I2C_MasterSendData(&I2C1Handle, &commandcode,1, SLAVE_ADDR, I2C_ENABLE_SR);

		I2C_MasterReceiveData(&I2C1Handle, rcv_buffer, len, SLAVE_ADDR, I2C_DISABLE_SR);

		rcv_buffer[len + 1]='\0';

		printf("Data: %s", rcv_buffer);
	}
}
