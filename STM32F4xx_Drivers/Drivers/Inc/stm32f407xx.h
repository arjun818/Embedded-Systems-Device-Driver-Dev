/*
 * stm32f407xx.h
 *
 *  Created on: Aug 28, 2024
 *      Author: Lenovo
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_
#include <stdint.h>
#include <stddef.h>
#define __vo                                     volatile
//Weak attribute def
#define __weak 									__attribute__((weak))

/*************Processor specific details***************/
//ARM Cortex M4 Processor NVIC ISRx register address

#define NVIC_ISR0                               ( (__vo uint32_t*)0xE000E100 )
#define NVIC_ISR1                               ( (__vo uint32_t*)0xE000E104 )
#define NVIC_ISR2                               ( (__vo uint32_t*)0xE000E108 )
#define NVIC_ISR3                               ( (__vo uint32_t*)0xE000E10C )

//ARM Cortex M4 Processor NVIC ICRx register address

#define NVIC_ICR0                               ( (__vo uint32_t*)0XE000E180 )
#define NVIC_ICR1                               ( (__vo uint32_t*)0XE000E184 )
#define NVIC_ICR2                               ( (__vo uint32_t*)0XE000E188 )
#define NVIC_ICR3                               ( (__vo uint32_t*)0XE000E18C )

//ARM Cortex M4 Processor NVIC Priority base register address
#define NVIC_PR_BASEADDR                        ( (__vo uint32_t*) 0xE000E400)


//No of priority bits implemented in ARM
#define NO_PR_BITS_IMPLEMENTED                  4

//Macros for flash and SRAM base addresses
#define FLASH_BASEADDR                           0x20000000U
#define SRAM1_BASEADDR                           0x08000000U
#define SRAM2_BASEADDR                           0x2001C000U
#define ROM                                      0x1FFF0000U

//Macros for AHB and APB buses
#define PERIPH_BASEADDR                          0x40000000U
#define AHB1_BASEADDR                            0x40020000U
#define AHB2_BASEADDR                            0x50000000U
#define AHB3_BASEADDR                            0xA0000000U

#define APB1_BASEADDR                            PERIPH_BASEADDR
#define APB2_BASEADDR                            0x40010000U

#define SRAM SRAM1_BASEADDR                      SRAM1_BASEADDR

//Macros for the hanging peripherals in AHB and APB
//Peripherals hanging on the AHB-1 bus

#define GPIOA_BASEADDR                           AHB1_BASEADDR
#define GPIOB_BASEADDR                           0x40020400U
#define GPIOC_BASEADDR                           0x40020800U
#define GPIOD_BASEADDR                           0x40020C00U
#define GPIOE_BASEADDR                           0x40021000U
#define GPIOF_BASEADDR                           0x40021400U
#define GPIOG_BASEADDR                           0x40021800U
#define GPIOH_BASEADDR                           0x40021C00U
#define GPIOI_BASEADDR                           0x40022000U
#define RCC_BASEADDR                             0x40023800U


//Peripherals hanging on the APB-1

#define I2C1_BASEADDR                            0x40005400U
#define I2C2_BASEADDR                            0x40005800U
#define I2C3_BASEADDR                            0x40005C00U
#define SPI2_BASEADDR                            0x40003800U
#define SPI3_BASEADDR                            0x40003C00U
#define USART2_BASEADDR                          0x40004400U
#define USART3_BASEADDR                          0x40004800U
#define UART4_BASEADDR                           0x40004C00U
#define UART5_BASEADDR                           0x40005000U

//Peripherals hanging on the APB-2

#define SPI1_BASEADDR                            0x40013000U
#define SPI4_BASEADDR                            0x40013400U
#define USART1_BASEADDR                          0x40011000U
#define USART6_BASEADDR                          0x40011400U
#define EXTI_BASEADDR                            0x40013C00U
#define SYSCFG_BASEADDR                          0x40013800U



/****************Peripheral definition structures*****************/

