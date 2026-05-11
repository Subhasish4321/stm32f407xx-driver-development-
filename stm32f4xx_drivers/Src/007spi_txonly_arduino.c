#include "stm32f407xx.h"
#include <string.h>

void delay()
{
	for(uint32_t i = 0;i < 999999;i++);
}
/**
 * SPI Pins
 * SPI2_MISO PB14 in AF5 mode
 * SPI2_MOSI PB15 in AF5 mode
 * SPI2_NSS PB12  in AF5
 * SPI2_SCK PB13 in AF5
 * @details This Function is used to initialze the GPIO pins to behave as SPI Pins.
 * @note - The SPI_GPIOInit is not a driver API instead it is an application level function,that uses the driver API GPIOInit()
 */
void SPI2_GPIOInit(void)
{
    GPIO_Handle_t SpiPins;
    SpiPins.pGPIOx = GPIOB;
    SpiPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    SpiPins.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_ALT_FUN_MODE_5;
    SpiPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    SpiPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    SpiPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    //Enable the peripheral clock
	GPIO_PeriClockControl(GPIOB,ENABLE);
    //MOSI
    SpiPins.GPIO_PinConfig.GPIO_PinNumber = 15;
    GPIO_Init(&SpiPins);

     SpiPins.GPIO_PinConfig.GPIO_PinNumber = 13;
    GPIO_Init(&SpiPins);
    //NSS
    SpiPins.GPIO_PinConfig.GPIO_PinNumber = 12;
    GPIO_Init(&SpiPins);
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
    SPIHandle.SPI_Config.SPI_SclkSpeed = SPI_SCK_SPEED_DIV8;//for 2 MHz clock we divide by 8.
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

	GPIO_PeriClockControl(GPIOA, ENABLE); //Enable peripheral clock.

	GPIO_Init(&GPIOBtn);
}
/**
 * @Note In this case we have a slave so we use SSM = 0. On the slave side the SS pin should be pulled to zero for it
 * to be active. So we want control the SS of slave according to SPE of master. i.e. when SSM =0.SPE= 0 then SS = 1
 * and when SSM =0, SPE =1 then SS should be 0. This can be achieved using the SSOE(Slave Select output enabled bit)
 */
int main()
{
    char data[] = "Hello World";

    /*Initialize GPIO for SPI Configuration Pins*/
    SPI2_GPIOInit();

    /*Initialize GPIO Button for sending the data on button press */
    SPI2_GPIOButtonInit();

    /*Initialize SPI*/
    SPI2_Init();
    while(1)
    {
    	/*Send Data only after button pressed*/
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
		/*Delay to avoid debouncing */
		delay();

		/*Slave Select control for single master using SSOE,see note for more info*/
		SPI_SSOE_Config(SPI2,ENABLE);

		/*Enable SPI*/
		SPI_EnableOrDisable(SPI2,ENABLE);

		/*Before we send data we will send data length to Slave*/
		uint8_t data_len = strlen(data);
		SPI_SendData(SPI2, &data_len, 1);

		/*Send data*/
		SPI_SendData(SPI2,(uint8_t*)data,data_len);

		/*Before we disable SPI we need to confirm SPI is not busy and we can do that by checking the BSY flag of SR register */
		while(SPI_GetFlagStatus(SPI2, SR_BUSY_FLAG));//IF BSY bit is 1 SPI is Busy else free and we can Deactivate.

		/*Disable SPI*/
		SPI_EnableOrDisable(SPI2,DISABLE);
    }
    return 0;
}
