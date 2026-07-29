/*
 * 010I2C_master_tx_testing.c
 *
 *  Created on: Feb 10, 2019
 *      Author: admin
 */

#include<string.h>
#include "stm32f407xx.h"
#include <stdio.h>

#define SLAVE_ADDRESS 0x68
#define MY_ADDRESS SLAVE_ADDRESS
void delay(void)
{
	for(uint32_t i = 0 ; i < 500000 ; i ++);
}

I2C_Handle_t I2C1Handle;
//rcv buffer
uint8_t Tx_buf[32] = "STM32 Slave mode testing ...";
/*
 * PB6 --> SCL
 * PB7 --> SDA   (changed from pb9 to pb7)
 * ALT function mode : 4
 */

void I2C_GPIOInits(void)
{
	GPIO_Handle_t I2CPins;
    I2CPins.pGPIOx = GPIOB;
    I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;         
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PIN_PU; 
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD ;         
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_ALT_FUN_MODE_4;

    //scl
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
    GPIO_Init(&I2CPins);
    /**
     * Concept: while tracing the logic levels fot sda i.e. pb9 we observed that there is some glitch in the data trace and verifying the d
     * data sheets and user manuals we came to observe that pb9 is also used as a SWIM pin amd hence we changed to pb7 from pb9.
     */
    //sda
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
    GPIO_Init(&I2CPins);

}

void I2C1Init(void)
{
    I2C1Handle.pI2Cx = I2C1;
    I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;
    I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDRESS; //for master mode this is not compulsory,but while assigning the addresses we shoul make sure that the address are
    I2C1Handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;// not part of the reserved address mentioned in the i2c bus specs.
    I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_2;//Also in standard mode we can use any duty cycle does not matter.  
    I2C_Init(&I2C1Handle);
}
void GPIOButtonInit(void)
{
	GPIO_Handle_t GPIOBtn;
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&GPIOBtn);
}


int main(void)
{
    //Gpio button init
    GPIOButtonInit();
    // Pin init
	I2C_GPIOInits();
    //I2c configuration
    I2C1Init();
    //Configure the IRQ NUMBERS ,this is required because the slave application is always in Interrupt mode.
    I2C_IRQ_Interrupt_Config(IRQ_NO_I2C1_EV,ENABLE);
    I2C_IRQ_Interrupt_Config(IRQ_NO_I2C1_ER,ENABLE);

    I2C_SlaveEnableDisableCallbackEvents(I2C1,ENABLE);
    //enable the i2c peripheral
    I2C_PeripheralControl(I2C1,ENABLE);
    //Ack bit is set to 1 after PE is set to 1 in CR1 register, so we need to set the ack bit after enabling the peripheral.
    I2C_ManageAcking(I2C1, I2C_ACK_ENABLE);

    while(1);

    return 0;
}

void I2C1_EV_IRQHandler(void)
{
	I2C_EV_IRQHandling(&I2C1Handle);
}
void I2C1_ER_IRQHandler(void)
{
	I2C_ER_IRQHandling(&I2C1Handle);
}


void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t app_event)
{
    /**
     * Concept: the commandCode and cnt are declared as static variables because we want to retain their values across multiple calls to this function.
     * Since I2C_ApplicationEventCallback is called repeatedly by the IRQ handler on each I2C event, non-static locals would be re-initialized to 0 on every call.
     */
    static uint8_t commandCode = 0;
    static uint8_t cnt = 0;
	if(app_event == I2C_EV_DATA_REQ)
    {
        //Master wants to receive data, slave should send it.
        if(commandCode == 0x51 )
        {
            //Send length information to master
            I2C_SlaveSendData(pI2CHandle->pI2Cx, strlen((char*)Tx_buf));
        }
        else if(commandCode == 0x52)
        {
            //Concept:For each byte of data received master fires I2C_EV_DATA_REQ event until the entire data is received, so the cnt is increased on each call to this function and hence the next byte of data is sent to master.
            //Send the data stored in TxBuf.
            I2C_SlaveSendData(pI2CHandle->pI2Cx, Tx_buf[cnt++]);
        }
    }
    else if(app_event == I2C_EV_DATA_RCV)
    {
        //This is the testing application for slave transmit and hence we are using the slave receive section only to receive the coomandcode to send the length and data only.
        //Master has sent some data, slave should read it.
        //Master sends the code 0x51 to send the length.
        commandCode = I2C_SlaveReceiveData(pI2CHandle->pI2Cx);

    }
    else if(app_event == I2C_ERROR_AF)
    {
        //This happens only during slave transmission.
        //Master has sent NACK, so slave should understand that master does not want more data.
        commandCode = 0xFF; //invalidate the command code, so that no data is sent.
        cnt = 0;
    }
    else if(app_event == I2C_EV_STOP)
    {
        //This happens only in slave reception mode.This is not implemented because in this application we are not using the slave reception mode.
        //Slave should understand that master has ended the communication.
    }
    
}
