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
/**
 * ErrorOverRun function weak implementation.
 */
__weak void ErrorOverRun(void)
{
    
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
               //Note: There is a hard limitaion in this mode,that is we cannot send data that is more than 8 bit, i.e. maximum we can send 0x7F.
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

/**
 * The USART_ReceiveData is practically not used or will never be used since it is a blocking or polling method and would block the CPU utilisation.
 * Instead for data reception we use Interrupt method for data reception.
 * We need to first recevie the length for this API to work and then the data.
 */
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    //1. Program the wordlength 8bit/9bit
    pUSARTHandle->pUSARTx->USART_CR1 |= (pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M);

    //2.Program the number of stop bits.
    pUSARTHandle->pUSARTx->USART_CR2 |= (pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP);

    //3.select the desired Baud rate
    USART_SetBaudRate(pUSARTHandle);

    //4.Enable the USART
    pUSARTHandle->pUSARTx->USART_CR1 |= (1 << USART_CR1_UE);

    //5.Receive the data 
    while(Len > 0)
    {
        //Wait until RXNE is set
        while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, (1 << USART_SR_RXNE)));
        //Check The USART word length ,(9 bit or 8 bit)
        if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
        {
            //if word length is 9 bits, load the data into the buffer.
            if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
            {
                //No parity is used in this transfer. so all 9 bits will be of user data.
                *((uint16_t*)pRxBuffer) = (pUSARTHandle->pUSARTx->USART_DR & (uint16_t)0x01FF);
                pRxBuffer++;
                pRxBuffer++;
                Len--;
                Len--;
            }
            else
            {
                //Parity is used in this transfer. so 8 bits will be of user data and 1 bit is parity.
                *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->USART_DR & (uint8_t)0xFF);
                pRxBuffer++;
                Len--;
            }
        }
        else
        {
            //Word length is 8 bits
            if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
            {
                //No parity is used in this transfer. so all 8 bits will be of user data.
                *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->USART_DR & (uint8_t)0xFF);
            }
            else
            {
                //Parity is used in this transfer. so 7 bits will be of user data and 1 bit is parity.
                *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->USART_DR & (uint8_t)0x7F);
            }
            pRxBuffer++;
            Len--;
        }
    }
    
}
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len)
{
    uint8_t txstate = pUSARTHandle->TxRxBusyState;
    if( txstate != USART_BUSY_IN_TX)
    {
        pUSARTHandle->pUSARTx->USART_CR1 |= (1 << USART_CR1_TXEIE);
        pUSARTHandle->pUSARTx->USART_CR1 |= (1 << USART_CR1_TCIE);
        pUSARTHandle->TxLen = Len;
        pUSARTHandle->pTxBuffer = pTxBuffer;
        pUSARTHandle->TxRxBusyState = USART_BUSY_IN_TX;
    }
    
    return txstate;
}
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    uint8_t rxstate = pUSARTHandle->TxRxBusyState;

    if(rxstate != USART_BUSY_IN_RX)
    {
        //Do a dummy read of DR to remove any stale RXNE flag before enabling the RXNE interrupt.
//        (void)pUSARTHandle->pUSARTx->USART_DR;
        
        pUSARTHandle->pUSARTx->USART_CR1 |= (1 << USART_CR1_RXNEIE);
        pUSARTHandle->TxRxBusyState = USART_BUSY_IN_RX;
        pUSARTHandle->RxLen = Len;
        pUSARTHandle->pRxBuffer = pRxBuffer;

    }

    return rxstate;
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
/*********************************************************************
 * @fn      		  - USART_IRQHandler
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - 

 */
void USART_IRQHandling(USART_Handle_t *pUSARTHandle)
{

    uint16_t *pdata;
	uint32_t temp1 , temp2, temp3;

/*************************Check for TC flag ********************************************/

    //Implement the code to check the state of TC bit in the SR
	temp1 = pUSARTHandle->pUSARTx->USART_SR & ( 1 << USART_SR_TC);
	
	 //Implement the code to check the state of TCEIE bit 
	temp2 = pUSARTHandle->pUSARTx->USART_CR1 & ( 1 << USART_CR1_TCIE);

	if(temp1 && temp2 )
	{
		//this interrupt is because of TC or the USART trnasmission is completed.
		
		//close transmission and call application callback if TxLen is zero
		if ( pUSARTHandle->TxRxBusyState == USART_BUSY_IN_TX)
		{
			//Check the TxLen . If it is zero then close the data transmission
			if(! pUSARTHandle->TxLen )
			{
				//Implement the code to clear the TC flag
				pUSARTHandle->pUSARTx->USART_SR &= ~( 1 << USART_SR_TC);
				
				//Implement the code to clear the TCIE control bit 
                pUSARTHandle->pUSARTx->USART_CR1 &= ~(1 << USART_CR1_TCIE);

				//Reset the application state
				pUSARTHandle->TxRxBusyState = USART_READY;
				
				//Reset Buffer address to NULL
				pUSARTHandle->pTxBuffer = NULL;
				
				//Reset the length to zero
				pUSARTHandle->TxLen = 0;
				
				//Call the applicaton call back with event USART_EVENT_TX_CMPLT
				USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_TX_CMPLT);
			}
		}
	}

