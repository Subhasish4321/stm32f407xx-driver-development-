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
    uint8_t USART_OverSampling; // possible values from @USART_OverSampling
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
#define USART_MODE_TX    0
#define USART_MODE_RX    1
#define USART_MODE_TXRX  2

/**
 * USART Oversampling Macros
 */
#define USART_OVR_SMPL_8  1
#define USART_OVR_SMPL_16 0
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
#define USART_BAUD_921600  921600
#define USART_BAUD_2M      2000000
#define USART_BAUD_3M      3000000

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

/*
 *@USART_ParityControl
 *Possible options for USART_ParityControl
 */
#define USART_PARITY_EN_ODD   2
#define USART_PARITY_EN_EVEN  1
#define USART_PARITY_DISABLE   0

/**
 * USART Hardware Flow Control Macros
 */
#define USART_HW_FLOW_CTRL_NONE  0
#define USART_HW_FLOW_CTRL_CTS   1
#define USART_HW_FLOW_CTRL_RTS   2
#define USART_HW_FLOW_CTRL_CTS_RTS  3

/**
 * USART_CR1 Register Bit Position Macros
 */
#define USART_CR1_SBK    0 /* Send Break */
#define USART_CR1_RWU    1 /* Receiver wakeup */
#define USART_CR1_RE     2 /* Receiver enable */
#define USART_CR1_TE     3 /* Transmitter enable */
#define USART_CR1_IDLEIE 4 /* IDLE interrupt enable */
#define USART_CR1_RXNEIE 5 /* RXNE interrupt enable */
#define USART_CR1_TCIE   6 /* Transmission complete interrupt enable */
#define USART_CR1_TXEIE  7 /* TXE interrupt enable */
#define USART_CR1_PEIE   8 /* PE interrupt enable */
#define USART_CR1_PS     9 /* Parity selection */
#define USART_CR1_PCE    10 /* Parity control enable */
#define USART_CR1_WAKE   11 /* Wakeup method */
#define USART_CR1_M      12 /* Word length */
#define USART_CR1_UE     13 /* USART enable */
#define USART_CR1_OVER8  15 /* Oversampling by 8 */

/**
 * USART_CR2 Register Bit Position Macros
 */
#define USART_CR2_CPHA 9 /* Clock phase */
#define USART_CR2_CPOL 10 /* Clock polarity */
#define USART_CR2_CLKEN 11 /* Clock enable */
#define USART_CR2_STOP 12 /* STOP bits */

/**
 * USART_CR3 Register Bit Position Macros
 */
#define USART_CR3_CTSIE 10 /* CTS interrupt enable */
#define USART_CR3_CTSE 9 /* CTS enable */
#define USART_CR3_RTSE 8 /* RTS enable */

/**
 * USART_SR Register Bit Position Macros
 */
#define USART_SR_PE     0 /* Parity error */
#define USART_SR_FE     1 /* Framing error */
#define USART_SR_NE     2 /* Noise error */
#define USART_SR_ORE    3 /* Overrun error */
#define USART_SR_IDLE   4 /* IDLE line detected */
#define USART_SR_RXNE   5 /* Read data register not empty */
#define USART_SR_TC     6 /* Transmission complete */
#define USART_SR_TXE    7 /* Transmit data register empty */
#define USART_SR_LBD    8 /* Local Interconnect Network(LIN) break detection */
#define USART_SR_CTS    9 /* Clear To Send flag */
/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/
/*
 * Peripheral Clock setup
 */
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_RegDef_t *pUSARTx);


/*
 * Data Send and Receive
 */
void USART_SendData(USART_Handle_t *pUSARTHandle ,uint8_t *pTxBuffer, uint32_t Len);
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void USART_IRQHandling(USART_Handle_t *pHandle);

/*
 * Other Peripheral Control APIs
 */
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx , uint32_t FlagName);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);

/*
 * Application callback
 */
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t AppEv);


#endif /* --end of INC_STM32F407XX_USART_DRIVER_H_ ---*/
