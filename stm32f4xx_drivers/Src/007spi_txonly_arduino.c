/*
 * 006spi_tx_testing.c
 *
 *  Created on: Feb 10, 2019
 *      Author: admin
 */

#include<string.h>
#include "stm32f407xx.h"
#include <stdio.h>

extern void initialise_monitor_handles();
void delay(void)
{
	for(uint32_t i = 0 ; i < 500000 ; i ++);
}

/*
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 -> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT function mode : 5
 */

/* SPI Slave Demo

 *
 * SPI pin numbers:
 * SCK   13  // Serial Clock.
 * MISO  12  // Master In Slave Out.
 * MOSI  11  // Master Out Slave In.
 * SS    10  // Slave Select . Arduino SPI pins respond only if SS pulled low by the master
 *

 */

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_PeriClockControl(GPIOB, ENABLE);
	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	//GPIO_Init(&SPIPins);


	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);


}

void SPI2_Init(void)
{
    SPI_Handle_t SPIHandle;
    SPIHandle.pSPIx= SPI2;
    SPIHandle.SPI_Config.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    SPIHandle.SPI_Config.SPI_DeviceMode = SPI_MODE_MASTER;
    SPIHandle.SPI_Config.SPI_CPHA = SPI_CPHA_LOW;
    SPIHandle.SPI_Config.SPI_CPOL = SPI_CPOL_LOW;
    SPIHandle.SPI_Config.SPI_DFF = SPI_DFF_8BITS;
    SPIHandle.SPI_Config.SPI_SclkSpeed = SPI_SCK_SPEED_DIV256;
    SPIHandle.SPI_Config.SPI_SSM = SPI_SSM_DISABLE;//Hardware Slave management.
    SPI_PeriClockControl(SPI2,ENABLE);
    SPI_Init(&SPIHandle);
}
void SPI2_GPIOButtonInit(void)
{
	GPIO_Handle_t GPIOBtn;
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;


//	GPIO_Handle_t GPIOPin;
//	GPIOPin.pGPIOx = GPIOA;
//	GPIOPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
//	GPIOPin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
//	GPIOPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
//	GPIOPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
//	GPIOPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE); //Enable peripheral clock.

	GPIO_Init(&GPIOBtn);
//	GPIO_Init(&GPIOPin);

}


int main(void)
{
	char user_data[] = "hello";
	initialise_monitor_handles();
	SPI2_GPIOButtonInit();
//    printf("Printing is on \n");
	//this function is used to initialize the GPIO pins to behave as SPI2 pins
	SPI2_GPIOInits();

//	while(1)
//	{
//		uint8_t button_state = GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0);
//		delay();
//		printf("Button state: %d \n",button_state);
//	}
	//This function is used to initialize the SPI2 peripheral parameters
	SPI2_Init();

	/*
	* making SSOE 1 does NSS output enable.
	* The NSS pin is automatically managed by the hardware.
	* i.e when SPE=1 , NSS will be pulled to low
	* and NSS pin will be high when SPE=0
	*/
	SPI_SSOE_Config(SPI2,ENABLE);
	while(1)
	{
		//wait till button is pressed
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		//enable the SPI2 peripheral
		SPI_EnableOrDisable(SPI2,ENABLE);
//		GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_5, 0);//using alternate pin since NSS seems to be not working.
		delay();
//		uint8_t Nss_state = GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_5);
//        printf("The pa5 is %d \n",Nss_state);
		//first send length information
		uint8_t dataLen = strlen(user_data);
		SPI_SendData(SPI2,&dataLen,1);
//		delay();
		//to send data
		SPI_SendData(SPI2,(uint8_t*)user_data,strlen(user_data));

		//lets confirm SPI is not busy
		while( SPI_GetFlagStatus(SPI2,SR_BUSY_FLAG) );
//		GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_5, 1);
		//Disable the SPI2 peripheral
		SPI_EnableOrDisable(SPI2,DISABLE);;
	}

	return 0;

}
