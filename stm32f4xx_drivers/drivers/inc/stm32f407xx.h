/*
 * stm32f407xx.h
 *
 *  Created on: Feb 19, 2026
 *      Author: ASUS
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>

#define __vo  volatile
/*
 * ARM Cortex MX Processor NVIC ISERx register Addresses
 * We can define the ISERs upto 7 but that's not required now hence making upto 3
 * We are adding 0x4 to addresses as the reg size is 32 bits(4 byte).
 */
#define NVIC_ISER0       ((__vo uint32_t*)0xE000E100 )
#define NVIC_ISER1       ((__vo uint32_t*)0xE000E104 )
#define NVIC_ISER2       ((__vo uint32_t*)0xE000E108 )
#define NVIC_ISER3       ((__vo uint32_t*)0xE000E10C )
/*
 *  ARM Cortex MX Processor NVIC ICERx register Addresses
 */
#define NVIC_ICER0       ((__vo uint32_t*)0XE000E180 )
#define NVIC_ICER1       ((__vo uint32_t*)0XE000E184 )
#define NVIC_ICER2       ((__vo uint32_t*)0XE000E188 )
#define NVIC_ICER3       ((__vo uint32_t*)0XE000E18C )
/*
 *  ARM Cortex Mx Processor NVIC Priority register Base Address.
 */
#define NVIC_PR_BASEADDR       ((__vo uint32_t*)0xE000E400 )
/*
 * The number of bits which is implemented in each section. Different for different Processor.
 */
#define NO_OF_PR_BITS_IMPLEMENTED  4
/*
 * base addresses of flash and SRAM memories
 */

#define FLASH_BASEADDR          0x08000000U        /*base address of flash memory*/
#define SRAM1_BASEADDR          0X20000000U        /*base address of SRAM1*/
#define SRAM                    SRAM1_BASEADDR     /*base address of SRAM*/
#define SRAM2_BASEADDR          0X2001C000U        /*base address of SRAM2*/
#define ROM_BASEADDR            0x1FFF0000U        /*base address of system memory*/

/*
 *  APBx and AHBx peripheral base addresses
 */

#define APB1_PERIPH_BASEADDR        0x40000000U        /*base address of APB1 peripheral bus*/
#define AHB1_PERIPH_BASEADDR        0x40020000U        /*base address of AHB1 peripheral bus*/
#define APB2_PERIPH_BASEADDR        0x40010000U        /*base address of APB2 peripheral bus*/
#define AHB2_PERIPH_BASEADDR        0x50000000U        /*base address of AHB2 peripheral bus*/

/*
 * Base addresses of AHB1 peripherals
 */
#define GPIOA_BASEADDR           (AHB1_PERIPH_BASEADDR) /* Base address of GPIOA */
#define GPIOB_BASEADDR           (AHB1_PERIPH_BASEADDR + 0x0400) /* Base address of GPIOB */
#define GPIOC_BASEADDR           (AHB1_PERIPH_BASEADDR + 0x0800) /* Base address of GPIOC */
#define GPIOD_BASEADDR           (AHB1_PERIPH_BASEADDR + 0x0C00) /* Base address of GPIOD */
#define GPIOE_BASEADDR           (AHB1_PERIPH_BASEADDR + 0x1000) /* Base address of GPIOE */
#define GPIOF_BASEADDR           (AHB1_PERIPH_BASEADDR + 0x1400) /* Base address of GPIOF */
#define GPIOG_BASEADDR           (AHB1_PERIPH_BASEADDR + 0x1800) /* Base address of GPIOG */
#define GPIOH_BASEADDR           (AHB1_PERIPH_BASEADDR + 0x1C00) /* Base address of GPIOH */
#define GPIOI_BASEADDR           (AHB1_PERIPH_BASEADDR + 0x2000) /* Base address of GPIOI */
#define RCC_BASEADDR             (AHB1_PERIPH_BASEADDR + 0x3800)  /* RCC base address */
/*
 * base addresses of APB1 peripherals
 */
