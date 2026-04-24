/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: April 24, 2026
 *  Author: Subhasish Singha
 */
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
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
    /* Enable Peripheral Clock*/
}
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{

}
/**
 * Data Send and receive APIs 
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
void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer, uint32_t len)
{

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
