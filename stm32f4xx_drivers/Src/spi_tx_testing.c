#include "stm32f407xx.h"
#include <string.h>
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
    SpiPins.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_AF_5;
    SpiPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;//For I2C we are supposed to use OD but SPI we can use PP
    SpiPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    SpiPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    
    //Enable the peripheral clock  
	GPIO_PeriClockControl(GPIOB,ENABLE);
    //MOSI
    SpiPins.GPIO_PinConfig.GPIO_PinNumber = 15;
    GPIO_Init(&SpiPins);
    //MISO
    SpiPins.GPIO_PinConfig.GPIO_PinNumber = 14;
    GPIO_Init(&SpiPins);
    //SCLK
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
    SPIHandle.SPI_Config.SPI_SclkSpeed =SPI_SCK_SPEED_DIV2;
    SPIHandle.SPI_Config.SPI_SSM = SPI_SSM_ENABLE; //Whenever there is no Slave we don't need to use NSS Pin hence Hardware Slave Management is not required.
    //Enable the peripheral clock,we could do this inside the SPI_Init function in case the user forgets.
    SPI_PeriClockControl(SPI2,ENABLE);
    SPI_Init(&SPIHandle);
}
void main()
{
    char data[] = "Hello World";
    SPI2_GPIOInit();
    SPI2_Init();
    SPI_SendData(SPI2,data,strlen(data));
}