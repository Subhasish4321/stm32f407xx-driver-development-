/**
 * @file stm32f407xx_i2c_driver.c
 * @brief Source file for I2C driver for STM32F407xx microcontroller.
 * @author Subhasish Singha
 */

 #include "stm32f407xx.h"
#include <stdio.h>
 uint16_t AHB1_PreScaler [8] = {2,4,8,16,64,128,256,512};
 uint8_t APB1_PreScaler [4] = {2,4,8,16};

static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2CX,uint8_t slaveAddress);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2CX,uint8_t slaveAddress);
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle);
 /**
  * Helper Functions
  */
 static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
 {
     pI2Cx->I2C_CR1 |= (1 << I2C_CR1_START);
 }
 static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2CX,uint8_t slaveAddress)
 {
     slaveAddress = slaveAddress << 1;
     slaveAddress &= ~(1);
     pI2CX->I2C_DR = slaveAddress;
 }
 static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx,uint8_t slaveAddress)
 {
    slaveAddress = slaveAddress << 1;
    slaveAddress |= 1; //r/~w bit = 1 for read
    pI2Cx->I2C_DR = slaveAddress;
 }
 static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle)
 {
	 uint8_t dummy_read;
     //Check for device mode
	 if(pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL) )
	 {
		 //Master mode
		 if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		 {
			 if(pI2CHandle->RxSize == 1)
			 {
				 //First disable acking
				 I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);

				 //Clear address Flag (read SR1,read SR2)
				 dummy_read = pI2CHandle->pI2Cx->I2C_SR1;
				 dummy_read = pI2CHandle->pI2Cx->I2C_SR2;
				 (void)dummy_read;

			 }
		 }
		 else
		 {
			 //Clear address Flag (read SR1,read SR2),this part is fine for BUSY_IN_TX
			 dummy_read = pI2CHandle->pI2Cx->I2C_SR1;
			 dummy_read = pI2CHandle->pI2Cx->I2C_SR2;
			 (void)dummy_read;
		 }

	 }
	 else
	 {
		 //Device is in slave mode.

		 //Clear address Flag (read SR1,read SR2)
		 dummy_read = pI2CHandle->pI2Cx->I2C_SR1;
		 dummy_read = pI2CHandle->pI2Cx->I2C_SR2;
		 (void)dummy_read;
	 }
 }
 void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
 {
    pI2Cx->I2C_CR1 |= (1 << I2C_CR1_STOP);
 }

 void I2C_ManageAcking(I2C_RegDef_t *pI2Cx,uint8_t EnOrDis)
 {
    if(EnOrDis == I2C_ACK_ENABLE)
    {
        pI2Cx->I2C_CR1 |= (1 << I2C_CR1_ACK);
    }
    else
    {
        pI2Cx->I2C_CR1 &= ~(1 << I2C_CR1_ACK);
    }
 }
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
    if(pI2Cx->I2C_SR1 & FlagName)
    {
        return FLAG_SET;
    }
    else
    {
        return FLAG_RESET;
    }
}
uint32_t RCC_GetPLLOutputClock()
{
    return 0;
}
uint32_t RCC_Get_PCLK1Value(void)
{
    uint32_t pclk1,SystemClk;
    uint8_t clksrc,temp,ahb1p,apb1p;
    clksrc = ((RCC->CFGR >> 2) & 0X3);
    if(clksrc == 0)
    {
        SystemClk = 16000000;
    }
    else if(clksrc == 1)
    {
        SystemClk = 8000000;
    }
    else if (clksrc == 2)
    {
        SystemClk = RCC_GetPLLOutputClock();
    }
    //ahb1 pre scaler
    temp = ((RCC->CFGR >> 4)& 0xF);
    if(temp < 8)
    {
        ahb1p = 1;
    }
    else
    {
        ahb1p = AHB1_PreScaler[temp-8];
    }  
    //apb1 pre scaler.
    temp = ((RCC->CFGR >> 10)& 0x7);
    if(temp < 4)
    {
        apb1p = 1;
    }
    else
    {
        apb1p = APB1_PreScaler[temp-4];
    }    
    pclk1 = (SystemClk /ahb1p)/apb1p;

    return  pclk1;
}
/*
 * Init and DeInit
 * Note- Make sure to check the table 10 of UM10204.pdf I2C datasheet, it contains the min amd max permittable tLow ,tHigh and fscl pecifications.
 * In the current scenario we have tL min= 4.7 micro sec and tH min =4.0 micro sec for standard mode.
 * For fast mode tL min 1.3 and tH min 0.s micro sec. 
 */
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
    I2C_PeriClockControl(pI2CHandle->pI2Cx,ENABLE);
    //ACK control fetch and assign.
    uint32_t tempreg = 0;
    tempreg |= pI2CHandle->I2C_Config.I2C_AckControl << I2C_CR1_ACK;
    pI2CHandle->pI2Cx->I2C_CR1 = tempreg;

    //Program frequency field of CR2
    tempreg = 0;
    tempreg = RCC_Get_PCLK1Value() /1000000;
    pI2CHandle->pI2Cx->I2C_CR2 = (tempreg & 0x3F);

    //Store the slave address in OAR
    tempreg = 0;
    tempreg |= (pI2CHandle->I2C_Config.I2C_DeviceAddress << 1);
    tempreg |= (1 << 14);// done according to keep it always 1 acc. to ref manual
    pI2CHandle->pI2Cx->I2C_OAR1 = tempreg;
    
    //CCR calculation
    uint16_t ccr_value = 0;
    tempreg = 0;
    if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
    {
        //Mode is standard mode,for formula of calculation see ref manual.
        ccr_value = (RCC_Get_PCLK1Value()/ (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
        tempreg |= (ccr_value & 0xFFF); //Only bit to be considered for ccr value

    }
    else
    {
        //Configure the mode s/m in ccr reg(15th bit)
        tempreg |= (1 << I2C_CCR_F_S);
        tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << I2C_CCR_DUTY);
        if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_2 )
        {
            ccr_value = (RCC_Get_PCLK1Value()/ (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
        }
        else 
        {
            ccr_value = (RCC_Get_PCLK1Value()/ (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
            
        }
        tempreg |= (ccr_value & 0xFFF); 
    }
    pI2CHandle->pI2Cx->I2C_CCR = tempreg ;

    //TRISE Configuration
    if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
    {
        //Standard mode according to I2c Bus Specification Max Trise is 1000ns.
        // Formula to calculate TRISE = (Max Rise time acc. to I2c Specs/Tpclk1) + 1.
        // => Fpclk1 * Max Rise time.
        tempreg = ( RCC_Get_PCLK1Value()/1000000U ) + 1; // 1000 ns means 1 micro sec. => 10^-6
    }
    else
    {
        tempreg = ( ( RCC_Get_PCLK1Value()*300)/1000000000U ) + 1;// acc. to specs Max rise time for Fm is 300ns
    }
    pI2CHandle->pI2Cx->I2C_TRISE = (tempreg & 0x3F); //Masking bit 0 to 5
}
void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{
    pI2Cx->I2C_CR1 = 0;
    pI2Cx->I2C_CCR = 0;
}
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi)
{
    if(EnOrDi)
    {
        pI2Cx->I2C_CR1 |= (1 << I2C_CR1_PE);
    }
    else
    {
        pI2Cx->I2C_CR1 &= ~(1 << 0);
    }
}
/**
 * Data Send and receive APIs 
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer,uint32_t len, uint8_t slaveAddr,uint8_t RptStart)
{
    //Generate the start condition
    I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

    // Confirm that start generation is completed by checking the SB flag in the SR1
    //Note: Until SB is cleared SCL will be stretched(pulled to LOW).
    //When SR1 SB bit is read the SB bit is cleared automatically and followed by witing the DR register.
    while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SR1_SB) );

    //Send the 7 bit address of the slave with r/~w bit(r =1,w =0) (toatl 8 bits)
    //Address is written to address bits(1 - 7) bit 0(r/~w)
    I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,slaveAddr);

    //confirm the address phase is completed by checking the ADDR flag SR1 register.
    while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SR1_ADDR));
    //Note: Until ADDR is cleared SCL will be stretched. 
    // Also clearing this flags are important because we need the same flag for the next data sent or recevied.
    I2C_ClearADDRFlag(pI2CHandle);

    //send data until len becomes 0
    while(len > 0)
    {
        while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SR1_TXE));//wait till TXE is set
        pI2CHandle->pI2Cx->I2C_DR = *pTxBuffer;
        pTxBuffer++;
        len--;
    }

    //When Len becomes 0 wait for TXE=1 and BTF=1 before generating the STOP condition.
     while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SR1_TXE));  
     while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SR1_BTF));

     if(RptStart == I2C_DISABLE_SR)
     {
        //Generate the STOP condition
        I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
     }
}

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer,uint8_t len, uint8_t slaveAddr,uint8_t RptStart)
{
    //generate the start condition
    I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
    //Confirm that start generation is completed by checking the SB flag in the SR1
      //Note.: Until SB is cleared SCL will be stretched(pulled to LOW).
    while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SR1_SB) );
    // Send the address of the slave with r/~w bit(r =1,w =0) (toatl 8 bits)  
    I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx,slaveAddr);
    //Wair until address phase is completed by checking the ADDR flag in the SR1 register.
    while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SR1_ADDR));
    //Procedure to read only i byte from slave
    if(len == 1)
    {
        //Disable acking before clearing the ADDR flag
        I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);
        //clear the addr flag
        I2C_ClearADDRFlag(pI2CHandle);
        //Wait until RXNE=1
        while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SR1_RXNE));
        if(RptStart == I2C_DISABLE_SR)
        {
            //generate the stop condition
            I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
        }
        //read data in to buffer
        *pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
    }

    //procedure to read more than 1 byte from slave
    if(len > 1)
    {
        //clear the ADDR flag
        I2C_ClearADDRFlag(pI2CHandle);
        //read the data until len becomes 0
        for( uint32_t i = len;i > 0; i--)
        {
            //wait until RXNE becomes 1
            while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SR1_RXNE));
            if(i == 2)//if last 2 bytes are remaining then.
            {
                // clear the ack bit and generate the stop condition.
                I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);
                if(RptStart == I2C_DISABLE_SR)
                {
                    I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
                }
            }
            //read the data from DR in to buffer
			*pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
			//increment the buffer address
			pRxBuffer++;
        }
        

    }
    //re-enable acking according to user choice.
    if(pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE)
    {
            I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_ENABLE);
    }
}

/*********************************************************************
 * @fn      		  - I2C_MasterSendDataIT
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -  Complete the below code . Also include the function prototype in header file

 */
uint8_t  I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxBuffer, uint32_t Len,uint8_t SlaveAddr,uint8_t RptStart)
{

	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->Txlen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = RptStart;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
	    pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITBUFEN);
		//Implement the code to enable ITEVTEN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITERREN);

	}

	return busystate;

}
/*********************************************************************
 * @fn      		  - I2C_MasterReceiveDataIT
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - Complete the below code . Also include the fn prototype in header file

 */
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer,uint8_t len, uint8_t slaveAddr,uint8_t RptStart)
{

	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->Rxlen = len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = len; //Rxsize is used in the ISR code to manage the data reception
		pI2CHandle->DevAddr = slaveAddr;
		pI2CHandle->Sr = RptStart;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVTEN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITERREN);
	}

	return busystate;
}
/**
 * IRQ Configuration and ISR Handling APIs
 */
