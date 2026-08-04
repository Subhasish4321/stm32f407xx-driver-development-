/**
 * @file stm32f407xx_USART_driver.c
 * @brief Source file for USART driver for STM32F407xx microcontroller.
 * @author Subhasish Singha
 */

#include "stm32f407xx.h"
#include <stdio.h>


/*
 * Peripheral Clock setup
 */
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(pUSARTx == USART1)
        {
            USART1_PCLK_EN();
        }
        else if(pUSARTx == USART2)
        {
            USART2_PCLK_EN();
        }
        else if(pUSARTx == USART3)
        {
            USART3_PCLK_EN();
        }
        else if(pUSARTx == UART4)
        {
            UART4_PCLK_EN();
        }
        else if(pUSARTx == UART5)
        {
            UART5_PCLK_EN();
        }
        else if(pUSARTx == USART6)
        {
            USART6_PCLK_EN();
        }
    }
    else
    {
        if(pUSARTx == USART1)
        {
            USART1_PCLK_DIS();
        }
        else if(pUSARTx == USART2)
        {
            USART2_PCLK_DIS();
        }
        else if(pUSARTx == USART3)
        {
            USART3_PCLK_DIS();
        }
        else if(pUSARTx == UART4)
        {
            UART4_PCLK_DIS();
        }
        else if(pUSARTx == UART5)
        {
            UART5_PCLK_DIS();
        }
        else if(pUSARTx == USART6)
        {
            USART6_PCLK_DIS();
        }
    }
}
    
/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t *pUSARTHandle)
{
    USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);
    if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TX)
    {
        // transmitter mode.
        pUSARTHandle->pUSARTx->USART_CR1 &= ~(1 << USART_CR1_RE); //Disable receive mode.
        pUSARTHandle->pUSARTx->USART_CR1 |= (1 << USART_CR1_TE);
    }
    else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_RX)
    {
        //receiver mode.
        pUSARTHandle->pUSARTx->USART_CR1 &= ~(1 << USART_CR1_TE); //Disable transmit mode.
        pUSARTHandle->pUSARTx->USART_CR1 |= (1 << USART_CR1_RE);
    }
    else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX )
    {
        //Both transmit and receive mode.
        pUSARTHandle->pUSARTx->USART_CR1 |= (1 << USART_CR1_TE);
        pUSARTHandle->pUSARTx->USART_CR1 |= (1 << USART_CR1_RE);
    }

    /* Initialise the Baud */
    if(pUSARTHandle->p)
    USART_SetBaudRate(pUSARTHandle->pUSARTx, )
    /* Initialise the NoOfStopBits*/
    /* Initialise WordLength */
    /* Initialise ParityControl */
    /* Initialise HWFlowControl */



}
void USART_DeInit(USART_RegDef_t *pUSARTx)
{
    USART_PeriClockControl(pUSARTx, DISABLE);
}


/*
 * Data Send and Receive
 */
void USART_SendData(USART_RegDef_t *pUSARTx,uint8_t *pTxBuffer, uint32_t Len)
{
    
}
void USART_ReceiveData(USART_RegDef_t *pUSARTx, uint8_t *pRxBuffer, uint32_t Len)
{
    
}
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len)
{
    return 0;
}
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    return 0;
}

 * IRQ Configuration and ISR handling
 */
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
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
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    //1.IPR register
	uint8_t iprx = IRQNumber/4;
	//2.Find the section to write data to
	uint8_t iprx_section = IRQNumber % 4;
	//3.We are Doing iprx_section*8 because Section 1 means we start from 8th bit and not 1st bit.
	uint8_t shift_amount = (iprx_section*8) + (8 - NO_OF_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASEADDR + iprx) |= (IRQPriority  << ( shift_amount ) );
}
void USART_IRQHandling(USART_Handle_t *pHandle)
{

}

/*
 * Other Peripheral Control APIs
 */
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi)
{

}
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx , uint32_t FlagName)
{

}
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName)
{

}

/*
 * Application callback
 */
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t AppEv)
{

}