/*************************Check for TXE flag ********************************************/

	//Implement the code to check the state of TXE bit in the SR
	temp1 = pUSARTHandle->pUSARTx->USART_SR & ( 1 << USART_SR_TXE);
	
	//Implement the code to check the state of TXEIE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->USART_CR1 & ( 1 << USART_CR1_TXEIE);


	if(temp1 && temp2 )
	{
		//this interrupt is because of TXE
		
		if(pUSARTHandle->TxRxBusyState == USART_BUSY_IN_TX)
		{
			//Keep sending data until Txlen reaches to zero
			if(pUSARTHandle->TxLen > 0)
			{
				//Check the USART_WordLength item for 9BIT or 8BIT in a frame
				if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
				{
					//if 9BIT , load the DR with 2bytes masking the bits other than first 9 bits
					pdata = (uint16_t*) pUSARTHandle->pTxBuffer;
					
					//loading only first 9 bits , so we have to mask with the value 0x01FF
					pUSARTHandle->pUSARTx->USART_DR = (*pdata & (uint16_t)0x01FF);

					//check for USART_ParityControl
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
					{
						//No parity is used in this transfer , so, 9bits of user data will be sent
						//Implement the code to increment pTxBuffer twice
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->pTxBuffer++;
						
						//Implement the code to decrement the length
						pUSARTHandle->TxLen-=2;
					}
					else
					{
						//Parity bit is used in this transfer . so , 8bits of user data will be sent
						//The 9th bit will be replaced by parity bit by the hardware
						pUSARTHandle->pTxBuffer++;
						
						//Implement the code to decrement the length
						pUSARTHandle->TxLen--;
					}
				}
				else
				{
					//This is 8bit data transfer
					pUSARTHandle->pUSARTx->USART_DR = (*pUSARTHandle->pTxBuffer  & (uint8_t)0xFF);

					//Implement the code to increment the buffer address
					pUSARTHandle->pTxBuffer++;
					
					//Implement the code to decrement the length
					pUSARTHandle->TxLen--;
				}
				
			}
			if (pUSARTHandle->TxLen == 0 )
			{
				//TxLen is zero 
				//Implement the code to clear the TXEIE bit (disable interrupt for TXE flag )
				pUSARTHandle->pUSARTx->USART_CR1 &= ~(1 << USART_CR1_TXEIE);
			}
		}
	}
	
/*************************Check for RXNE flag ********************************************/

	temp1 = pUSARTHandle->pUSARTx->USART_SR & ( 1 << USART_SR_RXNE);
	temp2 = pUSARTHandle->pUSARTx->USART_CR1 & ( 1 << USART_CR1_RXNEIE);


	if(temp1 && temp2 )
	{
		//this interrupt is because of rxne
		//this interrupt is because of txe
		if(pUSARTHandle->TxRxBusyState == USART_BUSY_IN_RX)
		{
			//TXE is set so send data
			if(pUSARTHandle->RxLen > 0)
			{
				//Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
				if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
				{
					//We are going to receive 9bit data in a frame

					//Now, check are we using USART_ParityControl control or not
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
					{
						//No parity is used. so, all 9bits will be of user data

						//read only first 9 bits so mask the DR with 0x01FF
						*((uint16_t*) pUSARTHandle->pRxBuffer) = (pUSARTHandle->pUSARTx->USART_DR  & (uint16_t)0x01FF);

						//Now increment the pRxBuffer two times
						pUSARTHandle->pRxBuffer++;
						pUSARTHandle->pRxBuffer++;
						
						//Implement the code to decrement the length
						pUSARTHandle->RxLen-=2;
					}
					else
					{
						//Parity is used. so, 8bits will be of user data and 1 bit is parity
						 *pUSARTHandle->pRxBuffer = (pUSARTHandle->pUSARTx->USART_DR  & (uint8_t)0xFF);
						 
						 //Now increment the pRxBuffer
						 pUSARTHandle->pRxBuffer++;
						 
						 //Implement the code to decrement the length
						 pUSARTHandle->RxLen--;
					}
				}
				else
				{
					//We are going to receive 8bit data in a frame

					//Now, check are we using USART_ParityControl control or not
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
					{
						//No parity is used , so all 8bits will be of user data

						//read 8 bits from DR
						 *pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->USART_DR  & (uint8_t)0xFF);
					}

					else
					{
						//Parity is used, so , 7 bits will be of user data and 1 bit is parity

						//read only 7 bits , hence mask the DR with 0X7F
						 *pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->USART_DR  & (uint8_t)0x7F);

					}

					//Now , increment the pRxBuffer
					pUSARTHandle->pRxBuffer++;
					
					//Implement the code to decrement the length
                    pUSARTHandle->RxLen--;
				}		
					
					
			}

			if(! pUSARTHandle->RxLen)
			{
				//disable the rxne
				pUSARTHandle->pUSARTx->USART_CR1 &= ~( 1 << USART_CR1_RXNEIE );
				pUSARTHandle->TxRxBusyState = USART_READY;
				USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_RX_CMPLT);
			}
		}
	}
	
	