//Structure for GPIO peripheral register
typedef struct
{
    volatile uint32_t PortModeRegister;
	volatile uint32_t PortOutTypeRegister;
	volatile uint32_t PortOutSpeedRegister;
	volatile uint32_t PortPullUpPulldownRegister;
	volatile uint32_t PortInDataRegister;
	volatile uint32_t PortOutDataRegister;
	volatile uint32_t PortBitSetResetRegister;
	volatile uint32_t PortConfLockRegister;
	volatile uint32_t AFR[2];
}GPIO_reg;

//Structure for RCC peripheral register

typedef struct
{
	volatile uint32_t RCC_CR;
	volatile uint32_t RCC_PLLCFGR;
	volatile uint32_t RCC_CFGR;
	volatile uint32_t RCC_CIR;
	volatile uint32_t RCC_AHB1RSTR;
	volatile uint32_t RCC_AHB2RSTR;
	volatile uint32_t RCC_AHB3RSTR;
	uint32_t RESERVED0;
	volatile uint32_t RCC_APB1RSTR;
	volatile uint32_t RCC_APB2RSTR;
	uint32_t RESERVED1[2];
	volatile uint32_t RCC_AHB1ENR;
	volatile uint32_t RCC_AHB2ENR;
	volatile uint32_t RCC_AHB3ENR;
	uint32_t RESERVED2;
	volatile uint32_t RCC_APB1ENR;
	volatile uint32_t RCC_APB2ENR;
	uint32_t RESERVED3[2];
	volatile uint32_t RCC_AHB1LPENR;
	volatile uint32_t RCC_AHB2LPENR;
	volatile uint32_t RCC_AHB3LPENR;
	uint32_t RESERVED4;
	volatile uint32_t RCC_APB1LPENR;
	volatile uint32_t RCC_APB2LPENR;
	uint32_t RESERVED5[2];
	volatile uint32_t RCC_BDCR;
	volatile uint32_t RCC_CSR;
	uint32_t RESERVED6[2];
	volatile uint32_t RCC_SSCGR;
	volatile uint32_t RCC_PLLI2SCFGR;
	volatile uint32_t RCC_PLLSAICFGR;
	volatile uint32_t RCC_DCKCFGR;

}RCC_reg;

//Structure for SPI peripheral register

typedef struct{
	__vo uint32_t SPI_CR1;
	__vo uint32_t SPI_CR2;
	__vo uint32_t SPI_SR;
	__vo uint32_t SPI_DR;
	__vo uint32_t SPI_CRCPR;
	__vo uint32_t SPI_RXCRCR;
	__vo uint32_t SPI_TXCRCR;
	__vo uint32_t SPI_I2SCFGR;
	__vo uint32_t SPI_I2SPR;
}SPI_reg;

//Structure for EXTI peripheral registers
typedef struct
{
	__vo uint32_t EXTI_IMR;
	__vo uint32_t EXTI_EMR;
	__vo uint32_t EXTI_RTSR;
	__vo uint32_t EXTI_FTSR;
	__vo uint32_t EXTI_SWIER;
	__vo uint32_t EXTI_PR;
}EXTI_reg;

//Structure for I2C peripheral registers
typedef struct{
	__vo uint32_t I2C_CR1;
	__vo uint32_t I2C_CR2;
	__vo uint32_t I2C_OAR1;
	__vo uint32_t I2C_OAR2;
	__vo uint32_t I2C_DR;
	__vo uint32_t I2C_SR1;
	__vo uint32_t I2C_SR2;
	__vo uint32_t I2C_CCR;
	__vo uint32_t I2C_TRISE;
	__vo uint32_t I2C_FLTR;
}I2C_reg;

//Structure for SYSCONFIG peripheral register
typedef struct
{
	__vo uint32_t SYSCFG_MEMRMP;
	__vo uint32_t SYSCFG_PMC;
	__vo uint32_t SYSCFG_EXTICR[4];
	uint32_t RESERVED1[2];
	__vo uint32_t CMPCR;
	uint32_t RESERVED2[2];
	__vo uint32_t CFGR;
}SYSCONFIG_reg;

