/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: Feb 22, 2026
 *  Author: Subhasish Singha
 */

#include "stm32f407xx_gpio_driver.h"

/************************************************************************
 *        APIs supported by this driver
 ************************************************************************/
/*
 * peripheral Clock setup
 */

/************************************************************************
 * @fn              - GPIO_PeriClockControl
 *
 * @brief           - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]       - base address of the gpio pheripheral
 * @param[in]       - ENABLE or DISABLE Macros
 * @param[in]       -
 *
 * @return          - none
 *
 * @Note            - none
 *
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx,uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}
		else if(pGPIOx == GPIOI)
		{
			GPIOI_PCLK_EN();
		}
	}
	else
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DIS();
		}else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DIS();
		}else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DIS();
		}else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DIS();
		}else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DIS();
		}else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DIS();
		}else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_DIS();
		}else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_DIS();
		}
		else if (pGPIOx == GPIOI)
		{
			GPIOI_PCLK_DIS();
		}
	}
}
/*
 * Init and DeInit
 */
/************************************************************************
 * @fn              - GPIO_Init
 *
 * @brief           - This function initializes a given GPIO port
 *
 * @param[in]       - base address of the gpio pheripheral
 * @param[in]       - none
 * @Understand : Here what we do is we set the value of GPIOHandle like GPIO_PinMode= GPIO_PIN_MODE_OUT
 * and then we use a pointer to access that handle and then do << as we want to reach the specific byte
 * that affects the specific pin number as each register is used for all 16 pin configuration.
 * pGPIOHandle->GPIO_PinConfig.GPIO_PinMode = GPIO_PIN_MODE_OUT and this line 121 just do this :
 * temp = 1 << 12 assuming Gpio pin 12 which equals 0x00 00 08 00(08 = 0000 1000) which is then assigned
 * to the MODER register by using the base address pointed by GPIOx->MODER.
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp = 0;
	//Configure the mode of GPIO pin
    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {
    	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    	pGPIOHandle->pGPIOx->MODER &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);//clearing
        pGPIOHandle->pGPIOx->MODER |= temp;//setting

    }else
    {
    	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			//configure the FTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//clear the corresponding RTSR bit
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
           //comfigure the RTSR
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//clear the corresponding RTSR bit
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			//configure both FTSR and RTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}

        /*
         * configure the GPIO port selection in SYSCFG_EXTICR
         *
         * we need to know which EXTI_CR to select for that we do :Gpio_PinNumber / 4
         * We need to also know which bit to use to write the config. val, we do:4*(pinnum%4)
         * for ex. if pin num=5 ,5%4 =1 and then 1*4 that means bit 4 i.e. 0,1,2,3,4 bit and not 4th bit
         * so we will left shift by 4 and then write the correspondic port selection va;. For Port A :0000
         * for Port D:0011 and so on
         *
         */
    	uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
    	uint8_t temp2 = 4* (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4);
    	uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
    	SYSCFG_PCLK_EN();
        SYSCFG->EXTICR[temp1] = portcode << temp2 ;

		//Enable the exti interrupt delivery using IMR(Interrupt Mask Register)
		EXTI->IMR |= 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
    }
    temp = 0;
	//COnfigure the speed
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);//clearing
    pGPIOHandle->pGPIOx->OSPEEDR |= temp ;

    temp = 0;
	//Configure the pupd settings
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); //clearing
    pGPIOHandle->pGPIOx->PUPDR |= temp ;

    temp = 0;
	//Configure the output type
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);//clearing
    pGPIOHandle->pGPIOx->OTYPER |= temp ;

    temp = 0;
	//Configure the alternate functionality
    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
    {
		uint8_t temp1,temp2;
		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF <<( 4* temp2)); //Clearing
		pGPIOHandle->pGPIOx->AFR[temp1] |= pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4*temp2);
    }
}
/************************************************************************
 * @fn              - GPIO_DeInit
 *
 * @brief           - This function deinitializes a given GPIO port
 *
 * @param[in]       - base address of the gpio pheripheral
 *
 * @Note: In order to reset all the GPIO peripheal registes we need to use the indibidual bus type
 * peripheral reset register(RCC_AHB1RSTR in this case) and then set the individual peripheral bit to 1 and then
 *in the next step we need to reset the same bit to 0 otherwise the register will be continuoulsy reset unneccessarily.
 *
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
    if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}else if(pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}else if(pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}else if(pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}else if(pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}else if(pGPIOx == GPIOF)
	{
		GPIOF_REG_RESET();
	}else if(pGPIOx == GPIOG)
	{
		GPIOG_REG_RESET();
	}else if(pGPIOx == GPIOH)
	{
		GPIOH_REG_RESET();
	}
	else if(pGPIOx == GPIOI)
	{
		GPIOI_REG_RESET();
	}
}
/*
 * Read and write
 */
