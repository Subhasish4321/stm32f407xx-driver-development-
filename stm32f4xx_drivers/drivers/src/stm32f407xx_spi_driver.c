/*
 *  stm32f407xx_spi_driver.c
 *
 *  Created on: April 24, 2026
 *  Author: Subhasish Singha
 */
#include "stm32f407xx.h"
#include "stm32f407xx_spi_driver.h"
/************************************************************************
 *        APIs supported by this driver
 ************************************************************************/
/*
 * peripheral Clock setup
 */

/************************************************************************
 * @fn              - SPI_PeriClockControl
 *
 * @brief           - This function enables or disables peripheral clock for the given SPI port.
 *
 * @param[in]       -
 * @param[in]       -
 * @param[in]       -
 *
 * @return          -
 *
 * @Note            - In this driver file we have not implemented SPI4 although it is avialable in the MCU.
 *
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx,uint8_t EnorDi)
{
    if (EnorDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}else if(pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
	}
	else
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_DIS();
		}else if(pSPIx == SPI2)
		{
			SPI2_PCLK_DIS();
		}else if(pSPIx == SPI3)
		{
			SPI3_PCLK_DIS();
		}
	}
}
/*
 * Init and DeInit
 */
/************************************************************************
 * @fn              - SPI_Init()
 *
 * @brief           - This function is used to initialize the SPI by configuring the registers.
 *
 * @param[in]       - *pSPIHandle which is pointer that holds the SPI configuration and pointer to the SPI(SPI1 or SPI 2..) being used.
 * @param[in]       -
 * @param[in]       -
 *
 * @return          -
 *
 * @Note            - Remember that once SPE is set to 1, many bits in SPI_CR1 (like MSTR, CPOL, and CPHA)
 * become read-only or should not be changed,hence the SPE bit is not enabled in this module,it should be enable after
 * all the init configuration is complete.The common practice is to set the SPE bit in a different module.
 *
 */

void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);
    /**
     *  Configure SPI_CR1 register
     */
	uint32_t tempreg = 0;

	/*Device mode,slave or master */
	tempreg |= pSPIHandle->SPI_Config.SPI_DeviceMode << SPI_CR1_MSTR;

	/*Bus configuration FD,HD or Simplex*/
	if(pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_FD )
	{
		//BIDI bit should be cleared for Full Duplex
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
	}else if(pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//BIDI bit should be set for Half Duplex
		tempreg |= (1 << SPI_CR1_BIDIMODE);
	}else if(pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RX_ONLY)
	{
		//The BIDI bit should be in FD and RXONLY Bit should be set
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
		tempreg |= (1 << SPI_CR1_RXONLY);
	}

	/*Data frame format 8 bit or 16 bit */
	tempreg |= (pSPIHandle->SPI_Config.SPI_DFF << SPI_CR1_DFF);

    /* Clock phase*/
	tempreg |= (pSPIHandle->SPI_Config.SPI_CPHA << SPI_CR1_CPHA);

	/* Clock polarity */
	tempreg |= (pSPIHandle->SPI_Config.SPI_CPOL << SPI_CR1_CPOL);

	/* Software slave management */
	tempreg |= (pSPIHandle->SPI_Config.SPI_SSM << SPI_CR1_SSM);

	/* Speed of SCLK or BAUD rate */
	tempreg |= (pSPIHandle->SPI_Config.SPI_SclkSpeed << SPI_CR1_BR);

	/*Assigning the SPI_CR1 register to tempreg  */
	pSPIHandle->pSPIx->SPI_CR1 = tempreg;

	/**
	 *  Configure SPI_CR2 register if needed
	 */

	/**
	 *  Configure SPI_SR register if needed
	 */
}
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->SPI_CR1 |= (1 << SPI_CR1_SPE);
	}
	else{
		pSPIx->SPI_CR1 &= ~(1 << SPI_CR1_SPE);
	}

}
void SPI_InternalSlaveSelectConfiguration(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
		if(EnOrDi == ENABLE)
		{
			pSPIx->SPI_CR1 |= (1 << SPI_CR1_SSI);
		}
		else{
			pSPIx->SPI_CR1 &= ~(1 << SPI_CR1_SSI);
		}

}
void SPI_SSOE_Config(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
		if(EnOrDi == ENABLE)
		{
			pSPIx->SPI_CR2 |= (1 << SPI_CR2_SSOE);
		}
		else{
			pSPIx->SPI_CR2 &= ~(1 << SPI_CR2_SSOE);
		}

}
/************************************************************************
 * @fn              - SPI_DeInit()
 *
 * @brief           - This function is used to deinitialize the SPI peripheral.
 *
 * @param[in]       - Pointer to the SPI being used
 * @param[in]       -
 * @param[in]       -
 *
 * @return          -
 *
 * @Note            - We can just set the SPI registers to their reset value.
 *
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	pSPIx->SPI_CR1 = 0;
}

/**
 * Data Send and receive APIs
 */
