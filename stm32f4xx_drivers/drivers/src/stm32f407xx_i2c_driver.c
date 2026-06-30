/**
 * @file stm32f407xx_i2c_driver.c
 * @brief Source file for I2C driver for STM32F407xx microcontroller.
 * @author Subhasish Singha
 */

 #include "stm32f407xx.h"

 uint16_t AHB1_PreScaler [8] = {2,4,8,16,64,128,256,512};
 uint8_t APB1_PreScaler [4] = {2,4,8,16};

static void I2C_ExecuteAddressPhase(I2C_RegDef_t *pI2CX,uint8_t slaveAddress);
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx);
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);
 /**
  * Helper Functions
  */
 static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
 {
     pI2Cx->CR1 |= (1 << I2C_CR1_START);
 }
 static void I2C_ExecuteAddressPhase(I2C_RegDef_t *pI2CX,uint8_t slaveAddress)
 {
     slaveAddress = slaveAddress << 1;
     slaveAdress &= ~(1);
     pI2CX->I2C_DR = slaveAddress;
 }
 static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx)
 {
     uint32_t dummyRead = pI2Cx->I2C_SR1;
     dummyRead = pI2Cx->I2C_SR1;
     (void) dummyRead;
 }
 static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
 {
    pI2Cx->I2C_CR1 |= (1 << I2C_CR1_STOP);
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
        return FLAG_SET
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
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer,uint32_t len, uint8_t slaveAddr)
{
    //Generate the start condition
    I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

    // Confirm that start generation is completed by checking the SB flag in the SR1
    //Note: Until SB is cleared SCL will be stretched(pulled to LOW).
    //When SR1 SB bit is read the SB bit is cleared automatically and followed by witing the DR register.
    while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SR1_SB) );
    
    //Send the 7 bit address of the slave with r/~w bit(r =1,w =0) (toatl 8 bits)
    //Address is written to address bits(1 - 7) bit 0(r/~w)
    I2C_ExecuteAddressPhase(pI2CHandle->pI2CX,slaveAddress);

    //confirm the address phase is completed by checking the ADDR flag SR1 register.
    while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SR1_ADDR));
    //Note: Until ADDR is cleared SCL will be stretched. 
    // Also clearing this flags are important because we need the same flag for the next data sent or recevied.
    I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

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

     //Generate the STOP condition
     I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
}

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