/************************************************************************
 * @fn              - GPIO_ReadFromInputPin
 *
 * @brief           - This function allows reading from an input pin
 *
 * @param[in]       - base address of the gpio pheripheral
 * @param[in]       - pin number
 * @param[in]       - none
 *
 * @return          - the current value in the pin
 *
 * @Note            - none
 *
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);
	return value;
}
/************************************************************************
 * @fn              - GPIO_ReadFromInputPort
 *
 * @brief           - This function allows reading from an input port
 *
 * @param[in]       - base address of the gpio pheripheral
 * @param[in]       - none
 * @param[in]       - none
 *
 * @return          - the current value in the port
 *
 * @Note            - none
 *
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
    uint16_t value;
	value = (uint16_t)pGPIOx->IDR ;
	return value;
}
/************************************************************************
 * @fn              - GPIO_WriteToOutputPin
 *
 * @brief           - This function allows writing to an output pin.
 *
 * @param[in]       - base address of the gpio pheripheral
 * @param[in]       - pin number
 * @param[in]       - the value to be written
 *
 * @return          - none
 *
 * @Note            - none
 *
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber,uint8_t value)
{
	if (value == GPIO_PIN_SET)
	{
		pGPIOx->ODR |= (1 << PinNumber);
	}
	else
	{
		pGPIOx->ODR &= ~(1 << PinNumber);
	}
}
/************************************************************************
 * @fn              - GPIO_WriteToOutputPort
 *
 * @brief           - This function allows writing to an output port.
 *
 * @param[in]       - base address of the gpio pheripheral
 * @param[in]       - none
 * @param[in]       - the value to be written
 *
 * @return          - none
 *
 * @Note            - none
 *
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx,uint16_t value)
{
	pGPIOx->ODR = value;
}
/************************************************************************
 * @fn              - GPIO_ToggleOutputPin
 *
 * @brief           - This function allows toggling the value of an output pin.
 *
 * @param[in]       - base address of the gpio pheripheral
 * @param[in]       - pin number
 * @param[in]       - none
 *
 * @return          - none
 *
 * @Note            - we can use the XOR operation to achieve this. (0^1=1 and 1^1=0)
 *
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber)
{
   pGPIOx->ODR ^= (1 << PinNumber);
}
/*
 * IRQ Handling and ISR handling
 */
/************************************************************************
 * @fn              - GPIO_IRQ_IT_Config
 *
 * @brief           - This function allows interrupt configuration of an Interrupt.
 *
 * @param[in]       - Interrupt number
 * @param[in]       - Interrupt priority
 * @param[in]       - ENABLE or DISABLE
 *
 * @return          - none
 *
 * @Note            - This is a processor specific APi so we need to refer to the cortex-M4 user guide.
 *                  - NVIC_ISER: Nested Vector Interrupt Controller Interrupt Service Enable Register.
 *                  - there are a total of 8 ISERs ISER0-ISER7.
 */
