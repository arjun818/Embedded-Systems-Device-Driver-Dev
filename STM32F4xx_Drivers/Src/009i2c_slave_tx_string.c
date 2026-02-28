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
#define MY_ADDR				SLAVE_ADDR


I2C_Handle I2C1Handle;
uint8_t Tx_buf[32] = "Hi, This is Arjun";


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

	//Initialize button functionality
	GpioBtnInit();

	//1. I2C pins initializations
	I2C1GpioInit();

	//2. I2C peripheral initialization
	I2C1Init();

	//I2C IRQ Configurations
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);


	I2c_SlaveEnableDisableCallbackEvents(I2C1,ENABLE);

	//3. Enable the peripheral
	I2C_PeripheralControl(I2C1, ENABLE);

	//Ack bit is made 1 after PE=1
	I2C_ManageAcking(I2C1, I2C_ACK_ENABLE);

	//Wait for button press
	while(1);
}
void I2C1_EV_IRQHandler(void){
	I2C_EV_IRQHandling(&I2C1Handle);
}

void I2C1_ER_IRQHandler(void){
	I2C_ER_IRQHandling(&I2C1Handle);
}

void I2C_ApplicationEventCallback(I2C_Handle *pI2CHandle,uint8_t AppEv){
	static uint8_t commandcode = 0;
		static uint8_t count = 0;

		if(AppEv == I2C_EV_DATA_REQ)
		{
			// Master require data, slave -> send
			if(commandcode == 0x51)
			{
				//Send Len
				I2C_SlaveSendData(pI2CHandle->pI2Cx, strlen((char*)Tx_buf));
			}else if(commandcode == 0x52)
			{
				//Send contents of Tx_buf
				I2C_SlaveSendData(pI2CHandle->pI2Cx, Tx_buf[count++]);
			}

		}else if(AppEv == I2C_EV_DATA_RCV)
		{
			// slave -> Receive
			commandcode = I2C_SlaveReceiveData(pI2CHandle->pI2Cx);

		}else if(AppEv == I2C_ERROR_AF)
		{
			// Happens in Slave TX, master -> NACK
			commandcode = 0xff;
			count = 0;

		}else if(AppEv == I2C_EV_STOP)
		{
			// Happens in Slave RX, master ended slave communication
			count = 0;
		}
}