/*The below function */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if((pSPIx->SPI_SR & FlagName))
	{
		return FLAG_SET;
	}
	else
	{
		return FLAG_RESET;
	}
}
/************************************************************************
 * @fn              - SPI_SendData()
 *
 * @brief           - This function is used to send Data through SPI.
 *
 * @param[in]       -
 * @param[in]       -
 *
 * @Note            - This API is called a blocking API or Polling based API as this will wait until all the bytes are transmitted.
 *
 * -We first check if len is 0,if len is 0 we exit from function else we wait until Tx Buffer is empty because only if
 * TX buffer is empty we can put the data to be transmitted as we dont want to corrupt the data that is already existing
 * in the buffer. When Tx buffer is not empty we check the DFF and load data reg and  increment buffer address accordingly.
 * -Once we load 1 byte(DFF =0 or 8 bits data) data we decrement the len by 1 and for 16 bit we load 2 byte to DR and do len-2.
 * -We repeat all this until len becomes 0.
 *
 * -We can confirm whether the Tx buffer is empty or not by checking the TXE bit of the status register.
 * -Firmware has no direct access to TX and RX buffer.All loading and reading of data happens thorugh SPI_DR(Data register)which the
 * firmware has access to.
 * **Imp Note: A write to DR will write to the Tx Buffer and read from the DR will be data fetched from the Rx Buffer.Before writing
 * to DR we need to make sure Tx Buffer is Empty by reading status reg TXE flag and similarly while reading from DR we need to check
 * RXNE(Receive Buffer Not empty) flag.
 */
void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer, uint32_t len)
{
    while(len > 0)
    {
    	//Wait until TXE is set
    	while(!SPI_GetFlagStatus(pSPIx,SPI_SR_TXE_FLAG)){}
    	// check for DFF from CR1
    	if(pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF) )
    	{
    		//16 Bits data frame format.
    		//before dereferencing using *(pointer type)we need to type cast to 16 bit pointer type to get 16 bit data.
    		pSPIx->SPI_DR = *((uint16_t*)pTxBuffer);
    		len --;
    		len --;
    		//Increment the buffer address for next data bytes
    		(uint16_t*)pTxBuffer++;// This simply means pTxBuffer is a pointer to a data which of unsigned int 16 bit data type.
    	}
    	else
    	{
    		//8 bit Data frame format.
    		pSPIx->SPI_DR = *pTxBuffer;
    		len--;
    		pTxBuffer++;
    	}

    }
}
void SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRxBuffer,uint32_t len)
{
	 while(len > 0)
    {
    	//Wait until RXNE is set
    	while(!SPI_GetFlagStatus(pSPIx,SPI_SR_RXNE_FLAG)){}
    	// check for DFF from CR1
    	if(pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF) )
    	{
    		//16 Bits data frame format.
    		//before dereferencing using *(pointer type)we need to type cast to 16 bit pointer type to get 16 bit data.
    		*((uint16_t*)pRxBuffer) = pSPIx->SPI_DR;
    		len --;
    		len --;
    		//Increment the buffer address for next data bytes
    		(uint16_t*)pRxBuffer++;// This simply means pTxBuffer is a pointer to a data which of unsigned int 16 bit data type.
    	}
    	else
    	{
    		//8 bit Data frame format.
    		*pRxBuffer = pSPIx->SPI_DR;
    		len--;
    		pRxBuffer++;
    	}

    }
}
/**
 * IRQ Configuration and ISR Handling APIs
 */
/************************************************************************
 * @fn              -
 *
 * @brief           - This function
 *
 * @param[in]       -
 * @param[in]       -
 * @param[in]       -
 *
 * @return          -
 *
 * @Note            -
 *
 */
void SPI_IRQ_IT_Config(uint8_t IRQNumber,uint8_t EnorDis)
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
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
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
 * Data Send and receive APIs using Interrupts.
 */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle,uint8_t *pTxBuffer, uint32_t len)
{
	/*
	 * 1. Save the buffer address and length information in some global variables.
	 * 2. Mark the SPI state as busy in transmission so that so that no other
	 * code can take over same SPI peripheral until transmission is over.
	 * 3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
	 * 4.Data transmission will be handled by the ISR code.(Will implement later)
	 */
     uint8_t state = pSPIHandle->TxState;

     if(state != SPI_BUSY_IN_TX)
     {
	 pSPIHandle->pTxBuffer = pTxBuffer;
	 pSPIHandle->Txlen = len;

	 pSPIHandle->TxState = SPI_BUSY_IN_TX;

	 pSPIHandle->pSPIx->SPI_CR2 |= (1 << SPI_CR2_TXEIE );
     }
     return state;
}
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle,uint8_t *pRxBuffer,uint32_t len)
{
	 uint8_t state = pSPIHandle->RxState;

	 if(state != SPI_BUSY_IN_RX)
	 {
	 pSPIHandle->pRxBuffer = pRxBuffer;
	 pSPIHandle->Rxlen = len;

	 pSPIHandle->RxState = SPI_BUSY_IN_RX;

	 pSPIHandle->pSPIx->SPI_CR2 |= (1 << SPI_CR2_RXNEIE );
	 }
	 return state;
}
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
/**
 *  Event sequence:
 *  Interrupt Triggered -> Understand which event caused the interrupt to trigger(Check SR)-->
 *  1.Interupt is due to RXNE flag -> Handle RXNE event.
 *  2.Interrupt is due to TXE flag -> Handle TXE event.
 *  3.Interrupt is due to ERROR Flag -> Handle Error.
 */
