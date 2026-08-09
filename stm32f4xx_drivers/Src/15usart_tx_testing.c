/*
 * 010USART_master_tx_testing.c
 *
 *  Created on: Feb 10, 2019
 *      Author: admin
 */

#include<string.h>
#include "stm32f407xx.h"
#include <stdio.h>

void delay(void)
{
    for(uint32_t i = 0 ; i < 500000 ; i ++);
}
USART_Handle_t USART2_Handle;
uint8_t data[] = "Congratulations to me ,I dit it. \n";
void USART_GPIOInits(void)
{
	GPIO_Handle_t USARTPins;
    USARTPins.pGPIOx = GPIOA;
    USARTPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    USARTPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;         
	USARTPins.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PIN_PU; 
	USARTPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP ;         
	USARTPins.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_ALT_FUN_MODE_7;

    //USART2_TX
    USARTPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_2;
    GPIO_Init(&USARTPins);

    //USART2_RX
    USARTPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
    GPIO_Init(&USARTPins);

}

void USART2Init(void)
{
    USART2_Handle.pUSARTx = USART2;
    USART2_Handle.USART_Config.USART_Baud = USART_BAUD_115200;
    USART2_Handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    USART2_Handle.USART_Config.USART_Mode = USART_MODE_TX;
    USART2_Handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
    USART2_Handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
    USART2_Handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
    USART2_Handle.USART_Config.USART_OverSampling = USART_OVR_SMPL_16;

    USART_Init(&USART2_Handle);
}
void GPIOButtonInit(void)
{
	GPIO_Handle_t GPIOBtn;
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PD;
	GPIO_Init(&GPIOBtn);
}


int main(void)
{
    //Gpio button init
    GPIOButtonInit();
    // Pin init
	USART_GPIOInits();

    //USART configuration
     USART2Init();

    //enable the USART peripheral
    USART_PeripheralControl(USART2,ENABLE);

    while(1)
    {
        while(! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

        //delay for button de-bouncing
        delay();

        //to send data
        USART_SendData(&USART2_Handle,data, strlen((char*)data));

        
    }
    
    return 0;
}



