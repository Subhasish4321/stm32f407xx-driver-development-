#ifndef INC_STM32F407XX_RCC_DRIVER_H_
#define INC_STM32F407XX_RCC_DRIVER_H_

#include "stm32f407xx.h"

//This returns the PLL output clock frequency in Hz.
uint32_t RCC_GetPLLOutputClock(void );

//This returns the APB1 peripheral clock frequency in Hz.
uint32_t RCC_Get_PCLK1Value(void);

//This returns the APB2 peripheral clock frequency in Hz.
uint32_t RCC_Get_PCLK2Value(void);

#endif /* INC_STM32F407XX_RCC_DRIVER_H_ */