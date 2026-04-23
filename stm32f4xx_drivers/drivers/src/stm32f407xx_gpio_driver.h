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
	uint8_t GPIO_PinNumber;			 /*!< Possible values from @GPIO_PIN_NUMBERS >*/
	uint8_t GPIO_PinMode;            /*!< Possible values from @GPIO_PIN_MODES >*/
	uint8_t GPIO_PinSpeed;           /*!< Possible values from @GPIO_PIN_SPEED >*/
	uint8_t GPIO_PinPuPdControl;	 /*!< Possible values from @GPIO_PIN_PUPD_CONTROL >*/
	uint8_t GPIO_PinOPType;          /*!< Possible values from @GPIO_PIN_OP_TYP >*/
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

/*
 * @GPIO_PIN_NUMBERS
 */
#define GPIO_PIN_NO_0     0
#define GPIO_PIN_NO_1     1
#define GPIO_PIN_NO_2     2
#define GPIO_PIN_NO_3     3
#define GPIO_PIN_NO_4     4
#define GPIO_PIN_NO_5     5
#define GPIO_PIN_NO_6     6
#define GPIO_PIN_NO_7     7
#define GPIO_PIN_NO_8     8
#define GPIO_PIN_NO_9     9
#define GPIO_PIN_NO_10    10
#define GPIO_PIN_NO_11    11
#define GPIO_PIN_NO_12    12
#define GPIO_PIN_NO_13    13
#define GPIO_PIN_NO_14    14
#define GPIO_PIN_NO_15    15
/*
 * @GPIO_PIN_MODES
 * Modes set according the reference manual for the device/
 */
#define GPIO_MODE_IN     0
#define GPIO_MODE_OUT    1
#define GPIO_MODE_ALTFN  2
#define GPIO_MODE_ANALOG 3
#define GPIO_MODE_IT_FT  4   /*Interrupt Falling edge trigger */
#define GPIO_MODE_IT_RT  5
#define GPIO_MODE_IT_RFT 6  /*Interrupt Rising-Falling edge trigger */
/*
 * @GPIO_PIN_OP_TYP
 * GPIO PIN output type registers
 */
#define GPIO_OP_TYPE_PP  0
#define GPIO_OP_TYPE_OD  1
/*
 * @GPIO_PIN_SPEED
 * GPIO pin possible ouput speeds
 */
#define GPIO_SPEED_LOW     0
#define GPIO_SPEED_MEDIUM  1
#define GPIO_SPEED_FAST    2
#define GPIO_SPEED_HIGH    3
/*
 * @GPIO_PIN_PUPD_CONTROL
 * GPIO pin pull up and pull down configuration register.
 */
#define GPIO_NO_PUPD      0
#define GPIO_PIN_PU       1
#define GPIO_PIN_PD       2

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
void GPIO_IRQ_IT_Config(uint8_t IRQNumber,uint8_t EnorDis);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);





#endif /* SRC_STM32F407XX_GPIO_DRIVER_H_ */