#define I2C1_BASEADDR			(APB1_PERIPH_BASEADDR + 0x5400) /*Base address of I2C1 */
#define I2C2_BASEADDR			(APB1_PERIPH_BASEADDR + 0x5800) /*Base address of I2C2 */
#define I2C3_BASEADDR			(APB1_PERIPH_BASEADDR + 0x5C00) /*Base address of I2C3 */
#define SPI2_BASEADDR			(APB1_PERIPH_BASEADDR + 0x3800) /*Base address of SPI2 */
#define SPI3_BASEADDR			(APB1_PERIPH_BASEADDR + 0x3C00) /*Base address of SPI3 */
#define USART2_BASEADDR			(APB1_PERIPH_BASEADDR + 0x4400) /*Base address of USART2 */
#define USART3_BASEADDR			(APB1_PERIPH_BASEADDR + 0x4800) /*Base address of USART3 */
#define UART4_BASEADDR			(APB1_PERIPH_BASEADDR + 0x4C00) /*Base address of UART4 */
#define UART5_BASEADDR			(APB1_PERIPH_BASEADDR + 0x5000) /*Base address of UART5 */

/*
 * base addresses of APB2 peripherals
 */
#define SPI1_BASEADDR           (APB2_PERIPH_BASEADDR + 0x3000) /*Base address of SPI1 */
#define USART1_BASEADDR			(APB2_PERIPH_BASEADDR + 0x1000) /*Base address of USART1 */
#define USART6_BASEADDR         (APB2_PERIPH_BASEADDR + 0x1400) /*Base address of USART6 */
#define EXTI_BASEADDR			(APB2_PERIPH_BASEADDR + 0x3C00) /*Base address of EXTI */
#define SYSCFG_BASEADDR         (APB2_PERIPH_BASEADDR + 0x3800) /*Base address of SYSCFG */

/* Note : The registers of a peripheral are specific to a NCU
 * Please check always device reference manual
 */
typedef struct
{
	__vo uint32_t MODER;		/*GPIO port mode register    Address offset : 0x00 */
	__vo uint32_t OTYPER;		/*GPIO port output type register    Address offset : 0x04 */
	__vo uint32_t OSPEEDR;		/*GPIO port output speed register    Address offset : 0x08 */
	__vo uint32_t PUPDR;		/*GPIO port pull-up/pull-down register    Address offset : 0x0C */
	__vo uint32_t IDR;	 		/*GPIO port input data register    Address offset : 0x10 */
	__vo uint32_t ODR;			/*GPIO port output data register    Address offset : 0x14 */
	__vo uint32_t BSSR;		    /*GPIO port bit set/reset register    Address offset : 0x18 */
	__vo uint32_t LCKR;			/*GPIO port configuration lock register    Address offset : 0x1C */
	__vo uint32_t AFR[2];		/*AFR[0]:GPIO Alternate function low register  AFR[1]:AF High register Address offset : 0x20 */
} GPIO_RegDef_t;
/*
 * Peripheral register definition structure for RCC
 */
typedef struct
{
	__vo uint32_t CR;
	__vo uint32_t PLLCFGR;
	__vo uint32_t CFGR;
	__vo uint32_t CIR;
	__vo uint32_t AHB1RSTR;
	__vo uint32_t AHB2RSTR;
	__vo uint32_t AHB3RSTR;
	     uint32_t RESERVED0;
	__vo uint32_t APB1RSTR;
	__vo uint32_t APB2RSTR;
	     uint32_t RESERVED1[2];
	__vo uint32_t AHB1ENR;
	__vo uint32_t AHB2ENR;
	__vo uint32_t AHB3ENR;
		 uint32_t RESERVED2;
	__vo uint32_t APB1ENR;
	__vo uint32_t APB2ENR;
		 uint32_t RESERVED3[2];
    __vo uint32_t AHB1LPENR;
	__vo uint32_t AHB2LPENR;
	__vo uint32_t AHB3LPENR;
	     uint32_t RESERVED4;
	__vo uint32_t APB1LPENR;
	__vo uint32_t APB2LPENR;
	     uint32_t RESERVED5[2];
	__vo uint32_t BDCR;
	__vo uint32_t CSR;
		 uint32_t RESERVED6[2];
	__vo uint32_t SSCGR;
	__vo uint32_t PLLI2SCFGR;
	__vo uint32_t PLLSAICFGR;
	__vo uint32_t DCKCFGR;

}RCC_Reg_Def_t;
/*
 * Peripheral register definition structure for EXTI
 */