void I2C_IRQ_Interrupt_Config(uint8_t IRQNumber,uint8_t EnorDis)
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

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle)
{
	//Implement the code to disable ITBUFEN control bit
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Implement the code to disable ITEVTEN control bit
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxSize = 0;
	pI2CHandle->Rxlen = 0;
	if(pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE)
		I2C_ManageAcking(pI2CHandle->pI2Cx, ENABLE);

}

void I2C_CloseSendData(I2C_Handle_t *pI2CHandle)
{
	//Implement the code to disable ITBUFEN control bit
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Implement the code to disable ITEVTEN control bit
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->Txlen = 0;

}

static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle)
{
	if(pI2CHandle->Txlen > 0)
	{
		//Load the data into DR
		pI2CHandle->pI2Cx->I2C_DR = *(pI2CHandle->pTxBuffer);

		//Decreament Len
		pI2CHandle->Txlen--;

		//Increament TxBuffer address
		pI2CHandle->pTxBuffer++;
	}
}
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle)
{
	if(pI2CHandle->RxSize == 1)
	{
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
		pI2CHandle->Rxlen--;
	}

	if(pI2CHandle->RxSize > 1)
	{
		if(pI2CHandle->Rxlen == 2)
		{
			//clear the ack bit
			I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);
		}
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
		 pI2CHandle->Rxlen--;
		 pI2CHandle->pRxBuffer++;

	}

	if(pI2CHandle->Rxlen == 0)
	{
		// Close the I2C data reception and notify the application
		//generate the stop condition if repeat start is not enabled
		if(pI2CHandle->Sr == I2C_DISABLE_SR)
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//Close the I2C rx
		I2C_CloseReceiveData(pI2CHandle);

		//notify the application
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);

	}
}


