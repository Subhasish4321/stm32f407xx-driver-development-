/**
 * @file stm32f407xx_i2c_driver.c
 * @brief Source file for I2C driver for STM32F407xx microcontroller.
 * @author Subhasish Singha
 */

 #include "stm32f407xx.h"
/************************************************************************
 *        APIs supported by this driver
 * For more information about the APIs check the function definitions
 ************************************************************************/
/*
 * peripheral Clock setup
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx,uint8_t EnorDi)
{
    if(EnorDi)
    {
        if(pI2Cx == I2C1)
        {
            I2C1_PCLK_EN();
        }
        else if(pI2Cx == I2C2)
        {
            I2C2_PCLK_EN();
        }
        else if(pI2Cx == I2C3)
        {
            I2C3_PCLK_EN();
        }
    }
    else
    {
        if(pI2Cx == I2C1)
        {
            I2C1_PCLK_DIS();
        }
        else if(pI2Cx == I2C2)
        {
            I2C2_PCLK_DIS();
        }
        else if(pI2Cx == I2C3)
        {
            I2C3_PCLK_DIS();
        }
    }
}
/*
 * I2C Flag Status.
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{

}
/*
 * Init and DeInit
 */
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
    if(pI2CHandle->I2C_Config.I2C_SCLSpeed == I2C_SCL_SPEED_SM)
    {
        pI2CHandle->pI2Cx->I2C_CR2 |= (I2C_SCL_SPEED_SM << 5)
    }
    else if(pI2CHandle->I2C_Config.I2C_SCLSpeed == I2C_SCL_SPEED_FM2K)
    {
        pI2CHandle->pI2Cx->I2C_CR2 |= (I2C_SCL_SPEED_FM2K << 5)
    }
    else if(pI2CHandle->I2C_Config.I2C_SCLSpeed == I2C_SCL_SPEED_FM4K)
    {
        pI2CHandle->pI2Cx->I2C_CR2 |= (I2C_SCL_SPEED_FM4K << 5)
    }
}
void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{
    pI2Cx->I2C_CR1 = 0;
    pI2Cx->I2C_CCR = 0;
}
void I2C_EnableOrDisable(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi)
{
    if(EnOrDi)
    {
        pI2Cx->I2C_CR1 |= (1 << 0);
    }
    else
    {
        pI2Cx->I2C_CR1 &= ~(1 << 0);
    }
}
/**
 * Data Send and receive APIs 
 */


/**
 * IRQ Configuration and ISR Handling APIs
 */
void I2C_IRQ_IT_Config(uint8_t IRQNumber,uint8_t EnorDis)
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
void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1.IPR register
	uint8_t iprx = IRQNumber/4;
	//2.Find the section to write data to
	uint8_t iprx_section = IRQNumber % 4;
	//3.We are Doing iprx_section*8 because Section 1 means we start from 8th bit and not 1st bit.
	uint8_t shift_amount = (iprx_section*8) + (8 - NO_OF_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASEADDR + iprx) |= (IRQPriority  << ( shift_amount ) );
}
/**
 * Application Callback.
 * Concept: The below function is to implemented on the application side i.e. it could have diff. defines for diff.requirement.
 * So we will leave a weak implementation in the driver.c file and 
 * the application will overwrite this function. We write a weak implementaion to prevent compiler errors.
 * we can use the __attribute__((weak))
 * attribute to define a weak function implementaion
*/
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t app_event);