typedef struct
{
	__vo uint32_t IMR; /**< Interrupt mask register */
	__vo uint32_t EMR; /**< Event mask register */
	__vo uint32_t RTSR; /**< Rising trigger selection register */
	__vo uint32_t FTSR; /**< Falling trigger selection register */
	__vo uint32_t SWIER; /**< Software interrupt event register */
	__vo uint32_t PR; /**< Pending register */
}EXTI_RegDef_t;
/*
 * Peripheral register definition structure for SYSCFG
 */
typedef struct
{
	__vo uint32_t MEMRMP;     /*SYSCFG memory remap register*/
	__vo uint32_t PMC;        /*SYSCFG peripheral mode configuration register*/
	__vo uint32_t EXTICR[4];  /*SYSCFG external interrupt configuration register 1 to 4 */
    uint32_t RESERVED1[2];
	__vo uint32_t CMPCR;      /* Compensation cell control register */
	uint32_t RESERVED2[2];
	__vo uint32_t CFGR;
}SYSCFG_RegDef_t;
/**
 * SPI Register definition structure for SPIx peripheral
 */
typedef struct
{
    uint32_t SPI_CR1;
	uint32_t SPI_CR2;
	uint32_t SPI_SR;
	uint32_t SPI_DR;
	uint32_t SPI_CRCPR;
	uint32_t SPI_RXCRCR;
	uint32_t SPI_TXCRCR;
	uint32_t SPI_I2SCFGR;
	uint32_t SPI_I2SPR;
}SPI_RegDef_t;
/*
 * Peripheral definitions (Peripheral base address type casted to xxxRegDef_t)
 */
#define GPIOA  ((GPIO_RegDef_t*)GPIOA_BASEADDR)  /*type casted peripheral base address for GPIOA */
#define GPIOB  ((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC  ((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD  ((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE  ((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF  ((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG  ((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH  ((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI  ((GPIO_RegDef_t*)GPIOI_BASEADDR)  /*type casted peripheral base address for GPIOI */

#define RCC    ((RCC_Reg_Def_t*)RCC_BASEADDR)    /*type casted peripheral base address for RCC */

#define EXTI   ((EXTI_RegDef_t*)EXTI_BASEADDR)  /*type casted peripheral base address for EXTI */

#define SYSCFG  ((SYSCFG_RegDef_t*)SYSCFG_BASEADDR) /*type casted peripheral base address for SYSCFG */
#define SPI1    ((SPI_RegDef_t*)SPI1_BASEADDR) /*type casted peripheral base address for SPI1 */
#define SPI2    ((SPI_RegDef_t*)SPI2_BASEADDR) /*type casted peripheral base address for SPI2 */
#define SPI3    ((SPI_RegDef_t*)SPI3_BASEADDR) /*type casted peripheral base address for SPI3 */
/*
 * Clock enable macros for GPIOx peripherals.
 */
#define GPIOA_PCLK_EN()    (RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN()    (RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN()    (RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN()    (RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN()    (RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN()    (RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN()    (RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN()    (RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN()    (RCC->AHB1ENR |= (1 << 8))

/*
 * Clock enable macros for SPIx peripherals
 */
#define SPI1_PCLK_EN()      (RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()      (RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()      (RCC->APB1ENR |= (1 << 15))

/*
 * Clock enable macros for USARTx peripherals.
 */
#define USART1_PCLK_EN()      (RCC->APB2ENR |= (1 << 4))
#define USART6_PCLK_EN()      (RCC->APB2ENR |= (1 << 6))
#define USART2_PCLK_EN()      (RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()      (RCC->APB1ENR |= (1 << 18))
#define UART4_PCLK_EN()       (RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()       (RCC->APB1ENR |= (1 << 20))


/*
 * Clock enable macros for I2Cx peripherals.
 */
#define I2C1_PCLK_EN()       (RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()       (RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()       (RCC->APB1ENR |= (1 << 23))

/*
 * Clock enable macros for SYSCFG peripherals.
 */
#define SYSCFG_PCLK_EN()    (RCC->APB2ENR |= (1 << 14))

/*
 * Clock disable macros for GPIOx peripherals.
 */
#define GPIOA_PCLK_DIS()    (RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DIS()    (RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DIS()    (RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DIS()    (RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DIS()    (RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DIS()    (RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DIS()    (RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DIS()    (RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DIS()    (RCC->AHB1ENR &= ~(1 << 8))
/*
 * Clock disable macros for SPIx peripherals
 */
#define SPI1_PCLK_DIS()      (RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DIS()      (RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DIS()      (RCC->APB1ENR &= ~(1 << 15))

