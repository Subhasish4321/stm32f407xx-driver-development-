/**
 * @file stm32f407xx_USART_driver.c
 * @brief Source file for USART driver for STM32F407xx microcontroller.
 * @author Subhasish Singha
 */

#include "stm32f407xx.h"

static void USART_SetBaudRate(USART_Handle_t *pUSARTHandle);

/**
 * This function programs BRR by splitting USARTDIV into mantissa and fraction
 * according to the STM32F407 reference manual.
 */
static void USART_SetBaudRate(USART_Handle_t *pUSARTHandle)
{
    uint32_t pclkx;
    uint32_t usartdiv;
    uint32_t mantissa;
    uint32_t fraction;
    uint32_t temp;

    if(pUSARTHandle->pUSARTx == USART1 || pUSARTHandle->pUSARTx == USART6)
    {
        // PCLK2 is the clock source for USART1 and USART6
        pclkx = RCC_Get_PCLK2Value();
    }
    else
    {
        // PCLK1 is the clock source for USART2, USART3, UART4, and UART5
        pclkx = RCC_Get_PCLK1Value();
    }

    if(pUSARTHandle->USART_Config.USART_OverSampling == USART_OVR_SMPL_8)
    {
        // Scale USARTDIV by 100 so the fractional part can be preserved in integer math.
        usartdiv = (25U * pclkx) / (2U * pUSARTHandle->USART_Config.USART_Baud);
    }
    else
    {
        // For oversampling by 16, USARTDIV = fCK / (16 * baud).
        usartdiv = (25U * pclkx) / (4U * pUSARTHandle->USART_Config.USART_Baud);
    }

    // Example: if scaled USARTDIV is 868, mantissa = 8 and temp = 68.
    mantissa = usartdiv / 100U;
    temp = usartdiv - (mantissa * 100U);

    if(pUSARTHandle->USART_Config.USART_OverSampling == USART_OVR_SMPL_8)
    {
        // Convert the decimal remainder into 1/8 steps and round to the nearest value.
        fraction = (((temp * 8U) + 50U) / 100U) & 0x07U;
        // In OVER8 mode, BRR[15:4] holds mantissa and BRR[2:0] holds fraction.
        pUSARTHandle->pUSARTx->USART_BRR = (mantissa << 4) | fraction;
    }
    else
    {
        // Convert the decimal remainder into 1/16 steps and round to the nearest value for rounding we add 50 before we divide.
        fraction = (((temp * 16U) + 50U) / 100U) & 0x0FU;
        // In OVER16 mode, BRR[15:4] holds mantissa and BRR[3:0] holds fraction.
        pUSARTHandle->pUSARTx->USART_BRR = (mantissa << 4) | fraction;
    }
}
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
    /*Initialise the OverSampling*/
    pUSARTHandle->pUSARTx->USART_CR1 |= (pUSARTHandle->USART_Config.USART_OverSampling << USART_CR1_OVER8);
    /* Initialise the Baud */
    USART_SetBaudRate(pUSARTHandle);
    /* Initialise the NoOfStopBits*/
    pUSARTHandle->pUSARTx->USART_CR2 |= (pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP);
    /* Initialise WordLength */
    pUSARTHandle->pUSARTx->USART_CR1 |= (pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M);
    /* Initialise ParityControl */
    if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
    {
        pUSARTHandle->pUSARTx->USART_CR1 &= ~(1 << USART_CR1_PCE );
    }
    else if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN)
    {
        pUSARTHandle->pUSARTx->USART_CR1 |= (1 << USART_CR1_PCE );
        pUSARTHandle->pUSARTx->USART_CR1 &= ~(1 << USART_CR1_PS );
    }
    else if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD)
    {
        pUSARTHandle->pUSARTx->USART_CR1 |= (1 << USART_CR1_PCE );
        pUSARTHandle->pUSARTx->USART_CR1 |= (1 << USART_CR1_PS );
    }
    /* Initialise HWFlowControl */
    if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS )
    {    
        pUSARTHandle->pUSARTx->USART_CR3 &= ~(pUSARTHandle->USART_Config.USART_HWFlowControl << USART_CR3_RTSE);
        pUSARTHandle->pUSARTx->USART_CR3 |= (pUSARTHandle->USART_Config.USART_HWFlowControl << USART_CR3_CTSE);
    }
    else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS )
    {
        pUSARTHandle->pUSARTx->USART_CR3 &= ~(pUSARTHandle->USART_Config.USART_HWFlowControl << USART_CR3_CTSE);
        pUSARTHandle->pUSARTx->USART_CR3 |= (pUSARTHandle->USART_Config.USART_HWFlowControl << USART_CR3_RTSE);
    }
    else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS )
    {
        pUSARTHandle->pUSARTx->USART_CR3 |= (pUSARTHandle->USART_Config.USART_HWFlowControl << USART_CR3_RTSE);
        pUSARTHandle->pUSARTx->USART_CR3 |= (pUSARTHandle->USART_Config.USART_HWFlowControl << USART_CR3_CTSE);
    }
    else
    {
        pUSARTHandle->pUSARTx->USART_CR3 &= ~(pUSARTHandle->USART_Config.USART_HWFlowControl << USART_CR3_CTSE);
        pUSARTHandle->pUSARTx->USART_CR3 &= ~(pUSARTHandle->USART_Config.USART_HWFlowControl << USART_CR3_RTSE);
    }

}
void USART_DeInit(USART_RegDef_t *pUSARTx)
{
    USART_PeriClockControl(pUSARTx, DISABLE);
}


