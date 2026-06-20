#include "stm32f407xx.h"
#include <string.h>
#include <stdio.h>
extern void initialise_monitor_handles();
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
void small_delay()
{
	for(uint32_t i = 0;i < 9999;i++);
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
uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
  if(ackbyte == 0xF5)
  {
    return 1;
  }
  return 0;
}
void make_shift_reg_ready_for_read(void)
{
	uint8_t dummy_write = 0xff;// for 1 byte. For 2 bytes 0xffff
	uint8_t dummy_read;
	//Do dummy read to clear off the RXNE i,e, we discard the data send by slave shift reg when we send 1 byte data
	SPI_ReceiveData(SPI2,&dummy_read,1);

	/**Concept: In Spi inorder to fetch the response from slave we need to send some dummy bytes.
	 * SPI in Slave will not initiate data transfer to move the data from shift register,slave makes ready the response
	 * and put the response in the Shift registers of SPI but to fetch the ready data we need to send some dummy bytes.
	 */
	SPI_SendData(SPI2,&dummy_write,1);
}
/**
 * @Note In this case we have a slave so we use SSM = 0. On the slave side the SS pin should be pulled to zero for it
 * to be active. So we want control the SS of slave according to SPE of master. i.e. when SSM =0.SPE= 0 then SS = 1
 * and when SSM =0, SPE =1 then SS should be 0. This can be achieved using the SSOE(Slave Select output enabled bit)
 */
int main()
{
	initialise_monitor_handles();
	printf("Hello subha\n");
    uint8_t dummy_write = 0xff;// for 1 byte. For 2 bytes 0xffff
    uint8_t dummy_read;
    uint8_t ackbyte;
    uint8_t args[2];
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
        delay();
		//1. CMD_LED_CTRL  <pin no(1)>  <value(1)>
		uint8_t commandcode = COMMAND_LED_CTRL;
		SPI_SendData(SPI2, &commandcode, 1);
		// clean the registers for receiving data.
		make_shift_reg_ready_for_read();

		//receive the ack from the slave
		SPI_ReceiveData(SPI2,&ackbyte,1);

		//Verify the ackbyte is valid or not
		if (SPI_VerifyResponse(ackbyte))
		{
			printf("Received ACK %d \n",ackbyte);
			args[0] = LED_PIN;
			args[1] = LED_ON;

			//send arguments
			SPI_SendData(SPI2,args,2);
		}
        /*--------end of COMMAND_LED_CTRL--------------*/

		/*-------2. COMMAND_SENSOR_READ  <analog pin number (1)>----*/

		/*Send Data only after button pressed*/
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
		/*Delay to avoid debouncing */
		delay();

		commandcode = COMMAND_SENSOR_READ;
		//Send command
		SPI_SendData(SPI2, &commandcode, 1);

		// clean the registers for receiving data.
		make_shift_reg_ready_for_read();

		//receive the ack from the slave
		SPI_ReceiveData(SPI2,&ackbyte,1);

		//verify the data
		if(SPI_VerifyResponse(ackbyte))
		{
			args[0] = ANALOG_PIN0;
			//Send arguments
			SPI_SendData(SPI2,args,1);

			// We sent the command to read the sensor data from the slave
			//Next we receive the sensor analog value from slave.

			/** we will introduce some delay for the salve to Do the ADC conversions since it reads the data from the Analog Pin
			 * And converts it to digital after it receives sensor read command and then only the data gets ready to be sent to shift reg.
			 * So remember whenever there is any involvement of ADC or DAC we should wait before we fetch the data.
			 */
			small_delay();
			// clean the registers for receiving data.
			make_shift_reg_ready_for_read();
			//we receive the analog value in a variable
			uint8_t analog_value;
			SPI_ReceiveData(SPI2,&analog_value,1);
			printf("COMMAND_SENSOR READ : %d \n",analog_value);
		}
		/*----------End of reading analog value from Slave----------*/
        /*----------------------------------------COMMAND_LED_READ----------------------------------------------*/
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
		delay();
		commandcode = COMMAND_LED_READ;
		SPI_SendData(SPI2,&commandcode,1);
		SPI_ReceiveData(SPI2,&dummy_read,1);
		SPI_SendData(SPI2,&dummy_write,1);
		SPI_ReceiveData(SPI2,&ackbyte,1);
		if(SPI_VerifyResponse(ackbyte))
		{
			args[0] = LED_PIN;
			SPI_SendData(SPI2,args,1);
			SPI_ReceiveData(SPI2,&dummy_read,1);
			small_delay();
			SPI_SendData(SPI2,&dummy_write,1);
			uint8_t led_status;
			SPI_ReceiveData(SPI2,&led_status,1);
			printf("The LED status is %d \n",led_status);
		}

		/*----------------------------------------End of COMMAND_LED_READ----------------------------------------------*/

		/*----------------------------------------COMMAND_PRINT-------------------------------------------------*/
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		commandcode = COMMAND_PRINT;

		//send command
		SPI_SendData(SPI2,&commandcode,1);

		//do dummy read to clear off the RXNE
		SPI_ReceiveData(SPI2,&dummy_read,1);

		//Send some dummy byte to fetch the response from the slave
		SPI_SendData(SPI2,&dummy_write,1);

		//read the ack byte received
		SPI_ReceiveData(SPI2,&ackbyte,1);

		uint8_t message[] = "Hello ! How are you ??";
		if( SPI_VerifyResponse(ackbyte))
		{
			args[0] = strlen((char*)message);

			//send arguments
			SPI_SendData(SPI2,args,1); //sending length

			//do dummy read to clear off the RXNE
			SPI_ReceiveData(SPI2,&dummy_read,1);

			delay();

			//send message
			for(int i = 0 ; i < args[0] ; i++){
				SPI_SendData(SPI2,&message[i],1);
				SPI_ReceiveData(SPI2,&dummy_read,1);
			}

			printf("COMMAND_PRINT Executed \n");

		}

		/*----------------------------------------End of COMMAND_PRINT-------------------------------------------------*/

		/*----------------------------------------COMMAND_ID_READ-----------------------------------------------*/
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		commandcode = COMMAND_ID_READ;

		//send command
		SPI_SendData(SPI2,&commandcode,1);

		//do dummy read to clear off the RXNE
		SPI_ReceiveData(SPI2,&dummy_read,1);

		//Send some dummy byte to fetch the response from the slave
		SPI_SendData(SPI2,&dummy_write,1);

		//read the ack byte received
		SPI_ReceiveData(SPI2,&ackbyte,1);

		uint8_t id[11];
		uint32_t i=0;
		if( SPI_VerifyResponse(ackbyte))
		{
			//read 10 bytes id from the slave
			for(  i = 0 ; i < 10 ; i++)
			{
				//send dummy byte to fetch data from slave
				SPI_SendData(SPI2,&dummy_write,1);
				SPI_ReceiveData(SPI2,&id[i],1);
			}

			id[10] = '\0';

			printf("COMMAND_ID : %s \n",id);

		}
		/*----------------------------------------End of COMMAND_ID_READ-----------------------------------------------*/
		//DISABLE SPI
		/*Before we disable SPI we need to confirm SPI is not busy and we can do that by checking the BSY flag of SR register */
		while(SPI_GetFlagStatus(SPI2, SR_BUSY_FLAG));//IF BSY bit is 1 SPI is Busy else free and we can Deactivate.

		/*Disable SPI*/
		SPI_EnableOrDisable(SPI2,DISABLE);
		printf("SPI comm disabled \n");
    }
    return 0;
}
