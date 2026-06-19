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
    uint8_t      *pTxBuffer; /* To store the app. Tx Buffer address */
    uint8_t      *pRxBuffer; /* To store the app. Rx Buffer address */
    uint32_t     Txlen; /* TO store TX Lenght*/
    uint32_t     Rxlen; /* TO store RX Lenght*/
    uint8_t      TxState; /* To store Tx state */
    uint8_t      RxState; /* To store Rx state */
} SPI_Handle_t;
/*
 *SPI application Macros
 */
#define  SPI_READY   0
#define  SPI_BUSY_IN_RX   1
#define  SPI_BUSY_IN_TX    2
/**
 * Possible SPI application events
 */
#define SPI_EVENT_TX_CMPLT  1
#define SPI_EVENT_RX_CMPLT  2
#define SPI_EVENT_OVR_ERR   3
#define SPI_EVENT_CRC_ERR   4
/**
 * SPI Device Modes Macros
 */
#define SPI_MODE_MASTER 1
#define SPI_MODE_SLAVE 0
/**
 * SPI_Bus configuration macros
 */
#define SPI_BUS_CONFIG_FD 1
#define SPI_BUS_CONFIG_HD 2
#define SPI_BUS_CONFIG_SIMPLEX_RX_ONLY 3
/**
 * SPI Data Frame Format macros
 */
#define SPI_DFF_8BITS  0
#define SPI_DFF_16BITS 1
/**
 * SPI Clock Phase macros
 */
#define SPI_CPHA_LOW 0
#define SPI_CPHA_HIGH 1
/**
 * SPI Clock Polarity macros
 */
#define SPI_CPOL_LOW 0
#define SPI_CPOL_HIGH 1
/**
 * SPI Software Slave Management macros
 */
#define SPI_SSM_ENABLE 1
#define SPI_SSM_DISABLE 0
/** 
 * SPI SCLK Speed macros
 */
#define SPI_SCK_SPEED_DIV2 0
#define SPI_SCK_SPEED_DIV4 1
#define SPI_SCK_SPEED_DIV8 2
#define SPI_SCK_SPEED_DIV16 3
#define SPI_SCK_SPEED_DIV32 4
#define SPI_SCK_SPEED_DIV64 5
#define SPI_SCK_SPEED_DIV128 6
#define SPI_SCK_SPEED_DIV256 7
/**
 * Status Register flag Macros for masking.
 */
#define SR_TXE_FLAG   (1 << SPI_SR_TXE)
#define SR_RXNE_FLAG  (1 << SPI_SR_RXNE)
#define SR_BUSY_FLAG  (1 << SPI_SR_BSY)

/************************************************************************
 *        APIs supported by this driver
 * For more information about the APIs check the function definitions
 ************************************************************************/
/*
 * peripheral Clock setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx,uint8_t EnorDi);
/*
 * SPI Flag Status.
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);
/*
 * Init and DeInit
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);
void SPI_EnableOrDisable(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_InternalSlaveSelectConfiguration(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSOE_Config(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
/**
 * Data Send and receive APIs 
 */
void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer, uint32_t len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRxBuffer,uint32_t len);
/**
 * Interrupt based Data Send and receive APIs
 */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle,uint8_t *pTxBuffer, uint32_t len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle,uint8_t *pRxBuffer,uint32_t len);
/**
 * IRQ Configuration and ISR Handling APIs
 */
void SPI_IRQ_IT_Config(uint8_t IRQNumber,uint8_t EnorDis);
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);
/**
 * Application Callback.
 * Concept: The below function is to implemented on the application side i.e. it could have diff. defines for diff.requirement.
 * So we will leave a weak implementation in the driver.c file and 
 * the application will overwrite this function. We write a weak implementaion to prevent compiler errors.
 * we can use the __attribute__((weak))
 * attribute to define a weak function implementaion
*/
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t app_event);
#endif /* SRC_STM32F407XX_SPI_DRIVER_H_ */