/*
 * Clock disable macros for USARTx peripherals.
 */
#define USART1_PCLK_DIS()      (RCC->APB2ENR &= ~(1 << 4))
#define USART6_PCLK_DIS()      (RCC->APB2ENR &= ~(1 << 6))
#define USART2_PCLK_DIS()      (RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DIS()      (RCC->APB1ENR &= ~(1 << 18))
#define UART4_PCLK_DIS()       (RCC->APB1ENR &= ~(1 << 19))
#define UART5_PCLK_DIS()       (RCC->APB1ENR &= ~(1 << 20))
/*
 * Clock disable macros for I2Cx peripherals.
 */
#define I2C1_PCLK_DIS()       (RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DIS()       (RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DIS()       (RCC->APB1ENR &= ~(1 << 23))

/*
 * Clock disable macros for SYSCFG peripherals.
 */
#define SYSCFG_CLK_DIS()    (RCC->APB2ENR &= ~(1 << 14))

/*
 * Macros to reset GPIOx peripherals.
 * Note: we will first set the bit and then do a reset of the same bit for the purpose explained in DeInit() function
 */
#define GPIOA_REG_RESET()     do{ (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); } while(0)
#define GPIOB_REG_RESET()     do{ (RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); } while(0)
#define GPIOC_REG_RESET()     do{ (RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); } while(0)
#define GPIOD_REG_RESET()     do{ (RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3)); } while(0)
#define GPIOE_REG_RESET()     do{ (RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4)); } while(0)
#define GPIOF_REG_RESET()     do{ (RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5)); } while(0)
#define GPIOG_REG_RESET()     do{ (RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6)); } while(0)
#define GPIOH_REG_RESET()     do{ (RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7)); } while(0)
#define GPIOI_REG_RESET()     do{ (RCC->AHB1RSTR |= (1 << 8)); (RCC->AHB1RSTR &= ~(1 << 8)); } while(0)
/*
 * A simple function like Macro that takes the GPIPx(Base addr) as input and accordingly defines the macro
 * according to the GPIO Port given as input.
 * The \ is used in macro to represent continuitiy of the macro in the next line.
 */
#define GPIO_BASEADDR_TO_CODE(x)  ((x == GPIOA) ? 0:\
		                           (x == GPIOB) ? 1:\
		                           (x == GPIOC) ? 2:\
								   (x == GPIOD) ? 3:\
								   (x == GPIOE) ? 4:\
								   (x == GPIOF) ? 5:\
								   (x == GPIOG) ? 6:\
								   (x == GPIOH) ? 7:8)
/*
 * IRQ(Interrupt Request) Number of STM32F407x MCU
 * Note: update these macros with valid values according to the MCU
 * NVIC : Nested Vector Interrupt Controller. It is a hardware unit in ARM-Cortex-M processors
 * that manages and prioritizes asynchronous events(Interrupts).
 */
#define IRQ_NO_EXTI0       6
#define IRQ_NO_EXTI1  	   7
#define IRQ_NO_EXTI2       8
#define IRQ_NO_EXTI3       9
#define IRQ_NO_EXTI4   	  10
#define IRQ_NO_EXTI9_5 	  23
#define IRQ_NO_EXTI15_10  40
/*
 * IRQ Priority
 */
#define NVIC_IRQ_PRI0  0
#define NVIC_IRQ_PRI1  1
#define NVIC_IRQ_PRI2  2
#define NVIC_IRQ_PRI3  3
#define NVIC_IRQ_PRI4  4
#define NVIC_IRQ_PRI5  5
#define NVIC_IRQ_PRI6  6
#define NVIC_IRQ_PRI7  7
#define NVIC_IRQ_PRI8  8
#define NVIC_IRQ_PRI9  9
#define NVIC_IRQ_PRI10  10
#define NVIC_IRQ_PRI11  11
#define NVIC_IRQ_PRI12  12
#define NVIC_IRQ_PRI13  13
#define NVIC_IRQ_PRI14  14
#define NVIC_IRQ_PRI15  15
/*
 * Some generic Macros
 */
#define ENABLE    1
#define DISABLE   0
#define SET       1
#define RESET     0
#define GPIO_PIN_SET   1
#define GPIO_PIN_RESET   0

#include "stm32f407xx_gpio_driver.h"
#endif /* INC_STM32F407XX_H_ */
