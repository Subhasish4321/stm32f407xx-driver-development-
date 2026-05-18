#include "stm32f407xx.h"
#include <string.h>
//COMMAND CODES
#define COMMAND_LED_CTRL    0x50
#define COMMAND_SENSOR_READ  0x51
#define COMMAND_LED_READ     0x52
#define COMMAND_PRINT        0x53
#define COMMAND_ID_READ      0x54

#define LED_ON     1
#define LED_OFF    0
//ARDUINO ANALOG PINS MACRO
#define ANALOG_PIN0  0
#define ANALOG_PIN1  1
#define ANALOG_PIN2  2
#define ANALOG_PIN3  3
#define ANALOG_PIN4  4

//ARDUINO led Pin
#define LED_PIN 9
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
    // SPI SCLK
    SpiPins.GPIO_PinConfig.GPIO_PinNumber = 13;
    GPIO_Init(&SpiPins);
    // SPI MISO
    SpiPins.GPIO_PinConfig.GPIO_PinNumber = 14;
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
void SPI_VerifyResponse(uint8_t ackbyte)
{
  if(ackbyte == 0x05)
  {
    //ack
  }
  else
  {
  }
}
/**
 * @Note In this case we have a slave so we use SSM = 0. On the slave side the SS pin should be pulled to zero for it
 * to be active. So we want control the SS of slave according to SPE of master. i.e. when SSM =0.SPE= 0 then SS = 1
 * and when SSM =0, SPE =1 then SS should be 0. This can be achieved using the SSOE(Slave Select output enabled bit)
 */
int main()
{
    uint8_t dummy_byte = 0xff;// for 1 byte. For 2 bytes 0xffff
    uint8_t ackbyte;
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
      
    //1. CMD_LED_CTRL  <pin no(1)>  <value(1)>
      uint8_t commandcode = COMMAND_LED_CTRL;
      SPI_SendData(SPI2, &commandcode, 1);
      
		/**Concept: In Spi inorder to fetch the response from slave we need to send some dummy bytes.
     * SPI in Slave will not initiate data transfer to move the data from shift register,clave makes ready the response
     * and put the response in the Shift registers of SPI but to fetch the ready data we need to send some dummy bytes.
     */
    SPI_SendData(SPI2,dummy_byte,1);
    //receive the ack from the slave 
    SPI_ReceiveData(SPI2,&ackbyte,1);
    //Verify the ackbyte is valid or not
    SPI_VerifyResponse(uint8_t ackbyte);
      
		/*Before we disable SPI we need to confirm SPI is not busy and we can do that by checking the BSY flag of SR register */
		while(SPI_GetFlagStatus(SPI2, SR_BUSY_FLAG));//IF BSY bit is 1 SPI is Busy else free and we can Deactivate.

		/*Disable SPI*/
		SPI_EnableOrDisable(SPI2,DISABLE);
    }
    return 0;
}