/*
 * Data Send and Receive
 */
void USART_SendData(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len)
{
    uint16_t *pdata;

    if((pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS) &&
       (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) &&
       ((Len % 2U) != 0U))
    {
        return;
    }

    //Loop until Len number of Bytes is transferred
    while(Len > 0)
    {
        //Wait until TXE is set
        while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, (1 << USART_SR_TXE)));
        //Check The USART word length ,(9 bit or 8 bit)
        if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
        {
            pdata = (uint16_t*)pTxBuffer;
            pUSARTHandle->pUSARTx->USART_DR = (*pdata & (uint16_t)0x01FF);
            
            //check for parity,because if parity is enabled and word length is 9 then the last bit will be the parity data & if disabled then all the 9 bits are data.
            if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
            {
                pTxBuffer++;
                pTxBuffer++;
                Len--;
                Len--; 
                 
            }
            else
            {
                //8 bits data and 1 bit parity total 9 bits or 2 bytes. The hardware will replace the 9th bit with the parity value.
                pTxBuffer++;
                Len--;
            }
        }
        else
        {
            // Usart word length 8 bit
            if((pUSARTHandle->pUSARTx->USART_CR1 >> USART_CR1_PCE) &  0x1)
            {
                pUSARTHandle->pUSARTx->USART_DR = (*pTxBuffer & (uint8_t)0x7F);
                pTxBuffer++;
                Len--;
            }
            else
            {
                pUSARTHandle->pUSARTx->USART_DR = (*pTxBuffer & (uint8_t)0xFF);
                pTxBuffer++;
                Len--;
            }
            

        }
        
    }
    
    //Wait until TC bit is set in the SR reg to mark that transmission is completed.
     while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx , (1 << USART_SR_TC)));
    
}
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
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

/**
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
    if(EnOrDi == ENABLE)
    {
        pUSARTx->USART_CR1 |= (1 << USART_CR1_UE);
    }
    else
    {
        pUSARTx->USART_CR1 &= ~(1 << USART_CR1_UE);
    }
}
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx , uint32_t FlagName)
{
    if(pUSARTx->USART_SR & FlagName)
    {
        return FLAG_SET;
    }
    else
    {
        return FLAG_RESET;
    }
}
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName)
{
    pUSARTx->USART_SR &= ~(StatusFlagName);
}

/*
 * Application callback
 */
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t AppEv)
{

}
