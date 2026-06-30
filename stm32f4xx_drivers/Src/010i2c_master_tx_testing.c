/*
 * 010I2C_master_tx_testing.c
 *
 *  Created on: Feb 10, 2019
 *      Author: admin
 */

#include<string.h>
#include "stm32f407xx.h"
#include <stdio.h>

#define MY_ADDRESS 0x61
#define SLAVE_ADDRESS 0x68
extern void initialise_monitor_handles();
void delay(void)
{
	for(uint32_t i = 0 ; i < 500000 ; i ++);
}

I2C_Handle_t I2C1Handle;
//data
uint8_t master_data[] = "We are testing the SPI Tx. \n";
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
	initialise_monitor_handles();
    //Button init
    GPIOButtonInit();
    // Pin init
	I2C_GPIOInits();
    //I2c configuration
    I2C1Init();

    //enable the i2c peripheral
    I2C_PeripheralControl(I2C1,ENABLE);
    
    while(1)
	{
		//wait till button is pressed
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );
		//to avoid button de-bouncing related issues 200ms of delay
		delay();
        //Send some data to slave
        I2C_MasterSendData(I2C1Handle,&master_data,strlen((char*)master_data),SLAVE_ADDRESS);
    }
    return 0;
}
