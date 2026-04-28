/*
 * stm32f407xx_spi_driver.c
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
    	while(!SPI_GetFlagStatus(pSPIx,SR_TXE_FLAG)){}
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

}
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{

}
void SPI_IRQHandling(SPI_Handle_t *pHandle)
{

}
