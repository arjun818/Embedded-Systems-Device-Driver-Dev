/*
 * stm32f407xx_gpio_driver.h
 *
 *  Created on: Aug 28, 2024
 *      Author: Lenovo
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_
#include "stm32f407xx.h"

/* The below is the configuration structure*/

typedef struct
{
	uint8_t GPIO_PinNumber;////@GPIO pin numbering values
	uint8_t GPIO_PinMode;//@GPIO pinmode possible values
	uint8_t GPIO_PinSpeed;//@GPIO pinspeed possible values
	uint8_t GPIO_PinPuPdControl;//@GPIO pinPUPD possible values
	uint8_t GPIO_PinOpType;
	uint8_t GPIO_PinAltFunMode;
}GPIO_Pin_Config;


/*The below is the Handle structure*/

typedef struct
{
	//pointer to hold the base address of the GPIO peripheral register
	GPIO_reg *pGPIOx;// This pointer holds the base address of any GPIO port's base address
	GPIO_Pin_Config GPIO_PinConf;

}GPIO_Handle;



/****Define Macros for the different modes of GPIO pin in all possible modes****/

//@GPIO pinmode possible values
#define GPIO_MODE_IN                    0
#define GPIO_MODE_OUT                   1
#define GPIO_MODE_ALTFN                 2
#define GPIO_MODE_ANALOG                3

//Our own code numbers given here for interrupt modes
#define GPIO_MODE_IT_FT                 4
#define GPIO_MODE_IT_RT                 5
#define GPIO_MODE_IT_RFT                6

//GPIO pin possible output type
#define GPIO_OP_TYPE_PP                 0
#define GPIO_OP_TYPE_OD                 1//OD-Open Drain

//GPIO pin possible output speeds
//@GPIO pin speed possible values
#define GPIO_OP_TYPE_LS                 0
#define GPIO_OP_TYPE_MS                 1
#define GPIO_OP_TYPE_HS                 2
#define GPIO_OP_TYPE_VHS                3

//GPIO pin possible PU/PD
//@GPIO pinPUPD possible values
#define GPIO_NO_PUPD                    0
#define GPIO_PIN_PU                     1
#define GPIO_PIN_PD                     2

//GPIO pin number (0-15)
//@GPIO pin numbering values
#define GPIO_PIN_NO_0                   0
#define GPIO_PIN_NO_1                   1
#define GPIO_PIN_NO_2                   2
#define GPIO_PIN_NO_3                   3
#define GPIO_PIN_NO_4                   4
#define GPIO_PIN_NO_5                   5
#define GPIO_PIN_NO_6                   6
#define GPIO_PIN_NO_6                   6
#define GPIO_PIN_NO_7                   7
#define GPIO_PIN_NO_8                   8
#define GPIO_PIN_NO_9                   9
#define GPIO_PIN_NO_10                  10
#define GPIO_PIN_NO_11                  11
#define GPIO_PIN_NO_12                  12
#define GPIO_PIN_NO_13                  13
#define GPIO_PIN_NO_14                  14
#define GPIO_PIN_NO_15                  15



/*****Protyping of all functions*****/

//Peripheral clock setup API
void GPIO_PeriClckInit(GPIO_reg *pGPIOx,uint8_t EnorDi);

//Init and Deinit of ports
void GPIO_init(GPIO_Handle *pGPIOHandle);
void GPIO_Deinit(GPIO_reg *pGPIOx);

//Data read and write in ports and pin
uint8_t GPIO_ReadFromInputPin(GPIO_reg *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_reg *pGPIOx);
void GPIO_WriteFromOutputPin(GPIO_reg *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteFromOutputPort(GPIO_reg *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_reg *pGPIOx, uint8_t PinNumber);

//IRQ configuration and ISR handling
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);





#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */
