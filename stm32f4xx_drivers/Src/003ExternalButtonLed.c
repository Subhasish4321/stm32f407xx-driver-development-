/*
* Write a program to connect external button to the pin number PB12 and external LED to PA14
* and toggle the LED when the extenal button in pressed.
* Later we changed the LED pin to PA8 because PA14 is configured to be operating in debug mode
* See section 8.3.1 of reference manual for more details.
*/

#include "stm32f407xx_gpio_driver.h"
#define HIGH 1
#define LOW 0
#define BUTTON_PRESSED LOW
void delay(void)
{
    for(uint32_t i =0; i< 500000/2 ;i++);
}
int main(void)
{
    GPIO_Handle_t GPIOBtn, GPIOLed;
    GPIOBtn.pGPIOx = GPIOB;
    GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GPIOBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

    GPIOLed.pGPIOx = GPIOA;
    GPIOLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
    GPIOLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GPIOLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GPIOLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GPIOLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;


    // peripheral Clock starts
    GPIO_PeriClockControl(GPIOB,ENABLE);
    GPIO_PeriClockControl(GPIOA,ENABLE);

    //GPIO peripheral init configurations
    GPIO_Init(&GPIOBtn);
    GPIO_Init(&GPIOLed);
    while(1)
    {
        if(GPIO_ReadFromInputPin(GPIOB,GPIO_PIN_NO_12) == BUTTON_PRESSED)
        {
            delay();
            GPIO_ToggleOutputPin(GPIOA,GPIO_PIN_NO_8);
        }
    }


}