void SPI_IRQHandling(SPI_Handle_t *pHandle)
{

    // First check the status reg to verify the interrupt cause
	uint8_t temp1,temp2;
	//check if it is due to TXE
	temp1 = pHandle->pSPIx->SPI_SR & (1 << SPI_SR_TXE);
	temp2 = pHandle->pSPIx->SPI_CR2 & (1 << SPI_CR2_TXEIE);
	if(temp1 && temp2)
	{
		//Interrupt due to TXE
		spi_txe_interrupt_handle(pHandle);
	}
	//Check for RXNE
	temp1 = pHandle->pSPIx->SPI_SR & (1 << SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->SPI_CR2 & (1 << SPI_CR2_RXNEIE);
	if(temp1 && temp2)
	{
		//Interrupt due to RXE
		spi_rxe_interrupt_handle(pHandle);
	}
	//We are ignoring the ERRIE based interrupt events as they are not applicable in this project.
	//Check for Overrun error may occur in the ERRIE section.
	temp1 = pHandle->pSPIx->SPI_SR & (1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->SPI_CR2 & (1 << SPI_CR2_ERRIE);
	if(temp1 && temp2)
	{
		//Interrupt due to Overrun error.(Refer ref_manual for OVR err.)
		spi_ovr_err_interrupt_handle(pHandle);
	}
}

/*Event based helper Functions. */
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	if(pSPIHandle->pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF) )
	{
		//16 Bits data frame format.
		//before dereferencing using *(pointer type)we need to type cast to 16 bit pointer type to get 16 bit data.
		pSPIHandle->pSPIx->SPI_DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->Txlen --;
		pSPIHandle->Txlen --;
		//Increment the buffer address for next data bytes
		(uint16_t*)pSPIHandle->pTxBuffer++;// This simply means pTxBuffer is a pointer to a data which of unsigned int 16 bit data type.
	}
	else
	{
		//8 bit Data frame format.
		pSPIHandle->pSPIx->SPI_DR = *(pSPIHandle->pTxBuffer);
		pSPIHandle->Txlen --;
		pSPIHandle->pTxBuffer++;
	}
	if(! pSPIHandle->Txlen)
	{
		//TXlen 0 means SPI transmission is done
		//inform the application that txe is over SPI can be tunred off.
		SPI_CloseTransmission(pSPIHandle);

	}
}
static void spi_rxe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	// check for DFF from CR1
	if(pSPIHandle->pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF) )
	{
		//16 Bits data frame format.
		//before dereferencing using *(pointer type)we need to type cast to 16 bit pointer type to get 16 bit data.
		*((uint16_t*)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->SPI_DR;
		pSPIHandle->Rxlen --;
		pSPIHandle->Rxlen --;
		//Increment the buffer address for next data bytes
		(uint16_t*)pSPIHandle->pRxBuffer++;// This simply means pTxBuffer is a pointer to a data which of unsigned int 16 bit data type.
	}
	else
	{
		//8 bit Data frame format.
		*(pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->SPI_DR;
		pSPIHandle->Rxlen--;
		pSPIHandle->pRxBuffer++;
	}
	if(! pSPIHandle->Rxlen)
	{
		//This part makes the SPI ready for the next interrupt and lets the application side know that SPI communication is completed.
		SPI_CloseReception(pSPIHandle);
	}

}
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp;
     //Clear the OVR flag
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		//We can clear the OVR flag by just reading the SPI_DR And SPI_SR.
		temp = pSPIHandle->pSPIx->SPI_DR;
		temp = pSPIHandle->pSPIx->SPI_SR;
	}
	(void)temp;
	 //Inform the application overrun error occured
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->SPI_DR;
	temp = pSPIx->SPI_SR;
	(void)temp;
}
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->SPI_CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->Txlen = 0;
	pSPIHandle->TxState = SPI_READY;
	SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_TX_CMPLT);
}
void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->SPI_CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->Rxlen = 0;
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxState = SPI_READY;
	SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);
}

__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t app_event)
{
	//if the application side does not implement a function for this weak function then by default this function is called.
}