typedef struct
{
	__vo uint32_t USART_SR;         /*!< USART Status register   								Address offset: 0x00 */
	__vo uint32_t USART_DR;         /*!< USART Data register   								Address offset: 0x04 */
	__vo uint32_t USART_BRR;        /*!< USART Baud rate register    							Address offset: 0x08 */
	__vo uint32_t USART_CR1;        /*!< USART Control register 1    							Address offset: 0x0C */
	__vo uint32_t USART_CR2;        /*!< USART Control register 2    							Address offset: 0x10 */
	__vo uint32_t USART_CR3;        /*!< USART Control register 3    							Address offset: 0x14 */
	__vo uint32_t USART_GTPR;       /*!< USART Guard time and prescaler register   			Address offset: 0x18 */
} USART_reg;



/********Peripheral definition macros********/
//For GPIOs
#define GPIOA 		((GPIO_reg *)GPIOA_BASEADDR)
#define GPIOB 		((GPIO_reg *)GPIOB_BASEADDR)
#define GPIOC 		((GPIO_reg *)GPIOC_BASEADDR)
#define GPIOD 		((GPIO_reg *)GPIOD_BASEADDR)
#define GPIOE       ((GPIO_reg *)GPIOE_BASEADDR)
#define GPIOF       ((GPIO_reg *)GPIOF_BASEADDR)
#define GPIOG       ((GPIO_reg *)GPIOG_BASEADDR)
#define GPIOH       ((GPIO_reg *)GPIOH_BASEADDR)
#define GPIOI       ((GPIO_reg *)GPIOI_BASEADDR)

//For RCC
#define RCC         ((RCC_reg*)RCC_BASEADDR)

//For EXTI
#define EXTI        ((EXTI_reg*)EXTI_BASEADDR)

//For System Configuration
#define SYS_CONFIG  ((SYSCONFIG_reg*)SYSCFG_BASEADDR)

//For SPI
#define SPI1        ((SPI_reg*)SPI1_BASEADDR)
#define SPI2        ((SPI_reg*)SPI2_BASEADDR)
#define SPI3        ((SPI_reg*)SPI3_BASEADDR)
#define SPI4        ((SPI_reg*)SPI4_BASEADDR)

//For I2C
#define I2C1		((I2C_reg*)I2C1_BASEADDR)
#define I2C2		((I2C_reg*)I2C2_BASEADDR)
#define I2C3		((I2C_reg*)I2C3_BASEADDR)

//For USART
#define USART1  ( (USART_reg*)USART1_BASEADDR )
#define USART2  ( (USART_reg*)USART2_BASEADDR )
#define USART3  ( (USART_reg*)USART3_BASEADDR )
#define UART4  	( (USART_reg*)UART4_BASEADDR )
#define UART5  	( (USART_reg*)UART5_BASEADDR )
#define USART6  ( (USART_reg*)USART6_BASEADDR )

/*Clock enable macros for GPIOs*/

//Setting clock using OR operators
#define GPIOA_PERI_CLOCK_ENABLE()     (RCC->RCC_AHB1ENR|=(1<<0))
#define GPIOB_PERI_CLOCK_ENABLE()     (RCC->RCC_AHB1ENR|=(1<<1))
#define GPIOC_PERI_CLOCK_ENABLE()     (RCC->RCC_AHB1ENR|=(1<<2))
#define GPIOD_PERI_CLOCK_ENABLE()     (RCC->RCC_AHB1ENR|=(1<<3))
#define GPIOE_PERI_CLOCK_ENABLE()     (RCC->RCC_AHB1ENR|=(1<<4))
#define GPIOF_PERI_CLOCK_ENABLE()     (RCC->RCC_AHB1ENR|=(1<<5))
#define GPIOG_PERI_CLOCK_ENABLE()     (RCC->RCC_AHB1ENR|=(1<<6))
#define GPIOH_PERI_CLOCK_ENABLE()     (RCC->RCC_AHB1ENR|=(1<<7))
#define GPIOI_PERI_CLOCK_ENABLE()     (RCC->RCC_AHB1ENR|=(1<<8))