/**
 * Data Send and receive APIs for slave mode
 */
void I2C_SlaveSendData(I2C_RegDef_t *pI2C, uint8_t data)
{
     pI2C->I2C_DR = data;
}
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C)
{
    return (uint8_t)pI2C->I2C_DR;
}
/**
 * @brief This function handles the I2C event interrupt request.
 * @param pI2CHandle: Pointer to the I2C handle structure.
 * @note This function should be called from the I2C event interrupt handler.
 */
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle)
{
    //Interrupt handling for both master and slave mode of a device.
    uint32_t temp1, temp2, temp3;
    
    temp1 = pI2CHandle->pI2Cx->I2C_CR2 & (1 << I2C_CR2_ITEVTEN);
    temp2 = pI2CHandle->pI2Cx->I2C_CR2 & (1 << I2C_CR2_ITBUFEN);

    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_SB);
    //1. handle for interrupt generated by SB event
    //Note: SB flag is only aplicable in master mode.Because for slave SB is always set to 0
    if(temp1 && temp3)
    {
        //SB flag event(start condition is generated)
        //In this block we will execute the address phase because after SB is enabled next step is address phase.
        //Depending on the application states we will call The read or write addrase execute phase.
        if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
        {
            I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx , pI2CHandle->DevAddr);
        }
        else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
        {
            I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
        }
    }

    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_ADDR);
    //2. handle for interrupt generated by ADDR event
    //Note: When master mode :Address is sent.
    //      When slave mode:  Address matched with own address
    if(temp1 && temp3)
    {
        //ADDR flag event
        I2C_ClearADDRFlag(pI2CHandle);

    }

    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_BTF);
    //3. handle for interrupt generated by BTF(Byte Transfer Finished) event
    if(temp1 && temp3)
    {
        //BTF flag event
        //  When TXE and BTF both are set then we can say the transmission is complete and we can generate the stop condition.
        if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
        {
            if(pI2CHandle->pI2Cx->I2C_SR1 & I2C_FLAG_SR1_TXE)
            {
                //BTF, TXE = 1
                if(pI2CHandle->Txlen == 0)
                {
                    //1. Generate stop condition if length 0
                    if(pI2CHandle->Sr == I2C_DISABLE_SR)
                    {
                        I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
                    }
                    
                    //2. reset all the member elements of the handle structure 
                    I2C_CloseSendData(pI2CHandle);
                    //3.Notify the application about the transmission complete.
                    I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_TX_CMPLT);
                }
                
            }
             
        }
        else if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
        {
            ;
        }
        
    }

    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_STOPF);
    //4. handle for interrupt generated by STOPF event
    // Note: Stop detection flag is applicable only in master mode.For Master this flag will never be set
    //The below code block won't be executed in master since STOPF will not set in master mode.
    if(temp1 && temp3)
    {
        //STOPF flag is set
        //Clear the STOPF flag i.e. read SR1 and the Write to CR1,read is aready done in temp3
        pI2CHandle->pI2Cx->I2C_CR1 |= 0x0000; //Should not write anything random since it's control reg.

        // Notify the application that stop is generated
        I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_STOP);
    }

    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_TXE);
    //5. handle for interrupt generated by TXE event
    //Note. For RXNE and TXE events to happen both ITEVFEN and ITBUFEN bith the flags bit must be enabled
    if(temp1 && temp2 && temp3)
    {
        //TXE flag event
    	//We will send the data only if the device is in master mode
    	//We can identify the device working mode at an instant by reading the MSL (Master slave)bit of SR2,which is set or reset by hardware on detecting SB=1 or STOP condition
    	if(pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL))
    	{
			//We have to do the data transmission
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
				I2C_MasterHandleTXEInterrupt(pI2CHandle);
    	}
    	else
		{
			//slave
			//make sure slave is really in transmitter mode by verifying the Transmitter/Receiver bit in SR2
			if(pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_TRA))
				I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_DATA_REQ);

		}
    }

    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_RXNE);
    //6. handle for interrupt generated by RXNE event
    if(temp1 && temp2 && temp3)
    {
        //RXNE flag event
    	//We have to do the data reception
    	//Since this function is for master mode only we will do all this for master mode only
    	if(pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL))
    	{
    		//the device is in master mode
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
				I2C_MasterHandleRXNEInterrupt(pI2CHandle);
        }
    	else
		{
			//Slave
			//Verify the TRA bit is reset to make sure slave is in receiver mode
			if(!(pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_TRA)) )
				I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_DATA_RCV);
		}
    }
}
/**
 * @brief This function handles the I2C error interrupt request.
 * @param pI2CHandle: Pointer to the I2C handle structure.
 * @note This function should be called from the I2C error interrupt handler.
 */