void GPIO_IRQ_IT_Config(uint8_t IRQNumber, uint8_t EnorDis)
{
	if(EnorDis == ENABLE)
	{
		if(IRQNumber <= 31 )
		{
			//Program NVIC_ISER0 according to the cortex-M4 user guide.
			*NVIC_ISER0 |= (1 << IRQNumber); // Dereference the address and set the value
		}else if(IRQNumber >31 && IRQNumber < 64){
			//Program ISER1
			*NVIC_ISER1 |= ( 1 << (IRQNumber % 32));
		}
		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			//program ISER2
			*NVIC_ISER2 |= (1 << (IRQNumber % 64 ));
		}
	}
	else
	{
		// Program ICER (Interrupt Clear Enable Register)
		if(IRQNumber <= 31 )
		{
			//Program NVIC_ICER0 according to the cortex-M4 user guide.
			*NVIC_ICER0 |= (1 << IRQNumber); // Dereference the address and set the value
		}else if(IRQNumber >31 && IRQNumber < 64){
			//Program ICER1
			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32));
		}
		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			//program ICER2
			*NVIC_ICER2 |= (1 << (IRQNumber % 64 ));
		}
	}
}
/************************************************************************
 * @fn              - GPIO_IRQPriorityConfig
 *
 * @brief           - This is the interrupt priority configuration API .
 *
 * @param[in]       - IRQ number which corresponds to Interrupt vector table.
 * @param[in]       - IRQPriority
 *
 * @Note            - This is done on the cortex M processor side hence refer to Cortex M processor guide.
 *                  - Each IPRx register is divided into 4 sections(8 bits each).
 *                  - Hence first 4 IRQ numbers will have their priority set in IPR0 reg.And corresponding
 *                  - IPRx reg is selected by x = IRQNumber/4.
 * @Note            - Also note that only the upper nibble is used to set priority and lower 4 bits
 *                  - are not implemented. Hence we need to left shift the IRQPriroty by 4 again.
 *                  - NO_OF_PR_BITS_IMPLEMENTED changes according to MCU hecnce the MACRO is defined.
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1.IPR register
	uint8_t iprx = IRQNumber/4;
	//2.Find the section to write data to
	uint8_t iprx_section = IRQNumber % 4;
	//3.We are Doing iprx_section*8 because Section 1 means we start from 8th bit and not 1st bit.
	uint8_t shift_amount = (iprx_section*8) + (8 - NO_OF_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASEADDR + iprx) |= (IRQPriority  << ( shift_amount ) );

}
/************************************************************************
 * @fn              - GPIO_IRQHandling
 *
 * @brief           - This is the interrupt handler .
 *
 * @param[in]       - Pin number where the interrupt is raised
 * @param[in]       - none
 * @param[in]       - none
 *
 * @Concept: When the interrupt is triggered we have a edge detection block in the EXTI Engine and it sends
 * the Interrupt on the Corresponding EXTI line which is associated with an IRQNumber. The processor then
 * looks up the vector table and finds the associated address with the IRQNumber where we store the address
 * of the ISR Handler.Hence the processor can start executing the Interrupt handling instructions stored at
 * the location pointed by the vector table address.
 * There is a Interrupt pending register on both the processor side and EXI Engine side where the Interrupt initially
 * gets registered.
 *
 * Step 1: Implement the ISR function,
 * Step 2: Store the address of the ISR at the vector address location corresponding to the IRQNumber.
 *
 * @Note: The startup code startup_stm32f407vgtx.s already has written the ISR functions we just need to override it.
 * The ISR is not driver specific as we might want the ISR to perform different operations with an interrupt.
 * ex. we can use the interrupt to turn on a led or create a PWM signal hence we override the already existing ISR in
 * the Application side and not on the driver side. We override the ISR by creating our own IRQ Handler and calling it
 * directly from the already existing ISR.
 *
 * The ISR address is also already updated in the corresponding vector table location. Hence we just need to
 * find the ISR from startup copy it in application side and call the user defined IRQHandler iside it.
 *
 */
void GPIO_IRQHandling(uint8_t PinNumber)
{
	//Clear the pending register bit corresponding to the pin number
	if(EXTI->PR & (1 << PinNumber))
	{
		//CLEAR
		EXTI->PR |= (1 << PinNumber);//to clear the pending state we write 1 acc. to ref. manual
	}

}