/*Clock enabling for I2C*/
#define I2C1_PERI_CLOCK_ENABLE()       (RCC->RCC_APB1ENR|=(1<<21))
#define I2C2_PERI_CLOCK_ENABLE()       (RCC->RCC_APB1ENR|=(1<<22))
#define I2C3_PERI_CLOCK_ENABLE()       (RCC->RCC_APB1ENR|=(1<<23))

/*Clock enabling for SPI*/
#define SPI1_PERI_CLOCK_ENABLE()       (RCC->RCC_APB2ENR|=(1<<12))
#define SPI2_PERI_CLOCK_ENABLE()       (RCC->RCC_APB1ENR|=(1<<14))
#define SPI3_PERI_CLOCK_ENABLE()       (RCC->RCC_APB1ENR|=(1<<15))
#define SPI4_PERI_CLOCK_ENABLE()       (RCC->RCC_APB1ENR|=(1<<13))

/*Clock enabling for USART and UART*/
#define USART1_PERI_CLOCK_ENABLE()		(RCC->RCC_APB2ENR|=(1 << 4))
#define USART2_PERI_CLOCK_ENABLE()      (RCC->RCC_APB1ENR|=(1<<17))
#define USART3_PERI_CLOCK_ENABLE()      (RCC->RCC_APB1ENR|=(1<<18))
#define UART4_PERI_CLOCK_ENABLE()       (RCC->RCC_APB1ENR|=(1<<19))
#define UART5_PERI_CLOCK_ENABLE()       (RCC->RCC_APB1ENR|=(1<<20))
#define USART6_PERI_CLOCK_ENABLE()		(RCC ->RCC_APB2ENR|=(1 << 5))

/*Clock enabling for system configuration*/
#define SYS_PERI_CLOCK_ENABLE()       (RCC->RCC_APB2ENR|=(1<<14))




/*Clock disabling macros for GPIOs */
#define GPIOA_PERI_CLOCK_DISABLE()     (RCC->RCC_AHB1ENR&=~(1<<0))
#define GPIOB_PERI_CLOCK_DISABLE()     (RCC->RCC_AHB1ENR&=~(1<<1))
#define GPIOC_PERI_CLOCK_DISABLE()     (RCC->RCC_AHB1ENR&=~(1<<2))
#define GPIOD_PERI_CLOCK_DISABLE()     (RCC->RCC_AHB1ENR&=~(1<<3))
#define GPIOE_PERI_CLOCK_DISABLE()     (RCC->RCC_AHB1ENR&=~(1<<4))
#define GPIOF_PERI_CLOCK_DISABLE()     (RCC->RCC_AHB1ENR&=~(1<<5))
#define GPIOG_PERI_CLOCK_DISABLE()     (RCC->RCC_AHB1ENR&=~(1<<6))
#define GPIOH_PERI_CLOCK_DISABLE()     (RCC->RCC_AHB1ENR&=~(1<<7))
#define GPIOI_PERI_CLOCK_DISABLE()     (RCC->RCC_AHB1ENR&=~(1<<8))

/*Clock disabling for I2C*/
#define I2C1_PERI_CLOCK_DISABLE()       (RCC->RCC_APB1ENR&=~(1<<21))
#define I2C2_PERI_CLOCK_DISABLE()       (RCC->RCC_APB1ENR&=~(1<<22))
#define I2C3_PERI_CLOCK_DISABLE()       (RCC->RCC_APB1ENR&=~(1<<23))

/*Clock disabling for SPI*/
#define SPI1_PERI_CLOCK_DISABLE()      (RCC->RCC_APB2ENR&=~(1<<12))
#define SPI2_PERI_CLOCK_DISABLE()      (RCC->RCC_APB1ENR&=~(1<<14))
#define SPI3_PERI_CLOCK_DISABLE()      (RCC->RCC_APB1ENR&=~(1<<15))
#define SPI4_PERI_CLOCK_DISABLE()      (RCC->RCC_APB2ENR&=~(1<<13))

