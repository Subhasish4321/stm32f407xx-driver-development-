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
 } I2C_Handle_t;
 
 /**
  * @I2C_SCLSpeed
  */
#define I2C_SCL_SPEED_SM    100000
#define I2C_SCL_SPEED_FM4K  400000
#define I2C_SCL_SPEED_FM2K  200000

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

/************************************************************************
 *        APIs supported by this driver
 * For more information about the APIs check the function definitions
 ************************************************************************/
/*
 * peripheral Clock setup
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx,uint8_t EnorDi);
/*
 * I2C Flag Status.
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);
/*
 * Init and DeInit
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);
void I2C_EnableOrDisable(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);
/**
 * Data Send and receive APIs 
 */


/**
 * IRQ Configuration and ISR Handling APIs
 */
void I2C_IRQ_Interrupt_Config(uint8_t IRQNumber,uint8_t EnorDis);
void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority);
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