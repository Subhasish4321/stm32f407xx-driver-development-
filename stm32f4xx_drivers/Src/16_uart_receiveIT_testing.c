/*
 *      16_uart_receiveIT_testing.c
 *      Author: Subhasish Singha
 */

#include<string.h>
#include "stm32f407xx.h"
#include <stdio.h>

extern void initialise_monitor_handles();
void delay(void)
{
    for(uint32_t i = 0 ; i < 500000 ; i ++);
}
USART_Handle_t USART2_Handle;
uint8_t data[] = "Congratulations to me ,I dit it. \n";

uint8_t rcv_data[100];
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
    USART2_Handle.USART_Config.USART_Mode = USART_MODE_TXRX;
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
volatile uint8_t rxstate = RESET;

int main(void)
{
	initialise_monitor_handles();
	printf("Printing\n");
    //Gpio button init
    GPIOButtonInit();
    // Pin init
	USART_GPIOInits();

    //USART configuration
     USART2Init();

    //enable the USART peripheral
    USART_PeripheralControl(USART2,ENABLE);
    
    //enable the interrupt for USART2
    USART_IRQInterruptConfig(IRQ_NO_USART2, ENABLE);

    while(1)
    {
        //wait until button is pressed
        while(! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

        //delay for button de-bouncing
        delay();
        // activate the IT receive datato initialise the RXNEIE bit of CR1 register .
        //Note: We manually call the receive data function in the main loop and it is not called automatically when the interrupt is generated.
        while(USART_ReceiveDataIT(&USART2_Handle, rcv_data, strlen((char*)data)) != USART_READY);

        //to send data
        USART_SendData(&USART2_Handle,data, strlen((char*)data));
        
        while(! rxstate);

        rcv_data[sizeof(data)] = '\0';
        
        rxstate = RESET;
        printf("Data received is : %s \n", (char*)rcv_data);


    }
    
    return 0;
}


/*
 * Application callback
 */
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t AppEv)
{
    if(AppEv == USART_EVENT_TX_CMPLT)
    {
        printf("Transmission is completed \n");
        

    }
    else if(AppEv == USART_EVENT_RX_CMPLT)
    {
        rxstate = SET;
    }

}

void USART2_IRQHandler(void)
{
    USART_IRQHandling(&USART2_Handle);
}