/*Clock disabling for USART and UART*/
#define USART1_PERI_CLOCK_DISABLE()	   (RCC ->RCC_APB2ENR&=~(1 << 4))
#define USART2_PERI_CLOCK_DISABLE()    (RCC->RCC_APB1ENR&=~(1<<17))
#define USART3_PERI_CLOCK_DISABLE()    (RCC->RCC_APB1ENR&=~(1<<18))
#define UART4_PERI_CLOCK_DISABLE()     (RCC->RCC_APB1ENR&=~(1<<19))
#define UART5_PERI_CLOCK_DISABLE()     (RCC->RCC_APB1ENR&=~(1<<20))
#define USART6_PERI_CLOCK_DISABLE()		(RCC->RCC_APB2ENR&=~(1 << 5))

/*Clock disabling for system configuration*/
#define SYS_PERI_CLOCK_DISABLE()       (RCC->RCC_APB2ENR&=~(1<<14))

//Reset macros for GPIO Register Resetting

#define GPIOA_REG_RESET()               do{(RCC->RCC_AHB1RSTR)|=(1<<0);  ((RCC->RCC_AHB1RSTR)&=~(1<<0));}while(0)
#define GPIOB_REG_RESET()               do{(RCC->RCC_AHB1RSTR)|=(1<<1);  ((RCC->RCC_AHB1RSTR)&=~(1<<1));}while(0)
#define GPIOC_REG_RESET()               do{(RCC->RCC_AHB1RSTR)|=(1<<2);  ((RCC->RCC_AHB1RSTR)&=~(1<<2));}while(0)
#define GPIOD_REG_RESET()               do{(RCC->RCC_AHB1RSTR)|=(1<<3);  ((RCC->RCC_AHB1RSTR)&=~(1<<3));}while(0)
#define GPIOE_REG_RESET()               do{(RCC->RCC_AHB1RSTR)|=(1<<4);  ((RCC->RCC_AHB1RSTR)&=~(1<<4));}while(0)
#define GPIOF_REG_RESET()               do{(RCC->RCC_AHB1RSTR)|=(1<<5);  ((RCC->RCC_AHB1RSTR)&=~(1<<5));}while(0)
#define GPIOG_REG_RESET()               do{(RCC->RCC_AHB1RSTR)|=(1<<6);  ((RCC->RCC_AHB1RSTR)&=~(1<<6));}while(0)
#define GPIOH_REG_RESET()               do{(RCC->RCC_AHB1RSTR)|=(1<<7);  ((RCC->RCC_AHB1RSTR)&=~(1<<7));}while(0)
#define GPIOI_REG_RESET()               do{(RCC->RCC_AHB1RSTR)|=(1<<8);  ((RCC->RCC_AHB1RSTR)&=~(1<<8));}while(0)


//Reset macro for SPI Register Resetting
#define SPI1_REG_RESET()               do{(RCC->RCC_APB2ENR)|=(1<<12);  ((RCC->RCC_APB2ENR)&=~(1<<12));}while(0)
#define SPI2_REG_RESET()               do{(RCC->RCC_APB1ENR)|=(1<<14);  ((RCC->RCC_APB1ENR)&=~(1<<14));}while(0)
#define SPI3_REG_RESET()               do{(RCC->RCC_APB1ENR)|=(1<<15);  ((RCC->RCC_APB1ENR)&=~(1<<15));}while(0)
#define SPI4_REG_RESET()               do{(RCC->RCC_APB2ENR)|=(1<<13);  ((RCC->RCC_APB2ENR)&=~(1<<13));}while(0)