/*************************Check for CTS flag ********************************************/
//Note : CTS feature is not applicable for UART4 and UART5

	//Implement the code to check the status of CTS bit in the SR
	temp1 = USART_GetFlagStatus(pUSARTHandle->pUSARTx, (1 << USART_SR_CTS));
	
	//Implement the code to check the state of CTSE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->USART_CR3 & ( 1 << USART_CR3_CTSE);
	
	//Implement the code to check the state of CTSIE bit in CR3 (This bit is not available for UART4 & UART5.)
	temp3 = pUSARTHandle->pUSARTx->USART_CR3 & ( 1 << USART_CR3_CTSIE);

    (void)temp3;
	if(temp1  && temp2 )
	{
		//Implement the code to clear the CTS flag in SR
		pUSARTHandle->pUSARTx->USART_SR &=~(1 << USART_SR_CTS);
		
		//this interrupt is because of cts
		USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_CTS);
	}

/*************************Check for IDLE detection flag ********************************************/

	//Implement the code to check the status of IDLE flag bit in the SR
	temp1 = USART_GetFlagStatus(pUSARTHandle->pUSARTx, (1 << USART_SR_IDLE) );
	
	//Implement the code to check the state of IDLEIE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->USART_CR1 & ( 1 << USART_CR1_IDLEIE);


	if(temp1 && temp2)
	{
		//Implement the code to clear the IDLE flag. Refer to the RM to understand the clear sequence 
		temp1 = pUSARTHandle->pUSARTx->USART_SR;
        temp1 = pUSARTHandle->pUSARTx->USART_DR;
		//this interrupt is because of idle
		USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_IDLE);
	}

/*************************Check for Overrun detection flag ********************************************/

	//Implement the code to check the status of ORE flag  in the SR
	temp1 = USART_GetFlagStatus(pUSARTHandle->pUSARTx, (1 << USART_SR_ORE) );
	
	//Implement the code to check the status of RXNEIE  bit in the CR1
	temp2 = pUSARTHandle->pUSARTx->USART_CR1 & (1 << USART_CR1_RXNEIE);


	if(temp1  && temp2 )
	{
		//Need not to clear the ORE flag here, instead give an api for the application to clear the ORE flag . 
		ErrorOverRun();
		//this interrupt is because of Overrun error 
		USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_ORE);
	}



/*************************Check for Error Flag ********************************************/

//Noise Flag, Overrun error and Framing Error in multibuffer communication
//We dont discuss multibuffer communication in this course. please refer to the RM
//The below code will get executed in only if multibuffer mode is used. 

	temp2 =  pUSARTHandle->pUSARTx->USART_CR3 & ( 1 << USART_CR3_EIE) ;

	if(temp2 )
	{
		temp1 = pUSARTHandle->pUSARTx->USART_SR;
		if(temp1 & ( 1 << USART_SR_FE))
		{
			/*
				This bit is set by hardware when a de-synchronization, excessive noise or a break character
				is detected. It is cleared by a software sequence (an read to the USART_SR register
				followed by a read to the USART_DR register).
			*/
			USART_ApplicationEventCallback(pUSARTHandle,USART_ERREVENT_FE);
		}

		if(temp1 & ( 1 << USART_SR_NE) )
		{
			/*
				This bit is set by hardware when noise is detected on a received frame. It is cleared by a
				software sequence (an read to the USART_SR register followed by a read to the
				USART_DR register).
			*/
			USART_ApplicationEventCallback(pUSARTHandle,USART_ERREVENT_NE);
		}

		if(temp1 & ( 1 << USART_SR_ORE) )
		{
			USART_ApplicationEventCallback(pUSARTHandle,USART_ERREVENT_ORE);
		}
	}


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
__weak void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t AppEv)
{

}
