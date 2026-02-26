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
#define SYSCFG_CLK_EN()    (RCC->APB2ENR |= (1 << 14))

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
