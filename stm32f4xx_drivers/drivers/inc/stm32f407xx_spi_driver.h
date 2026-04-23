/** 
 *  \file stm32f407xx_spi_driver.h
 *
 *  \date Created on: April 23, 2026
 *  \author Subhasish Singha
 *
 **/

#ifndef SRC_STM32F407XX_SPI_DRIVER_H_
#define SRC_STM32F407XX_SPI_DRIVER_H_

#include <stdint.h>
#include "stm32f407xx.h"
/**
 * This is a configuration structure for SPIx peripheral
 */
typedef struct 
{
    uint8_t  SPI_DeviceMode;      /*!< Master or slave mode>*/
    uint8_t  SPI_BusConfig;       /*!< Full duplex, half duplex or simplex rx only>*/
    uint8_t  SPI_DFF;             /*!< 8 bit data or 16 bit data>*/
    uint8_t  SPI_CPHA;            /*!< Clock phase>*/
    uint8_t  SPI_CPOL;            /*!< Clock polarity>*/ 
    uint8_t  SPI_SSM;             /*!< Software slave management>*/
    uint8_t  SPI_SclkSpeed;       /*!< Speed of SCLK >*/
} SPI_Config_t;
/**
 * This is a handle structure for SPIx peripheral
 */
typedef struct 
{
    SPI_RegDef_t *pSPIx; /* This holds the base address of the SPIx(x:0,1,2) peripheral to which the pin belongs */
    SPI_Config_t SPI_Config; /* This holds the SPI configuration settings */
} SPI_Handle_t;
/**
 * SPI Device Modes Macros
 */
#define SPI_MODE_MASTER 1
#define SPI_MODE_SLAVE 0
/**
 * SPI_Bus configuration macros
 */
#define SPI_BUS_CONFIG_FD 0
#define SPI_BUS_CONFIG_HD 1
#define SPI_BUS_CONFIG_SRX 2
#define SPI_BUS_CONFIG_STX 3
/**
 * SPI Data Frame Format macros
 */
#define SPI_DFF_8BITS 0
#define SPI_DFF_16BITS 1
/**
 * SPI Clock Phase macros
 */
#define SPI_CPHA0 0
#define SPI_CPHA1 1
/**
 * SPI Clock Polarity macros
 */
#define SPI_CPOL0 0
#define SPI_CPOL1 1
/**
 * SPI Software Slave Management macros
 */
#define SPI_SSM_EN 1
#define SPI_SSM_DI 0
/** 
 * SPI SCLK Speed macros
 */
#define SPI_SCK_SPEED_DIV2 0
#define SPI_SCK_SPEED_DIV4 1
#define SPI_SCK_SPEED_DIV8 2
#define SPI_SCK_SPEED_DIV16 3


#endif /* SRC_STM32F407XX_SPI_DRIVER_H_ */