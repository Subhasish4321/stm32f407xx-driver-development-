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
 uint32_t data_len=0;
 uint8_t commandCode;
void delay(void)
{
	for(uint32_t i = 0 ; i < 500000 ; i ++);
}

I2C_Handle_t I2C1Handle;
//rcv buffer
uint8_t Tx_buf[] = "STM32 Slave modsfsddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddde testing ...";
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


	//initialise_monitor_handles();

	//printf("Application is running\n");

	 data_len = strlen((char*)Tx_buf);

	 GPIOButtonInit();

	//i2c pin inits
	I2C_GPIOInits();

	//i2c peripheral configuration
	I2C1Init();

	//enable the i2c peripheral
	I2C_PeripheralControl(I2C1,ENABLE);

	//ack bit is made 1 after PE=1
	I2C_ManageAcking(I2C1,I2C_ACK_ENABLE);

	I2C_IRQ_Interrupt_Config(IRQ_NO_I2C1_ER,ENABLE);
	I2C_IRQ_Interrupt_Config(IRQ_NO_I2C1_EV,ENABLE);

	I2C_SlaveEnableDisableCallbackEvents(I2C1,ENABLE);


	while(1);
}


void I2C1_EV_IRQHandler(void)
{
	I2C_EV_IRQHandling(&I2C1Handle);
}
void I2C1_ER_IRQHandler(void)
{
	I2C_ER_IRQHandling(&I2C1Handle);
}


void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv)
{


	static uint32_t cnt = 0;
	static uint32_t w_ptr = 0;



	if(AppEv == I2C_ERROR_AF)
	{
		//This will happen during slave transmitting data to master .
		// slave should understand master needs no more data
		//slave concludes end of Tx


		//if the current active code is 0x52 then dont invalidate
		if(! (commandCode == 0x52))
			commandCode = 0XFF;

		//reset the cnt variable because its end of transmission
		cnt = 0;

		//Slave concludes it sent all the bytes when w_ptr reaches data_len
		if(w_ptr >= (data_len))
		{
			w_ptr=0;
			commandCode = 0xff;
		}

	}else if (AppEv == I2C_EV_STOP)
	{
		//This will happen during end slave reception
		//slave concludes end of Rx

		cnt = 0;

	}else if (AppEv == I2C_EV_DATA_REQ)
	{
		//Master is requesting for the data . send data
		if(commandCode == 0x51)
		{
			//Here we are sending 4 bytes of length information
			I2C_SlaveSendData(I2C1,((data_len >> ((cnt%4) * 8)) & 0xFF));
		    cnt++;
		}else if (commandCode == 0x52)
		{
			//sending Tx_buf contents indexed by w_ptr variable
			I2C_SlaveSendData(I2C1,Tx_buf[w_ptr++]);
		}
	}else if (AppEv == I2C_EV_DATA_RCV)
	{
		//Master has sent command code, read it
		 commandCode = I2C_SlaveReceiveData(I2C1);

	}
}
