/**
 * @file stm32f407xx_i2c_driver.h
 * @brief Header file for I2C driver for STM32F407xx microcontroller.
 * @author Subhasish Singha
 */
#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_

#include <stdint.h>
#include "stm32f407xx.h"
 /**
  * Configuration structure for I2C peripheral.
  */
 typedef struct
 {
     uint32_t I2C_SCLSpeed;
     uint8_t  I2C_DeviceAddress;
     uint8_t  I2C_AckControl;
     uint8_t  I2C_FMDutyCycle;  /*Fast mode duty cycle*/
 } I2C_Config_t;

 /**
  * Handle structure for I2C peripheral.
  */
 typedef struct 
 {
    I2C_RegDef_t *pI2Cx;
    I2C_Config_t I2C_Config;
    uint8_t      *pTxBuffer; /* To store the app. Tx Buffer address */
    uint8_t      *pRxBuffer; /* To store the app. Rx Buffer address */
    uint32_t     Txlen; /* TO store TX Lenght*/
    uint32_t     Rxlen; /* TO store RX Lenght*/
    uint8_t      TxRxState; /* To store Tx state */
    uint8_t      DevAddr; /* To store the slave/device address */
    uint32_t     RxSize; /* To store the receive size */
    uint8_t      Sr; /* To store repeated start value */
 } I2C_Handle_t;
 
 /**
  * I2C Application states macros
  */
 #define I2C_READY 0
 #define I2C_BUSY_IN_RX 1
 #define I2C_BUSY_IN_TX 2

 /**
  * @I2C_SCLSpeed
  */
#define I2C_SCL_SPEED_SM    100000
#define I2C_SCL_SPEED_FM4K  400000
#define I2C_SCL_SPEED_FM2K  200000
/**
 * I2c repeated start control based macro
 * When a I2C communication is done with a slave device, the master can either send a stop condition or a repeated start condition after the communication.
 * If the master sends a stop condition, it releases the bus and the slave can communicate with other masters. If the master sends a repeated start condition, 
 * it keeps control of the bus and can continue communicating with the same slave or switch to another slave without releasing the bus.
 * Note:Sometimes if the master genearates a stop condition between two communication then another master can get control of the bus and can communicate with the slave device.
 * This can lead to data loss or corruption. To avoid this, we can use repeated start condition.
 * 
 * Below macros helps to control the repeated start condition in the I2C communication.
 */
#define I2C_ENABLE_SR  1
#define I2C_DISABLE_SR 0
/**
  * @I2C_ACKControl
  */
#define I2C_ACK_ENABLE  1
#define I2C_ACK_DISABLE 0
/**
  * @I2C_FMDutyCycle
  */
#define I2C_FM_2     0
#define I2C_FM_16_9  1

/**
 * Status Register flag Macros for masking.
 */
#define I2C_FLAG_SR1_TXE   (1 << I2C_SR1_TXE)
#define I2C_FLAG_SR1_RXNE  (1 << I2C_SR1_RXNE)
#define I2C_FLAG_SR1_SB   (1 << I2C_SR1_SB)
#define I2C_FLAG_SR1_ADDR  (1 << I2C_SR1_ADDR)
#define I2C_FLAG_SR1_BTF   (1 << I2C_SR1_BTF)
#define I2C_FLAG_SR1_STOPF   (1 << I2C_SR1_STOPF)
#define I2C_FLAG_SR1_BERR   (1 << I2C_SR1_BERR)
#define I2C_FLAG_SR1_ARLO   (1 << I2C_SR1_ARLO)
#define I2C_FLAG_SR1_AF   (1 << I2C_SR1_AF)
#define I2C_FLAG_SR1_OVR   (1 << I2C_SR1_OVR)
#define I2C_FLAG_SR1_TIMEOUT   (1 << I2C_SR1_TIMEOUT)

 /**
  * I2C Application event Macros
  */

#define I2C_EV_TX_CMPLT 0
#define I2C_EV_RX_CMPLT 1
#define I2C_EV_STOP     2
#define I2C_ERROR_BERR  3
#define I2C_ERROR_ARLO  4
#define I2C_ERROR_AF    5
#define I2C_ERROR_OVR   6
#define I2C_ERROR_TIMEOUT 7
#define I2C_EV_DATA_REQ  8
#define I2C_EV_DATA_RCV  9
#define I2C_EV_DATA_REQ  8
#define I2C_EV_DATA_RCV  9

/************************************************************************
 *        APIs supported by this driver
 * For more information about the APIs check the function definitions
 ************************************************************************/
/*
 * peripheral Clock setup
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx,uint8_t EnorDi);
/*
 * I2C Flag Status and acking management
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx,uint8_t EnOrDis);
/*
 * Init and DeInit
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);
/**
 * Data Send and receive APIs for master mode
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer,uint32_t len, uint8_t slaveAddr,uint8_t RptStart);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer,uint8_t len, uint8_t slaveAddr,uint8_t RptStart);

/**
 * Data Send and receive APIs for slave mode
 */
void I2C_SlaveSendData(I2C_RegDef_t *pI2C, uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C);
/**
 * Data Send and receive APIs using interrupt
 */
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer,uint32_t len, uint8_t slaveAddr,uint8_t RptStart);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer,uint8_t len, uint8_t slaveAddr,uint8_t RptStart);
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);
void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle);
void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);

/**
 * IRQ Configuration and ISR Handling APIs
 */
void I2C_IRQ_Interrupt_Config(uint8_t IRQNumber,uint8_t EnorDis);
void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority);
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);
/**
 * Application Callback.
 * Concept: The below function is to implemented on the application side i.e. it could have diff. defines for diff.requirement.
 * So we will leave a weak implementation in the driver.c file and 
 * the application will overwrite this function. We write a weak implementaion to prevent compiler errors.
 * we can use the __attribute__((weak))
 * attribute to define a weak function implementaion
*/
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t app_event);


#endif /* --end of INC_STM32F407XX_I2C_DRIVER_H_ ---*/
