/*
 * stm32f407xx_gpio_driver.h
 *
 *  Created on: Feb 22, 2026
 *      Author: ASUS
 */

#ifndef SRC_STM32F407XX_GPIO_DRIVER_H_
#define SRC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"

typedef struct
{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;
}GPIO_Pin_Config_t;

/*
 * This is a handle structure for a GPIO Pin
 */
typedef struct
{
	GPIO_RegDef_t *pGPIOx; /*This holds the base address of the GPIO Port to which the pin belongs*/
	GPIO_Pin_Config_t GPIO_PinConfig; /*This holds GPIo -pin configuration settings */
}GPIO_Handle_t;

/************************************************************************
 *        APIs supported by this driver
 * For more information about the APIs check the function definitions
 ************************************************************************/
/*
 * peripheral Clock setup
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx,uint8_t EnorDi);
/*
 * Init and DeInit
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);
/*
 * Read and write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber,uint8_t value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx,uint16_t value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber);
/*
 * IRQ Handling and ISR handling
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority,uint8_t EnorDis);
void GPIO_IRQHandling(uint8_t PinNumber);






#endif /* SRC_STM32F407XX_GPIO_DRIVER_H_ */