void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle)
{

	uint32_t temp1,temp2;

    //Know the status of  ITERREN control bit in the CR2
	temp2 = (pI2CHandle->pI2Cx->I2C_CR2) & ( 1 << I2C_CR2_ITERREN);


/***********************Check for Bus error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1<< I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		//This is Bus error

		//Implement the code to clear the buss error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~( 1 << I2C_SR1_BERR);

		//Implement the code to notify the application about the error
	    I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_BERR);
	}

/***********************Check for arbitration lost error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_ARLO );
	if(temp1  && temp2)
	{
		//This is arbitration lost error

		//Implement the code to clear the arbitration lost error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~I2C_FLAG_SR1_ARLO;
		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_ARLO);

	}

/***********************Check for ACK failure  error************************************/

	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_AF);
	if(temp1  && temp2)
	{
		//This is ACK failure error

	    //Implement the code to clear the ACK failure error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~I2C_FLAG_SR1_AF;
		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_AF);

	}

/***********************Check for Overrun/underrun error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		//This is Overrun/underrun

	    //Implement the code to clear the Overrun/underrun error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~I2C_FLAG_SR1_OVR;
		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_OVR);
	}

/***********************Check for Time out error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_TIMEOUT);
	if(temp1  && temp2)
	{
		//This is Time out error

	    //Implement the code to clear the Time out error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~I2C_FLAG_SR1_TIMEOUT;
		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_TIMEOUT);
	}

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
