/**
 * @file stm32f407xx_USART_driver.h
 * @brief Header file for USART driver for STM32F407xx microcontroller.
 * @author Subhasish Singha
 */
#ifndef INC_STM32F407XX_USART_DRIVER_H_
#define INC_STM32F407XX_USART_DRIVER_H_

#include <stdint.h>
#include "stm32f407xx.h"
 /**
  * Configuration structure for USART peripheral.
  */
 typedef struct
 {
    uint8_t  USART_Mode; // possible values from @USART_Mode
    uint32_t USART_Baud; // possible values from @USART_Baud
    uint8_t USART_NoOfStopBits; // possible values from @USART_NoOfStopBits
    uint8_t USART_WordLength; // possible values from @USART_WordLength
    uint8_t USART_ParityControl; // possible values from @USART_ParityControl
    uint8_t USART_HWFlowControl; // possible values from @USART_HWFlowControl
 } USART_Config_t;

 /**
  * Handle structure for USART peripheral.
  */
 typedef struct 
 {
    USART_RegDef_t *pUSARTx;
    USART_Config_t USART_Config;
 } USART_Handle_t;
 
 /**
  * USART Application states macros
  */
 #define USART_READY 0
 #define USART_BUSY_IN_RX 1
 #define USART_BUSY_IN_TX 2

/**
 * USART Mode Macros
 */
#define USART_MODE_TX_RX 0
#define USART_MODE_TX    1
#define USART_MODE_RX    2

/**
 * USART Baud Rate Macros
 */
#define USART_BAUD_1200    1200
#define USART_BAUD_2400    2400
#define USART_BAUD_9600    9600
#define USART_BAUD_19200   19200
#define USART_BAUD_38400   38400
#define USART_BAUD_57600   57600
#define USART_BAUD_115200  115200
#define USART_BAUD_230400  230400
#define USART_BAUD_460800  460800

/**
 * USART Number of Stop Bits Macros
 */
#define USART_STOPBITS_1    0
#define USART_STOPBITS_0_5  1
#define USART_STOPBITS_2    2
#define USART_STOPBITS_1_5  3

/**
 * USART Word Length Macros
 */
#define USART_WORDLEN_8BITS  0
#define USART_WORDLEN_9BITS  1

/**
 * USART Parity Control Macros
 */
#define USART_PARITY_EN_EVEN  0
#define USART_PARITY_EN_ODD   1
/**
 * USART Hardware Flow Control Macros
 */
#define USART_HW_FLOW_CTRL_NONE  0
#define USART_HW_FLOW_CTRL_CTS   1
#define USART_HW_FLOW_CTRL_RTS   2
#define USART_HW_FLOW_CTRL_CTS_RTS  3


/**
 * Status Register flag Macros for masking.
 */
#define USART_FLAG_SR1_TXE    (1 << USART_SR1_TXE)
#define USART_FLAG_SR1_RXNE   (1 << USART_SR1_RXNE)
#define USART_FLAG_SR1_SB     (1 << USART_SR1_SB)
#define USART_FLAG_SR1_ADDR   (1 << USART_SR1_ADDR)
#define USART_FLAG_SR1_BTF    (1 << USART_SR1_BTF)
#define USART_FLAG_SR1_STOPF  (1 << USART_SR1_STOPF)
#define USART_FLAG_SR1_BERR   (1 << USART_SR1_BERR)
#define USART_FLAG_SR1_ARLO   (1 << USART_SR1_ARLO)
#define USART_FLAG_SR1_AF     (1 << USART_SR1_AF)
#define USART_FLAG_SR1_OVR    (1 << USART_SR1_OVR)
#define USART_FLAG_SR1_TIMEOUT  (1 << USART_SR1_TIMEOUT)

 /**
  * USART Application event Macros
  */
#define USART_EV_TX_CMPLT 0
#define USART_EV_RX_CMPLT 1
#define USART_EV_STOP     2
#define USART_ERROR_BERR  3
#define USART_ERROR_ARLO  4
#define USART_ERROR_AF    5
#define USART_ERROR_OVR   6
#define USART_ERROR_TIMEOUT 7
#define USART_EV_DATA_REQ  8
#define USART_EV_DATA_RCV  9
#define USART_EV_DATA_REQ  8
#define USART_EV_DATA_RCV  9

/************************************************************************
 *        APIs supported by this driver
 * For more information about the APIs check the function definitions
 ************************************************************************/
/*
 * peripheral Clock setup
 */
void USART_PeriClockControl(USART_RegDef_t *pUSARTx,uint8_t EnorDi);
/*
 * USART Flag Status and acking management
 */
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName);
void USART_ManageAcking(USART_RegDef_t *pUSARTx,uint8_t EnOrDis);
/*
 * Init and DeInit
 */
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_RegDef_t *pUSARTx);
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);
/**
 * Data Send and receive APIs for master mode
 */
void USART_MasterSendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer,uint32_t len, uint8_t slaveAddr,uint8_t RptStart);
void USART_MasterReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer,uint8_t len, uint8_t slaveAddr,uint8_t RptStart);

/**
 * Data Send and receive APIs for slave mode
 */
void USART_SlaveSendData(USART_RegDef_t *pUSART, uint8_t data);
uint8_t USART_SlaveReceiveData(USART_RegDef_t *pUSART);
/**
 * Data Send and receive APIs using interrupt
 */
uint8_t USART_MasterSendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer,uint32_t len, uint8_t slaveAddr,uint8_t RptStart);
uint8_t USART_MasterReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer,uint8_t len, uint8_t slaveAddr,uint8_t RptStart);
void USART_CloseSendData(USART_Handle_t *pUSARTHandle);
void USART_CloseReceiveData(USART_Handle_t *pUSARTHandle);
void USART_GenerateStopCondition(USART_RegDef_t *pUSARTx);

/**
 * IRQ Configuration and ISR Handling APIs
 */
void USART_IRQ_Interrupt_Config(uint8_t IRQNumber,uint8_t EnorDis);
void USART_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority);
void USART_EV_IRQHandling(USART_Handle_t *pUSARTHandle);
void USART_ER_IRQHandling(USART_Handle_t *pUSARTHandle);

void USART_SlaveEnableDisableCallbackEvents(USART_RegDef_t *pUSARTx,uint8_t EnorDi);
/**
 * Application Callback.
 * Concept: The below function is to implemented on the application side i.e. it could have diff. defines for diff.requirement.
 * So we will leave a weak implementation in the driver.c file and 
 * the application will overwrite this function. We write a weak implementaion to prevent compiler errors.
 * we can use the __attribute__((weak))
 * attribute to define a weak function implementaion
*/
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t app_event);


#endif /* --end of INC_STM32F407XX_USART_DRIVER_H_ ---*/