//Reset macro for I2C Register Resetting
#define I2C1_REG_RESET()               do { (RCC->RCC_APB1ENR |= (1 << 21)); (RCC->RCC_APB1ENR &= ~(1 << 21)); } while(0)
#define I2C2_REG_RESET()               do { (RCC->RCC_APB1ENR |= (1 << 22)); (RCC->RCC_APB1ENR &= ~(1 << 22)); } while(0)
#define I2C3_REG_RESET()               do { (RCC->RCC_APB1ENR |= (1 << 23)); (RCC->RCC_APB1ENR &= ~(1 << 23)); } while(0)



//Return the port code for given GPIO address
#define GPIO_BASEADDR_TO_CODE(x)     ((x==GPIOA) ? 0:\
		                              (x==GPIOB) ? 1:\
		                              (x==GPIOC) ? 2:\
		                              (x==GPIOB) ? 3:\
		                              (x==GPIOA) ? 4:\
		                              (x==GPIOB) ? 5:\
		                              (x==GPIOA) ? 6:\
		                              (x==GPIOB) ? 7:0 )


//Interrupt request numbers of STM32f407xx MCU
#define IRQ_NO_EXTI0                 6
#define IRQ_NO_EXTI1                 7
#define IRQ_NO_EXTI2                 8
#define IRQ_NO_EXTI3                 9
#define IRQ_NO_EXTI4                 10
#define IRQ_NO_EXTI9_5               23
#define IRQ_NO_EXTI15_10             40
#define IRQ_NO_I2C1_EV				 31
#define IRQ_NO_I2C1_ER				 32
#define IRQ_NO_I2C2_EV				 33
#define IRQ_NO_I2C2_ER				 34
#define IRQ_NO_I2C3_EV				 79
#define IRQ_NO_I2C3_ER				 80
#define IRQ_NO_USART1	    		 37
#define IRQ_NO_USART2	    		 38
#define IRQ_NO_USART3	    		 39
#define IRQ_NO_UART4	    		 52
#define IRQ_NO_UART5	   			 53
#define IRQ_NO_USART6	   			 71

//Interrupt request priorities number
#define NVIC_IRQ_PRIORITY_0          13
#define NVIC_IRQ_PRIORITY_1  		 14
#define NVIC_IRQ_PRIORITY_2          15
#define NVIC_IRQ_PRIORITY_3          16
#define NVIC_IRQ_PRIORITY_4          17
#define NVIC_IRQ_PRIORITY_9_5        30
#define NVIC_IRQ_PRIORITY_15         47

//Some generic macros
#define ENABLE                          1
#define DISABLE                         0
#define SET                             ENABLE
#define RESET                           DISABLE
#define GPIO_PIN_SET                    SET
#define GPIO_PIN_RESET                  RESET
#define FLAG_RESET                      RESET
#define FLAG_SET                        SET

//Bit position definition for SPI_CR1 peripherals
#define SPI_CR1_CPHA                    0
#define SPI_CR1_CPOL                    1
#define SPI_CR1_MSTR                    2
#define SPI_CR1_BR                      3
#define SPI_CR1_SPE                     6
#define SPI_CR1_LSBFIRST                7
#define SPI_CR1_SSI                     8
#define SPI_CR1_SSM                     9
#define SPI_CR1_RXONLY                  10
#define SPI_CR1_DFF                     11
#define SPI_CR1_CRCNEXT                 12
#define SPI_CR1_CRCEN                   13
#define SPI_CR1_BIDIOE                  14
#define SPI_CR1_BIDIMODE                15

//Bit positions definition for SPI_CR2 peripherals
#define SPI_CR2_RXDMAEN                 0
#define SPI_CR2_TXDMAEN                 1
#define SPI_CR2_SSOE                    2
#define SPI_CR2_FRF                     4
#define SPI_CR2_ERRIE                   5
#define SPI_CR2_RXNEIE                  6
#define SPI_CR2_TXEIE                   7

//Bit positions definition for SPI_SR peripherals
#define SPI_SR_RXNE                     0
#define SPI_SR_TXE                      1
#define SPI_SR_CHSIDE                   2
#define SPI_SR_UDR                      3
#define SPI_SR_CRCERR                   4
#define SPI_SR_MODF                     5
#define SPI_SR_OVR                      6
#define SPI_SR_BSY                      7
#define SPI_SR_FRE                      8

//Bit positions definition for I2C_CR1 peripherals
#define I2C_CR1_PE						0
#define I2C_CR1_NOSTRETCH				7
#define I2C_CR1_START					8
#define I2C_CR1_STOP					9
#define I2C_CR1_ACK						10
#define I2C_CR1_SWRST					15

//Bit positions definition for I2C_CR2 peripherals
#define I2C_CR2_FREQ					0
#define I2C_CR2_ITERREN					8
#define I2C_CR2_ITEVTEN					9
#define I2C_CR2_ITBUFEN					10

//Bit positions definition for I2C_SR1 peripherals
#define I2C_SR1_SB						0
#define I2C_SR1_ADDR					1
#define I2C_SR1_BTF						2
#define I2C_SR1_ADD10					3
#define I2C_SR1_STOPF					4
#define I2C_SR1_RXNE					6
#define I2C_SR1_TXE						7
#define I2C_SR1_BERR					8
#define I2C_SR1_ARLO					9
#define I2C_SR1_AF						10
#define I2C_SR1_OVR						11
#define I2C_SR1_PECERR					12
#define I2C_SR1_TIMEOUT					14
#define I2C_SR1_SMBALERT				15

//Bit positions definition for I2C_SR2 peripherals
#define I2C_SR2_MSL						0
#define I2C_SR2_BUSY					1
#define I2C_SR2_TRA						2
#define I2C_SR2_GENCALL					4
#define I2C_SR2_DUALF					7

//Bit positions definition for I2C_CCR peripherals
#define I2C_CCR_CCR						0
#define I2C_CCR_DUTY					14
#define I2C_CCR_FS						15

/*
 * Bit position definitions USART_CR1
 */
#define USART_CR1_SBK					0
#define USART_CR1_RWU 					1
#define USART_CR1_RE  					2
#define USART_CR1_TE 					3
#define USART_CR1_IDLEIE 				4
#define USART_CR1_RXNEIE  				5
#define USART_CR1_TCIE					6
#define USART_CR1_TXEIE					7
#define USART_CR1_PEIE 					8
#define USART_CR1_PS 					9
#define USART_CR1_PCE 					10
#define USART_CR1_WAKE  				11
#define USART_CR1_M 					12
#define USART_CR1_UE 					13
#define USART_CR1_OVER8  				15


/*
 * Bit position definitions USART_CR2
 */
#define USART_CR2_ADD   				0
#define USART_CR2_LBDL   				5
#define USART_CR2_LBDIE  				6
#define USART_CR2_LBCL   				8
#define USART_CR2_CPHA   				9
#define USART_CR2_CPOL   				10
#define USART_CR2_STOP   				12
#define USART_CR2_LINEN   				14


/*
 * Bit position definitions USART_CR3
 */
#define USART_CR3_EIE   				0
#define USART_CR3_IREN   				1
#define USART_CR3_IRLP  				2
#define USART_CR3_HDSEL   				3
#define USART_CR3_NACK   				4
#define USART_CR3_SCEN   				5
#define USART_CR3_DMAR  				6
#define USART_CR3_DMAT   				7
#define USART_CR3_RTSE   				8
#define USART_CR3_CTSE   				9
#define USART_CR3_CTSIE   				10
#define USART_CR3_ONEBIT   				11


/*
 * Bit position definitions USART_SR
 */

#define USART_SR_PE        				0
#define USART_SR_FE        				1
#define USART_SR_NE        				2
#define USART_SR_ORE       				3
#define USART_SR_IDLE       			4
#define USART_SR_RXNE        			5
#define USART_SR_TC        				6
#define USART_SR_TXE        			7
#define USART_SR_LBD        			8
#define USART_SR_CTS        			9

#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"
#include "stm32f407xx_i2c_driver.h"
#include "stm32f407xx_usart_driver.h"
#include "stm32f407xx_rcc_driver.h"
#endif /* INC_STM32F407XX_H_ */
